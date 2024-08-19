#pragma once

#include <wrl.h>
#include <d3d11.h>

#include <engine/D3DObjects/Mesh.h>
#include <engine/D3DObjects/Pipeline/Pipeline.h>

#include "ParticleManager.h"

/*Full render process
	-Update vertex buffer with particle positions
	-Bind separate render target
	-Render each particle as a quad using instancing

	POST PROCESSING STUFF

	-Bind default render target
	-Render separate texture to screen
*/



class ParticleRenderer
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ParticleRenderer(DirectX::XMFLOAT2* positions, DirectX::XMFLOAT3* colours, int particleCount);
	~ParticleRenderer();

	//Call to udpate positions in renderer
	void updateParticlePositions();

	void renderParticles();
protected:
	//Used to render particles
	DirectX::XMFLOAT2* positions;
	DirectX::XMFLOAT3* colours;

	int particleCount;
	
	//Mesh for all particles
	Mesh particlesMesh;

	//Render pipeline for particles
	Pipeline particlePipeline;

	float vertexPositions[8]
	{
		0,0,
		0,1,
		1,0,
		1,1
	};

	int indices[6]
	{
		0,1,3,
		0,3,2
	};
};