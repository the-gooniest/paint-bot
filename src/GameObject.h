#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Transform.h"
#include "unordered_set"

class MatrixStack;
class ShaderProgram;

class GameObject {
protected:

	Mesh* mesh;
	ShaderProgram* shader;
	Transform* transform;

public:
	std::string name;
	glm::vec3 meshScale;

	GameObject();
	GameObject(std::string name);
	~GameObject();

	void draw(glm::mat4 P);
	void draw(MatrixStack &mStack, glm::mat4 P);

	virtual void setShaderParameters();
	ShaderProgram* getShader() { return shader; }
	void setShader(ShaderProgram* shader) { this->shader = shader; }

	Mesh* getMesh() { return mesh; }
	void setMesh(Mesh* mesh) { this->mesh = mesh; }

	Transform* getTransform() { return transform; }
};