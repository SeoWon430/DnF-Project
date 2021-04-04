#include "stdafx.h"
#include "ColliderFactory.h"


ColliderFactory* ColliderFactory::instance = NULL;

ColliderFactory::ColliderFactory()
{
}

ColliderFactory::~ColliderFactory()
{
	for (auto collider : colliders)
		SafeDelete(collider.second);
	//for (auto collider : colliders2)
	//	SafeDelete(collider);
}

void ColliderFactory::Create()
{
	assert(instance == NULL);

	instance = new ColliderFactory();
}

void ColliderFactory::Delete()
{
	SafeDelete(instance);
}

ColliderFactory * ColliderFactory::Get()
{
	return instance;
}

string ColliderFactory::CreateCollider(string name, Collider* collider)
{
	//if (name.size() < 1 || colliders.count(name) > 0)
	//	return "";

	colliders[name] = collider;
	colliders2.push_back(collider);
	return name;
}

Vector3 ColliderFactory::InRange(Vector3 center, float range, ColliderTag tag)
{
	Vector3 result = Vector3(0, 0, 0);
	for (auto collider : colliders2)
	{
		if (collider->Tag() == tag &&
			(range * range) >= Math::LengthXZ(center, collider->Position()))
			result = collider->Position();
	}
	return result;
}

bool ColliderFactory::Collision(string a, string b)
{
	//return Aabb(colliders[a]->World(), colliders[b]->World());
	return true;
}

bool ColliderFactory::Collision(BoxCollider * collider, Vector3 target)
{

	return false;
}

bool ColliderFactory::Collision(CylinderCollider * collider, Vector3 target)
{
	if (collider->Position().y > target.y)
		return false;
	if(collider->Height() < target.y)
		return false;
	


}

bool ColliderFactory::Collision(BoxCollider * collider1, CylinderCollider * collider2)
{
	return false;
}

bool ColliderFactory::Collision(BoxCollider * collider1, BoxCollider * collider2)
{
	float groundY = collider1->GetWorld()._43 - collider2->GetWorld()._43;

	// 점프 햇을 경우를 가정한 y값 차이
	float jumpY = collider1->GetWorld()._42 - collider2->GetWorld()._42 - groundY;

	return AabbEX(groundY, jumpY, collider1, collider2);
}



// 점과 영역의 충돌
//	영역 : world
//	점 : position
bool ColliderFactory::Aabb(const Matrix & world, const Vector2 & position)
{
	// Scale에는 Rotation도 포함 됨
	//	=> 방향을 무시하기 위해 절대값으로 계산
	float halfWidth = fabsf(world._11 * 0.5f);
	float halfHeight = fabsf(world._22 * 0.5f);

	float left = world._41 - halfWidth;
	float right = world._41 + halfWidth;
	float top = world._42 + halfHeight;
	float bottom = world._42 - halfHeight;

	bool result = true;
	result &= position.x >= left;
	result &= position.x <= right;
	result &= position.y >= bottom;
	result &= position.y <= top;


	return result;
}

// 영역과 영역의 충돌
bool ColliderFactory::Aabb(const Matrix & world1, const Matrix & world2, CenterPoint m1, CenterPoint m2)
{
	bool result = true;
	float halfWidth;
	float halfHeight;
	float left1, left2;
	float right1, right2;
	float top1, top2;
	float bottom1, bottom2;


	halfWidth = fabsf(world1._11 * 0.5f);
	halfHeight = fabsf(world1._22 * 0.5f);
	left1 = world1._41 - halfWidth;
	right1 = world1._41 + halfWidth;
	if (m1 == CenterPoint::Center)
	{
		top1 = world1._42 + halfHeight;
		bottom1 = world1._42 - halfHeight;
	}
	else if (m1 == CenterPoint::CenterDown)
	{
		top1 = world1._42 + halfHeight * 2;
		bottom1 = world1._42;
	}


	halfWidth = fabsf(world2._11 * 0.5f);
	halfHeight = fabsf(world2._22 * 0.5f);
	left2 = world2._41 - halfWidth;
	right2 = world2._41 + halfWidth;
	if (m2 == CenterPoint::Center)
	{
		top2 = world2._42 + halfHeight;
		bottom2 = world2._42 - halfHeight;
	}
	else if (m2 == CenterPoint::CenterDown)
	{
		top2 = world2._42 + halfHeight * 2;
		bottom2 = world2._42;
	}

	result &= left1 <= right2;
	result &= right1 >= left2;
	result &= top1 >= bottom2;
	result &= bottom1 <= top2;

	return result;
}

bool ColliderFactory::AabbEX(float groundGap, float jumpGap, BoxCollider* collider1, BoxCollider* collider2)
{
	Matrix world1 = collider1->GetWorld();
	Matrix world2 = collider2->GetWorld();
	CenterPoint mode1 = collider1->CenterMode();
	CenterPoint mode2 = collider2->CenterMode();


	if ((collider1->Type() == ColliderType::Flat))
	{
		if ((jumpGap > 0) && (jumpGap > fabsf(world2._22)))
			return false;

		if ((jumpGap <= 0) && (-jumpGap > fabsf(world1._22)))
			return false;
	}

	if ((groundGap > 0) && (groundGap > fabsf(world2._33)))
		return false;
	else if ((groundGap <= 0) && (-groundGap > fabsf(world2._33)))
		return false;



	return ColliderFactory::Aabb(world1, world2, mode1, mode2);
}

bool ColliderFactory::AabbEX(Vector3 pos1, Vector3 pos2, Vector3 scale1, Vector3 scale2, Collider* collider1, Collider* collider2)
{
	float scaleX = scale1.x*0.5f;
	float scaleY = scale1.y*0.5f;
	float scaleZ = scale1.z*0.5f;

	float left_1 = pos1.x - scaleX;
	float right_1 = pos1.x + scaleX;
	float jump_bottom_1;
	float jump_top_1;
	float ground_bottom_1;
	float ground_top_1;

	BoxCollider* coll = dynamic_cast<BoxCollider*>(collider1);
	if (coll != NULL)
	{
		if (coll->Type() == ColliderType::Flat)
		{
			jump_bottom_1 = fabsf(pos1.y - pos1.z);
			jump_top_1 = fabsf(jump_bottom_1 + scaleZ);

			ground_bottom_1 = pos1.y - scaleY;
			ground_top_1 = pos1.y + scaleY;
		}
		else if (coll->Type() == ColliderType::Stand)
		{
			if (coll->CenterMode() == CenterPoint::Center)
			{
				ground_bottom_1 = pos1.z - scaleZ;
				ground_top_1 = pos1.z + scaleZ;
				jump_bottom_1 = fabsf(pos1.y - pos1.z - scaleY);
				jump_top_1 = fabsf(pos1.y - pos1.z + scaleY);
			}
			else if (coll->CenterMode() == CenterPoint::CenterDown)
			{
				ground_bottom_1 = pos1.z;
				ground_top_1 = pos1.z + scaleZ * 2.0f;
				jump_bottom_1 = fabsf(pos1.y - pos1.z);
				jump_top_1 = fabsf(jump_bottom_1 + scaleY * 2.0f);
			}
		}
	}

	scaleX = scale2.x*0.5f;
	scaleY = scale2.y*0.5f;
	scaleZ = scale2.z*0.5f;

	float left_2 = pos2.x - scaleX;
	float right_2 = pos2.x + scaleX;
	float jump_bottom_2;
	float jump_top_2;
	float ground_bottom_2;
	float ground_top_2;
	coll = dynamic_cast<BoxCollider*>(collider2);
	if (coll != NULL)
	{
		if (coll->Type() == ColliderType::Flat)
		{
			jump_bottom_2 = fabsf(pos2.y - pos2.z);
			jump_top_2 = fabsf(jump_bottom_2 + scaleZ);

			ground_bottom_2 = pos2.y - scaleY;
			ground_top_2 = pos2.y + scaleY;
		}
		else if (coll->Type() == ColliderType::Stand)
		{
			if (coll->CenterMode() == CenterPoint::Center)
			{
				ground_bottom_2 = pos2.z - scaleZ;
				ground_top_2 = pos2.z + scaleZ;
				jump_bottom_2 = fabsf(pos2.y - pos2.z - scaleY);
				jump_top_2 = fabsf(jump_bottom_2 + scaleY);
			}
			else if (coll->CenterMode() == CenterPoint::CenterDown)
			{
				ground_bottom_2 = pos2.z;
				ground_top_2 = pos2.z + scaleZ * 2.0f;
				jump_bottom_2 = fabsf(pos2.y - pos2.z);
				jump_top_2 = fabsf(pos1.y - pos1.z + scaleY * 2.0f);
			}
		}
	}

	if (right_1 < left_2)
		return false;
	else if (left_1 > right_2)
		return false;

	else if (ground_bottom_1 > ground_top_2)
		return false;
	else if (ground_top_1 < ground_bottom_2)
		return false;

	else if (jump_bottom_1 > jump_top_2)
		return false;
	else if (jump_top_1 < jump_bottom_2)
		return false;

	return true;
}


bool ColliderFactory::Obb(Sprite * a, Sprite * b)
{
	ObbDesc A, B;
	Vector2 halfSize;

	// OBB객체 생성
	halfSize = Vector2(a->Scale().x * a->TextureSize().x, a->Scale().y * a->TextureSize().y)*0.5f;
	CreateObb(&A, halfSize, a->World());
	halfSize = Vector2(b->Scale().x * b->TextureSize().x, b->Scale().y * b->TextureSize().y)*0.5f;
	CreateObb(&B, halfSize, b->World());

	Vector2 distance = a->Position() - b->Position();

	// 1) A.Right 축 투영
	float lengthA = D3DXVec2Length(&A.Length_Right);
	//float lengthB = fabsf(D3DXVec2Dot(&A.Right, &B.Length_Right)) + fabsf(D3DXVec2Dot(&A.Right, &B.Length_Up));
	float lengthB = SeprateAxis(A.Right, B.Length_Right, B.Length_Up);
	float length = fabsf(D3DXVec2Dot(&A.Right, &distance));
	if (length > lengthA + lengthB) return false;


	// 2) A.Up 축 투영
	lengthA = D3DXVec2Length(&A.Length_Up);
	//lengthB = fabsf(D3DXVec2Dot(&A.Up, &B.Length_Right)) + fabsf(D3DXVec2Dot(&A.Up, &B.Length_Up));
	lengthB = SeprateAxis(A.Up, B.Length_Right, B.Length_Up);
	length = fabsf(D3DXVec2Dot(&A.Up, &distance));
	if (length > lengthA + lengthB) return false;


	// 3) B.Right 축 투영
	lengthA = D3DXVec2Length(&B.Length_Right);
	//lengthB = fabsf(D3DXVec2Dot(&B.Right, &A.Length_Right)) + fabsf(D3DXVec2Dot(&B.Right, &A.Length_Up));
	lengthB = SeprateAxis(B.Up, A.Length_Right, A.Length_Up);
	length = fabsf(D3DXVec2Dot(&B.Right, &distance));
	if (length > lengthA + lengthB) return false;


	// 4) B.Up 축 투영
	lengthA = D3DXVec2Length(&B.Length_Up);
	//lengthB = fabsf(D3DXVec2Dot(&B.Up, &A.Length_Right)) + fabsf(D3DXVec2Dot(&B.Up, &A.Length_Up));
	lengthB = SeprateAxis(B.Up, A.Length_Right, A.Length_Up);
	length = fabsf(D3DXVec2Dot(&B.Up, &distance));
	if (length > lengthA + lengthB) return false;


	// 위 4가지 경우를 거쳐 오면 충돌 true
	return true;

}

void ColliderFactory::CreateObb(ObbDesc * out, Vector2 & half, Matrix & transform)
{
	out->HalfSize = half;

	// Normalize Vector
	D3DXVec2Normalize(&out->Right, &Vector2(transform._11, transform._12));
	D3DXVec2Normalize(&out->Up, &Vector2(transform._21, transform._22));

	// Direction Vector
	out->Length_Right = out->Right * out->HalfSize.x;
	out->Length_Up = out->Up * out->HalfSize.y;


}

float ColliderFactory::SeprateAxis(Vector2 & init, Vector2 & e1, Vector2 & e2)
{
	return fabsf(D3DXVec2Dot(&init, &e1)) + fabsf(D3DXVec2Dot(&init, &e2));
}
