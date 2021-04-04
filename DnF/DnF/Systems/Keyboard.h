#pragma once

#define KEYMAX 256	// Ű���� �� ����(�˳��ϰ�)

enum class KEYPRESS { NONE, SINGLE, DOUBLE };

// �Է��� �Ѱ����� �ؾ� �ϱ⿡ �ַ� �̱������� ����
class Keyboard 
{
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);		// key : ����Ű�� �ƽ�Ű�ڵ�
	bool Up(int key);
	KEYPRESS Press(int key);
	bool Toggle(int key);

	void Update();
	void Render();

private:
	bitset<KEYMAX> up;		// == bool up[KEYMAX], vector<bool> up
	bitset<KEYMAX> down;

	float doublePressTime;
	float prevPress[KEYMAX];

	//queue<int> command;

};
