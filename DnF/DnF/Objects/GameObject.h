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
	/* ����ǥ��
		position ( x:���� ��ġ / y:���� ��ġ / z:���� y��ġ )
			=> ���� �� ��, y��ġ���� �ٲ�� ���� ���� ��ġ�� z�� ����
			=> ���� y�� �߷¿� ���� �ٽ� z������ �ǵ��ƿ�

		scale ( x:ȭ���� ���� / y:ȭ���� ���� / z: y��ǥ�� �ʺ� )
			=> x,y�� �̹����� ����,���ο� ���� ��
			=> z�� ������ �ƴ� ����� y�� �Ÿ� ��꿡 ���

		=> position�� y,z�� scale�� y,z�� �ǹ̰� �ٸ�
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