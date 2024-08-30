#include <iostream>
#include <d3d11.h>
#include <glfw3.h>

#include <engine/Engine/Window.h>
#include <engine/Engine/TimeManager.h>
#include <engine/Engine/Input/Input.h>
#include <engine/Engine/Input/InputActionManager.h>
#include <engine/Engine/Delay.h>

#include <engine/D3DObjects/Device.h>

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

	ParticleEmitter particleEmitter(DirectX::XMFLOAT2(4,4),0.25, 2, 3, DirectX::XMFLOAT3(1, 0, 0));

	ParticleRenderer particleRenderer{};

	timeManager.Start();


	while (!window->getWindowShouldClose())
	{
		//Update engine objects
		timeManager.Tick();
		inputActionManager->update();

		window->bindRTV();
		window->clearBackBuffer();

		//Update particle manager
		particleManager.updateParticles(&timeManager);

		particleRenderer.bindPipeline();

		particleEmitter.render();
		

		window->presentBackBuffer();
	}

	std::cout << "Average FPS: " << timeManager.FPS()<<'\n';
	std::cout << "Average MSPT: " << timeManager.MSPF()<<'\n';
	return 0;
}