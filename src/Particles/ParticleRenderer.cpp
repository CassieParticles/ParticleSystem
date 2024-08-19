#include "ParticleRenderer.h"

#include <engine/D3DObjects/Device.h>

ParticleRenderer::ParticleRenderer(DirectX::XMFLOAT2* positions, DirectX::XMFLOAT3* colours, int particleCount) :positions{ positions },colours{colours}, particleCount{particleCount}
{
	particlesMesh.addVertexBuffer(static_cast<void*>(vertexPositions), false, sizeof(vertexPositions), sizeof(DirectX::XMFLOAT2), 0);
	particlesMesh.addVertexBuffer(static_cast<void*>(positions), true, sizeof(DirectX::XMFLOAT2) * particleCount, sizeof(DirectX::XMFLOAT2), 0);
	particlesMesh.addVertexBuffer(static_cast<void*>(colours), false, sizeof(DirectX::XMFLOAT3) * particleCount, sizeof(DirectX::XMFLOAT3), 0);

	particlesMesh.addIndexBuffer(indices,false,sizeof(indices), 0);


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

void ParticleRenderer::updateParticlePositions()
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>& positionBuffer = particlesMesh.getVertexBuffer(1);

	Device* device = Device::Instance();
	D3D11_MAPPED_SUBRESOURCE map;

	device->getDeviceContext()->Map(positionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, positions, particleCount * sizeof(DirectX::XMFLOAT2));
	device->getDeviceContext()->Unmap(positionBuffer.Get(), 0);
}

void ParticleRenderer::renderParticles()
{
	//TODO: Update instancePosition buffer to new data
	updateParticlePositions();

	particlesMesh.setBuffers();

	particlePipeline.bind();

	Device::Instance()->getDeviceContext()->DrawIndexedInstanced(6, particleCount, 0, 0, 0);

}
