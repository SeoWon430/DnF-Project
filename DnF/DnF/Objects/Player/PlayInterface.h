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
	Camera* camera;					// 카메라 : Scene이 가진 Main Camera
	Sprite* interfaceFrame;			// 플레이어 인터페이스 : 케릭터의 hp,mp,스킬 표시

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

	GameObject* target = nullptr;	// 타격한 몬스터
	Sprite* targetInterface;		// target의 hp를 표시할 인터페이스
	Vector2 targetInterfaceOffset;	// targetInterface의 위치
	float targetTime = 0;			// 인터페이스 유지 시간 (일정 이상 타격 없으면 해제)
	int ratioHpTarget = 100;

	Sprite* targetHpAll;					// target의 체력 비율 (0~100%로 표시)
	ID3DX11EffectScalarVariable* fillHpAll;	// 비율을 셰이더로 전달

	Sprite* targetHp[5];					// target의 체력바 (1줄당 10억, 5가지 색상)
	ID3DX11EffectScalarVariable* fillHp[5];	// 각 체력바 당 비율을 셰이더로 전달
	UINT hpCount = 0;			// 현재 체력 줄수
	UINT currentLine = 0;		// 현재 표시 할 체력바 번호(0~4)
	UINT nextLine = 1;			// 다음 표시 할 체력바 번호(0~4) = currentLine+1


};