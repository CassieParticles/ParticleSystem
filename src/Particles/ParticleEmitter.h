#pragma once

#define NOMINMAX
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include <engine/Engine/Timer.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>

class ParticleEmitter
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ParticleEmitter(DirectX::XMFLOAT2 position, float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour);
	~ParticleEmitter();

	void render();
	void update();
protected:

	//Variables for particle simulation
	DirectX::XMFLOAT2* particlePositions;
	DirectX::XMFLOAT2* particleVelocities;
	int particleCount;
	Timer emmisionTimer;
	DirectX::XMFLOAT2 position;
	float particleSpeed;

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