#pragma once
#include "Rectangle2D.h"
#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

//fisier header pentru clasa Tema1

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	//metode pentru crearea meshelor si pentru mecanici
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	void CreateCircleMesh(const char* name, int numOfVertex, int z, glm::vec3 color);
	void AddBlackScreen();
	void BirdMotion(float deltaTimeSeconds);
	void PipeMotion(float deltaTimeSeconds);
	int NewRandomPipe();

	//metode pentru frame-uri si pentru event-uri
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:

	glm::mat3 modelMatrix;
	//variabile declarate pentru crearea si pozitionarea pasarii
	float birdPosX, birdPosY;
	float birdAngle;
	float wingAngle;
	int sign;
	glm::vec2 birdCenter;
	float birdRadius;

	//variabile create pentru mecanica pasarii
	float timeForJump, startTimeForJump;
	float startGravity, gravity;

	//variabile create pentru controlul starii jocului
	bool startGame;
	bool gameOver = false;
	bool circleEnabled = false;
	int score, highScore;

	//variabile create pentru crearea si mecanica obstacolelor
	std::vector<Rectangle2D> rectList;
	std::vector<const char*> rectangleColor;
	float height;
	float width;
	int hole;
	float offset;
	int obstacleSpeed;

	//variabile create pentru scalarea imaginii
	int screenW, screenH;	//dimensiunea spatiului definit logic
	int windowW, windowH;	//dimensiunea ecranului real

};
