#include "Input.h"
#include "Camera.h"
#include <iostream>

using namespace std;
using namespace glm;

int Input::penWidth;
int Input::penWidthMax;
int Input::penWidthIncrement;
unordered_set<int> Input::keysPressed;
vec2 Input::camDirection;
float Input::scrollWheel;
float Input::link1Direction;
float Input::link2Direction;
float Input::link3Direction;
bool Input::automateToggle;

void Input::InitInput() {
	camDirection = vec2(0,0);
	scrollWheel = 0.0f;
	link1Direction = 0.0f;
	link2Direction = 0.0f;
	link3Direction = 0.0f;
	penWidth = 1;
	penWidthMax = 30;
	penWidthIncrement = 2;
	automateToggle = false;
}

bool Input::IsKeyPressed(int key) {
	return (keysPressed.find(key) != keysPressed.end());
}

template <typename T>
void Input::ToggleValue(T &value, T value1, T value2) {
	if (value == value1)
		value = value2;
	else
		value = value1;
}

void Input::IncrementKey(int &prop, int key1, int key2, int min, int max, int increm) {
	if (IsKeyPressed(key1) && !IsKeyPressed(key2)) {
		if (penWidth - increm > min)
			penWidth -= increm;
		else
			penWidth = min;
	}
	else if (!IsKeyPressed(key1) && IsKeyPressed(key2)) {
		if (penWidth + increm < max)
			penWidth += increm;
		else
			penWidth = max;
	}
}

void Input::BindDirectionalKeys(float &prop, int key1, int key2) {
	if (IsKeyPressed(key1) && !IsKeyPressed(key2)) {
		prop = 1.0f;
	}
	else if (!IsKeyPressed(key1) && IsKeyPressed(key2)) {
		prop = -1.0f;
	}
	else {
		prop = 0.0f;
	}
}

void Input::UpdateKeyboardInput(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		keysPressed.insert(key);
	}

	if (action == GLFW_RELEASE) {
		keysPressed.erase(key);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	BindDirectionalKeys(link1Direction, GLFW_KEY_COMMA, GLFW_KEY_SLASH);
	BindDirectionalKeys(link2Direction, GLFW_KEY_K, GLFW_KEY_SEMICOLON);
	BindDirectionalKeys(link3Direction, GLFW_KEY_I, GLFW_KEY_P);
	BindDirectionalKeys(camDirection.x, GLFW_KEY_A, GLFW_KEY_D);
	BindDirectionalKeys(camDirection.y, GLFW_KEY_S, GLFW_KEY_W);
	IncrementKey(penWidth, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, 1, penWidthMax, penWidthIncrement);

	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		ToggleValue(Camera::MainCamera()->projection, Camera::CameraType::PERSPECTIVE, Camera::CameraType::ORTHOGRAPHIC);
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		ToggleValue(automateToggle, true, false);
	}

	/*
	if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
	if (compIndex == 2)
	compIndex = 0;
	else
	compIndex++;
	int index = compIndex;
	RectPrism::selectedPrism = (RectPrism*)depthFirstSearch(&index, link1->getTransform())->gameObject;
	}

	if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
	if (compIndex == 0)
	compIndex = 2;
	else
	compIndex--;

	int index = compIndex;
	RectPrism::selectedPrism = (RectPrism*)depthFirstSearch(&index, link1->getTransform())->gameObject;
	}*/
}

void Input::UpdateScrollWheelInput(GLFWwindow* window, double xoffset, double yoffset) {
	scrollWheel = (float)yoffset;
}

void Input::LateUpdate() {
	scrollWheel = 0.0f;
}
