#pragma once

#define NOMINMAX
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include <engine/Engine/Timer.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>

class ParticleStream
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ParticleStream(DirectX::XMFLOAT2 position, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour);
	~ParticleStream();

	void render();
	void update(TimeManager* timeManager);

	void setAngle(float angle) { this->angle = angle; }
	void setSpread(float spread) { this->spread = spread; }
protected:

	//Variables for particle simulation
	DirectX::XMFLOAT2* particlePositions;
	DirectX::XMFLOAT2* particleVelocities;
	int particleCount;
	float invParticleCount;
	Timer emmisionTimer;
	DirectX::XMFLOAT2 position;
	float particleSpeed;
	int currentParticleReset{ 0 };	//Used to reset particles once they've outlived their time
	bool firstIteration{ false };	//Used to selectively not render particles on first iteration so they aren't emmited
	float angle = 0;
	float spread = 2 * 3.14159f;
	//TODO: make better way to stop rendering particles

	//Objects needed for rendering
	ComPtr<ID3D11Buffer> vertexPositions;
	ComPtr<ID3D11Buffer> vertexIndices;

	ComPtr<ID3D11Buffer> instancePositions;
	ComPtr<ID3D11Buffer> instanceColours;

	//Constants for vertices
	static constexpr float vertexPos[8]
	{
		0,0,
		0,1,
		1,0,
		1,1
	};

	constexpr static int indices[6]
	{
		0,1,3,
		0,3,2
	};
};