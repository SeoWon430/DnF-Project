#pragma once

#define PerLine 1000000000
#define KeyCount 14

class PlayInterface
{
private:
	PlayInterface();
	~PlayInterface();

public:
	static void Create();
	static void Delete();

	static PlayInterface* Get();

	void Update(Matrix& V, Matrix &P);
	void Render();

	void SetCamera(Camera* camera);
	void SetSkillset(class SkillSet* skillset);

	void SetTarget(GameObject* target);

	void UpdatePlayerHp(INT64 hp, INT64 hpMax);
	void UpdatePlayerMp(INT64 mp, INT64 mpMax);

	Vector2 ScreenCenter() { return camera->Position(); }


private:
	void PlayerUpdate(Matrix& V, Matrix &P);
	void PlayerRender();
	void TargetUpdate();



private:
	static PlayInterface* instance;
	Camera* camera;					// ī�޶� : Scene�� ���� Main Camera
	Sprite* interfaceFrame;			// �÷��̾� �������̽� : �ɸ����� hp,mp,��ų ǥ��

	Sprite* hpBar;
	int ratioHp = 100;
	Sprite* mpBar;
	int ratioMp = 100;
	Vector2 positionHp = Vector2();
	ID3DX11EffectScalarVariable* fillHpBar;
	Vector2 positionMp = Vector2();
	ID3DX11EffectScalarVariable* fillMpBar;

	int keys[KeyCount] = { 0, };
	class Skill_Active* skills[KeyCount] = { NULL, };
	Vector2 positionSkill[KeyCount] = { Vector2(), };
	ID3DX11EffectScalarVariable* skillCoolTime[KeyCount] = { NULL, };

	GameObject* target = nullptr;	// Ÿ���� ����
	Sprite* targetInterface;		// target�� hp�� ǥ���� �������̽�
	Vector2 targetInterfaceOffset;	// targetInterface�� ��ġ
	float targetTime = 0;			// �������̽� ���� �ð� (���� �̻� Ÿ�� ������ ����)
	int ratioHpTarget = 100;

	Sprite* targetHpAll;					// target�� ü�� ���� (0~100%�� ǥ��)
	ID3DX11EffectScalarVariable* fillHpAll;	// ������ ���̴��� ����

	Sprite* targetHp[5];					// target�� ü�¹� (1�ٴ� 10��, 5���� ����)
	ID3DX11EffectScalarVariable* fillHp[5];	// �� ü�¹� �� ������ ���̴��� ����
	UINT hpCount = 0;			// ���� ü�� �ټ�
	UINT currentLine = 0;		// ���� ǥ�� �� ü�¹� ��ȣ(0~4)
	UINT nextLine = 1;			// ���� ǥ�� �� ü�¹� ��ȣ(0~4) = currentLine+1


};