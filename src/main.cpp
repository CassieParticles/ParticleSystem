#include <iostream>
#include <d3d11.h>
#include <glfw3.h>

#include <engine/Engine/Window.h>
#include <engine/Engine/TimeManager.h>
#include <engine/Engine/Input/Input.h>
#include <engine/Engine/Input/InputActionManager.h>

#include <engine/D3DObjects/Device.h>
#include <engine/D3DObjects/Mesh.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>
#include <engine/D3DObjects/Pipeline/PipelineStages.h>

#include <engine/DataManagers/CBufferManager.h>

#include "ParticleManager.h"

int main()
{
	//Initialize singletons for engine
	Window* window = Window::InitializeWindow("Test window", 800, 800);
	Device* device = Device::Instance();
	CBufferManager* resourceManager = CBufferManager::Instance();
	InputActionManager* inputActionManager = InputActionManager::Instance();

	//Set engine parameters
	inputActionManager->setUpdateInput();	//Set action manager to update input singleton
	window->changeBackBufferClearColour(DirectX::XMFLOAT4(0.2, 0.4, 0.6, 1.0));	//Change default render target clear colour

	//Initialize timer for main gameloop
	TimeManager timeManager;

	//Initialize things for particle sim
	ParticleManager particleManager(500);

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
		

		window->presentBackBuffer();
	}

	std::cout << "Average FPS: " << timeManager.FPS()<<'\n';
	std::cout << "Average MSPT: " << timeManager.MSPF()<<'\n';
	return 0;
}