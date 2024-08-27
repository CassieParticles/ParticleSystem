#include "ParticleEmitter.h"

#include <iostream>
#include <engine/D3DObjects/Device.h>

ParticleEmitter::ParticleEmitter(float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour) :emmisionTimer{ interval }
{
	//Get the maximum number of particles
	particleCount = ceil(particleLifeTime / interval);

	//Create the arrays to store particle positions and velocities
	particlePositions = new DirectX::XMFLOAT2[particleCount];
	particleVelocities = new DirectX::XMFLOAT2[particleCount];

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
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] particlePositions;
	delete[] particleVelocities;
}
