#include "RectPrism.h"
#include <iostream>
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

RectPrism* RectPrism::selectedPrism = nullptr;

RectPrism::RectPrism() : GameObject() {
}

RectPrism::RectPrism(string name) : GameObject(name) {
}

void RectPrism::setShaderParameters() {
	GLint selectedUni = shader->getUniform("selected");
	bool selected = (RectPrism::selectedPrism == this);
	glUniform1i(selectedUni, (GLint)selected);
}

