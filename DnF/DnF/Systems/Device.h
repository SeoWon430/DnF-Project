#pragma once


// Function (Device.cpp)
void InitWindow(HINSTANCE hInstance, int nCmdShow);	// 초기화, 생성자
void InitDirect3D(HINSTANCE hInstance);
void Destroy();		//  소멸자
WPARAM Running();	// main update
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Function (Scene.cpp -> Main.cpp로 변경)
void InitScene();
void DestroyScene();
void Update();
void Render();


// 화면 resize할 때
// BackBuffer(& RTV), Viewport, Projection의 크기도 재설정
//		Projection는 실시간으로 Scene.cpp의 Update()에서 자동으로 하게 됨
void CreateBackBuffer();	// resize할때, 초기세팅 시 호출하게 됨
void DeleteBackBuffer();
