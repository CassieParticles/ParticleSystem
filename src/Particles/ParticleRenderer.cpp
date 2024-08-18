#include "ParticleRenderer.h"

#include <engine/D3DObjects/Device.h>

ParticleRenderer::ParticleRenderer(Particle* particleArray, int particleCount):particleArray{particleArray},particleCount{particleCount}
{
	DirectX::XMFLOAT2* positionArray = new DirectX::XMFLOAT2[particleCount];
	DirectX::XMFLOAT3* colourArray = new DirectX::XMFLOAT3[particleCount];

	for (int i = 0; i < particleCount; ++i)
	{
		positionArray[i] = particleArray[i].position;
		colourArray[i] = particleArray[i].colour;
	}

	particlesMesh.addVertexBuffer(static_cast<void*>(vertexPositions), false, sizeof(vertexPositions), sizeof(DirectX::XMFLOAT2), 0);
	particlesMesh.addVertexBuffer(static_cast<void*>(positionArray), false, sizeof(DirectX::XMFLOAT2) * particleCount, sizeof(DirectX::XMFLOAT2), 0);
	particlesMesh.addVertexBuffer(static_cast<void*>(colourArray), false, sizeof(DirectX::XMFLOAT3) * particleCount, sizeof(DirectX::XMFLOAT3), 0);

	particlesMesh.addIndexBuffer(indices,false,sizeof(indices), 0);
	
	delete[] positionArray;
	delete[] colourArray;

	//Add shaders to pipeline
	particlePipeline.addVertexShader(L"shaders/vertex.hlsl");
	particlePipeline.addPixelShader(L"shaders/fragment.hlsl");

	//Add vertex components for IA
	particlePipeline.addVertexComponent({"VertexPosition",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0});
	particlePipeline.addVertexComponent({ "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1 });
	particlePipeline.addVertexComponent({ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,2,0,D3D11_INPUT_PER_INSTANCE_DATA,1 });

	//Set the primitive type
	particlePipeline.setPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Declare constant buffers to be attached to pipeline
	particlePipeline.bindConstantBuffer("WindowSize", Shaders::VERTEX_SHADER, 0);
	particlePipeline.bindConstantBuffer("ParticleData", Shaders::VERTEX_SHADER, 1);

	//Compile pipeline
	particlePipeline.compilePipeline();
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::renderParticles()
{
	//TODO: Update isntancePosition buffer to new data

	particlesMesh.setBuffers();

	particlePipeline.bind();

	Device::Instance()->getDeviceContext()->DrawIndexedInstanced(6, particleCount, 0, 0, 0);

}
