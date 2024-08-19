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

	DirectX::XMFLOAT2* getPositionArray() { return particlePositionArray; }
	DirectX::XMFLOAT2* getVelocityArray() { return particleVelocityArray; }
	DirectX::XMFLOAT3* getColourArray() { return particleColourArray; }
protected:
	DirectX::XMFLOAT2* particlePositionArray;
	DirectX::XMFLOAT2* particleVelocityArray;
	DirectX::XMFLOAT3* particleColourArray;

	int particleCount;
};