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
	ParticleRenderer();
	~ParticleRenderer();

	void bindPipeline();
protected:
	Pipeline particlePipeline;
};