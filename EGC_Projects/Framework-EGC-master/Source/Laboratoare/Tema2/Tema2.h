#pragma once
#include <Component/SimpleScene.h>
#include <string>
#include <vector>
#include <Core/Engine.h>
#include "HomeworkCamera.h"

//fisier header pentru clasa Tema2
struct Obstacol
{
	float radius;
	glm::vec3 center;
	glm::vec2 direction;
	bool hit = false;
};

struct LifeUp
{
	float radius;
	glm::vec3 center;
	//glm::vec2 direction;
	bool collected = false;
};

struct Fuel 
{
	float radius;
	glm::vec3 position;
	bool collected = false;
};

struct Smoke 
{
	glm::vec3 position;
	float scale;
};

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	//metode pentru crearea meshelor si pentru mecanici
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	void CreateFlatGround(int width, int height, const char* name);
	void CreateCircleMesh(const char* name, int numOfVertex, int z, glm::vec3 color);
	void CreateCylinderMesh(const char* name, int numOfVertexPerSegment, int segments);
	void CreateSphereMesh(const char* name, int sectorCount, int stackCount, std::vector<glm::vec3> colors);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), const glm::vec3& color2 = glm::vec3(1));

	void GenerateObstacles(float deltaTimeSeconds);
	void DisplayObstacles(float deltaTimeSeconds);

	void GenerateLifeUp(float deltaTimeSeconds);
	void DisplayLifeUp(float deltaTimeSeconds);

	bool CollisionCheck(glm::vec3 planePosition, float planeRadius, glm::vec3 objPosition, float objRadius);

	void CreateCloud(int numberOfCubes, int numberOfClouds);
	void DisplayCloud(float deltaTimeSeconds);

	void CreateSmoke(float deltaTimeSeconds);
	void DisplaySmoke(float deltaTimeSeconds);

	void DisplayPilot();
	void DisplayPlane(float deltaTimeSeconds);
	void PlaneMotion(float deltaTimeSeconds);
	void DeathAnimation(float deltaTimeSeconds);

	void CreateCurbPoints(int numberOfPoints, float scaleX, float scaleY, float start, float end, float verticalOffset);
	void GenerateFuel(float deltaTimeSeconds);
	void DisplayFuel(float startPoint, float deltaTimeSeconds);

	void UserInterface(float deltaTimeSeconds);
	void Restart();

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
	Tema::Camera* camera;
	glm::mat4 projectionMatrix;
	float fov;
	float height, width;
	glm::mat4 modelMatrix;
	float propellerAngle = 0;
	float planePositionX = -10;
	float planePositionY = 0;

	float prevPlanePositionX;
	float prevPlanePositionY;
	float prevPlaneAngle;

	float planeDestination = 0;
	float planeAngle = 0;
	float planeInclination = 0;
	int inclinationSide = 1;
	float seaAngle = 0;
	float distanceBetweenVertexes = 0;
	float cloudSpeed = 0;
	std::vector<float> randomVec;
	std::vector<glm::mat4> cloudSegments;
	std::vector<float> cloudDepths;
	bool planeMotionStatus = true;
	bool startGame = false;
	Tema::Camera cam1;
	Tema::Camera cam2;
	std::vector<Obstacol> obstacles = {};
	int obstacleLimit = 5;
	float obstacleTime = 1;
	int numOfLives = 3;
	bool invincibility = false;
	float startInvincibilityTime = 2;
	float invincibilityTime = 0;
	float startHitTime = 0.3f;
	float hitTime = 0;
	bool hit = false;
	float fuel;
	float startFuel = 500;
	float fuelDrain = 5;
	bool fpMode = false;

	glm::vec3 barAngle;// = glm::vec3(atan(10 / 50), -atan(10 / 50), 0);
	glm::vec3 barPosition;

	//glm::vec3 barAngle;// = glm::vec3(atan(10 / 50), -atan(10 / 50), 0);
	glm::vec3 heartPosition;

	std::vector<Fuel> fuelList;
	float fuelSpeed = 20;
	float fuelAngle = 0;
	float startTimeUntilNextFuel = 10;
	float timeUntilNextFuel = 10;
	float fuelValue = 5;
	//glm::vec2 ctr = glm::vec2(RADIANS(11.3f), -RADIANS(11.3f));

	int skinNumber = 0;
	int totalSkins = 6;

	int seaNumber = 0;
	int totalSeas = 6;

	LifeUp life;
	float startTimeLifeUp = 30;
	float timeLifeUp = 30;
	float time = 0;
	float startTime = 5;
	float gameSpeed = 1;

	std::vector<Smoke> smokeTrail;
	float startTimeSmoke = 0.2f;
	float timeSmoke = 0.2f;

	float score = 0;
	float highScore = 0;
	bool printed = false;

	bool paused = false;

	glm::vec3 cylinderCenter = glm::vec3(0);

	glm::vec2 birdCenter;
	glm::vec3 lightPosition;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;
};
