#include "ParticleManager.h"

#include <engine/Engine/TimeManager.h>
#include <engine/DataManagers/CBufferManager.h>

#include <engine/Engine/VectorMathOverloads.h>



ParticleManager::ParticleManager(int particleCount):particleCount{particleCount}
{
	particlePositionArray = new DirectX::XMFLOAT2[particleCount];
	particleVelocityArray = new DirectX::XMFLOAT2[particleCount];
	particleColourArray = new DirectX::XMFLOAT3[particleCount];

	float inc = 1.f / particleCount;
	for (int i = 0; i < particleCount; ++i)
	{
		particlePositionArray[i]= DirectX::XMFLOAT2(cos(inc * i * 2 * 3.14159f) * 2, sin(inc * i * 2 * 3.14159f) * 2);
		particlePositionArray[i].x += 4;
		particlePositionArray[i].y += 4;

		particleVelocityArray[i] = DirectX::XMFLOAT2(0,0);


		DirectX::XMFLOAT3 colour = {};
		colour.x = inc * i;
		colour.y = 0.5f;
		colour.z = 0.5f;

		particleColourArray[i] = colour;
	}




}

ParticleManager::~ParticleManager()
{
	delete[] particlePositionArray;
	delete[] particleVelocityArray;
	delete[] particleColourArray;
}

//Particles are assumed to have unit mass
void ParticleManager::updateParticles(TimeManager* timer)
{
	//TODO: Update this to run using a shader
	for (int i = 0; i < particleCount; ++i)
	{
		//Get vectors into XMVECTOR form (makes math calculation faster)
		DirectX::XMVECTOR position = DirectX::XMLoadFloat2(particlePositionArray + i);
		DirectX::XMVECTOR velocity = DirectX::XMLoadFloat2(particleVelocityArray + i);
		DirectX::XMVECTOR acceleration = DirectX::XMVECTOR{0,0,0,0};

		//Force of gravity
		acceleration += DirectX::XMVECTOR{0, -98.1f * 3, 0, 0};

		//Apply dynamics
		float deltaTime = timer->DeltaTime();

		velocity += acceleration * deltaTime;

		position += velocity * deltaTime;

		//Store new velocity and position
		DirectX::XMStoreFloat2(particlePositionArray + i, position);
		DirectX::XMStoreFloat2(particleVelocityArray + i, velocity);
	}
}
