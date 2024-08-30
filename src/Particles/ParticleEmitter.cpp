#include "ParticleEmitter.h"

#include <iostream>
#include <engine/D3DObjects/Device.h>

ParticleEmitter::ParticleEmitter(DirectX::XMFLOAT2 position, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour) :emmisionTimer{interval}, particleSpeed{particleSpeed}
{
	//Get the maximum number of particles
	particleCount = ceil(particleLifeTime / interval);

	//Create the arrays to store particle positions and velocities
	particlePositions = new DirectX::XMFLOAT2[particleCount];
	particleVelocities = new DirectX::XMFLOAT2[particleCount];

	DirectX::XMFLOAT3* particleColours = new DirectX::XMFLOAT3[particleCount];

	for (int i = 0; i < particleCount; ++i)
	{
		particlePositions[i] = DirectX::XMFLOAT2(position);
		particleColours[i] = DirectX::XMFLOAT3(colour);
	}

	//Structures used to make buffers
	D3D11_SUBRESOURCE_DATA dat{};
	D3D11_BUFFER_DESC desc{};
	HRESULT err{};

	//Make vertex buffer
	dat.pSysMem = vertexPos;
	
	desc.ByteWidth = sizeof(vertexPos);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	
	err = Device::Instance()->getDevice()->CreateBuffer(&desc, &dat, &vertexPositions);
	if (FAILED(err))
	{
		std::cout << "Error in creating vertex buffer\n";
	}

	//Make index buffer
	dat.pSysMem = indices;

	desc.ByteWidth = sizeof(indices);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	err = Device::Instance()->getDevice()->CreateBuffer(&desc, &dat, &vertexIndices);
	if (FAILED(err))
	{
		std::cout << "Error in creating index buffer\n";
	}

	//Make instance buffer using particle positions
	dat.pSysMem = particlePositions;

	desc.ByteWidth = sizeof(DirectX::XMFLOAT2) * particleCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	err = Device::Instance()->getDevice()->CreateBuffer(&desc, &dat, &instancePositions);

	if (FAILED(err))
	{
		std::cout << "Error in creating vertex buffer\n";
	}

	//Make instance colours (all same)
	
	dat.pSysMem = particleColours;

	desc.ByteWidth = sizeof(DirectX::XMFLOAT3) * particleCount;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	err = Device::Instance()->getDevice()->CreateBuffer(&desc, &dat, &instanceColours);

	if (FAILED(err))
	{
		std::cerr << "Error in creating vertex buffer\n";
	}

	delete[] particleColours;
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] particlePositions;
	delete[] particleVelocities;
}

void ParticleEmitter::render()
{
	//Pipeline should already be bound
	ID3D11Buffer* buffers[3]
	{
		vertexPositions.Get(),
		instancePositions.Get(),
		instanceColours.Get()
	};

	UINT strides[3]
	{
		sizeof(DirectX::XMFLOAT2),
		sizeof(DirectX::XMFLOAT2),
		sizeof(DirectX::XMFLOAT3)
	};
	UINT offsets[3]
	{
		0,0,0
	};

	Device::Instance()->getDeviceContext()->IASetVertexBuffers(0, 3, buffers, strides, offsets);
	Device::Instance()->getDeviceContext()->IASetIndexBuffer(vertexIndices.Get(), DXGI_FORMAT_R32_UINT, 0);

	Device::Instance()->getDeviceContext()->DrawIndexedInstanced(6, particleCount, 0, 0, 0);
}

void ParticleEmitter::update()
{

}
