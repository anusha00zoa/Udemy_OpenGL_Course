#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	// GETTER FUNCTIONS
	GLfloat getBufferWidth() { return (GLfloat)bufferWidth;  }
	GLfloat getBufferHeight() { return (GLfloat)bufferHeight;  }
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();


	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow);  }

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~MyWindow();

private:
	GLFWwindow *mainWindow;

	GLint width, 
		height, 
		bufferWidth, 
		bufferHeight;

	void CreateCallbacks();

	// HANDLE KEYBOARD INPUT
	bool keys[1024];	// TO STORE STATE OF ALL ASCII CHARACTERS
	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);

	// HANDLE MOUSE INPUT
	GLfloat lastX,
		lastY,
		xChange,
		yChange;
	bool mouseFirstMoved;
	static void HandleMouse(GLFWwindow *window, double xPos, double yPos);


};

