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

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow);  }

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~MyWindow();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

