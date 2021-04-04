#include "stdafx.h"
#include "PatternSet.h"
#include "Pattern.h"

PatternSet::PatternSet()
{
}

PatternSet::~PatternSet()
{
	for (auto pattern : patterns)
		SafeDelete(pattern);
}

void PatternSet::AddPattern(Pattern * pattern)
{
	patterns.push_back(pattern);
}

void PatternSet::Update(Matrix & V, Matrix & P)
{
	for (auto pattern : patterns)
	{
		pattern->Update(V, P);
	}
}

void PatternSet::Render()
{
	for (auto pattern : patterns) {
		if (pattern->IsAction() == true
			|| pattern->IsEnd() == false)
			pattern->Render();
	}
}

void PatternSet::Initialize(MonsterState * state)
{
	for (auto pattern : patterns)
	{
		pattern->Initialize(state);
	}
}

void PatternSet::Attack(GameObject * target, UINT atk, Vector3 & monsterPosition)
{
	for (auto pattern : patterns)
	{

		if (pattern->IsAction() == true)
		{

			function<void(float)> attack = [=](float damage) {
				target->Attacked((INT64)(atk * damage), 1.0f);
			};
			pattern->Attack(target, atk, monsterPosition, attack);

		}
	}
}

float PatternSet::PatternAction(float speed, Vector3 poistion, Vector3 rotation)
{
	// 패턴 쓰고 난 후 다음 패턴까지의 강제 딜레이
	float delay = 1.0f;

	monsterPosition = poistion;
	// 스킬 실행
	for (UINT i = 0; i < patterns.size(); i++)
	{
		bool a = patterns[i]->IsEnable();
		bool b = patterns[i]->IsActive();
		if (a&&b)
		{
			currentPattern = i;
			// 스킬 실행 시작
			patterns[i]->Rotation(rotation);
			patterns[i]->Position(poistion);
			//vector<PlayerMotion> mostions = actives[i]->Motions();
			//SetMotion(&mostions[0], mostions.size());
			patterns[i]->SkillStart(speed);
			delay = patterns[i]->Delay();
			break;
		}
		
	}
	return delay;
}

Pattern * PatternSet::CurrentActive()
{
	if (currentPattern == -1)
		return NULL;
	if (patterns[currentPattern]->IsAction() == true)
		return patterns[currentPattern];
	return NULL;
}
