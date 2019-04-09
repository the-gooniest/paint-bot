#pragma once
#include <glm/glm.hpp>
#include "unordered_set"

class GameObject;

class Transform {

	Transform* parent;
	std::unordered_set<Transform*> children;
	glm::vec3 localRotation;

public:
	glm::vec3 localPosition;

	glm::vec3 centerOffset;
	glm::vec3 localScale;
	glm::vec3 meshScale;
	glm::vec2 worldPosition;
	GameObject* gameObject;

	Transform();
	Transform(GameObject* object);
	~Transform();

	glm::vec2 ScreenPosition() { return worldPosition; }

	Transform* Parent() { return parent; }
	void Parent(Transform* parent);

	int ChildCount() { return children.size(); }
	Transform* Child(size_t ithChild);
	std::unordered_set<Transform*>& Children() { return children; }
	size_t GroupSize();

	float ClampRotation(float angle);
	glm::vec3 LocalRotation() { return localRotation; }
	void LocalRotation(glm::vec3 localRotation);
};