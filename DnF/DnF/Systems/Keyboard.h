#pragma once

#define KEYMAX 256	// 키보드 총 갯수(넉넉하게)

enum class KEYPRESS { NONE, SINGLE, DOUBLE };

// 입력은 한곳에서 해야 하기에 주로 싱글톤으로 만듦
class Keyboard 
{
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);		// key : 누른키의 아스키코드
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
