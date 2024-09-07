#pragma once

#include <engine/D3DObjects/RenderTarget.h>
#include <engine/D3DObjects/Pipeline/RenderScreen.h>

class PostProcessingEffect
{
	/*
		Effect
		2 render targets, one read one write
		Render to write target
			-Each pixel is a weighted average of itself and neighbors
			-Render particles over the top
		-Swap textures 
	*/

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	PostProcessingEffect();
	~PostProcessingEffect() = default;

	void doEffect();

	void render();
protected:
	//Handle swapping textures
	RenderTarget renderTargets[2];
	int writeTexture{};

	RenderScreen ppEffect;
	RenderScreen renderScreen;

};