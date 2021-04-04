#pragma once

enum class PlayMode { End = 0, Loop };


struct Frame
{
	Sprite* Image;	// 프레임 이미지
	float Time;		// 프레임 유지 시간

	Frame(Sprite* image, float time);
	~Frame();
};


class Clip
{
public:
	Clip(PlayMode mode = PlayMode::End, float speed = 1.0f);
	~Clip();

	void AddFrame(Sprite* sprite, float time);
	bool EndFrame();
	UINT FrameCount() { return frames.size(); }

	void Play(UINT startFrame=0, float speed = 1.0f);
	void Play(function<void()>& func, UINT startFrame=0, float speed = 1.0f);
	void Stop();

	void Update(Matrix &V, Matrix &P);
	void Render();


	void Position(float x, float y);
	void Position(Vector2& position);
	void Position(Vector3& position);
	Vector3 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& scale);
	void Scale(Vector3& scale);
	Vector3 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& rotation);
	Vector3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& rotation);
	Vector3 RotationDegree();

	void Speed(float val) { speed = val; }

	void Mode(PlayMode mode) { this->mode = mode; }
	PlayMode Mode() { return mode; }

	Vector2 TextureSize();
	Sprite* GetSprite();
	void SetFrameTime(UINT frameNumber, float time);


private:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	float length;		// 전체 재생 시간
	float speed;		// 재생 속도
	bool isPlay;		// 재생중인지
	UINT currentFrame;	// 현재 프레임 번호
	float playTime;		// 현재 시간, 다음 프레임으로 갈 타이머 역할

	PlayMode mode;

	vector<Frame*> frames;
	function<void()> endFunction = NULL;
};