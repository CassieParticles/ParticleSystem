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
#include <engine/DataManagers/AssetLoader.h>

#include "Particles/ParticleRenderer.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/PostProcessingEffect.h"

int main()
{
	int windowSize[2] = { 800,800 };

	//Initialize singletons for engine
	Window* window = Window::InitializeWindow("Test window", windowSize[0], windowSize[1]);
	Device* device = Device::Instance();
	CBufferManager* cBufferManager = CBufferManager::Instance();
	InputActionManager* inputActionManager = InputActionManager::Instance();
	Random* random = Random::Instance();
	AssetLoader* assetLoader = AssetLoader::Instance();
	
	assetLoader->getTexture("assets/bunny.png");

	random->setSeed(time(0));
	float timeData[2] = {0,0};

	cBufferManager->addBuffer("WindowSize", windowSize, true, 16);
	cBufferManager->addBuffer("Timer", timeData, true, 16);

	//Set engine parameters
	inputActionManager->setUpdateInput();	//Set action manager to update input singleton
	window->changeBackBufferClearColour(DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0));	//Change default render target clear colour

	//Initialize timer for main gameloop
	TimeManager timeManager;
	timeManager.setUniversalTimeManager();

	ParticleEmitter particleEmitter(DirectX::XMFLOAT2(4,4),0.1, 3, 3, DirectX::XMFLOAT3(1, 0, 0));
	ParticleEmitter particleEmitter2(DirectX::XMFLOAT2(6, 4), 0.2, 5, 2, DirectX::XMFLOAT3(0, 1, 0));

	ParticleRenderer particleRenderer{};

	PostProcessingEffect postProcess{};

	timeManager.Start();

	particleEmitter.setAngle(0);
	particleEmitter.setSpread(3.14159 / 4);

	particleEmitter2.setAngle(3.14159);
	particleEmitter2.setSpread(3.14159 / 4);

	postProcess.renderInitialTexture(assetLoader->getTexture("assets/bunny.png")->getSRV());


	while (!window->getWindowShouldClose())
	{
		//Update engine objects
		timeManager.Tick();
		inputActionManager->update();

		//
		window->bindRTV();
		window->clearBackBuffer();

		//Update emitters
		particleEmitter.update(&timeManager);
		particleEmitter2.update(&timeManager);


		//swaps textures and handles rendering of 
		postProcess.doEffect();

		//Render particles to render target in postProcess
		particleRenderer.bindPipeline();
		particleEmitter.render();
		particleEmitter2.render();

		window->bindRTV();
		postProcess.render();

		window->presentBackBuffer();
	}

	std::cout << "Average FPS: " << timeManager.FPS()<<'\n';
	std::cout << "Average MSPT: " << timeManager.MSPF()<<'\n';
	return 0;
}