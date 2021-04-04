#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
	:doublePressTime(0.15f)
{
	for (int i = 0; i < KEYMAX; i++) {
		up.set(i, false);
		down.set(i, false);
		prevPress[i] = 0.0f;
	}
}

Keyboard::~Keyboard()
{
}



bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) * 0x8000) {
		if (down[key] == false) {
			down.set(key, true);
			return true;
		}
	}
	else {
		down.set(key, false);
	}
	return false;
}



bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) * 0x8000) {
		up.set(key, true);
	}
	else {
		if (up[key] == true) {
			up.set(key, false);
			return true;
		}
	}
	return false;
}



KEYPRESS Keyboard::Press(int key)
{

	if (Up(key) == true)
		prevPress[key] = doublePressTime;

	if (GetAsyncKeyState(key) * 0x8000) {
		if (prevPress[key] > 0) {
			prevPress[key] = doublePressTime;
			return KEYPRESS::DOUBLE;
		}
		return KEYPRESS::SINGLE;
	}

	return KEYPRESS::NONE;
}



bool Keyboard::Toggle(int key)
{
	if (GetAsyncKeyState(key) * 0x0001)
		return true;

	return false;
}


void Keyboard::Update()
{
	for (int i = 0; i < KEYMAX; i++) {
		if(prevPress[i] > 0.0f)
			prevPress[i]-= Time::Delta();
	}
}

void Keyboard::Render()
{
	//ImGui::Text("UP : %.2f", prevPress[VK_UP]);
	//ImGui::Text("DO : %.2f", prevPress[VK_DOWN]);
	//ImGui::Text("LE : %.2f", prevPress[VK_LEFT]);
	//ImGui::Text("RI : %.2f", prevPress[VK_RIGHT]);
}
