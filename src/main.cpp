#include <iostream>
#include <d3d11.h>
#include <glfw3.h>

#include <engine/Engine/Window.h>
#include <engine/Engine/TimeManager.h>
#include <engine/Engine/Input/Input.h>
#include <engine/Engine/Input/InputActionManager.h>
#include <engine/Engine/Delay.h>
#include <engine/Engine/Random.h>

#include <engine/D3DObjects/Device.h>
#include <engine/D3DObjects/Pipeline/RenderScreen.h>

#include <engine/DataManagers/CBufferManager.h>

#include "Particles/ParticleManager.h"
#include "Particles/ParticleRenderer.h"
#include "Particles/ParticleEmitter.h"

int main()
{
	int windowSize[2] = { 800,800 };

	//Initialize singletons for engine
	Window* window = Window::InitializeWindow("Test window", windowSize[0], windowSize[1]);
	Device* device = Device::Instance();
	CBufferManager* cBufferManager = CBufferManager::Instance();
	InputActionManager* inputActionManager = InputActionManager::Instance();
	Random* random = Random::Instance();
	RenderScreen* renderScreen = RenderScreen::Instance();

	random->setSeed(time(0));

	cBufferManager->addBuffer("WindowSize", windowSize, true, 16);

	//Set engine parameters
	inputActionManager->setUpdateInput();	//Set action manager to update input singleton
	window->changeBackBufferClearColour(DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0));	//Change default render target clear colour

	//Initialize timer for main gameloop
	TimeManager timeManager;
	timeManager.setUniversalTimeManager();

	//Initialize things for particle sim
	constexpr int particleCount = 500;

	ParticleManager particleManager(particleCount);

	ParticleEmitter particleEmitter(DirectX::XMFLOAT2(4,4),0.1, 3, 3, DirectX::XMFLOAT3(1, 0, 0));
	ParticleEmitter particleEmitter2(DirectX::XMFLOAT2(6, 4), 0.2, 5, 2, DirectX::XMFLOAT3(0, 1, 0));

	ParticleRenderer particleRenderer{};

	

	timeManager.Start();

	particleEmitter.setAngle(0);
	particleEmitter.setSpread(3.14159 / 4);

	particleEmitter2.setAngle(3.14159);
	particleEmitter2.setSpread(3.14159 / 4);

	RenderTarget testTarget{};

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = 1024;
	texDesc.Height = 1024;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;

	HRESULT err = device->getDevice()->CreateTexture2D(&texDesc, nullptr, &tex2D);
	if (FAILED(err))
	{
		std::cerr << "Failed to create texture\n";
	}

	testTarget.addRTV(tex2D, DXGI_FORMAT_R8G8B8A8_UNORM, DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0), true);

	while (!window->getWindowShouldClose())
	{
		//Update engine objects
		timeManager.Tick();
		inputActionManager->update();


		//window->bindRTV();
		testTarget.clear();
		window->clearBackBuffer();

		//Update particle manager
		particleManager.updateParticles(&timeManager);
		particleEmitter.update(&timeManager);
		particleEmitter2.update(&timeManager);

		testTarget.bind();

		particleRenderer.bindPipeline();

		particleEmitter.render();
		particleEmitter2.render();

		window->bindRTV();
		
		renderScreen->Instance()->renderTexture(testTarget.getRenderTargetSRV(0));

		window->presentBackBuffer();
	}

	std::cout << "Average FPS: " << timeManager.FPS()<<'\n';
	std::cout << "Average MSPT: " << timeManager.MSPF()<<'\n';
	return 0;
}