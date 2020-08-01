/*
This class deals with all the window and interface functions for the application
with the aid from GLFW 3.3.2
written by Julius Neudecker
v0.1 23.07.2020
v0.2 01.08.2020 - optimized some function calls for easier use
*/

#ifndef GLFWHANDLER_H
#define GLFWHANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <callbacks.h>

#include <jLog.h>
#include <camera.h>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

//new glad library
#define USE_VERSION_MAJOR 4
#define USE_VERSION_MINOR 3

#define WINDOW_TITLE "gl-ComputeShader"

struct fpvCam {
	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;
};

class glfwHandler {
public:
	glfwHandler(int viewportWidth, int viewportHeight);	
	void SetInputMode(int, int);

	static fpvCam* giveCamera();

	//Register Callbacks
	void SetFramebufferSizeCallback(GLFWframebuffersizefun);
	void SetCursorPosCallback(GLFWcursorposfun);
	void SetScrollCallback(GLFWscrollfun);

	//member functions
	bool WindowShouldClose();
	void BeginRenderLoop();
	void ConcludeRenderLoop();
	void processInput(float deltaTime);
	void SwapBuffers();
	void PollEvents();
	void Terminate();
	static std::string getErrorMsg();


	//members
	static fpvCam* SceneCamera; 

private:
	GLFWwindow* window;
	void _init();
	void _configDebugContext();
	void _configContext();

	int _scrWidth;
	int _scrHeight;

	float _deltaTime;
	float _lastFrame;
};

#endif