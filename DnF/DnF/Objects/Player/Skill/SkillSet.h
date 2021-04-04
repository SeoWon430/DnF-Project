#pragma once

class SkillSet
{
public:
	SkillSet();
	~SkillSet();

	void AddActive(class Skill_Active* skill);
	void AddPassive(class Skill_Passive* skill);

	void Update(Matrix& V, Matrix &P);
	void Render();
	void ActionPassive(UINT num);

	void Attack(GameObject* target, UINT atk, Vector3& playerPosition, function<void(GameObject*)> interfaceTarget);
	void SkillAction(PlayerState state, float speed, Vector3 poistion, Vector3 rotation);

	const vector<class Skill_Active*> Actives() { return actives; }

	Skill_Active* CurrentActive();

private:
	int currentActive;
	Vector3 playerPosition;
	vector<class Skill_Active*> actives;
	vector<class Skill_Passive*> passives;
};