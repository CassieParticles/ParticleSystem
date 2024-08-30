#include "ParticleEmitter.h"

#include <iostream>
#include <engine/D3DObjects/Device.h>

#include <engine/Engine/VectorMathOverloads.h>

#include <engine/Engine/Random.h>

ParticleEmitter::ParticleEmitter(DirectX::XMFLOAT2 position, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour) :position{ position },emmisionTimer { interval }, particleSpeed{ particleSpeed }
{
	//Get the maximum number of particles
	particleCount = ceil(particleLifeTime / interval);
	invParticleCount = 1.f / particleCount;

	//Create the arrays to store particle positions and velocities
	particlePositions = new DirectX::XMFLOAT2[particleCount];
	particleVelocities = new DirectX::XMFLOAT2[particleCount];

	DirectX::XMFLOAT3* particleColours = new DirectX::XMFLOAT3[particleCount];

	float inv = 1.f / particleCount;

	for (int i = 0; i < particleCount; ++i)
	{
		particlePositions[i] = DirectX::XMFLOAT2{ position };
		particleColours[i] = DirectX::XMFLOAT3{ colour };
		particleVelocities[i] = DirectX::XMFLOAT2{ 0,0 };
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

	//Render different amount if on first iteration so it isn't rendering particles that aren't moving yet
	Device::Instance()->getDeviceContext()->DrawIndexedInstanced(6, firstIteration ? particleCount : currentParticleReset, 0, 0, 0);
}

void ParticleEmitter::update(TimeManager* timeManager)
{
	emmisionTimer.Tick();
	
	//"Emit" a new particle, really, it resets an old one
	if (emmisionTimer.isEnabled())
	{
		//Reset particle
		//float angle = currentParticleReset * invParticleCount * 2 * 3.14159;	//Temporary, will make random angle possible
		float firingAngle = (Random::Instance()->genRand()-0.5f) * spread + this->angle;
		particlePositions[currentParticleReset] = position;
		particleVelocities[currentParticleReset] = DirectX::XMFLOAT2{cos(firingAngle),sin(firingAngle)};
		currentParticleReset++;

		//Check if counter needs to be wrapped around
		if (currentParticleReset >= particleCount)
		{
			firstIteration = true;
			currentParticleReset = 0;
		}
	}

	//TODO: change this to compute shader for larger particle simulation
	for (int i = 0; i < particleCount; ++i)
	{
		//Load vectors into XMVECTORs for faster calculation and to use overloads
		DirectX::XMVECTOR velocity = DirectX::XMLoadFloat2(particleVelocities + i);
		DirectX::XMVECTOR position = DirectX::XMLoadFloat2(particlePositions + i);

		//Do maths
		position += velocity * timeManager->DeltaTime();

		//Store vectors back into arrays
		DirectX::XMStoreFloat2(particleVelocities + i, velocity);
		DirectX::XMStoreFloat2(particlePositions + i, position);
	}


	//Update vertex instance buffer
	
	D3D11_MAPPED_SUBRESOURCE mapped;
	Device::Instance()->getDeviceContext()->Map(instancePositions.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, particlePositions, particleCount * sizeof(DirectX::XMFLOAT2));
	Device::Instance()->getDeviceContext()->Unmap(instancePositions.Get(), 0);
}
