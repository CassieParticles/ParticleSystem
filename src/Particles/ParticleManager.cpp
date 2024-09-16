#include "ParticleManager.h"
#include <engine/DataManagers/AssetLoader.h>
#include <engine/Engine/Window.h>


ParticleManager::~ParticleManager()
{
	for (int i = 0; i < particleStreams.size(); ++i)
	{
		delete particleStreams.at(i);
	}
	//delete particleStream;
}

void ParticleManager::addParticleStream(DirectX::XMFLOAT2 position, float angle, float spread, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour)
{
	ParticleStream* particleStream = new ParticleStream(position, interval, particleLifeTime, particleSpeed, colour);
	particleStream->setAngle(angle);
	particleStream->setSpread(spread);
	particleStreams.push_back(particleStream);
}

void ParticleManager::init(const std::string& initialTexture)
{
	if (initialTexture != "")
	{
		postProcess.renderInitialTexture(AssetLoader::Instance()->getTexture(initialTexture)->getSRV());
	}
}

void ParticleManager::update(TimeManager* timeManager)
{
	for (int i = 0; i < particleStreams.size(); ++i)
	{
		particleStreams.at(i)->update(timeManager);
	}
	//particleStream->update(timeManager);
}

void ParticleManager::render()
{
	//Do the smoke diffuse effect
	postProcess.doEffect();

	particleRenderer.bindPipeline();
	for (int i = 0; i < particleStreams.size(); ++i)
	{
		particleStreams.at(i)->render();
	}
	//particleStream->render();

	Window::Instance()->bindRTV();
	postProcess.render();
}


