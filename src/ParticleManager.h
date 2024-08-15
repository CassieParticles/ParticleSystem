#pragma once

#include <DirectXMath.h>

struct Particle
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT3 colour;
};

class TimeManager;

class ParticleManager
{
public:
	ParticleManager(int particleCount);
	~ParticleManager();

	void updateParticles(TimeManager* timer);
protected:
	Particle* particleArray;
	int particleCount;
};