/*
This class deals with all the glfw functions - it acts more like a wrapper for convenience.
written by Julius Neudecker
v0.1 22.07.2020
v0.2 27.07.2020 - added openGL Debugging interface
v0.2 28.07.2020 - streamlined the initialization-call

*/

#include <glfwHandler.h>
#include <glfwCallback.h>

glfwHandler::glfwHandler() {

	try {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, USE_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, USE_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4); //for MSAA -> no jagged edges
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); //for debugging -> don't use for productive rendering.

		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);

		if (!window) {
			jLog::Instance()->glError(getErrorMsg());
			throw 0;
		}

		// timing
		_deltaTime = 0.0f;
		_lastFrame = 0.0f;

		//setup camera
		SceneCamera = new fpvCam();
		SceneCamera->camera = Camera(glm::vec3(0.0f, 3.0f, 10.0f));
		SceneCamera->lastX = SCR_WIDTH / 2.0f;
		SceneCamera->lastY = SCR_HEIGHT / 2.0f;
		SceneCamera->firstMouse = true;

		jLog::Instance()->Log(INFO, "GLFW Handler successfully instantiated.");
	}
	catch (int e) {
		jLog::Instance()->Error("Error creating glfwHandler.");
	}
}

unsigned int glfwHandler::init() {

	if (window == NULL) {
		jLog::Instance()->Error("Failed to create GLFW Window");
		glfwTerminate();
		return 0;
	}
	jLog::Instance()->Log(INFO, "Window succesfully created.");

	glfwMakeContextCurrent(window);
	jLog::Instance()->Log(INFO, "Context succesfully created.");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		jLog::Instance()->Error("Failed to init GLAD");
		glfwTerminate();
		return 0;
	}
	//for MSAA -> no jagged edges
	glEnable(GL_MULTISAMPLE);

	jLog::Instance()->Log(INFO, "openGL succesfully loaded. Creating Viewport...");

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// enable this for openGL debugging. Disable for productive use, because performance!
	_configDebugContext();
	_configContext();

	return 1;
}

void glfwHandler::_configDebugContext() {

	int flags;

	jLog::Instance()->Log(INFO, "Configuring GLFW Debug Context...");

	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(_glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		jLog::Instance()->Log(INFO, "Configuring GLFW Debug Context successful.");

	}
	else {
		jLog::Instance()->Log(INFO, "Configuring GLFW Debug Context failed.");
	}

}

void glfwHandler::SetInputMode(int mode, int value) {
	glfwSetInputMode(window, mode, value);
	jLog::Instance()->Log(INFO, "GLFW InputMode set.");
}

fpvCam* glfwHandler::giveCamera() {
	return SceneCamera;
}

void glfwHandler::SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
	jLog::Instance()->Log(INFO, "Registered framebuffer callback.");
	glfwSetFramebufferSizeCallback(window, callback);
}
void glfwHandler::SetCursorPosCallback(GLFWcursorposfun callback) {
	jLog::Instance()->Log(INFO, "Registered cursor position callback.");
	glfwSetCursorPosCallback(window, callback);
}
void glfwHandler::SetScrollCallback(GLFWscrollfun callback) {
	jLog::Instance()->Log(INFO, "Registered scrolling callback.");
	glfwSetScrollCallback(window, callback);
}

bool glfwHandler::WindowShouldClose() {	
	return glfwWindowShouldClose(window);
}

void glfwHandler::processInput(float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		jLog::Instance()->Log(INFO, "User chose to close window...");
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		SceneCamera->camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		SceneCamera->camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		SceneCamera->camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		SceneCamera->camera.ProcessKeyboard(RIGHT, deltaTime);
}

void glfwHandler::SwapBuffers() {
	glfwSwapBuffers(window);
}
void glfwHandler::PollEvents() {
	glfwPollEvents();
}

void glfwHandler::Terminate() {
	jLog::Instance()->Log(WARNING, "Terminating glfw!");
	glfwTerminate();
}

std::string glfwHandler::getErrorMsg() {
	const char* description;
	glfwGetError(&description);

	return description;
}

void glfwHandler::_configContext() {

	jLog::Instance()->Log(INFO, "Configuring application context...");


	//SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED); //lock cursor to screen
	SetFramebufferSizeCallback(framebuffer_size_callback);
	SetCursorPosCallback(mouse_callback);
	SetScrollCallback(scroll_callback);

	//configure openGL state
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	jLog::Instance()->Log(INFO, "OpenGL application context successfully configured.");
}

void glfwHandler::BeginRenderLoop() {

	//temp variables
	double currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
	
	//check inputs
	processInput(_deltaTime); //parameter existed outside of class before. Left the declaration as it was.

	//prerequisites
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void glfwHandler::ConcludeRenderLoop() {
	//display and get callbacks
	SwapBuffers();
	PollEvents();
}

fpvCam* glfwHandler::SceneCamera = 0;