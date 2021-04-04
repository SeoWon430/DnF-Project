#include "stdafx.h"
#include "DirectWrite.h"

DirectWrite* DirectWrite::instance = NULL;
IDWriteFactory* DirectWrite::writeFactory = NULL;
ID2D1DeviceContext* DirectWrite::deviceContext = NULL;
ID2D1Bitmap1* DirectWrite::bitmap = NULL;
IDXGISurface* DirectWrite::surface = NULL;
ID2D1SolidColorBrush* DirectWrite::brush = NULL;
IDWriteTextFormat* DirectWrite::format = NULL;



void DirectWrite::Create()
{
	assert(instance == NULL);

	instance = new DirectWrite();
}

void DirectWrite::Delete()
{
	SafeDelete(instance);
}

DirectWrite * DirectWrite::Get()
{
	return instance;
}

DirectWrite::DirectWrite()
{
	HRESULT result;

	result = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,	// direct�� 2d�� 3d�� ���� ��
		__uuidof(IDWriteFactory),	// ��ġ�� ������
		(IUnknown**)&writeFactory	// ��� ���丮 ����
	);
	assert(SUCCEEDED(result));


	// create factory
	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	//									��Ƽ ������ ����
	assert(SUCCEEDED(result));


	// create device
	IDXGIDevice* dxgiDevice;
	result = Device->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(result));
	result = factory->CreateDevice(dxgiDevice, &device);
	assert(SUCCEEDED(result));


	// create devicecontext
	result = device->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,	// ��Ƽ ������ ����
		&deviceContext
	);
	assert(SUCCEEDED(result));

	CreateBackBuffer();


}

DirectWrite::~DirectWrite()
{
	DeleteBackBuffer();
}

void DirectWrite::CreateBackBuffer()
{
	HRESULT result;

	// 2d�� ������ ����� ����
	result = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	assert(SUCCEEDED(result));

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;	// ���� ��� ����
	bp.dpiX = 96;	// Ŭ���� Ŭ�ڰ� �����ϰ� ����
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = NULL;
	result = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
	assert(SUCCEEDED(result));

	deviceContext->SetTarget(bitmap);



	result = deviceContext->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &brush);
	assert(SUCCEEDED(result));


	writeFactory->CreateTextFormat(
		L"����ü",						// �۲� ����
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,	// ����ü ����(���̺���)
		DWRITE_FONT_STYLE_NORMAL,		// ���ڸ�ü ����(�⺻)
		DWRITE_FONT_STRETCH_NORMAL,		// ��Ʈ��ġ(�⺻)
		15,
		L"ko",				// ��� (�ѱ���)
		&format
	);
}

void DirectWrite::DeleteBackBuffer()
{
	deviceContext->SetTarget(NULL);
	SafeRelease(brush);
	SafeRelease(format);
	SafeRelease(bitmap);
	SafeRelease(surface);

}

void DirectWrite::RenderText(wstring & text, RECT & rect)
{
	D2D1_RECT_F rt;
	rt.left = (float)rect.left;
	rt.top = (float)rect.top;
	rt.right = (float)rect.right;
	rt.bottom = (float)rect.bottom;
	deviceContext->DrawTextW(text.c_str(), text.length(), format, &rt, brush);
}
