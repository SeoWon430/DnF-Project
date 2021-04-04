#pragma once

class DirectWrite
{
private:
	DirectWrite();
	~DirectWrite();

public:
	static void Create();
	static void Delete();

	static DirectWrite* Get();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }


	static void CreateBackBuffer();
	static void DeleteBackBuffer();

	static void RenderText(wstring& text, RECT& rect);

private:
	static DirectWrite* instance;
	ID2D1Factory1* factory;	// 2D용 Device 생성
	static IDWriteFactory* writeFactory;	// DirectWrite기능 생성

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;	

	static ID2D1Bitmap1* bitmap;	// 백버퍼 역할 (D3D의 Texture2D역할)
	static IDXGISurface* surface;	// RTV 역할
	
	static ID2D1SolidColorBrush* brush;	// 텍스트 색상
	static IDWriteTextFormat* format;	// 폰트


};