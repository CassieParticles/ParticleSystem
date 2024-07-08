#include <iostream>

#include <engine/Engine/Window.h>
#include <engine/Engine/TimeManager.h>
#include <engine/Engine/Input/Input.h>
#include <engine/Engine/Input/InputActionManager.h>

#include <engine/D3DObjects/Device.h>
#include <engine/D3DObjects/Mesh.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>
#include <engine/D3DObjects/Pipeline/PipelineStages.h>

#include <engine/DataManagers/CBufferManager.h>

#include <d3d11.h>
#include <glfw3.h>

int main()
{
	//Initialize singletons
	Window* window = Window::InitializeWindow("Test window", 800, 800);
	Device* device = Device::Instance();
	CBufferManager* cBufferManager = CBufferManager::Instance();
	TimeManager timeManager;

	Input* input = Input::Instance();
	InputActionManager* inputActionManager = InputActionManager::Instance();

	int inputs[3]
	{
		GLFW_KEY_A,
		GLFW_KEY_G,
		GLFW_KEY_K
	};


	//Set up input
	inputActionManager->addInputAction("Test", inputs, 3, InputAction::RELEASE);

	float vertices[9] =
	{
		0.f,0.5f,.15f,
		-0.5f,-0.5f,.15f,
		0.5f,-0.5f,.15f
	};
	int indices[3] =
	{
		0,2,1
	};

	float offset[3] =
	{
		0.25f,0.0f,0.0f
	};

	float newOffset[3] =
	{
		0.0f,1.f,0.0f
	};

	Pipeline pipeline;
	Mesh mesh;

	mesh.addVertexBuffer(vertices, false, sizeof(vertices), 3 * sizeof(float), 0);
	mesh.addIndexBuffer(indices, false, sizeof(indices), 0);

	pipeline.addVertexComponent({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 });

	pipeline.addVertexShader(L"shaders/vertex.hlsl");
	pipeline.addPixelShader(L"shaders/pixel.hlsl");

	pipeline.setPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pipeline.getRastierizerDesc().CullMode = D3D11_CULL_NONE;

	pipeline.compilePipeline();

	cBufferManager->addBuffer("offset", offset, true, 16);

	pipeline.bindConstantBuffer("offset", Shaders::VERTEX_SHADER, 0);

	

	timeManager.Start();
	while (!window->getWindowShouldClose())
	{
		//Update buffers
		timeManager.Tick();
		input->update();
		inputActionManager->update();

		if (inputActionManager->getActionTriggered("Test"))
		{
			newOffset[0] = cos(timeManager.ElapsedTime()) * 0.5f;
			newOffset[1] = sin(timeManager.ElapsedTime()) * 0.5f;

			cBufferManager->getCBuffer("offset")->updateCBuffer(newOffset, 16);
		}

		//Render to the screen
		window->bindRTV();

		window->clearBackBuffer(DirectX::XMFLOAT4(0.2, 0.4, 0.6, 0.4));

		pipeline.bind();
		mesh.setBuffers();
		
		device->getDeviceContext()->DrawIndexed(3, 0, 0);

		window->presentBackBuffer();
	}
	return 0;
}