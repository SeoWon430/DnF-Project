#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
	:isInvincibility(false)
	,isActive(false)
	,hp(1)
	, position(0, 0, 0)
{
	animation = new Animation();
	collider = new BoxCollider(CenterPoint::CenterDown, ColliderType::Stand);
	ColliderFactory::Get()->CreateCollider("aa", collider);

}

GameObject::~GameObject()
{
	SafeDelete(collider);
	SafeDelete(animation);
}

void GameObject::Attacked(INT64 damage, float hold)
{
	if (armorState == ArmorState::Invincible)
		return;
	
	if (hp >= damage)
		hp -= damage;
	else
		hp = 0;

	if (holdTime <= 0.0f && armorState != ArmorState::SuperArmor)
		holdTime = hold;
	

}

void GameObject::Initalize()
{
	armorColor = animation->GetSprite()->GetShader()->AsVector("outLineColor");
	armorSize = animation->GetSprite()->GetShader()->AsVector("outLineSize");
}

void GameObject::Update(Matrix & V, Matrix & P)
{
	if (animation->RotationDegree().y > 90)
		direction = Direction::Back;
	else
		direction = Direction::Front;

	if (holdTime > 0.0f)
	{
		holdTime -= Time::Delta();
		isHold = true;
	}else
		isHold = false;



	if (moveToTime > 0.0f)
	{
		moveToTime -= Time::Delta();
		Vector3 pos = animation->Position();
		pos += toPosition * Time::Delta();
		animation->Position(pos);

		if (moveToTime < 0.0f)
		{
			isMovable = true;
			moveToTime = 0.0f;
			toPosition = Vector3(0, 0, 0);
			accel = Vector3(0, 0, 0);
		}
	}
	if (accel != Vector3(0, 0, 0))
	{
		Vector3 pos = animation->Position();

		if (Math::Length(pos, toPosition) < 1.0f)
		{
			isMovable = true;
			moveToTime = 0.0f;
			toPosition = Vector3(0, 0, 0);
			accel = Vector3(0, 0, 0);
		}
		pos += (toPosition + accel) * Time::Delta();
		animation->Position(pos);
	}


	Vector2 size = animation->GetSprite()->ScaledSize();
	size.x = 1/size.x*0.1f;
	size.y = 1/size.y*0.1f;
	armorSize->SetFloatVector(size);
	armorTime += Time::Delta();
	D3DXVECTOR4 color;
	switch (armorState)
	{
		case ArmorState::Normal:
			color = D3DXVECTOR4(0, 0, 0, 0);
			break;
		case ArmorState::SuperArmor:
			if (armorTime > 0.5f)
				color = D3DXVECTOR4(1, 0, 0, 1);
			else
				color = D3DXVECTOR4(1, 0.8f, 0, 1);
			break;
		case ArmorState::Invincible:
			if (armorTime > 0.5f)
				color = D3DXVECTOR4(0.9f, 0.9f, 0.9f, 1);
			else
				color = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1);
			break;
		default:
			break;
	}
	if (armorTime > 1.0f)
		armorTime = 0.0f;
	armorColor->SetFloatVector(color);
}

void GameObject::Render()
{
}

Matrix GameObject::World()
{
	return Matrix();
}

void GameObject::CreateMotionClip(wstring shaderFile, wstring spriteFile, vector<UINT> idx)
{
	Clip* clip;
	UINT length = idx.size() - 1;


	// 위 idx를 가지고 도적 케릭터에 대한 이미지를 받아 클립으로 저장
	for (UINT i = 0; i < length; i += 3) {
		clip = new Clip((PlayMode)idx[i + 2], 1.0f);

		// idx순서가 정방향인 경우
		if (idx[i] < idx[i + 1])
			for (UINT j = idx[i]; j < idx[i + 1]; j++) {
				wstring fileName = spriteFile + to_wstring(j) + L".png";
				clip->AddFrame(new Sprite(fileName, shaderFile, CenterPoint::CenterDown), 0.2f);
			}
		// idx순서가 역방향인 경우
		else
			for (UINT j = idx[i]; j > idx[i + 1]; j--) {
				wstring fileName = spriteFile + to_wstring(j) + L".png";
				clip->AddFrame(new Sprite(fileName, shaderFile, CenterPoint::CenterDown), 0.2f);
			}

		// 클립 저장
		animation->AddClip(clip);
	}

}

void GameObject::MoveTo(Vector3 toPosition, float time)
{
	isMovable = false;
	this->toPosition = toPosition;
	this->moveToTime = time;
}

void GameObject::MoveTo(Vector3 toPosition, Vector3 accel)
{
	isMovable = false;
	this->toPosition = toPosition;
	this->accel = accel;
}


void GameObject::Position(float x, float y)
{
	position.x = x;
	position.y = y;
	animation->Position(position);
}
void GameObject::Position(Vector2 & position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	animation->Position(position);
}
void GameObject::Position(Vector3 & position)
{
	this->position = position;
	animation->Position(position);
}
Vector3 GameObject::Position()
{
	position = animation->Position();
	return position;
}

void GameObject::Scale(float x, float y)
{
	animation->GetSprite()->Scale(x, y);
}
void GameObject::Scale(Vector2 & scale)
{
	animation->GetSprite()->Scale(scale);
}
void GameObject::Scale(Vector3 & scale)
{
	animation->GetSprite()->Scale(scale);
}
Vector3 GameObject::Scale()
{
	return animation->GetSprite()->Scale();
}

void GameObject::Rotation(float x, float y, float z)
{
	animation->GetSprite()->Rotation(x, y, z);
}
void GameObject::Rotation(Vector3 & rotation)
{
	animation->GetSprite()->Rotation(rotation);
}
Vector3 GameObject::Rotation()
{
	return animation->GetSprite()->Rotation();
}

void GameObject::RotationDegree(float x, float y, float z)
{
	animation->GetSprite()->RotationDegree(x, y, z);
}
void GameObject::RotationDegree(Vector3 & rotation)
{
	animation->GetSprite()->RotationDegree(rotation);
}
Vector3 GameObject::RotationDegree()
{
	return animation->GetSprite()->RotationDegree();
}




Vector2 GameObject::GroundPosition()
{
	Vector2 result = animation->Position();
	result.y = Position().z;
	return result;
}

Vector2 GameObject::ScaledSize()
{
	Vector2 result;
	result.x = animation->Scale().x * animation->TextureSize().x;
	result.y = animation->Scale().y * animation->TextureSize().y;

	return result;
}

Vector3 GameObject::ScaledSize3D()
{
	Vector3 result;
	result.x = animation->Scale().x * animation->TextureSize().x;
	result.y = animation->Scale().y * animation->TextureSize().y;
	result.z = animation->Scale().z * animation->TextureSize().y;

	return result;
}
