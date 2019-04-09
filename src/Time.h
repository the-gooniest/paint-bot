#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Time {
	static float deltaTime;
	static float lastTime;
public:
	static float DeltaTime() { return Time::deltaTime; }
	static void InitTime();
	static void UpdateTime();
	static float GetTime();
};