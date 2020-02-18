#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

float r=0;
float g=0;
float b=1;
float a = 1;
std::vector<string> name;
int crtObjIndex = 0;
glm::vec3 playerPosition = glm::vec3(-2, 0.5f, 0);
float startTimeForJump = 1;
float timeForJump = -1;


float radius = 1;
float angle = 0.1;
glm::mat4 rotationMat;
glm::vec3 origin = glm::vec3(1, 0, 0);
glm::vec4 objPoint = glm::vec4(2, 0, 0, 1);


glm::vec3 direction;
float startDistance = 1;
float dist = 0;
float cosAngle = cos(angle);
float sinAngle = sin(angle);
// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;

		mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;


		Mesh* mesha = new Mesh("archer");
		mesha->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Archer", "Archer.fbx");
		meshes[mesha->GetMeshID()] = mesha;
		name = {"box", "teapot", "sphere"};

		rotationMat[0] = glm::vec4(cos(angle), -sin(angle), 0, 0);
		rotationMat[1] = glm::vec4(sin(angle), cos(angle), 0, 0);
		rotationMat[2] = glm::vec4(0, 0, 1, 0);
		rotationMat[3] = glm::vec4(0, 0, 0, 1);

		cout << sinAngle << " " << cosAngle << endl;
		objPoint.x = (double)(radius * sin(angle)) + 1;
		objPoint.y = (double)(radius * cos(angle)) + 1;

	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(r,g,b,a);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	
	objPoint = rotationMat * objPoint;
	

	//objPoint.x = origin.x + (objPoint.x - origin.x) * cos(angle) - (objPoint.y - origin.y) * sin(angle);
	//objPoint.y = origin.y + (objPoint.x - origin.x) * sin(angle) + (objPoint.y - origin.y) * cos(angle);
	//angle += 0.1f;

	// cout << objPoint.x << " " << objPoint.y << endl;
	RenderMesh(meshes[name[crtObjIndex]], (glm::vec3) objPoint);

	Jump();
	MoveDirection();
	RenderMesh(meshes["archer"], playerPosition, glm::vec3(0.01f));

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window -> KeyHold(GLFW_KEY_W)) {
		playerPosition += glm::vec3(0, 0, -1) * 5.f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		playerPosition += glm::vec3(-1, 0, 0) * 5.f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		playerPosition += glm::vec3(0, 0, 1) * 5.f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		playerPosition += glm::vec3(1, 0, 0) * 5.f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_E)) {
		playerPosition += glm::vec3(0, -1, 0) * 5.f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
		playerPosition += glm::vec3(0, 1, 0) * 5.f * deltaTime;
	}
};

void Laborator1::Jump()
{
	if (timeForJump > startTimeForJump / 2) {
		playerPosition += glm::vec3(0, 0.01, 0) * 5.f;
		cout << "up" << endl;
		timeForJump -= 0.03f;
		cout << timeForJump << endl;
	}
	else if (timeForJump >= 0) {
		playerPosition += glm::vec3(0, -0.01, 0) * 5.f;
		cout << "down" << endl;
		timeForJump -= 0.03f;
		cout << timeForJump <<endl;
	}
	
}

void Laborator1::MoveDirection()
{
	if (dist >= 0) {
		playerPosition += direction;
		dist -= 0.01f;
	}
}

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		// do something
		
		r = (rand() % 100) *0.01f;
		g = (rand() % 100) * 0.01f;
		b = (rand() % 100) * 0.01f;
	}

	if (key == GLFW_KEY_C) {
		crtObjIndex = (crtObjIndex + 1) % name.size();
	}

	if (key == GLFW_KEY_SPACE) {
		if (timeForJump < 0) {
			timeForJump = startTimeForJump;
		}
	}

	if (key == GLFW_KEY_R) {
		objPoint.x = origin.x + (objPoint.x - origin.x) * cosAngle - (objPoint.y - origin.y) * sinAngle;
		objPoint.y = origin.y + (objPoint.x - origin.x) * sinAngle + (objPoint.y - origin.y) * cosAngle;
		cout << objPoint.x << " " << objPoint.y << endl;
	}

	if (key == GLFW_KEY_W && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(0, 0, -.1f);
		dist = startDistance;
	}
	if (key == GLFW_KEY_A && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(-.1f, 0, 0);
		dist = startDistance;
	}
	if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(0, 0, .1f);
		dist = startDistance;
	}
	if (key == GLFW_KEY_D && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(.1f, 0, 0);
		dist = startDistance;
	}
	if (key == GLFW_KEY_Q && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(0, .1f, 0);
		dist = startDistance;
	}
	if (key == GLFW_KEY_E && mods == GLFW_MOD_CONTROL) {
		direction = glm::vec3(0, -.1f, 0);
		dist = startDistance;
	}
	
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
