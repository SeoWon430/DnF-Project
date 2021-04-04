#pragma once


// Function (Device.cpp)
void InitWindow(HINSTANCE hInstance, int nCmdShow);	// �ʱ�ȭ, ������
void InitDirect3D(HINSTANCE hInstance);
void Destroy();		//  �Ҹ���
WPARAM Running();	// main update
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Function (Scene.cpp -> Main.cpp�� ����)
void InitScene();
void DestroyScene();
void Update();
void Render();


// ȭ�� resize�� ��
// BackBuffer(& RTV), Viewport, Projection�� ũ�⵵ �缳��
//		Projection�� �ǽð����� Scene.cpp�� Update()���� �ڵ����� �ϰ� ��
void CreateBackBuffer();	// resize�Ҷ�, �ʱ⼼�� �� ȣ���ϰ� ��
void DeleteBackBuffer();
