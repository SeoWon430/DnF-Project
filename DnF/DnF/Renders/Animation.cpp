#include "stdafx.h"
#include "Animation.h"

Animation::Animation()
	:position(0, 0, 0)
	, scale(1, 1, 1)
	, currentClipNumber(-1)
	, rotation(0, 0, 0)
{
}

Animation::~Animation()
{
	for (Clip* c : clips)
		SafeDelete(c);
}

void Animation::AddClip(Clip * clip)
{
	clips.push_back(clip);
}

void Animation::Play(UINT clipNumber, float speed)
{
	if (clipNumber == currentClipNumber)
		return;
	if(clipNumber> -1)
		clips[currentClipNumber]->Stop();

	currentClipNumber = clipNumber;
	clips[currentClipNumber]->Play(0, speed);
}

void Animation::Play(function<void()> func, UINT clipNumber, float speed)
{
	if (clipNumber == currentClipNumber)
		return;
	if (clipNumber > -1)
		clips[currentClipNumber]->Stop();

	currentClipNumber = clipNumber;
	clips[currentClipNumber]->Play(func, 0, speed);
}

void Animation::Stop()
{
	if (currentClipNumber > -1)
		clips[currentClipNumber]->Stop();
}

void Animation::Update(Matrix & V, Matrix & P)
{
	if (currentClipNumber < 0)
		return;

	clips[currentClipNumber]->Position(position);
	clips[currentClipNumber]->Scale(scale);
	clips[currentClipNumber]->Rotation(rotation);
	clips[currentClipNumber]->Update(V, P);
}

void Animation::Render()
{
	if (currentClipNumber < 0)
		return;

	clips[currentClipNumber]->Render();
}

void Animation::SetClipSpeed(UINT clipNumber, float speed)
{
	if (clipNumber == currentClipNumber)
		return;

	clips[clipNumber]->Speed(speed);
}

Clip * Animation::GetCurrentClip()
{
	if (currentClipNumber < 0)
		return NULL;

	return clips[currentClipNumber];
}

Clip * Animation::GetClip(UINT clipNumber)
{
	if(clips.size() <= clipNumber)
		return NULL;

	return clips[clipNumber];
}

Sprite * Animation::GetSprite()
{
	return GetCurrentClip()->GetSprite();
}

Vector2 Animation::ScaledSize()
{
	Vector2 scaledSize;
	scaledSize.x = scale.x * TextureSize().x;
	scaledSize.y = scale.y * TextureSize().y;
	return scaledSize;
}

Vector2 Animation::TextureSize()
{
	assert(currentClipNumber > -1);
	return clips[currentClipNumber]->TextureSize();
}

void Animation::Position(float x, float y)
{
	Position(Vector2(x, y));
}
void Animation::Position(Vector2 & position)
{
	this->position.x = position.x;
	this->position.y = position.y;
}
void Animation::Position(Vector3 & position)
{
	this->position = position;
}

void Animation::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}
void Animation::Scale(Vector2 & scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
}
void Animation::Scale(Vector3 & scale)
{
	this->scale = scale;
}

void Animation::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}
void Animation::Rotation(Vector3 & rotation)
{
	this->rotation = rotation;
}

void Animation::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}
void Animation::RotationDegree(Vector3 & rotation)
{
	Vector3 r;
	r.x = Math::ToRadian(rotation.x);
	r.y = Math::ToRadian(rotation.y);
	r.z = Math::ToRadian(rotation.z);

	Rotation(r);
}
Vector3 Animation::RotationDegree()
{
	Vector3 rot;
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}
