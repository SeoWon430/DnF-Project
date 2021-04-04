#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
{
	this->width = (width < 1) ? WIDTH : width;
	this->height = (height < 1) ? HEIGHT : height;

	// backBuffer : 도화지에 그릴 픽셀 정보
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = this->width;
		textureDesc.Height = this->height;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		// D3D11_BIND_RENDER_TARGET : 화면에 띄우는 용도
		// D3D11_BIND_SHADER_RESOURCE : 셰이더에 보내는 용도
		textureDesc.MipLevels = 1;			// 원근에 따른 TEXTURE 축소, 확대
		textureDesc.SampleDesc.Count = 1;	// 불러올때 샘플링수

		assert(SUCCEEDED(Device->CreateTexture2D(&textureDesc, NULL, &backBuffer)));
		// 만든 결과가 backBuffer에 저장됨

		/* Texture
			Shader의 Texture는 이미지와 달리 버퍼의 의미

			Texture1D Array 는 Texture2D 와 유사
			- Texture1D Array는 각 행의 사이즈 가 달라질수 있음
			- Texture2D는 각 행의 사이즈가 동일

			Shader(GPU)는 가변길이가 아닌 고정길이를 사용
				=> 용량 낭비가 있어도 성능을 우선시
		*/

		/* Resource : 버퍼 텍스쳐
			CPU 메모리에서 만든 자원이 SubRsource
			GPU 메모리에 복사된 지원이 Rsource

		*/
	}


	// rtv : 실제 화면에 나갈 도화지
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = format;	// backBuffer와 맞춤 (다르면 업스케일, 다운스케일)
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		assert(SUCCEEDED(Device->CreateRenderTargetView(backBuffer, &rtvDesc, &rtv)));
	}


	// srv : 셰이더에 버퍼를 보내기 위한 정보
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
	// backBuffer에 있는 이미지를 file경로에 png로 저장
	D3DX11SaveTextureToFile(DeviceContext, backBuffer, D3DX11_IFF_PNG, file.c_str());
}

void RenderTarget::Set()
{
	// rtv를 OM에 SET => 화면에 나갈수 있게 됨
	DeviceContext->OMSetRenderTargets(1, &rtv, NULL);

	// rtv는 이후 다시 그려야 하기에 초기화
	DeviceContext->ClearRenderTargetView(rtv, Color(0.2f, 0.2f, 0.2f, 0.3f));
}

// Main에서 사용하고 있는 RTV에 대한 set
void RenderTarget::SetMainRender()
{
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	DeviceContext->ClearRenderTargetView(RTV, Color(0.2f, 0.2f, 0.2f, 1.0f));
}
