#include "stdafx.h"
#include "Collider.h"


Collider::Collider(ColliderTag tag)
	:tag(tag)
{
	shader = new Shader(L"Bounding.fx");
	D3DXMatrixIdentity(&world);


	// 셰이더 변수에 대한 접근을 최소화 하기 위해 저장 -> 최적화
	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");
	sColor = shader->AsVector("LineColor");
}

Collider::~Collider()
{
	SafeRelease(vertexBuffer);
	SafeDelete(shader);
}

void Collider::Update(Matrix & V, Matrix & P)
{
	sWorld->SetMatrix(world);
	sView->SetMatrix(V);
	sProjection->SetMatrix(P);

	if (tag == ColliderTag::Monster)
		int a = 0;
}

void Collider::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	sColor->SetFloatVector(isCollisionColor ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));

	shader->Draw(0, 0, 5);
}

void Collider::World(Matrix & world)
{
	this->world = world;
	if (tag == ColliderTag::Monster)
		int a = 0;
}

Vector3 Collider::Position()
{
	Vector3 pos;
	pos.x = world._41;
	pos.y = world._42;
	pos.z = world._43;

	if (tag == ColliderTag::Monster)
		int a = 0;
	return pos;
}


void Collider::CreateBuffer()
{
	Vertex vertices[5];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = Vector3(0.5f, 0.5f, 0.0f);
	vertices[3].Position = Vector3(0.5f, -0.5f, 0.0f);
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);


	HRESULT result;
	// Create Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		result = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(result));
	}
}
