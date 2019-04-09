#pragma once
#include "GameObject.h"

class RectPrism : public GameObject {
	

public:
	static RectPrism* selectedPrism;
	RectPrism();
	RectPrism(std::string name);
	void setShaderParameters();
};
