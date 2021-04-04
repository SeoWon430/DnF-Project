#include "stdafx.h"
#include "Canvas.h"

Canvas::Canvas(Vector2 position, Vector2 scale)
	:position(position)
	, scale(scale)
	, rotation(0, 0, 0)
	, pass(0)
{
	shader = new Shader(L"Canvas.fx");

	{
		//Position
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[3].position = Vector3(+0.5f, +0.5f, 0.0f);

		//Uv
		vertices[0].Uv = Vector2(0, 1);
		vertices[1].Uv = Vector2(0, 0);
		vertices[2].Uv = Vector2(1, 1);
		vertices[3].Uv = Vector2(1, 0);
	}

	CreateBuffer();

	// View 설정 : 카메라 (고정)
	D3DXMatrixLookAtLH(&vp.View, &Vector3(0, 0, -1), &Vector3(0, 0, 0), &Vector3(0, 1, 0));
	// Projection
	D3DXMatrixOrthoOffCenterLH(&vp.Projection, 0, (float) WIDTH, 0, (float) HEIGHT, -1, 1);
	

	sDiffuseMap = shader->AsShaderResource("DiffuseMap");
	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");

	UpdateWorld();
}

Canvas::~Canvas()
{
	SafeRelease(indexBuffer);
	SafeRelease(vertexBuffer);
	SafeDelete(shader);
}


void Canvas::Render()
{
	sView->SetMatrix(vp.View);
	sProjection->SetMatrix(vp.Projection);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6);

}

void Canvas::SRV(ID3D11ShaderResourceView * srv)
{
	sDiffuseMap->SetResource(srv);
}

void Canvas::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Canvas::Position(Vector2 & pos)
{
	position = pos;
	UpdateWorld();
}

void Canvas::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}

void Canvas::Scale(Vector2 & scal)
{
	scale = scal;
	UpdateWorld();
}

void Canvas::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Canvas::Rotation(Vector3 & rot)
{
	rotation = rot;
	UpdateWorld();
}

void Canvas::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Canvas::RotationDegree(Vector3 & rot)
{
	Vector3 r;
	r.x = Math::ToRadian(rot.x);
	r.y = Math::ToRadian(rot.y);
	r.z = Math::ToRadian(rot.z);

	Rotation(r);
}

Vector3& Canvas::RotationDegree()
{
	Vector3 rot = Vector3();
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}

void Canvas::CreateBuffer()
{
	HRESULT result;

	// Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
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

void Canvas::UpdateWorld()
{
	Matrix W, S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	W = S * R * T;

	sWorld->SetMatrix(W);
}
