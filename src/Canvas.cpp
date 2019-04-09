#include "Canvas.h"
#include <iostream>
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;


Canvas::Canvas() : GameObject() {
}

Canvas::Canvas(string name) : GameObject(name) {
}

void Canvas::setCanvas(int width, int height, int canvasTexID, GLubyte *buffer) {
	this->width = width;
	this->height = height;
	this->canvasTexID = canvasTexID;
	this->canvasBuffer = buffer;
	primed = false;
}

void Canvas::setShaderParameters() {
	glUniform1f(shader->getUniform("time"), (GLfloat)glfwGetTime());

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shader->getTexture());
}

vec4 Canvas::interpAdd(vec4 ci, vec4 cf) {
	float totalWeight = ci.a + cf.a;
	float weightI = ci.a / totalWeight;
	float weightF = cf.a / totalWeight;
	vec4 result = ci * weightI + cf * weightF;
	//cout << r.r << " " << r.g << " " << r.b << "\n";
	return result;
}

/*
void drawPixel(int x, int y, vec4 color) {
int index = (y * width + x) * 4;

GLuint point[] = {
(GLuint)(255 * color.r),
(GLuint)(255 * color.g),
(GLuint)(255 * color.b),
(GLuint)(255 * color.a)
};

glEnable(GL_TEXTURE_2D);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, canvasTexID);
glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, point);
GLSL::checkError(GET_FILE_LINE);
glBindTexture(GL_TEXTURE_2D, 0);
}*/

void Canvas::drawPixels(int x, int y, vec4 color, int penSize) {
	if (penSize > 40)
		penSize = 40;
	else if (penSize < 1)
		penSize = 1;

	int index = (y * width + x) * 4;
	GLubyte *sub = new GLubyte[penSize * penSize * 4];

	int xinital = x - (penSize / 2);
	int yinital = y - (penSize / 2);
	for (int j = 0; j < penSize; j++) {
		for (int i = 0; i < penSize; i++) {
			int index = (j*penSize + i) * 4;
			int canvasIndex = ((j + yinital) * width + (i + xinital)) * 4;
			sub[index] = canvasBuffer[canvasIndex];
			sub[index + 1] = canvasBuffer[canvasIndex + 1];
			sub[index + 2] = canvasBuffer[canvasIndex + 2];
			sub[index + 3] = canvasBuffer[canvasIndex + 3];
		}
	}

	for (int j = 0; j < penSize; j++) {
		for (int i = 0; i < penSize; i++) {
			int di = i - (penSize / 2);
			int dj = j - (penSize / 2);
			if ((di*di + dj*dj) >(penSize*penSize) / 4)
				continue;
			int index = (j*penSize + i) * 4;
			vec4 newColor = interpAdd(vec4(sub[index] / 255.0f, sub[index + 1] / 255.0f, sub[index + 2] / 255.0f, 1.0f), color);
			int canvasIndex = ((j + yinital) * width + (i + xinital)) * 4;
			sub[index] = (GLuint)(newColor.r * 255);
			sub[index + 1] = (GLuint)(newColor.g * 255);
			sub[index + 2] = (GLuint)(newColor.b * 255);
			canvasBuffer[canvasIndex] = sub[index];
			canvasBuffer[canvasIndex + 1] = sub[index + 1];
			canvasBuffer[canvasIndex + 2] = sub[index + 2];

			//cout << sub[index] << " " << sub[index + 1] << " " << sub[index + 2] << "\n";
		}
	}

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, canvasTexID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xinital, yinital, penSize, penSize, GL_RGBA, GL_UNSIGNED_BYTE, sub);
	GLSL::checkError(GET_FILE_LINE);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] sub;
}

void Canvas::DrawLine(int x1, int y1, int x2, int y2, vec4 color, int lineWidth)
{
	int cx, cy,
		ix, iy,
		dx, dy,
		ddx = x2 - x1, ddy = y2 - y1;

	if (!ddx) { //vertical line special case
		if (ddy > 0) {
			cy = y1;
			do drawPixels(x1, cy++, color, lineWidth);
			while (cy <= y2);
			return;
		}
		else {
			cy = y2;
			do drawPixels(x1, cy++, color, lineWidth);
			while (cy <= y1);
			return;
		}
	}
	if (!ddy) { //horizontal line special case
		if (ddx > 0) {
			cx = x1;
			do drawPixels(cx, y1, color, lineWidth);
			while (++cx <= x2);
			return;
		}
		else {
			cx = x2;
			do drawPixels(cx, y1, color, lineWidth);
			while (++cx <= x1);
			return;
		}
	}
	if (ddy < 0) { iy = -1; ddy = -ddy; }//pointing up
	else iy = 1;
	if (ddx < 0) { ix = -1; ddx = -ddx; }//pointing left
	else ix = 1;
	dx = dy = ddx*ddy;
	cy = y1, cx = x1;
	if (ddx < ddy) { // < 45 degrees, a tall line    
		do {
			dx -= ddy;
			do {
				drawPixels(cx, cy, color, lineWidth);
				cy += iy, dy -= ddx;
			} while (dy >= dx);
			cx += ix;
		} while (dx > 0);
	}
	else { // >= 45 degrees, a wide line
		do {
			dy -= ddx;
			do {
				drawPixels(cx, cy, color, lineWidth);
				cx += ix, dx -= ddy;
			} while (dx >= dy);
			cy += iy;
		} while (dy > 0);
	}
}

void Canvas::drawOnCanvas(int lineWidth, vec4 color) {
	vec2 pos = brush->getTransform()->ScreenPosition();
	pos = pos * (width / meshScale.x) + vec2(width / 2, height / 2);
	if (pos == lastBrushPos)
		return;
	if ((pos - lastBrushPos).length() > 100.0f)
		return;
	if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y > height)
		return;
	if (lastBrushPos.x < 0 || lastBrushPos.x >= width || lastBrushPos.y < 0 || lastBrushPos.y > height)
		return;
	DrawLine((int)lastBrushPos.x, (int)lastBrushPos.y, (int)pos.x, (int)pos.y, color, lineWidth);
}
