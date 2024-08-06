#include <iostream>

#include <engine/Engine/Window.h>
#include <engine/Engine/TimeManager.h>
#include <engine/Engine/Input/Input.h>
#include <engine/Engine/Input/InputActionManager.h>

#include <engine/D3DObjects/Device.h>
#include <engine/D3DObjects/Mesh.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>
#include <engine/D3DObjects/Pipeline/PipelineStages.h>

#include <engine/DataManagers/ResourceManager.h>

#include <d3d11.h>
#include <glfw3.h>

int main()
{
	//Initialize singletons
	Window* window = Window::InitializeWindow("Test window", 800, 800);
	Device* device = Device::Instance();
	ResourceManager* resourceManager = ResourceManager::Instance();
	TimeManager timeManager;

	InputActionManager* inputActionManager = InputActionManager::Instance();

	int inputs[3]
	{
		GLFW_KEY_A,
		GLFW_KEY_G,
		GLFW_KEY_K
	};


	//Set up input
	inputActionManager->setUpdateInput();
	inputActionManager->addInputAction("Test", inputs, 3, InputAction::DOWN_OR);

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

	float brightness[3] =
	{
		0.3f,0.8f,1.0f
	};

	Pipeline pipeline;
	Mesh mesh;

	resourceManager->addVertexBuffer("testVertices",vertices,D3D11_USAGE_IMMUTABLE,sizeof(vertices));
	resourceManager->addVertexBuffer("testID", indices, D3D11_USAGE_IMMUTABLE, sizeof(indices));

	//mesh.addVertexBuffer(vertices, false, sizeof(vertices), 3 * sizeof(float), 0);
	mesh.addVertexBuffer("testVertices", 3 * sizeof(float), 0);
	mesh.addVertexBuffer("testID", sizeof(int), 0);
	mesh.addIndexBuffer(indices, false, sizeof(indices), 0);

	pipeline.addVertexComponent({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 });
	pipeline.addVertexComponent({ "SV_VertexID",0,DXGI_FORMAT_R32_SINT,1,0,D3D11_INPUT_PER_VERTEX_DATA,0 });

	pipeline.addVertexShader(L"shaders/vertex.hlsl");
	pipeline.addPixelShader(L"shaders/pixel.hlsl");

	pipeline.setPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pipeline.getRastierizerDesc().CullMode = D3D11_CULL_NONE;

	pipeline.compilePipeline();

	resourceManager->addConstantBuffer("offset", offset, true, 16);

	pipeline.bindConstantBuffer("offset", Shaders::VERTEX_SHADER, 0);

	resourceManager->addStructuredBuffer<float>("brightness", brightness, D3D11_USAGE_IMMUTABLE, sizeof(brightness));

	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = 3;

	resourceManager->addShaderResourceView<StructuredBuffer>("brightnessView", "brightness", &desc);

	pipeline.bindSRV("brightnessView", Shaders::VERTEX_SHADER, 0);

	timeManager.Start();
	while (!window->getWindowShouldClose())
	{
		//Update buffers
		timeManager.Tick();
		inputActionManager->update();

		if (inputActionManager->getActionTriggered("Test"))
		{
			newOffset[0] = cos(timeManager.ElapsedTime()) * 0.5f;
			newOffset[1] = sin(timeManager.ElapsedTime()) * 0.5f;

			resourceManager->getConstantBuffer("offset")->updateBuffer(newOffset, 16);
		}

		//Render to the screen
		window->bindRTV();

		window->clearBackBuffer(DirectX::XMFLOAT4(0.2, 0.4, 0.6, 0.4));

		pipeline.bind();
		mesh.setBuffers();
		
		device->getDeviceContext()->DrawIndexed(3, 0, 0);

		window->presentBackBuffer();
	}

	std::cout << "Average FPS: " << timeManager.FPS()<<'\n';
	std::cout << "Average MSPT: " << timeManager.MSPF()<<'\n';
	return 0;
}