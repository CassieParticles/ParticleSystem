#pragma once

#include <vector>

#include "ParticleStream.h"
#include "PostProcessingEffect.h"
#include "ParticleRenderer.h"

class ParticleManager
{
public:
	~ParticleManager();

	void addParticleStream(DirectX::XMFLOAT2 position,float angle, float spread, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour);

	void init(const std::string& initialTexture = "");
	void update(TimeManager* timeManager);
	void render();
protected:
	PostProcessingEffect postProcess{};
	ParticleRenderer particleRenderer{};

	std::vector<ParticleStream*> particleStreams;

	ParticleStream* particleStream;

};