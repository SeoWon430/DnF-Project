#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

	// 스크린샷
	void SaveTexture(wstring file);

	void Set();
	static void SetMainRender();

private:
	UINT width, height;
	DXGI_FORMAT format;	// 한 픽셀을 표현하는 용량

	ID3D11Texture2D* backBuffer;	// 픽셀의 색상 정보, 텍스쳐
	ID3D11RenderTargetView* rtv;	// 화면에 보일 실제 그림
	ID3D11ShaderResourceView* srv;	// backBuffer정보를 shader로 보냄

};

/*
Com Interface : 구조체(desc)를 만들어 함수(Create)에 요청하면 
	주소를 리턴하여 이 값으로 대상에 접근
	ex) device, device context, swap chain, buffer 등

RTV : RS에서 만들어지고 OM에서 세팅되어 다 그려짐

*/