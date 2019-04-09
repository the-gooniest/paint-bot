#pragma once
#include "GameObject.h"
#include "GLSL.h"

class Canvas : public GameObject {
	int width, height;
	bool primed;
	int canvasTexID;
	GLubyte *canvasBuffer;
	glm::vec2 lastBrushPos;
	GameObject* brush;
public:

	Canvas();
	Canvas(std::string name);
	void setCanvas(int width, int height, int canvasTexID, GLubyte *buffer);
	void setBrush(GameObject* brush) { this->brush = brush; }
	void updateBrushPos(glm::vec2 pos) { lastBrushPos = pos * (width / meshScale.x) + glm::vec2(width/2, height/2); }
	void setShaderParameters();
	void drawOnCanvas(int lineWidth, glm::vec4 color);
	void drawPixels(int x, int y, glm::vec4 color, int penSize);
	void DrawLine(int x1, int y1, int x2, int y2, glm::vec4 color, int lineWidth);
	glm::vec4 interpAdd(glm::vec4 ci, glm::vec4 cf);
};