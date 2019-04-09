#pragma once
#include <unordered_set>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GameObject;

class Input {
	static int penWidth, penWidthMax, penWidthIncrement;
	static std::unordered_set<int> keysPressed;
	static glm::vec2 camDirection;
	static float scrollWheel;
	static float link1Direction, link2Direction, link3Direction;
	static bool automateToggle;

public:
	static void InitInput();
	static bool IsKeyPressed(int key);

	template <typename T>
	static void ToggleValue(T &value, T value1, T value2);

	static void IncrementKey(int &prop, int key1, int key2, int min, int max, int increm);
	static void BindDirectionalKeys(float &prop, int key1, int key2);
	static void UpdateKeyboardInput(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void UpdateScrollWheelInput(GLFWwindow* window, double xoffset, double yoffset);
	static float ScrollWheel() { return scrollWheel; }
	static glm::vec2 CamDirection() { return camDirection; }
	static float Link1Direction() { return link1Direction; }
	static bool Automating() { return automateToggle; }
	static float Link2Direction() { return link2Direction; }
	static float Link3Direction() { return link3Direction; }
	static int PenWidth() { return penWidth; }
	static void PenWidth(int width) { penWidth = width; }

	static void LateUpdate();
};