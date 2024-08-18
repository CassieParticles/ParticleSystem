#include "ParticleManager.h"

#include <engine/Engine/TimeManager.h>
#include <engine/DataManagers/CBufferManager.h>

ParticleManager::ParticleManager(int particleCount):particleCount{particleCount}
{
	particleArray = new Particle[particleCount];
	float inc = 1.f / particleCount;
	for (int i = 0; i < particleCount; ++i)
	{
		particleArray[i].position = DirectX::XMFLOAT2(cos(inc * i * 2 * 3.14159f) * 200, sin(inc * i * 2 * 3.14159f) * 200);
		particleArray[i].position.x += 400;
		particleArray[i].position.y += 400;

		particleArray[i].velocity = DirectX::XMFLOAT2(cos(inc * i), sin(inc * i));
		DirectX::XMFLOAT3 colour = {};
		colour.x = inc * i;
		colour.y = 0.5f;
		colour.z = 0.5f;

		particleArray[i].colour = colour;
	}

	int particleSize = 4;
	CBufferManager::Instance()->addBuffer("ParticleData", &particleSize, false, 16);
}

ParticleManager::~ParticleManager()
{
	delete[] particleArray;
}

void ParticleManager::updateParticles(TimeManager* timer)
{
	//TODO: Update this to run using a shader
	for (int i = 0; i < particleCount; ++i)
	{
		Particle* particle = particleArray + i;

		particle->position.x = particle->position.x + particle->velocity.x * timer->DeltaTime();
		particle->position.y = particle->position.y + particle->velocity.y * timer->DeltaTime();
	}
}
