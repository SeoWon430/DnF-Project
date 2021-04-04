#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
{
	this->width = (width < 1) ? WIDTH : width;
	this->height = (height < 1) ? HEIGHT : height;

	// backBuffer : ��ȭ���� �׸� �ȼ� ����
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = this->width;
		textureDesc.Height = this->height;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		// D3D11_BIND_RENDER_TARGET : ȭ�鿡 ���� �뵵
		// D3D11_BIND_SHADER_RESOURCE : ���̴��� ������ �뵵
		textureDesc.MipLevels = 1;			// ���ٿ� ���� TEXTURE ���, Ȯ��
		textureDesc.SampleDesc.Count = 1;	// �ҷ��ö� ���ø���

		assert(SUCCEEDED(Device->CreateTexture2D(&textureDesc, NULL, &backBuffer)));
		// ���� ����� backBuffer�� �����

		/* Texture
			Shader�� Texture�� �̹����� �޸� ������ �ǹ�

			Texture1D Array �� Texture2D �� ����
			- Texture1D Array�� �� ���� ������ �� �޶����� ����
			- Texture2D�� �� ���� ����� ����

			Shader(GPU)�� �������̰� �ƴ� �������̸� ���
				=> �뷮 ���� �־ ������ �켱��
		*/

		/* Resource : ���� �ؽ���
			CPU �޸𸮿��� ���� �ڿ��� SubRsource
			GPU �޸𸮿� ����� ������ Rsource

		*/
	}


	// rtv : ���� ȭ�鿡 ���� ��ȭ��
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = format;	// backBuffer�� ���� (�ٸ��� ��������, �ٿ����)
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		assert(SUCCEEDED(Device->CreateRenderTargetView(backBuffer, &rtvDesc, &rtv)));
	}


	// srv : ���̴��� ���۸� ������ ���� ����
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		assert(SUCCEEDED(Device->CreateShaderResourceView(backBuffer, &srvDesc, &srv)));
	}

}

RenderTarget::~RenderTarget()
{
	SafeRelease(srv);
	SafeRelease(rtv);
	SafeRelease(backBuffer);
}

void RenderTarget::SaveTexture(wstring file)
{
	// backBuffer�� �ִ� �̹����� file��ο� png�� ����
	D3DX11SaveTextureToFile(DeviceContext, backBuffer, D3DX11_IFF_PNG, file.c_str());
}

void RenderTarget::Set()
{
	// rtv�� OM�� SET => ȭ�鿡 ������ �ְ� ��
	DeviceContext->OMSetRenderTargets(1, &rtv, NULL);

	// rtv�� ���� �ٽ� �׷��� �ϱ⿡ �ʱ�ȭ
	DeviceContext->ClearRenderTargetView(rtv, Color(0.2f, 0.2f, 0.2f, 0.3f));
}

// Main���� ����ϰ� �ִ� RTV�� ���� set
void RenderTarget::SetMainRender()
{
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	DeviceContext->ClearRenderTargetView(RTV, Color(0.2f, 0.2f, 0.2f, 1.0f));
}
