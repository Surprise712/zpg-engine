#pragma once

#include <GLFW/glfw3.h>

#include <cstddef>
#include <memory>
#include <vector>

class Scene;
class ShaderProgram;

class Application
{
private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<Scene>> scenes;
	std::size_t currentScene;

	ShaderProgram* clockShaderProgram = nullptr;

	bool init(int width, int height, const char* title);
	void createScenes();

	void processInput(float deltaTime);
	void beginFrame();
	void drawCurrentScene();
	void endFrame();

	double lastMouseX;
	double lastMouseY;
	bool firstMouse;

	void cleanup();

	static void framebufferSizeCallback(
		GLFWwindow* window,
		int width,
		int height);

public:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	int run();

	
};