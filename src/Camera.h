#pragma once
#include "GameObject.h"

class Camera : public GameObject {
public:
	enum CameraType {
		PERSPECTIVE,
		ORTHOGRAPHIC
	};
	Camera();
	Camera(std::string name);
	void setCamProperties(int width, int height);
	glm::mat4 getCameraMatrix();


	static Camera* MainCamera() { return mainCamera; }
	CameraType projection;
private:
	static Camera* mainCamera;
	int width, height;
	
};