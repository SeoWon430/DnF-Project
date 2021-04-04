#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

	// ��ũ����
	void SaveTexture(wstring file);

	void Set();
	static void SetMainRender();

private:
	UINT width, height;
	DXGI_FORMAT format;	// �� �ȼ��� ǥ���ϴ� �뷮

	ID3D11Texture2D* backBuffer;	// �ȼ��� ���� ����, �ؽ���
	ID3D11RenderTargetView* rtv;	// ȭ�鿡 ���� ���� �׸�
	ID3D11ShaderResourceView* srv;	// backBuffer������ shader�� ����

};

/*
Com Interface : ����ü(desc)�� ����� �Լ�(Create)�� ��û�ϸ� 
	�ּҸ� �����Ͽ� �� ������ ��� ����
	ex) device, device context, swap chain, buffer ��

RTV : RS���� ��������� OM���� ���õǾ� �� �׷���

*/