#include "ParticleManager.h"

#include <engine/Engine/TimeManager.h>
#include <engine/DataManagers/CBufferManager.h>

ParticleManager::ParticleManager(int particleCount):particleCount{particleCount}
{

	particlePositionArray = new DirectX::XMFLOAT2[particleCount];
	particleVelocityArray = new DirectX::XMFLOAT2[particleCount];
	particleColourArray = new DirectX::XMFLOAT3[particleCount];

	float inc = 1.f / particleCount;
	for (int i = 0; i < particleCount; ++i)
	{
		particlePositionArray[i]= DirectX::XMFLOAT2(cos(inc * i * 2 * 3.14159f) * 200, sin(inc * i * 2 * 3.14159f) * 200);
		particlePositionArray[i].x += 400;
		particlePositionArray[i].y += 400;

		particleVelocityArray[i] = DirectX::XMFLOAT2(cos(inc * i) * 25, sin(inc * i) * 25);


		DirectX::XMFLOAT3 colour = {};
		colour.x = inc * i;
		colour.y = 0.5f;
		colour.z = 0.5f;

		particleColourArray[i] = colour;
	}

	int particleSize = 4;
	CBufferManager::Instance()->addBuffer("ParticleData", &particleSize, false, 16);
}

ParticleManager::~ParticleManager()
{
	delete[] particlePositionArray;
	delete[] particleVelocityArray;
	delete[] particleColourArray;
}

void ParticleManager::updateParticles(TimeManager* timer)
{
	//TODO: Update this to run using a shader
	for (int i = 0; i < particleCount; ++i)
	{
		particlePositionArray[i].x += particleVelocityArray[i].x * timer->DeltaTime();
		particlePositionArray[i].y += particleVelocityArray[i].y * timer->DeltaTime();
		
	}
}
