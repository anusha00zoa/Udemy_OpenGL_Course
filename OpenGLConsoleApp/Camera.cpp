#include "Camera.h"

// CONSTRUCTOR
Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, 0.0f);
	right = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 0.0f, 0.0f);
	
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;

	moveSpeed = 0.0f;
	turnSpeed = 0.0f;
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	roll = 0.0f;

	Update();
}

void Camera::KeyControl(bool *keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {	// MOVE FORWARD
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) { // MOVE BACKWARD
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A]) { // MOVE LEFT
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D]) { // MOVE RIGHT
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	Update();
}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}


void Camera::Update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);

	// RIGHT = CROSS PRODUCT OF FRONT AND WORLD UP
	right = glm::normalize(glm::cross(front, worldUp));
	// UP = CROSS PRODUCT OF FRONT AND RIGHT, ORDER IS IMPORTANT
	up = glm::normalize(glm::cross(right, front));
}

// DESTRUCTOR
Camera::~Camera()
{
}
