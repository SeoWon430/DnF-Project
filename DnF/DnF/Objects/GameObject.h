#pragma once

enum class ArmorState
{
	Normal,
	SuperArmor,
	Invincible
};

enum class Direction {
	Back = -1,
	None = 0,
	Front = 1

};
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Attacked(INT64 damage = 0, float hold = 0.0f);
	
	void Initalize();


	virtual void Update(Matrix& V, Matrix &P);
	virtual void Render();

	virtual Matrix World();

	UINT64 Hp() { return hp; }
	UINT64 HpMax() { return hpMax; }


	void CreateMotionClip(wstring shaderFile, wstring spriteFile, vector<UINT> idx);


	void MoveTo(Vector3 toPosition, float time);
	void MoveTo(Vector3 toPosition, Vector3 accel);

	//void Hold();
	//void HoldStart();
	//void HoldEnd();


	void Position(float x, float y);
	void Position(Vector2& position);
	void Position(Vector3& position);
	Vector3 Position();

	void Scale(float x, float y);
	void Scale(Vector2& scale);
	void Scale(Vector3& scale);
	Vector3 Scale();

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& rotation);
	Vector3 Rotation();

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& rotation);
	Vector3 RotationDegree();

	Vector2 GroundPosition();

	Vector2 ScaledSize();
	Vector3 ScaledSize3D();
	Direction Direction_() { return direction; }
	Collider* GetCollider() { return collider; }

protected:
	Animation* animation;
	Collider* collider;

	Vector3 position = Vector3();
	/* ★좌표계
		position ( x:수평 위치 / y:수직 위치 / z:지면 y위치 )
			=> 점프 할 때, y위치값이 바뀌고 점프 지면 위치를 z에 저장
			=> 이후 y는 중력에 따라 다시 z값으로 되돌아옴

		scale ( x:화면의 수평 / y:화면의 수직 / z: y좌표상 너비 )
			=> x,y는 이미지의 가로,세로와 같은 축
			=> z는 점프가 아닌 지면상 y의 거리 계산에 사용

		=> position의 y,z는 scale의 y,z와 의미가 다름
	*/

	bool isInvincibility;
	bool isActive;
	UINT64 hp;
	UINT64 hpMax;

	Direction direction = Direction::Front;

	bool isMovable = true;
	float moveToTime = 0.0f;
	Vector3 toPosition = Vector3(0, 0, 0);
	Vector3 accel = Vector3();


	float holdTime = 0.0f;
	bool isHold = false;

	ArmorState armorState = ArmorState::Normal;
	ID3DX11EffectVectorVariable* armorColor;
	ID3DX11EffectVectorVariable* armorSize;
	float armorTime = 0.0f;
};