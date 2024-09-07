#include "ParticleRenderer.h"

#include <engine/DataManagers/CBufferManager.h>

ParticleRenderer::ParticleRenderer()
{
	//Set up pipeline information
	particlePipeline.addVertexShader(L"shaders/Particle/vertex.hlsl");
	particlePipeline.addPixelShader(L"shaders/Particle/fragment.hlsl");

	particlePipeline.addVertexComponent({"VertexPosition",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0});
	particlePipeline.addVertexComponent({"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1});
	particlePipeline.addVertexComponent({"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,2,0,D3D11_INPUT_PER_INSTANCE_DATA,1});

	particlePipeline.setPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set up constant buffers
	char data[16];

	int particleSize = 4;	//4 bytes
	int pixelsPerUnit = 100;	//4 bytes

	memcpy(data, &particleSize, 4);
	memcpy(data + 4, &pixelsPerUnit, 4);

	CBufferManager::Instance()->addBuffer("ParticleData", data, false, 16);


	particlePipeline.bindConstantBuffer("WindowSize", Shaders::VERTEX_SHADER, 0);
	particlePipeline.bindConstantBuffer("ParticleData", Shaders::VERTEX_SHADER, 1);

	particlePipeline.compilePipeline();
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::bindPipeline()
{
	particlePipeline.bind();
}
