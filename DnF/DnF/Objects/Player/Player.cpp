#include "stdafx.h"
#include "Player.h"
#include "Skill/SkillSet.h"
#include "Skill/Skill_Active.h"
#include "PlayInterface.h"

Player::Player(wstring name)
	:GameObject()
{
	this->name = name;
	
	skillset = new SkillSet();
	collider->Tag(ColliderTag::Player);
}

Player::~Player()
{
	SafeDelete(skillset);
	SafeDelete(collider);
	SafeDelete(animation);
}

void Player::Update(Matrix & V, Matrix & P)
{
	__super::Update(V, P);

	if (hp <= 0)
	{
		motion = PlayerMotion::Down;
	}
	
	else if (isHold == true)
	{
		motion = PlayerMotion::Hit;
	}
	else
	{
		motion = PlayerMotion::Idle;

		// 현재 플레이어의 위치
		position = animation->Position();

		if (isMovable == true)
		{
			// 점프 : C
			Jump();

			// 이동 : 방향키
			Move();

			// 평타를 포함한 모든 스킬
			Skill();
		}
	}



	// 상태에 따른 애니메이션 실행
	AnimPlay();

	skillset->Update(V, P);

	collider->Update(V, P);
	collider->World(animation->GetSprite()->World());
	animation->Position(position);
	animation->Update(V, P);
	PlayInterface::Get()->UpdatePlayerHp(hp, hpMax);
}

void Player::Render()
{
	// Text
	{
		ImGui::Text("Player Motion : %d", (UINT)motion);
		ImGui::Text("Player Hp : %I64d ", hp);
		ImGui::Text("Player Position : %.2f, %.2f(%.2f)", Position().x, Position().y, Position().z);
	}

	animation->Render();
	skillset->Render();

	//collider->Render();
}



void Player::Jump()
{
	// 평지에 있을 때
	if (isGround == true)
	{
		position.z = position.y;
		jumpVelocity = 0.0f;
	}
	// 공중에 있을 때 : 낙하 및 점프 상태 유지
	else
	{
		isGround = false;
		state = PlayerState::Jump;
		jumpVelocity += gravity * Time::Delta();
		position.y += jumpVelocity;

		if (motion != PlayerMotion::JumpAttack)
			motion = PlayerMotion::JumpStart;

		if (position.y <= position.z)
		{
			position.y = position.z;
			isGround = true;
			jumpCount = 0;
			motion = PlayerMotion::Idle;
		}

	}

	// 점프 조건을 위한 상태 체크
	switch (state)
	{
	case PlayerState::Idle:
	case PlayerState::Walk:
	case PlayerState::Run:
	case PlayerState::Jump:
		break;
	default:
		return;
	}

	// 연속 점프 제한
	if (jumpCount > 1) return;

	// C키 입력으로 점프
	if (Key->Down('C')) 
	{
		// 착지지점(groundY) 저장
		if (jumpCount == 0)
			position.z = animation->Position().y;

		// 점프 시작속도 설정
		jumpVelocity = jumpPower;
		isGround = false;

		jumpCount++;
	}

}


void Player::Move()
{
	// 상태 체크
	switch (state)
	{
	case PlayerState::Idle:
	case PlayerState::Walk:
	case PlayerState::Run:
	case PlayerState::Jump:
		break;
	default:
		return;
	}

	// 좌우:X1 / 상하:X0.75 / 달리기:X2

	// 좌우 이동
	{
		if (Key->Press(VK_LEFT) == KEYPRESS::DOUBLE) {
			if (isGround == true)
				motion = PlayerMotion::Run;
			position.x -= 250 * moveSpeed * Time::Delta()* 2.0f;
			animation->RotationDegree(0, 180, 0);
			direction = Direction::Back;
		}
		else if (Key->Press(VK_LEFT) == KEYPRESS::SINGLE) {
			if (isGround == true)
				motion = PlayerMotion::Walk;
			position.x -= 250 * moveSpeed * Time::Delta();
			animation->RotationDegree(0, 180, 0);
			direction = Direction::Back;
		}

		if (Key->Press(VK_RIGHT) == KEYPRESS::DOUBLE) {
			if (isGround == true)
				motion = PlayerMotion::Run;
			position.x += 250 * moveSpeed * Time::Delta()* 2.0f;
			animation->RotationDegree(0, 0, 0);
			direction = Direction::Front;
		}
		else if (Key->Press(VK_RIGHT) == KEYPRESS::SINGLE) {
			if (isGround == true)
				motion = PlayerMotion::Walk;
			position.x += 250 * moveSpeed * Time::Delta();
			animation->RotationDegree(0, 0, 0);
			direction = Direction::Front;
		}
	}

	// 상하 이동
	{
		if (Key->Press(VK_UP) == KEYPRESS::DOUBLE) {
			if (isGround == false) {
				position.z += 250 * moveSpeed * Time::Delta()* 0.75f;
				position.y += 250 * moveSpeed * Time::Delta()* 0.75f;
			}
			else {
				motion = PlayerMotion::Run;
				position.y += 250 * moveSpeed * Time::Delta()* 1.25f;
			}

		}
		else if (Key->Press(VK_UP) == KEYPRESS::SINGLE) {
			if (isGround == false) {
				position.z += 250 * moveSpeed * Time::Delta()* 0.5f;
				position.y += 250 * moveSpeed * Time::Delta()* 0.5f;
			}
			else {
				if (motion != PlayerMotion::Run)
					motion = PlayerMotion::Walk;
				position.y += 250 * moveSpeed * Time::Delta()* 0.75f;
			}
		}

		if (Key->Press(VK_DOWN) == KEYPRESS::DOUBLE) {
			if (isGround == false) {
				position.z -= 250 * moveSpeed * Time::Delta()* 0.75f;
				position.y -= 250 * moveSpeed * Time::Delta()* 0.75f;
			}
			else {
				motion = PlayerMotion::Run;
				position.y -= 250 * moveSpeed * Time::Delta()* 1.25f;
			}
		}
		else if (Key->Press(VK_DOWN) == KEYPRESS::SINGLE) {
			if (isGround == false) {
				position.z -= 250 * moveSpeed * Time::Delta()* 0.5f;
				position.y -= 250 * moveSpeed * Time::Delta()* 0.5f;
			}
			else {
				if (motion != PlayerMotion::Run)
					motion = PlayerMotion::Walk;
				position.y -= 250 * moveSpeed * Time::Delta()* 0.75f;
			}
		}
	}
}


void Player::Skill()
{
	// 이펙트의 위치
	// xy:이펙트 중심 좌표 / z:플레이어 바닥부터의 y차이 값
	Vector3 effectPosition = position;
	effectPosition.y += animation->ScaledSize().y * 0.5f;

	// 키입력에 따라 스킬 실행
	skillset->SkillAction(state, attackSpeed, effectPosition, animation->Rotation());

	// 현재 진행중인 스킬
	Skill_Active* skill = skillset->CurrentActive();
	if (skill != NULL) 
	{

		// 현재 진행중인 스킬의 정보를 적용
		//	모션, 상태, 속도, 이동량
		if (animation->GetCurrentClip()->EndFrame() == false)
		{
			// 스킬의 모션과 상태를 가져옴
			SkillData data = skill->CurrentSkillData();
			motion = data.Motion;
			state = data.State;
			skillSpeed = data.Speed;
			position += data.Move * (float)direction;
			armorState = data.ArmorState_;
		}
		// 스킬에 해당하는 모션이 끝났을 때
		else
		{
			skill->ActionEnd();
			armorState = ArmorState::Normal;
		}

		// 스킬에 의한 케릭터 순간이동
		{
			SkillMoveTo to = skill->CurrentSkillMoveTo();
			// 이동 위치
			if (to.MoveTo != Vector3(0, 0, 0))
				position = to.MoveTo;

			// 이동 후 볼 방향
			if (to.Direction_ != Direction::None)
				animation->RotationDegree(0, 90 - ((float)to.Direction_ * 90), 0);
		}
			
	}


}


void Player::AnimPlay()
{

	float speed = 1.0f;
	if (skillSpeed == 0.0f)
	{
		// 상태에 따른 애니메이션 재생 속도 처리
		switch (motion)
		{
		case PlayerMotion::Idle:
		case PlayerMotion::Ready:
			state = PlayerState::Idle;
			break;
		case PlayerMotion::Hit:
			state = PlayerState::Hit;
			break;
		case PlayerMotion::Down:
			state = PlayerState::Die;
			break;
		case PlayerMotion::JumpStart:
		case PlayerMotion::JumpEnd:
			state = PlayerState::Jump;
			break;
		case PlayerMotion::Walk:		// 걷기 속도
			state = PlayerState::Walk;
			speed *= moveSpeed;
			speed += 0.25f;
			break;
		case PlayerMotion::Run:		// 뛰기 속도
			state = PlayerState::Run;
			speed *= moveSpeed;
			speed += 1;
			break;
		default:
			speed *= attackSpeed;
			speed += 1;

		}
	}
	else
	{
		speed = skillSpeed;
	}

	animation->Play((UINT)motion, speed);
	skillSpeed = 0.0f;
}


void Player::SkillAttack(GameObject* target)
{
	
	skillset->Attack(target, atk, Position(), bind(&PlayInterface::SetTarget, PlayInterface::Get(), placeholders::_1));
}




void Player::Focus(Vector2 * position, Vector2 * size)
{
	Vector2 pos;
	pos.x = animation->Position().x;
	pos.y = animation->Position().y;
	*position = pos;
	*size = focusOffset;
}
