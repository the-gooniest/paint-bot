#include <iostream>

#include "Transform.h"
#include "GameObject.h"

using namespace std;
using namespace glm;

Transform::Transform() : Transform(nullptr) {
}

Transform::Transform(GameObject* object) {
	this->localPosition = vec3(0, 0, 0);
	this->localRotation = vec3(0, 0, 0);
	this->centerOffset = vec3(0, 0, 0);
	this->localScale = vec3(1.0, 1.0, 1.0);
	this->parent = nullptr;
	gameObject = object;
}

Transform::~Transform() {
	
	if (parent != nullptr) {
		parent->Children().erase(this);
	}

	if (children.size() < 1)
		return;

	for (auto iter = children.begin(); iter != children.end();) {
		Transform* child = *iter;
		iter = children.erase(iter);
		if (child->gameObject != nullptr) {
			delete child->gameObject;
		}		
		else {
			delete child;
		}
	}
}

Transform* Transform::Child(size_t ithChild) {
	if (ithChild >= children.size()) {
		cout << "Accessing non-existant child\n";
		return nullptr;
	}

	auto childitr = children.begin();
	size_t index = 0;
	while (index < ithChild) {
		childitr++;
		index++;
	}
	return *childitr;
}

size_t Transform::GroupSize() {
	if (children.size() == 0)
		return 1;

	size_t size = 1;
	for (Transform* child : children)
		size += child->GroupSize();
	return size;
}

void Transform::Parent(Transform* parent) {
	if (this->parent != nullptr) {
		this->parent->Children().erase(parent);
	}
	this->parent = parent;
	parent->Children().insert(this);
}

float Transform::ClampRotation(float angle) {
	angle = (float)fmod(angle, 360);
	if (angle < -180) {
		angle = angle + 360;
	}
	else if (angle > 180) {
		angle = angle - 360;
	}
	return angle;
}

void Transform::LocalRotation(glm::vec3 localRotation) {
	this->localRotation.x = ClampRotation(localRotation.x);
	this->localRotation.y = ClampRotation(localRotation.y);
	this->localRotation.z = ClampRotation(localRotation.z);
}
