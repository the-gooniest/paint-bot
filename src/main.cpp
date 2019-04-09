#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "GLSL.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Canvas.h"
#include "RectPrism.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include <stack>
#include "SOIL.h"
#include <unordered_set>

using namespace std;
using namespace glm;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<ShaderProgram> cubeShaders, planeShaders;
shared_ptr<Mesh> cubeShape, planeShape;
shared_ptr<GameObject> world, canvas, base, link1, link2, link3, brush;
shared_ptr<Camera> camera;

int compIndex;
int width = 1024, height = 1024;
GLuint canvasTexID;
unsigned char *canvasBuffer;
float rotationSpeed = 100.0f;
float cameraSpeed = 200.0f;
vec2 brushPos = vec2(width / 2, height-1);

Transform* depthFirstSearch(int* index, Transform* transform) {
	if ((*index) == 0)
		return transform;
	(*index)--;
	for (auto childObject : transform->Children()) {
		Transform* returnedObject = depthFirstSearch(index, childObject);
		if (returnedObject != nullptr) {
			return returnedObject;
		}		
	}
	return nullptr;
}

// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when the mouse is clicked
static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Do nothing
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// General setup ---------------------------------------------------------
	Time::InitTime();
	Input::InitInput();

	// Set background color.
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	// Shader Programs Setup ----------------------------------------------------
	string vShaderName = RESOURCE_DIR + "vert.glsl";
	string fShaderName = RESOURCE_DIR + "frag.glsl";
	cubeShaders = shared_ptr<ShaderProgram>(new ShaderProgram(vShaderName, fShaderName));
	cubeShaders->addUniform("selected");
	cubeShaders->addUniform("time");

	vShaderName = RESOURCE_DIR + "canvas_vert.glsl";
	fShaderName = RESOURCE_DIR + "canvas_frag.glsl";
	planeShaders = shared_ptr<ShaderProgram>(new ShaderProgram(vShaderName, fShaderName));
	planeShaders->addAttribute("texChoord");
	planeShaders->addUniform("time");
	string texPath = RESOURCE_DIR + "canvas.jpg";
	int texWidth = 2048, texHeight = 2048;
	canvasTexID = planeShaders->generateTexture(texPath, texWidth, texHeight, &canvasBuffer);
	
	// Mesh setup ----------------------------------------------------------
	string meshName = RESOURCE_DIR + "cube.obj";
	cubeShape = shared_ptr<Mesh>(new Mesh(meshName));

	meshName = RESOURCE_DIR + "plane.obj";
	planeShape = shared_ptr<Mesh>(new Mesh(meshName));

	// Define World Object --------------------------------------------------
	world = shared_ptr<GameObject>(new GameObject("world"));
	
	// Define Camera
	camera = shared_ptr<Camera>(new Camera());
	camera->getTransform()->localPosition.z = 1024;
	camera->getTransform()->LocalRotation(vec3(-45,0,-45));

	// Define Canvas --------------------------------------------------------
	canvas = shared_ptr<GameObject>(new Canvas("Canvas"));
	dynamic_cast<Canvas*>(canvas.get())->setCanvas(texWidth, texHeight, canvasTexID, canvasBuffer);
	canvas->meshScale = vec3(1024.0f, 1024.0f, 1.0f);
	canvas->setMesh(planeShape.get());
	canvas->setShader(planeShaders.get());
	canvas->getTransform()->Parent(world.get()->getTransform());

	// Define Robot Body ----------------------------------------------------
	base = shared_ptr<GameObject>(new RectPrism("base"));
	base->meshScale = vec3(10.0f, 10.0f, 100.0f);
	base->getTransform()->localPosition = vec3(0, 0, 51.0f);
	base->setMesh(cubeShape.get());
	base->setShader(cubeShaders.get());
	base->getTransform()->Parent(world.get()->getTransform());

	link1 = shared_ptr<GameObject>(new RectPrism("link1"));
	link1->getTransform()->centerOffset = vec3(0, 75.0f, 0.0f);
	link1->getTransform()->localPosition = vec3(0, 0.0f, 50.0f);
	link1->meshScale = vec3(15.0f, 150.0f, 15.0f);
	link1->setMesh(cubeShape.get());
	link1->setShader(cubeShaders.get());
	link1->getTransform()->Parent(base.get()->getTransform());

	link2 = shared_ptr<GameObject>(new RectPrism("link2"));
	link2->getTransform()->localPosition = vec3(0, 75.0f, -10.0f);
	link2->getTransform()->centerOffset = vec3(0, 50.0f, 0);
	link2->meshScale = vec3(10.0f, 100.0f, 10.0f);
	link2->setMesh(cubeShape.get());
	link2->setShader(cubeShaders.get());
	link2->getTransform()->Parent(link1.get()->getTransform());

	link3 = shared_ptr<GameObject>(new RectPrism("link3"));
	link3->getTransform()->localPosition = vec3(0, 50.0f, -5.0f);
	link3->getTransform()->centerOffset = vec3(0, 37.5f, 0);
	link3->meshScale = vec3(5.0f, 75.0f, 5.0f);
	link3->setMesh(cubeShape.get());
	link3->setShader(cubeShaders.get());
	link3->getTransform()->Parent(link2.get()->getTransform());

	brush = shared_ptr<GameObject>(new RectPrism("brush"));
	brush->getTransform()->localPosition = vec3(0, 37.5f, -40.0f);
	brush->meshScale = vec3(5.0f, 5.0f, 80.0f);
	brush->setMesh(cubeShape.get());
	brush->setShader(cubeShaders.get());
	brush->getTransform()->Parent(link3.get()->getTransform());
	dynamic_cast<Canvas*>(canvas.get())->setBrush(brush.get());
}

void AnimateRobot() {
	link1->getTransform()->LocalRotation(vec3(0, 0, link1->getTransform()->LocalRotation().z + Time::DeltaTime() * 200.0f));
	link2->getTransform()->LocalRotation(vec3(0, 0, link2->getTransform()->LocalRotation().z + Time::DeltaTime() * 202.0f));
	link3->getTransform()->LocalRotation(vec3(0, 0, link3->getTransform()->LocalRotation().z + Time::DeltaTime() * 204.0f));
	//Input::PenWidth((int)(1 + 5*cos(Time::GetTime())));
}

void MoveRobot() {
	if (Input::Automating()) {
		AnimateRobot();
		return;
	}
	vec3 linkRot = link1->getTransform()->LocalRotation();
	link1->getTransform()->LocalRotation(vec3(
		linkRot.x,
		linkRot.y,
		linkRot.z + (rotationSpeed * Input::Link1Direction() * Time::DeltaTime())
	));
	linkRot = link2->getTransform()->LocalRotation();
	link2->getTransform()->LocalRotation(vec3(
		linkRot.x,
		linkRot.y,
		linkRot.z + (rotationSpeed * Input::Link2Direction() * Time::DeltaTime())
	));
	linkRot = link3->getTransform()->LocalRotation();
	link3->getTransform()->LocalRotation(vec3(
		linkRot.x,
		linkRot.y,
		linkRot.z + (rotationSpeed * Input::Link3Direction() * Time::DeltaTime())
	));
}

void MoveCamera() {
	vec3 &camPos = camera->getTransform()->localPosition;
	float scrollDistMax = 2048.0f, scrollDistMin = 200.0f;
	float scrollDist = -Input::ScrollWheel() * 3000.0f * Time::DeltaTime();
	float newCamZ = camPos.z + scrollDist;
	if (scrollDist > 0) {
		if (newCamZ < scrollDistMax)
			camPos.z = newCamZ;
		else
			camPos.z = scrollDistMax;
	}
	else if (scrollDist < 0) {
		if (newCamZ > scrollDistMin)
			camPos.z = newCamZ;
		else
			camPos.z = scrollDistMin;
	}

	if (camera->projection == Camera::CameraType::ORTHOGRAPHIC) {
		camPos.x += cameraSpeed * 5.0f * Input::CamDirection().x * Time::DeltaTime();
		camPos.y -= cameraSpeed * 5.0f * -Input::CamDirection().y * Time::DeltaTime();
	}
	else {
		vec3 camRotation = camera->getTransform()->LocalRotation();
		camera->getTransform()->LocalRotation(camRotation + vec3(Input::CamDirection().y, 0, Input::CamDirection().x) * Time::DeltaTime());
	}
}

// This function is called every frame to draw the scene.
static void render()
{
	Time::UpdateTime();

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Check for resized window
	glfwGetFramebufferSize(window, &width, &height);
	camera->setCamProperties(width, height);

	// Update Positions
	MoveCamera();
	MoveRobot();

	// Draw World
	world->draw(camera->getCameraMatrix());

	// Update Canvas
	Canvas* canvasPtr = dynamic_cast<Canvas*>(canvas.get());
	if (Input::IsKeyPressed(GLFW_KEY_SPACE) || Input::Automating())
		canvasPtr->drawOnCanvas(Input::PenWidth(), vec4(0.5f, 0.0f, 0.0f, 1.0f));
	canvasPtr->updateBrushPos(brush->getTransform()->ScreenPosition());

	Input::LateUpdate();
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);

	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}


	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(width, height, "Steven Leal", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW.
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}

	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLSL::checkVersion();

	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Make everything real smooth
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, Input::UpdateKeyboardInput);
	// Set the mouse call back.
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, Input::UpdateScrollWheelInput);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);

	// Initialize scene.
	init();

	// Main Loop
	while(!glfwWindowShouldClose(window)) {
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
