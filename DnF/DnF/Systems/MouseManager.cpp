#include "stdafx.h"
#include "MouseManager.h"

MouseManager::MouseManager(HWND hwnd)
	:handle(hwnd)
	,position(0, 0)
	,wheelValue(0)
	,wheelPrevValue(0)
	,wheelMoveValue(0)
{
	ZeroMemory(buttonStatus, sizeof(BYTE)*8);
	ZeroMemory(buttonPrevStatus, sizeof(BYTE)*8);
	ZeroMemory(buttonMap, sizeof(Button)*8);

	ZeroMemory(startDoubleClickTime, sizeof(DWORD)*8);
	ZeroMemory(buttonCount, sizeof(int)*8);

	doubleClickTime = GetDoubleClickTime();	// 윈도우에서 설정한 더블클릭 시간
}

MouseManager::~MouseManager()
{
}

void MouseManager::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_MOUSEMOVE) {
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}
	if (msg == WM_MOUSEWHEEL) {
		short temp = (short)HIWORD(wParam);
		wheelPrevValue = wheelValue;
		wheelValue += (float)temp;
	}
}

void MouseManager::Update()
{
	memcpy(buttonPrevStatus, buttonStatus, sizeof(BYTE) * 8);
	ZeroMemory(buttonStatus, sizeof(BYTE) * 8);
	ZeroMemory(buttonMap, sizeof(Button) * 8);

	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) * 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) * 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) * 0x8000 ? 1 : 0;

	// 클릭
	for (UINT i = 0; i < 8; i++) 
	{
		BYTE prevStatus = buttonPrevStatus[i];
		BYTE currentStatus = buttonStatus[i];

		if (prevStatus == 0 && currentStatus == 1)
			buttonMap[i] = Button::Down;
		else if (prevStatus == 1 && currentStatus == 0)
			buttonMap[i] = Button::Up;
		else if (prevStatus == 1 && currentStatus == 1)
			buttonMap[i] = Button::Press;
		else
			buttonMap[i] = Button::None;
	}

	// 더블클릭
	UINT currentTime = GetTickCount();	// 현재까지의 윈도우 부팅 시간

	for (UINT i = 0; i < 8; i++) 
	{
		if (buttonMap[i] == Button::Down) 
		{
			if (buttonCount[i] == 1) {
				if (currentTime - startDoubleClickTime[i] >= doubleClickTime) {
					buttonCount[i] = 0;
				}
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1) {
				startDoubleClickTime[i] = currentTime;
			}

			

		}
		else if (buttonMap[i] == Button::Up) {
			if (buttonCount[i] == 1) {
				if (currentTime - startDoubleClickTime[i] >= doubleClickTime) {
					buttonCount[i] = 0;
				}
			}
			else if (buttonCount[i] == 2) {
				if (currentTime - startDoubleClickTime[i] < doubleClickTime) {
					buttonMap[i] = Button::DoubleClick;
				}
				buttonCount[i] = 0;
			}
		}
	}


	// 휠
	wheelPrevValue = wheelValue;
	wheelMoveValue = wheelValue - wheelPrevValue;

}

Vector2 MouseManager::ScreenPosition(Vector2 cameraPosition)
{
	// 화면 크기 고려한 마우스 좌표 변경
	Vector2 mousePosition = Mouse->Position();

	mousePosition.x = mousePosition.x - (float)WIDTH * 0.5f;
	mousePosition.y = (mousePosition.y - (float)HEIGHT * 0.5f) * -1.0f;

	// 카메라 위치 고려한 마우스 좌표 변경
	mousePosition += cameraPosition;
	return mousePosition;
}
