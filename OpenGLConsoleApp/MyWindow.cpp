#include "MyWindow.h"

// CONSTRUCTORS
MyWindow::MyWindow() {
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
}


// GETTER FUNCTIONS
GLfloat MyWindow::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat MyWindow::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


int MyWindow::Initialize() {
	if (!glfwInit()) {                                                                      	// INITIALIZE GLFW
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
	}

	// // SETUP GLFW WINDOW PROPERTIES
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                           	// OPENGL VERSION
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                          	// CORE PROFILE = NO BACKWARDS COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                                    	// ALLOW FORWARD COMPATIBILITY

	mainWindow = glfwCreateWindow(width, height, "OpenGLConsoleAppWindow", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);                        	// GET BUFFER SIZE INFORMATION
	glfwMakeContextCurrent(mainWindow);                                                     	// SET CONTEXT FOR GLEW TO USE

	CreateCallbacks();                                                                       	// HANDLE KEY + MOUSE INPUT

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                        	// TO HIDE THE CURSOR IN THE WINDOW
	glewExperimental = GL_TRUE;                                                             	// ALLOW MODERN EXTENSION FEATURES

	if (glewInit() != GLEW_OK) {                                                            	// INITIALIZE GLEW
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);                                                                	// ENABLE DEPTH BUFFER
	glViewport(0, 0, bufferWidth, bufferHeight);                                            	// SETUP VIEWPORT SIZE
	glfwSetWindowUserPointer(mainWindow, this);                                             	// SET USER OF THIS WINDOW

	return 0;
}

void MyWindow::CreateCallbacks() {
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void MyWindow::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	MyWindow *theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
			// printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
			// printf("Released: %d\n", key);

		}
	}
}

void MyWindow::HandleMouse(GLFWwindow *window, double xPos, double yPos) {
	MyWindow *theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));
	
	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;	                                          // TO MATCH THE Y DIRECTION OF THE COORDINATE SYSTEM 

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	
	// printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}



// DESTRUCTOR
MyWindow::~MyWindow() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
