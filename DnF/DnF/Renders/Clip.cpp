#include "stdafx.h"
#include "Clip.h"


//#############################################################################
// Frame
//#############################################################################
Frame::Frame(Sprite * image, float time)
{
	Image = image;
	Time = time;
}
Frame::~Frame()
{
	SafeDelete(Image);
}


//#############################################################################
// Clip(Class) : Frames
//#############################################################################

Clip::Clip(PlayMode mode, float speed)
	:mode(mode)
	, speed(1.0f/speed)
	, position(0, 0, 0)
	, scale(1, 1, 1)
	, rotation(0, 0, 0)
	, playTime(0.0f)
	, length(0.0f)
	, isPlay(false)
	, currentFrame(0)
	, endFunction(NULL)
{
}

Clip::~Clip()
{
	for (Frame* f : frames)
		SafeDelete(f);
}

void Clip::AddFrame(Sprite * sprite, float time)
{
	frames.push_back(new Frame(sprite, time));
}

bool Clip::EndFrame()
{
	if (mode == PlayMode::Loop) return false;
	return !isPlay;
}

void Clip::Play(UINT startFrame, float speed)
{
	playTime = 0.0f;
	currentFrame = startFrame;
	isPlay = true;
	this->speed = 1.0f/speed;
}

void Clip::Play(function<void()>& func, UINT startFrame, float speed)
{
	// 클립 끌날때 실행 할 함수 지정
	endFunction = bind(func);

	Play(startFrame, speed);
}

void Clip::Stop()
{
	isPlay = false;
	currentFrame = 0;
	currentFrame = frames.size() - 1;

	// 현재 클립이 끝나면 지정된 함수 실행
	if (endFunction != NULL)
		endFunction();
	endFunction = NULL;
}

void Clip::Update(Matrix & V, Matrix & P)
{
	Frame* frame = frames[currentFrame];
	if (isPlay == true) {
		float time = frame-> Time * speed;
		playTime += Time::Delta();

		if (playTime >= time)
		{
			currentFrame++;


			switch (mode)
			{
				case PlayMode::End:
					if (currentFrame >= frames.size())
						Stop();
					break;
				case PlayMode::Loop:
					currentFrame %= frames.size();
					break;
			}
			playTime = 0.0f;
		}
	}


	frames[currentFrame]->Image->Scale(scale);
	frames[currentFrame]->Image->Rotation(rotation);
	frames[currentFrame]->Image->Position(position);
	frames[currentFrame]->Image->Update(V, P);
}

void Clip::Render()
{
	frames[currentFrame]->Image->Render();
}

Vector2 Clip::TextureSize()
{
	return frames[currentFrame]->Image->TextureSize();
}

Sprite * Clip::GetSprite()
{
	return frames[currentFrame]->Image;
}

void Clip::SetFrameTime(UINT frameNumber, float time)
{
	if(frames.size() > frameNumber)
		frames[frameNumber]->Time = time;
	else
		frames[frames.size()-1]->Time = time;
}


void Clip::Position(float x, float y)
{
	Position(Vector2(x, y));
}
void Clip::Position(Vector2 & position)
{
	this->position.x = position.x;
	this->position.y = position.y;
}
void Clip::Position(Vector3 & position)
{
	this->position = position;
}

void Clip::Scale(float x, float y)
{
	Scale(Vector2(x, y));
}
void Clip::Scale(Vector2 & scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
}
void Clip::Scale(Vector3 & scale)
{
	this->scale = scale;
}

void Clip::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}
void Clip::Rotation(Vector3 & rotation)
{
	this->rotation = rotation;
}
void Clip::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Clip::RotationDegree(Vector3 & rotation)
{
	Vector3 r;
	r.x = Math::ToRadian(rotation.x);
	r.y = Math::ToRadian(rotation.y);
	r.z = Math::ToRadian(rotation.z);

	Rotation(r);
}
Vector3 Clip::RotationDegree()
{
	Vector3 rot;
	rot.x = Math::ToDegree(rotation.x);
	rot.y = Math::ToDegree(rotation.y);
	rot.z = Math::ToDegree(rotation.z);
	return rot;
}


