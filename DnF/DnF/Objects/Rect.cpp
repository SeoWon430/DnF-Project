#include "stdafx.h"
#include "Rect.h"

Rect::Rect(wstring shaderFile)
	:position(0, 0), scale(1, 1), color(1, 1, 1, 1), pass(0)
{
	Initialize(shaderFile);
}

Rect::Rect(wstring shaderFile, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: position(position), scale(scale), color(color), pass(0)
{
	Initialize(shaderFile);
	Color(color);
}

void Rect::Initialize(wstring shaderFile)
{

	shader = new Shader(shaderFile);
	collider = new Collider();
	D3DXMatrixIdentity(&world);

	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");
	sColor = shader->AsVector("Color");

	CreateBuffer(shaderFile);
	UpdateWorld();
}


Rect::~Rect()
{
	SafeRelease(indexBuffer);
	SafeRelease(vertexBuffer);
	SafeDelete(collider);
	SafeDelete(shader);
}

void Rect::Update(Matrix& V, Matrix &P)
{
	sView->SetMatrix(V);
	sProjection->SetMatrix(P);

	collider->World(world);
	collider->Update(V, P);
}

void Rect::Render()
{
	// Start Pipeline
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
											// 용량 형식 인덱스버퍼가 4바이트이기에 크기 맞춰서 넣음

		//shader->Draw(0, 0, 6);

		// 인덱스 버퍼를 사용한 드로우
		shader->DrawIndexed(0, 0, 6);

	}
	collider->Render();
}


void Rect::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Rect::Position(Vector2 & pos)
{
	position = pos;
	UpdateWorld();
}

Vector2 Rect::Position()
{
	return position;
}

void Rect::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Rect::Scale(Vector2 & scal)
{
	scale = scal;
	UpdateWorld();
}

Vector2 Rect::Scale()
{
	return scale;
}

void Rect::Color(float r, float g, float b, float a)
{
	Color(D3DXCOLOR(r, g, b, a));
}

void Rect::Color(D3DXCOLOR & color)
{
	this->color = color;

	sColor->SetFloatVector(this->color);
}

D3DXCOLOR Rect::Color()
{
	return color;
}

void Rect::CreateBuffer(wstring shaderFile)
{
	// 삼각형 2개 : 정점6개
	//	=> 사각형 1개 = 삼각형 2개
	//	=> 점 4개만 있으면 삼각형 2개 표현 가능 => 인덱스 사용
	Vertex vertices[4];
	HRESULT result;


	// Vertex Setting
	{
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = Vector3(-0.5f, 0.5f, 0.0f);
		vertices[2].Position = Vector3(0.5f, -0.5f, 0.0f);
		vertices[3].Position = Vector3(0.5f, 0.5f, 0.0f);
	}



	// Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		// 정점 하나를 바꿀 경우 버퍼를 변경하기에 SUBRESOURCE_DATA를 사용
		desc.Usage = D3D11_USAGE_IMMUTABLE;		//CPU, GPU둘다 접근 불가 -> 여기서는 정점 변경 X
		//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	

		desc.ByteWidth = sizeof(Vertex) * 4;	
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	


		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;


		// 버퍼 생성
		result = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(result));
	}


	// Index Setting
	UINT indices[] = { 0,1,2,2,1,3 };

	// Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;		// 버퍼 사용 용도 VB와 IB를 구분

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		// 버퍼 생성
		result = Device->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(result));
	}

}

void Rect::UpdateWorld()
{
	Matrix s, t;

	D3DXMatrixScaling(&s, scale.x, scale.y, 1);
	D3DXMatrixTranslation(&t, position.x, position.y, 0.0f);

	world = s * t;

	sWorld->SetMatrix(world);
}
