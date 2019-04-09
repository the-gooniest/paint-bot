#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::lastTime = 0.0f;

void Time::InitTime() {
	glfwSetTime(0.0);
	deltaTime = 0.0f;
	lastTime = 0.0f;
}

void Time::UpdateTime() {
	float currentTime = (float)glfwGetTime();
	deltaTime = (float)glfwGetTime() - lastTime;
	lastTime = currentTime;
}

float Time::GetTime() {
	return (float)glfwGetTime();
}