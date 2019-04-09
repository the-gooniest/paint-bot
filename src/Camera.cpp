#define _USE_MATH_DEFINES
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "MatrixStack.h"


using namespace std;
using namespace glm;

Camera* Camera::mainCamera = nullptr;

Camera::Camera() : Camera("camera") {}

Camera::Camera(string name) : GameObject(name) {
	if (mainCamera == nullptr)
		mainCamera = this;
	this->projection = ORTHOGRAPHIC;
}

void Camera::setCamProperties(int width, int height) {
	this->width = width;
	this->height = height;
}

mat4 Camera::getCameraMatrix() {
	float aspect = width / (float)height;
	MatrixStack camMatrix;
	camMatrix.pushMatrix();
	camMatrix.loadIdentity();

	// Set up projection matrix (camera intrinsics)
	mat4 P;
	if (projection == PERSPECTIVE) {
		P = perspective((float)(45.0*M_PI / 180.0), aspect, 1.0f, 10000.0f);
		camMatrix.multMatrix(P);
		camMatrix.translate(-vec3(0,0,transform->localPosition.z));
		camMatrix.rotate(transform->LocalRotation());
	}
	else {
		P = ortho(0.0f, (float)width, 0.0f, (float)height, -10000.0f, 10000.0f);
		camMatrix.multMatrix(P);

		camMatrix.translate((float)width / 2.0f, (float)height / 2.0f, 0.0f);
		camMatrix.translate(transform->localPosition);

		float scale = width / transform->localPosition.z;
		camMatrix.scale(vec3(scale, scale, scale));
	}
	return camMatrix.topMatrix();
}