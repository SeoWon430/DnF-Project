#pragma once
#include "Viewer/IFollowing.h"

enum class PlayerMotion {
	Idle = 0		// 0
	, Ready			// 1
	, Walk			// 2
	, JumpStart		// 3
	, JumpEnd		// 4
	, JumpAttack	// 5
	, Run			// 6
	, Hit			// 7
	, Down			// 8
	, Attack1		// 9
	, Attack2		// 10
	, Attack3		// 11
	, Attack4		// 12
	, Sting			// 13
	, DrawOff		// 14
	, Slash			// 15
	, Extract
	, Invisible
	, None
};

enum class PlayerState
{
	None,
	Idle,
	Walk,	// Run & Walk
	Run,
	Jump,
	Hit,
	Die,
	Attack,
	ShadowCut,
	Cardioclasis,
	BladeGust,
	Assassination,
	Extractheart,
	NeoBuff
};

class Player : public GameObject, IFollowing
{
public:
	Player(wstring name);
	virtual ~Player();


	virtual void Update(Matrix& V, Matrix &P);
	virtual void Render();


	bool IsGround() { return isGround; };
	void Focus(Vector2 * position, Vector2 * size) override;
	void SkillAttack(GameObject* target);

private:
	virtual void CreateAvataClip() = 0;
	virtual void CreateSkill() = 0;

	void Jump();
	void Move();
	void Skill();
	void AnimPlay();

protected:
	class SkillSet* skillset = NULL;

	PlayerState state = PlayerState::Idle;
	PlayerMotion motion = PlayerMotion::Idle;		// 현 상태

	bool isGround = true;
	float gravity = -0.5f;
	float jumpPower = 0.3f;
	float jumpVelocity = 0.0f;
	UINT jumpCount = 0;

	UINT level = 1;
	wstring name = L"";

	float moveSpeed = 1.0f;		// 이동속도
	float attackSpeed = 1.0f;	// 공격속도
	float skillSpeed = 0.0f;
	
	UINT atk = 1;				// 공격력



};