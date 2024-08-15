#include "ParticleManager.h"

#include <engine/Engine/TimeManager.h>

ParticleManager::ParticleManager(int particleCount):particleCount{particleCount}
{
	particleArray = new Particle[particleCount];
	float inc = 1.f / particleCount;
	for (int i = 0; i < particleCount; ++i)
	{
		particleArray[i].velocity = DirectX::XMFLOAT2(cos(inc * i), sin(inc * i));
	}
}

ParticleManager::~ParticleManager()
{
	delete[] particleArray;
}

void ParticleManager::updateParticles(TimeManager* timer)
{
	for (int i = 0; i < particleCount; ++i)
	{
		Particle* particle = particleArray + i;

		particle->position.x = particle->position.x + particle->velocity.x * timer->DeltaTime();
		particle->position.y = particle->position.y + particle->velocity.y * timer->DeltaTime();
	}
}
