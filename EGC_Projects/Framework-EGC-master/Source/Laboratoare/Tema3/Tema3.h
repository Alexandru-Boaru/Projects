#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "HomeworkCamera.h"

struct Projectile
{
	float radius;
	glm::vec3 center;
	glm::vec3 direction;
	bool hit = false;
};

struct Worm
{
	glm::vec3 position;
	glm::vec2 angle;
	float hp = 100.0;
};

class Tema3 : public SimpleScene
{
	public:
		Tema3();
		~Tema3();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int mode = 0, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, glm::vec4 color = glm::vec4(1));
		Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
		void CreateFlatGround(int width, int height, const char* name);
		void CreateSphere(const char* name, int sectorCount, int stackCount, std::vector<glm::vec3> colors);
		void DisplayProjectiles(float deltaTimeSeconds);
		void CreateHole(float z, float x);
		void SetCamera();


		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		std::unordered_map<std::string, Texture2D*> mapTextures;
		GLuint randomTextureID;

		//proprietati camera
		Tema_3::Camera* camera;
		glm::mat4 projectionMatrix;
		float fov;
		bool renderCameraTarget = false;
		bool cameraSet = false;
		bool altChanged = false;
		float lastAlt[2];
		glm::vec3 oldCameraPosition[2];
		glm::vec3 oldCameraUp[2];


		//proprietati pentru luminile scenei
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		glm::vec3 startLightDirection;

		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		float cut_off;
		float lightAngleX = 0;
		float lightAngleY = 0;
		int isSpotLight = 1;

		//proprietati pentru pozttionare viermi
		Worm worms[2];
		glm::vec3 spotPositions[2];
		glm::vec3 spotDirections[2] = {glm::vec3(0,-1,0), glm::vec3(0,-1,0) };
		int currentWorm = 0;
		bool projLaunched = false;


		//variabile pentru lucru cu buffer-ul de inaltimi
		unsigned char* heightPixels;
		int width, height, channels;

		//variabile care sunt folosite pentru parte de gameplay
		float startMaxSteps = 10;
		float maxSteps = 10;

		std::vector<Projectile> projectiles = {};
		glm::vec2 barAngle[2] = { glm::vec2(0), glm::vec2(0) };

		bool alreadyLaunched = false;
		bool gameOver = false;
		bool locked = false;
		float delayTime = -1;

		float terrainScale = 20.0;

};
