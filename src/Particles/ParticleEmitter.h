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
	ParticleEmitter(float interval, float particleLifeTime, float particleSpeed, DirectX::XMFLOAT3 colour);
	~ParticleEmitter();
protected:
	//Variables for particle simulation
	DirectX::XMFLOAT2* particlePositions;
	DirectX::XMFLOAT2* particleVelocities;
	int particleCount;
	Timer emmisionTimer;
	float particleSpeed;

	//Objects needed for rendering
	ComPtr<ID3D11Buffer> vertexPositions;
	ComPtr<ID3D11Buffer> vertexIndices;

	ComPtr<ID3D11Buffer> instancePositions;

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