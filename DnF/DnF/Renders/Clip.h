#pragma once

enum class PlayMode { End = 0, Loop };


struct Frame
{
	Sprite* Image;	// ������ �̹���
	float Time;		// ������ ���� �ð�

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

	float length;		// ��ü ��� �ð�
	float speed;		// ��� �ӵ�
	bool isPlay;		// ���������
	UINT currentFrame;	// ���� ������ ��ȣ
	float playTime;		// ���� �ð�, ���� ���������� �� Ÿ�̸� ����

	PlayMode mode;

	vector<Frame*> frames;
	function<void()> endFunction = NULL;
};