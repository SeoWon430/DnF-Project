#pragma once

class MouseManager
{
public:
	MouseManager(HWND hwnd);
	~MouseManager();

	void WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void Update();

	Vector2 Position() { return position; }
	Vector2 ScreenPosition(Vector2 cameraPosition);
	bool Down(UINT button) { return buttonMap[button] == Button::Down; }
	bool Up(UINT button) { return buttonMap[button] == Button::Up; }
	bool Press(UINT button) { return buttonMap[button] == Button::Press; }
	bool DoubleClick(UINT button) { return buttonMap[button] == Button::DoubleClick; }
	float Wheel() { return wheelMoveValue; }


private:
	enum class Button { None, Down, Up, Press, DoubleClick };

private:
	HWND handle;	

	Vector2 position;
	BYTE buttonStatus[8];		
	BYTE buttonPrevStatus[8];
	Button buttonMap[8];

	float wheelValue;
	float wheelPrevValue;
	float wheelMoveValue;

	DWORD doubleClickTime;	
	DWORD startDoubleClickTime[8];
	int buttonCount[8];

};