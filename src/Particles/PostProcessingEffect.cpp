#include "PostProcessingEffect.h"

#include <iostream>

#include <engine/D3DObjects/Device.h>

PostProcessingEffect::PostProcessingEffect():ppEffect{ L"shaders/PostProcessing/Dissipate.hlsl" }, renderScreen{}
{
	//Description for the textures in the render targets
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = 1024;
	texDesc.Height = 1024;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ComPtr<ID3D11Texture2D> textures[2];

	//Error would trigger in both
	HRESULT err = Device::Instance()->getDevice()->CreateTexture2D(&texDesc, nullptr, &textures[0]);
	err = Device::Instance()->getDevice()->CreateTexture2D(&texDesc, nullptr, &textures[1]);

	if (FAILED(err))
	{
		std::cerr << "Error: Failed to create textures\n";
	}
	
	renderTargets[0].addRTV(textures[0], DXGI_FORMAT_R32G32B32A32_FLOAT, DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0), true);
	renderTargets[1].addRTV(textures[1], DXGI_FORMAT_R32G32B32A32_FLOAT, DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0), true);

	renderTargets[0].changeViewport({ 0,0,1024,1024,0,1 });
	renderTargets[1].changeViewport({ 0,0,1024,1024,0,1 });

	renderTargets[0].clear();
	renderTargets[1].clear();

	ppEffect.getPipeline().bindConstantBuffer("Timer",Shaders::VERTEX_SHADER,0);
}

void PostProcessingEffect::renderInitialTexture(ComPtr<ID3D11ShaderResourceView>& SRV)
{
	renderTargets[1-writeTexture].bind();

	renderScreen.renderTexture(SRV);
}

void PostProcessingEffect::doEffect()
{
	renderTargets[writeTexture].clear();

	renderTargets[writeTexture].bind();

	ppEffect.renderTexture(renderTargets[1 - writeTexture].getRenderTargetSRV(0));
}

void PostProcessingEffect::render()
{
	renderScreen.renderTexture(renderTargets[writeTexture].getRenderTargetSRV(0));

	writeTexture = 1 - writeTexture;
}
