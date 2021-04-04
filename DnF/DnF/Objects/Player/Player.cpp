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

		// ���� �÷��̾��� ��ġ
		position = animation->Position();

		if (isMovable == true)
		{
			// ���� : C
			Jump();

			// �̵� : ����Ű
			Move();

			// ��Ÿ�� ������ ��� ��ų
			Skill();
		}
	}



	// ���¿� ���� �ִϸ��̼� ����
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
	// ������ ���� ��
	if (isGround == true)
	{
		position.z = position.y;
		jumpVelocity = 0.0f;
	}
	// ���߿� ���� �� : ���� �� ���� ���� ����
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

	// ���� ������ ���� ���� üũ
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

	// ���� ���� ����
	if (jumpCount > 1) return;

	// CŰ �Է����� ����
	if (Key->Down('C')) 
	{
		// ��������(groundY) ����
		if (jumpCount == 0)
			position.z = animation->Position().y;

		// ���� ���ۼӵ� ����
		jumpVelocity = jumpPower;
		isGround = false;

		jumpCount++;
	}

}


void Player::Move()
{
	// ���� üũ
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

	// �¿�:X1 / ����:X0.75 / �޸���:X2

	// �¿� �̵�
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

	// ���� �̵�
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
	// ����Ʈ�� ��ġ
	// xy:����Ʈ �߽� ��ǥ / z:�÷��̾� �ٴں����� y���� ��
	Vector3 effectPosition = position;
	effectPosition.y += animation->ScaledSize().y * 0.5f;

	// Ű�Է¿� ���� ��ų ����
	skillset->SkillAction(state, attackSpeed, effectPosition, animation->Rotation());

	// ���� �������� ��ų
	Skill_Active* skill = skillset->CurrentActive();
	if (skill != NULL) 
	{

		// ���� �������� ��ų�� ������ ����
		//	���, ����, �ӵ�, �̵���
		if (animation->GetCurrentClip()->EndFrame() == false)
		{
			// ��ų�� ��ǰ� ���¸� ������
			SkillData data = skill->CurrentSkillData();
			motion = data.Motion;
			state = data.State;
			skillSpeed = data.Speed;
			position += data.Move * (float)direction;
			armorState = data.ArmorState_;
		}
		// ��ų�� �ش��ϴ� ����� ������ ��
		else
		{
			skill->ActionEnd();
			armorState = ArmorState::Normal;
		}

		// ��ų�� ���� �ɸ��� �����̵�
		{
			SkillMoveTo to = skill->CurrentSkillMoveTo();
			// �̵� ��ġ
			if (to.MoveTo != Vector3(0, 0, 0))
				position = to.MoveTo;

			// �̵� �� �� ����
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
		// ���¿� ���� �ִϸ��̼� ��� �ӵ� ó��
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
		case PlayerMotion::Walk:		// �ȱ� �ӵ�
			state = PlayerState::Walk;
			speed *= moveSpeed;
			speed += 0.25f;
			break;
		case PlayerMotion::Run:		// �ٱ� �ӵ�
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
