#pragma once
class Animation
{
public:
	Animation();
	~Animation();

	void AddClip(Clip* clip);
	void Play(UINT clipNumber, float speed = 1.0f);
	void Play(function<void()> func, UINT clipNumber, float speed = 1.0f);
	void Stop();

	void Update(Matrix& V, Matrix &P);
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

	Vector2 ScaledSize();
	Vector2 TextureSize();

	int GetCurrentClipNumber() { return currentClipNumber; }
	void SetClipSpeed(UINT clipNumber, float speed);
	Clip* GetCurrentClip();
	Clip* GetClip(UINT clipNumber);
	Sprite* GetSprite();


private:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	int currentClipNumber;	// 현재 재생 클립
	vector<Clip*> clips;
};