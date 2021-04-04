#pragma once

class PatternSet
{
public:
	PatternSet();
	~PatternSet();

	void AddPattern(class Pattern* pattern);

	void Update(Matrix& V, Matrix &P);
	void Render();

	void Initialize(MonsterState* state);

	void Attack(GameObject* target, UINT atk, Vector3& monsterPosition);
	float PatternAction(float speed, Vector3 poistion, Vector3 rotation);

	const vector<class Pattern*> Patterns() { return patterns; }

	Pattern* CurrentActive();

private:
	Vector3 monsterPosition;
	int currentPattern = -1;
	vector<class Pattern*> patterns;

};