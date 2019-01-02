#include "MyWindow.h"


// CONSTRUCTORS
MyWindow::MyWindow() {
	width = 800;
	height = 600;
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
}


int MyWindow::Initialize() {
	// INITIALIZE GLFW
	if (!glfwInit()) {
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
	}

	// // SETUP GLFW WINDOW PROPERTIES
	// OPENGL VERSION
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// CORE PROFILE = NO BACKWARDS COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ALLOW FORWARD COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "OpenGLConsoleAppWindow", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// GET BUFFER SIZE INFORMATION
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// SET CONTEXT FOR GLEW TO USE
	glfwMakeContextCurrent(mainWindow);

	// ALLOW MODERN EXTENSION FEATURES
	glewExperimental = GL_TRUE;

	// INITIALIZE GLEW
	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// ENABLE DEPTH BUFFER
	glEnable(GL_DEPTH_TEST);

	// SETUP VIEWPORT SIZE
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}

// DESTRUCTOR
MyWindow::~MyWindow() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
