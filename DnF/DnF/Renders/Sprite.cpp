#include "stdafx.h"
#include "Sprite.h"

//#############################################################################
// Sprite
//#############################################################################
Sprite::Sprite(wstring textureFile, wstring shaderFile, CenterPoint mode)
{
	// 이미지를 자르지 않고 그대로 처리
	Initialize(textureFile, shaderFile, 0, 0, 0, 0, mode);
}
Sprite::Sprite(wstring textureFile, wstring shaderFile, float endX, float endY, CenterPoint mode)
{
	// 왼쪽 상단의 이미지를 자를 때
	Initialize(textureFile, shaderFile, 0, 0, endX, endY, mode);
}
Sprite::Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, CenterPoint mode)
{
	// 중간의 이미지를 자를 때
	Initialize(textureFile, shaderFile, startX, startY, endX, endY, mode);
}

void Sprite::Initialize(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, CenterPoint mode)
{
	HRESULT result;
	// Set SRV
	{
		this->textureFile = L"./_Textures/" + textureFile;

		//shader = new Shader(shaderFile);
		pass = 0;

		srv = Sprites::Load(this->textureFile, shaderFile, &shader);
		shader->AsShaderResource("DiffuseMap")->SetResource(srv);
	}

	D3DX11_IMAGE_INFO info;	// 이미지 파일에 대한 정보 저장
							// 해상도, Depth, Mip(확대/축소), Format(rgb8) 등
	result = D3DX11GetImageInfoFromFile(this->textureFile.c_str(), NULL, &info, NULL);
	assert(SUCCEEDED(result));

	// UV 설정 : 픽셀 값으로 받은 것을 0~1값으로 재설정(정규화)
	{
		startX = (startX > 0) ? startX / (float)info.Width : 0.0f;
		startY = (startY > 0) ? startY / (float)info.Height : 0.0f;
		endX = (endX > 0) ? endX / (float)info.Width : 1.0f;
		endY = (endY > 0) ? endY / (float)info.Height : 1.0f;
	}


	// Vertex Setting
	// Set SubResource
	Vertex vertices[4];
	{
		if (mode == CenterPoint::Center) {
			vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
			vertices[1].position = Vector3(-0.5f, 0.5f, 0.0f);
			vertices[2].position = Vector3(0.5f, -0.5f, 0.0f);
			vertices[3].position = Vector3(0.5f, 0.5f, 0.0f);
		}
		else if (mode == CenterPoint::LeftUp) {
			vertices[0].position = Vector3(0.0f, -1.0f, 0.0f);
			vertices[1].position = Vector3(0.0f, 0.0f, 0.0f);
			vertices[2].position = Vector3(1.0f, -1.0f, 0.0f);
			vertices[3].position = Vector3(1.0f, 0.0f, 0.0f);
		}
		else if (mode == CenterPoint::CenterDown) {
			vertices[0].position = Vector3(-0.5f, 0.0f, 0.0f);
			vertices[1].position = Vector3(-0.5f, 1.0f, 0.0f);
			vertices[2].position = Vector3(0.5f, 0.0f, 0.0f);
			vertices[3].position = Vector3(0.5f, 1.0f, 0.0f);
		}

		vertices[0].uv = Vector2(startX, endY);
		vertices[1].uv = Vector2(startX, startY);
		vertices[2].uv = Vector2(endX, endY);
		vertices[3].uv = Vector2(endX, startY);
	}


	// 멤버변수 초기화
	{
		float sizeX = (endX > 0) ? endX * (float)info.Width : (float)info.Width;
		sizeX -= startX * (float)info.Width;
		float sizeY = (endY > 0) ? endY * (float)info.Height : (float)info.Height;
		sizeY -= startY * (float)info.Height;
		textureSize = Vector2(sizeX, sizeY);

		scale = Vector3(1, 1, 1);
		rotation = Vector3(0, 0, 0);
		position = Vector3(0, 0, 0);

		D3DXMatrixIdentity(&world);
		sWorld = shader->AsMatrix("World");
		sView = shader->AsMatrix("View");
		sProjection = shader->AsMatrix("Projection");

		UpdateWorld();
	}


	// Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = {0};	
		// 안에 포인터가 없으면 ZeroMemory안해도 됨
		//	포인터가 있더라도 모두 초기화 할경우도 안해도 됨
		data.pSysMem = vertices;

		result = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(result));
	}


	// Create IndexBuffer
	UINT indices[6] = { 0,1,2,2,1,3 };
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// gpu, cpu 둘다 접근 불가
									// 인덱스는 바뀔일이 거의 없음

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		result = Device->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(result));
	}

}

Sprite::~Sprite()
{
	SafeRelease(indexBuffer);
	SafeRelease(vertexBuffer);
	//SafeDelete(shader);
	Sprites::Remove(textureFile);
}



void Sprite::Update(Matrix & V, Matrix & P)
{
	sView->SetMatrix(V);
	sProjection->SetMatrix(P);
}

void Sprite::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6);
}

void Sprite::UpdateWorld()
{
	Matrix S, R, T;

	D3DXMatrixScaling(&S, textureSize.x*scale.x, textureSize.y*scale.y, textureSize.y*scale.z);
	D3DXMatrixRotationYawPitchRoll(&R,rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	world = S * R * T;

	sWorld->SetMatrix(world);
}



void Sprite::Position(float x, float y)
{
	Position(Vector2(x, y));
}
void Sprite::Position(Vector2 & position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	UpdateWorld();
}
void Sprite::Position(Vector3 & position)
{
	this->position = position;
	UpdateWorld();
}

void Sprite::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}
void Sprite::Scale(Vector3 & scale)
{
	this->scale = scale;
	UpdateWorld();
}
void Sprite::Scale(Vector2 & scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	UpdateWorld();
}

Vector2 Sprite::ScaledSize()
{
	Vector2 result;
	result.x = scale.x * textureSize.x;
	result.y = scale.y * textureSize.y;

	return result;
}

Vector3 Sprite::ScaledSize3D()
{
	Vector3 result;
	result.x = scale.x * textureSize.x;
	result.y = scale.y * textureSize.y;
	result.z = scale.z * textureSize.y;

	return result;
}

void Sprite::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}
void Sprite::Rotation(Vector3 & rotation)
{
	this->rotation = rotation;
	UpdateWorld();
}

void Sprite::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}
void Sprite::RotationDegree(Vector3 & rotation)
{
	Vector3 r;
	r.x = Math::ToRadian(rotation.x);
	r.y = Math::ToRadian(rotation.y);
	r.z = Math::ToRadian(rotation.z);

	Rotation(r);
}
Vector3 Sprite::RotationDegree()
{
	Vector3 rot;
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}











//#############################################################################
// Sprites : SRV관리 클래스
//#############################################################################
// static 변수 초기화
map<wstring, Sprites::SpriteDesc> Sprites::spriteMap;	// STL은 초기화 안하고 선언만 해도 됨
map< wstring, Shader*> Sprites::shaderMap;
// 이미지 파일에 대한 srv생성
ID3D11ShaderResourceView * Sprites::Load(wstring file, wstring shaderFile, Shader** shader)
{
	// 이미 srv가 만들어져 있는 경우
	if (spriteMap.count(file) > 0) {
		spriteMap[file].RefCount++;
		(*shader) = shaderMap[file];
		return spriteMap[file].SRV;
	}

	shaderMap[file] = new Shader(shaderFile);
	(*shader) = shaderMap[file];


	HRESULT result;
	ID3D11ShaderResourceView* srv;
	result = D3DX11CreateShaderResourceViewFromFile(Device, file.c_str(), NULL, NULL, &srv, NULL);
	assert(SUCCEEDED(result));

	SpriteDesc desc;
	desc.RefCount++;
	desc.SRV = srv;

	spriteMap[file] = desc;

	return desc.SRV;
}

void Sprites::Remove(wstring file)
{
	UINT count = spriteMap.count(file);
	assert(count > 0);

	spriteMap[file].RefCount--;

	if (spriteMap[file].RefCount < 1) {
		SafeRelease(spriteMap[file].SRV);
		spriteMap.erase(file);
	}

	map<wstring, Shader*>::iterator it ;
	for (it = shaderMap.begin(); it != shaderMap.end(); it++)
	{
		SafeDelete(it->second);
	}
	shaderMap.clear();

}
