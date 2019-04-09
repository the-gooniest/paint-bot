#define _USE_MATH_DEFINES
#include "GameObject.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Time.h"

using namespace std;
using namespace glm;

GameObject::GameObject() : GameObject("GameObject") {
}

GameObject::GameObject(string name) {
	transform = new Transform(this);
	this->mesh = nullptr;
	this->shader = nullptr;
	this->name = name;
}

GameObject::~GameObject(){
	delete transform;
}

void GameObject::draw(mat4 P) {
	MatrixStack MV;
	MV.pushMatrix();
	MV.loadIdentity();
	draw(MV, P);
}

void GameObject::draw(MatrixStack &mStack, mat4 P) {
	mStack.pushMatrix();

	// Shift coordinate frame
	mStack.translate(transform->localPosition);

	// Rotate Frame
	mStack.rotate(transform->LocalRotation().x * (float)M_PI / 180.0f, 1, 0, 0);
	mStack.rotate(transform->LocalRotation().y * (float)M_PI / 180.0f, 0, 1, 0);
	mStack.rotate(transform->LocalRotation().z * (float)M_PI / 180.0f, 0, 0, 1);

	// Scale proceeding Heiarchy of GameObjects
	mStack.scale(transform->localScale);

	// Reorient the Mesh's Position
	mStack.translate(transform->centerOffset);

	// Draw child GameObjects
	for (Transform* child : transform->Children()) {
		child->gameObject->draw(mStack, P);
	}

	if (mesh != nullptr && shader != nullptr) {
		// Scale the GameObject's Mesh
		mStack.scale(meshScale);

		// Calculate Global Position
		mat4 topM = mStack.topMatrix();
		vec3 origin = vec3(0, 0, 0);
		vec4 newOrigin = vec4(origin, 1.0f);
		newOrigin = topM * newOrigin;
		transform->worldPosition = vec2(newOrigin.x, newOrigin.y);

		// Draw GameObject
		if (mesh != nullptr) {
			mesh->bind(shader);
			glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(P));
			glUniformMatrix4fv(shader->getUniform("MV"), 1, GL_FALSE, value_ptr(mStack.topMatrix()));
			glUniform1f(shader->getUniform("time"), (GLfloat)Time::GetTime());
			setShaderParameters();
			glDrawArrays(GL_TRIANGLES, 0, mesh->vertCount());
			glBindTexture(GL_TEXTURE_2D, 0);
			mesh->unbind(shader);
		}
	}

	// Undo coordinate frame shift
	mStack.popMatrix();
}

void GameObject::setShaderParameters() {

}
