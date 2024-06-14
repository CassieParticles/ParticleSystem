#include <iostream>
#include <engine/Engine/Window.h>

int main()
{
	Window* window = Window::InitializeWindow("Test window", 800, 800);

	window->bindRTV();

	while (!window->getWindowShouldClose())
	{
		window->clearBackBuffer(DirectX::XMFLOAT4(0.2, 0.8, 0.6, 0.4));
		std::cout << "Window actions\n";
		window->presentBackBuffer();
	}

	std::cout << "Test\n";
	return 0;
}