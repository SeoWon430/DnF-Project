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
	ID2D1Factory1* factory;	// 2D�� Device ����
	static IDWriteFactory* writeFactory;	// DirectWrite��� ����

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;	

	static ID2D1Bitmap1* bitmap;	// ����� ���� (D3D�� Texture2D����)
	static IDXGISurface* surface;	// RTV ����
	
	static ID2D1SolidColorBrush* brush;	// �ؽ�Ʈ ����
	static IDWriteTextFormat* format;	// ��Ʈ


};