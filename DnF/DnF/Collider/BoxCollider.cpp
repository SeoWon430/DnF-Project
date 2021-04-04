#include "stdafx.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider(CenterPoint mode, ColliderType type, ColliderTag tag)
	:Collider(tag)
	, centerMode(mode)
	, type(type)
{
	D3DXMatrixIdentity(&world);
	CreateBuffer();
}

BoxCollider::~BoxCollider()
{
}


void BoxCollider::World(Matrix & world)
{
	this->world = world;
	//if (type == ColliderType::Flat)
	//{
	//	this->world._11;
	//	float tmp = this->world._22;
	//	this->world._22 = this->world._33;
	//	this->world._33 = tmp;
	//}

	if (tag == ColliderTag::Monster)
		int a = 0;
}

void BoxCollider::CreateBuffer()
{
	Vertex vertices[5];
	if (centerMode == CenterPoint::Center) {
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = Vector3(-0.5f, 0.5f, 0.0f);
		vertices[2].Position = Vector3(0.5f, 0.5f, 0.0f);
		vertices[3].Position = Vector3(0.5f, -0.5f, 0.0f);
		vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);
	}
	else if (centerMode == CenterPoint::CenterDown) {
		vertices[0].Position = Vector3(-0.5f, 0.0f, 0.0f);
		vertices[1].Position = Vector3(-0.5f, 1.0f, 0.0f);
		vertices[2].Position = Vector3(0.5f, 1.0f, 0.0f);
		vertices[3].Position = Vector3(0.5f, 0.0f, 0.0f);
		vertices[4].Position = Vector3(-0.5f, 0.0f, 0.0f);
	}


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