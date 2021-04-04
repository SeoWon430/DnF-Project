#include "stdafx.h"
#include "Device.h"


int APIENTRY WinMain
(	HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	InitWindow(hInstance, nCmdShow);
	InitDirect3D(hInstance);

	Running();
	Destroy();

	return 0;
}


// 전역 변수 초기화
UINT WIDTH = 1408;
UINT HEIGHT = 792;

HWND Hwnd = NULL;
wstring Title = L"Dungeon&Fighter Demo";

IDXGISwapChain* SwapChain = NULL;
ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
ID3D11RenderTargetView* RTV = NULL;

Keyboard* Key = NULL;
MouseManager* Mouse = NULL;
SoundManager* Audio = NULL;


void InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register Window Class
	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)WHITE_BRUSH;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = Title.c_str();
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		WORD check = RegisterClassEx(&wc);

		assert(check != NULL);
	}


	// Create HWND, Show Window
	{
		Hwnd = CreateWindowEx(
			NULL, 
			Title.c_str(), 
			Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,	
			CW_USEDEFAULT,
			WIDTH,
			HEIGHT,
			NULL,
			NULL,
			hInstance,
			NULL
		);
		assert(Hwnd != NULL);


	}


	// 윈도우 창 가운데에 뜨게 하기
	{
		RECT rect = { 0, 0, (LONG)WIDTH, (LONG)HEIGHT };
		UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)WIDTH) / 2;
		UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)HEIGHT) / 2;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow(Hwnd, centerX, centerY, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	}
	

	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
}



void InitDirect3D(HINSTANCE hInstance)
{

	// Swapchain
	DXGI_MODE_DESC bufferDesc;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = WIDTH;
		bufferDesc.Height = HEIGHT;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferDesc = bufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.OutputWindow = Hwnd;
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
						

	vector<D3D_FEATURE_LEVEL> featured_level = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};


	HRESULT result = D3D11CreateDeviceAndSwapChain
	(	NULL,
		D3D_DRIVER_TYPE_HARDWARE,	
		NULL,		
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,	
		featured_level.data(),		
		featured_level.size(),		
		D3D11_SDK_VERSION,			
		&swapDesc,			
		&SwapChain,			
		&Device,			
		NULL, 
		&DeviceContext		
	);
	assert(SUCCEEDED(result));	



	CreateBackBuffer();
}



void Destroy() 
{
	DeleteBackBuffer();
	SafeRelease(DeviceContext);
	SafeRelease(SwapChain);
	SafeRelease(Device);
}



WPARAM Running() 
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 런타임 디버깅용 GUI
	ImGui::Create(Hwnd,Device, DeviceContext);	
	ImGui::StyleColorsDark();

	DirectWrite::Create();
	Time::Create();
	Time::Get()->Start();

	Key = new Keyboard();
	Mouse = new MouseManager(Hwnd);
	Audio = new SoundManager();

	InitScene();

	while (true) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Time::Get()->Update();
			Mouse->Update();
			Key->Update();
			Audio->Update();
			ImGui::Update();

			Update();	
			Render();
		}
	}

	DestroyScene();

	SafeDelete(Audio);
	SafeDelete(Mouse);
	SafeDelete(Key);

	Time::Delete();
	DirectWrite::Delete();
	ImGui::Delete();


	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	if (ImGui::WndProc(hwnd, msg, wParam, lParam))
		return true;

	if (Mouse != NULL)
		Mouse->WndProc(msg, wParam, lParam);


	switch (msg)
	{
		/*
		// Window Resize
		case WM_SIZE:
		{
			if (Device == NULL)
				break;

			ImGui::Invalidate();

			int W = LOWORD(lParam);
			int H = HIWORD(lParam);
			WIDTH = W;
			HEIGHT = H;

			DeleteBackBuffer();
			DirectWrite::DeleteBackBuffer();

			HRESULT result = SwapChain->ResizeBuffers(0, WIDTH, HEIGHT, DXGI_FORMAT_UNKNOWN, 0);
			assert(SUCCEEDED(result));

			DirectWrite::CreateBackBuffer();
			CreateBackBuffer();

			ImGui::Validate();
		}
			break;
*/
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



void CreateBackBuffer() 
{
	HRESULT result;

	// Create Backbuffer
	{
		ID3D11Texture2D* backBuffer = NULL;	// 최초의 백버퍼

		result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		assert(SUCCEEDED(result));

		result = Device->CreateRenderTargetView(backBuffer, NULL, &RTV);	// RTV설정
		assert(SUCCEEDED(result));

		SafeRelease(backBuffer);

		DeviceContext->OMSetRenderTargets(1, &RTV, NULL);

		// RTV를 여러장 사용하여 나중에 합침 : 디퍼드 렌더링
		//	케릭터, 조명, 건물, 사물, 노멀맵, 스펙큘러 등을 각각 만든 후 합침
		//	한번에 계산해서 픽셀을 처리하는것이 아닌 각각 따로 하여 연산량을 줄임
	}
	

	// Create Viewport
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;	
		viewport.TopLeftY = 0;	
		viewport.Width = (FLOAT) WIDTH;
		viewport.Height = (FLOAT) HEIGHT;

		DeviceContext->RSSetViewports(1, &viewport);
	}
}



void DeleteBackBuffer() 
{
	SafeRelease(RTV);
}