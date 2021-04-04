#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	:position(0, 0)
{
	D3DXMatrixIdentity(&view);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	D3DXVECTOR3 eye(position.x, position.y, 0.0f);
	D3DXVECTOR3 at(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 1.0f);
	D3DXMatrixLookAtLH(&view, &eye, &(eye+at), &up);
}

void Camera::Position(float x, float y)
{
	Position(Vector2(x, y));
}

void Camera::PositionX(float x)
{
	position.x = x;
}

void Camera::PositionY(float y)
{
	position.y = y;
}

void Camera::Position(Vector2 & pos)
{
	position = pos;
}
