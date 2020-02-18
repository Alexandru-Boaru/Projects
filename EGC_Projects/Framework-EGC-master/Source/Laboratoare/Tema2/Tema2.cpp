#include "Tema2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"



using namespace std;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}
//functia de creare a meshelor
/*
	name = numele pentru mesha
	vertices = lista de vertecsi a meshei
	indices = lista de indici a meshei
*/
Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	//creare si bind VAO
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	CheckOpenGLError();

	//creare si bind VBO
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);

	//informatiile despre vertecsi este trimisa in buffer-ul VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	CheckOpenGLError();

	//creare si bind IBO
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);

	//informatiile despre indici este trimisa in buffer-ul IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	CheckOpenGLError();

	//seteaza informatia despre pozitia vertecsilor
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	//seteaza informatia despre normala vertecsilor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	//seteaza informatia despre coordonata texturii
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	//seteaza informatia despre culoarea vertecsilor
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	CheckOpenGLError();
	//Unbind VAO
	glBindVertexArray(0);

	//informatia este salvata intr-un obiect de tip Mesh
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

//functie pentru crearea meshelor cercurilor
/*	name = numele pentru mesha
	numOfVertex = numarul de veretecsi din care e format cercul (fara centru)
	z = pozitia pe coordonata z (pentru ordonarea obiectelor in scena)
	color = culoarea cercului
*/
void Tema2::CreateCircleMesh(const char* name, int numOfVertex, int z, glm::vec3 color) {

	vector<VertexFormat> vertices;
	vector<unsigned short> indices;
	vertices.push_back(VertexFormat(glm::vec3(0, 0, z), color));
	for (int i = 0; i < numOfVertex; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numOfVertex);
		float x = cosf(theta);
		float y = sinf(theta);
		vertices.push_back(VertexFormat(glm::vec3(x, y, z), color));
		
	}
	for (int i = 1; i < numOfVertex; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(0);
	indices.push_back(numOfVertex);
	indices.push_back(1);

	meshes[name] = new Mesh(name);
	Mesh* circle = CreateMesh(name, vertices, indices);
}

//functie pentru crearea cilindrului
/*
	name = numele meshei
	nuOfVertexPerSegment = numar de vertexi pentru fiecare segment din cilindru
	segment = numarul de segmente
*/

void Tema2::CreateFlatGround(int width, int height, const char* name)
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//cout << i << " " << j << endl;
			vertices.push_back(VertexFormat(glm::vec3((float)i, 0, (float)j), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2((float)i / (height - 1), (float)j / (width - 1))));
		}
	}

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			indices.push_back((j + i * width));
			indices.push_back((j + i * width + 1));
			indices.push_back((j + i * width + width));
			indices.push_back((j + i * width + width));
			indices.push_back((j + i * width + 1));
			indices.push_back((j + i * width + width + 1));
		}
	}
	meshes[name] = new Mesh(name);
	Mesh* plane = CreateMesh(name, vertices, indices);
}

void Tema2::CreateCylinderMesh(const char* name, int numOfVertexPerSegment, int segments) {

	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	for (int i = 0; i < segments; i++) {
		
		for (int j = 0; j < numOfVertexPerSegment; j++) {

			//se adauga vertexi pentru fiecare segment din cilindru (segmentul e un cerc)

			float theta = 2.0f * 3.1415926f * float(j) / float(numOfVertexPerSegment);
			float x = cosf(theta);
			float y = sinf(theta);
			int signX, signY, signZ;
			signX = rand() % 2 == 0 ? 1 : -1;
			signY = rand() % 2 == 0 ? 1 : -1;
			signZ = rand() % 2 == 0 ? 1 : -1;
			vertices.push_back(VertexFormat(glm::vec3(x, y, i), glm::vec3(1), glm::vec3((float)signX * rand() / RAND_MAX, (float)signY * rand() / RAND_MAX, (float)signZ * rand() / RAND_MAX)));
			randomVec.push_back((float)rand() / RAND_MAX);
		}
	}

	distanceBetweenVertexes = distance(vertices[0].position, vertices[1].position);
	cout << distanceBetweenVertexes << endl;
	int s = numOfVertexPerSegment;

	//se adaug indicii
	for (int i = 0; i < segments-1; i++) {
		for (int j = 0; j < numOfVertexPerSegment; j++) {
			int k = i * numOfVertexPerSegment + 2*numOfVertexPerSegment;
			
			if (j == numOfVertexPerSegment - 1) {
				indices.push_back((j+i*s));
				indices.push_back((j + i * s + 1 - numOfVertexPerSegment));
				indices.push_back((j + i * s + numOfVertexPerSegment));
				indices.push_back((j + i * s + numOfVertexPerSegment));
				indices.push_back((j + i * s + 1 - numOfVertexPerSegment));
				indices.push_back((j + i * s + 1));
				continue;
			}
			indices.push_back((j + i * s ));
			indices.push_back((j + i * s + 1));
			indices.push_back((j + i * s + numOfVertexPerSegment));
			indices.push_back((j + i * s + numOfVertexPerSegment));
			indices.push_back((j + i * s + 1));
			indices.push_back((j + i * s + numOfVertexPerSegment+ 1 ));
		}
	}

	meshes[name] = new Mesh(name);
	Mesh* cylinder = CreateMesh(name, vertices, indices);
}


//functie pentru crearea unei sfere
/*
	name = numele meshei
	sectorCount = numarul de sectoare
	stackCount = numarul de stack-uri
*/

void Tema2::CreateSphereMesh(const char* name, int sectorCount, int stackCount, vector<glm::vec3> colors) {
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;
	//pozitia vertexului
	float x, y, z, xy;


	float sectorStep = 2.0f * 3.1415926f / sectorCount;
	float stackStep = 3.1415926f / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = 3.1415926f / 2 - i * stackStep;
		xy = cosf(stackAngle);             
		z = sinf(stackAngle);              

		// adauga (sectorCount+1) vertexi pentru fiecare stack
		//primul si ultimul vertex au aceeasi pozitie
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(VertexFormat(glm::vec3(x, y, z), colors[rand() % colors.size()]));
		}
	}
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triunghiuri pentru fiecare sector mai putin pentru primul si ultimul stack
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	meshes[name] = new Mesh(name);
	Mesh* sphere = CreateMesh(name, vertices, indices);
}

void Tema2::Init()
{
	//setarea camerei
	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 0, 70), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	fov = 45;
	width = 20.0f;
	height = 20.0f;
	//definire meshe

	//corp avion
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -0.5,  1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-1,  0.5,  1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-1, -0.5, -1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-1,  0.5, -1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(1, 0, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("body", vertices, indices);
	}

	//cub
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);
	}

	//bara alba
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, -1,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-1,  1,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(1, 1, 1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
		};

		CreateMesh("whitebar", vertices, indices);
	}

	//bara neagra
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1, -1,  0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-1,  1,  0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 0, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
		};

		CreateMesh("blackbar", vertices, indices);
	}

	//simbol inima
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, 0,  0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0, -1,  0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1,  0,  0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0.5f, 0.5f,  0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-0.5f, 0.5f,  0), glm::vec3(1, 0, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		2, 3, 4,		4, 5, 0
		};

		CreateMesh("heart", vertices, indices);
	}

	//combustibil
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -sqrtf(4 - powf(2 * sqrtf(3) / 3,2)) / 3,  -sqrtf(3)/3), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(1, -sqrtf(4 - powf(2 * sqrtf(3) / 3,2)) / 3,  -sqrtf(3)/3), glm::vec3(0.251f, 0.8784f, 0.8157f)),
			VertexFormat(glm::vec3(0, -sqrtf(4 - powf(2 * sqrtf(3) / 3,2)) / 3,  2*sqrtf(3)/3), glm::vec3(0.251f, 0.8784f, 0.8157f)),
			VertexFormat(glm::vec3(0, 2*sqrtf(4-powf(2*sqrtf(3)/3,2)) / 3, 0), glm::vec3(0.251f, 0.8784f, 0.8157f)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		0, 2, 3,		0, 3, 1,			1, 2, 3
		};

		CreateMesh("fuel", vertices, indices);
	}

	CreateFlatGround(500, 500, "plane");
	//crearea apei, obstacolului si lifeUp (recapata o viata)
	CreateCylinderMesh("sea", 50, 40);
	CreateSphereMesh("obstacle", 6, 20, { glm::vec3(1, 0, 0) , glm::vec3(1, 0.647f, 0), glm::vec3(0.647f, 0.165f, 0.165f) });
	CreateSphereMesh("lifeUp", 3, 3, { glm::vec3(1,1,0) });

	for (int i = 0; i < randomVec.size(); i++) {
		randomVec[i] *= distanceBetweenVertexes / 2;
	}

	//definirea shaderelor

	//shader pentru apa
	{
		Shader* shader = new Shader("SeaShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderSea.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//shader pentru nori
	{
		Shader* shader = new Shader("CloudShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderCloud.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//shader pentru diverse
	{
		Shader* shader = new Shader("PlaneShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderPlane.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//creare nor
	CreateCloud(10, 70);

	//setare combustibil
	fuel = startFuel;

	//setare UI
	barPosition = glm::vec3(20, 15, 50);
	heartPosition = glm::vec3(-25, 15, 50);

	//setare proprietati lumina de tip spot
	{
		lightPosition = glm::vec3(20, 100, -300);
		materialShininess = 2;
		materialKd = 55;
		materialKs = 30;
	}
}

void Tema2::FrameStart()
{
	
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 1000.0f);
	glm::vec4 color;

	//setarea fundalui
	switch (seaNumber)
	{
	case 0:
		color = glm::vec4((float)254 / 255, (float)192 / 255, (float)158 / 255, 1);
		break;

	case 1:
		color = glm::vec4(0.5, 0, 0, 1);
		break;

	case 2:
		color = glm::vec4((float)197/255, (float)139/255, (float)231/255, 1);
		break;

	case 3:
		color = glm::vec4(0.95f, 0.95f, 0.95f, 1);
		break;

	case 4:
		color = glm::vec4(0.992f, 0.369f, 0.325f, 1);
		break;

	case 5:
		color = glm::vec4(0.2f, 0.2f, 0.2f, 1);
		break;

	default:
		color = glm::vec4(1);
		break;
	}

	glClearColor(color.x,color.y,color.z,color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setarea ecranului
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

}



void Tema2::Update(float deltaTimeSeconds)
{
	//cand se termina jocul, se afiseaza scorul final
	if ((numOfLives <= 0 || fuel < 0) && !printed) {
		cout << "FINAL SCORE: " << (int)score*10 << endl;
		if (score > highScore) {
			highScore = score;
			cout << "NEW HIGHSCORE: " << (int)highScore*10 << endl;
		}
		printed = true;
	}

	if(!paused && !startGame)
		score += deltaTimeSeconds;

	//viteza si dificultatea jocului creste
	if (startGame) {
		gameSpeed += deltaTimeSeconds / 100;
	}

	//randare nori
	DisplayCloud(deltaTimeSeconds);

	//randare User Interface
	UserInterface(deltaTimeSeconds);

	//randare pilot, avion si mecanica avionului
	DisplayPilot();
	DisplayPlane(deltaTimeSeconds);
	PlaneMotion(deltaTimeSeconds);

	//creare si randare fum
	CreateSmoke(deltaTimeSeconds);
	DisplaySmoke(deltaTimeSeconds);

	//genereare si randare obstacole
	if (startGame)
		GenerateObstacles(deltaTimeSeconds);

	if(!obstacles.empty())
		DisplayObstacles(deltaTimeSeconds);

	//generare si randare combustibil
	if (timeUntilNextFuel<=0 && startGame && !paused) {
		GenerateFuel(deltaTimeSeconds);
		timeUntilNextFuel = startTimeUntilNextFuel;
	}
	else if(!paused){
		timeUntilNextFuel -= deltaTimeSeconds;
	}
	DisplayFuel(50, deltaTimeSeconds);

	if (fuel >= 0 && !paused && startGame)
		fuel -= gameSpeed * fuelDrain * deltaTimeSeconds;

	//cand se termina jocul, se activeaza animatia de final a avionului

	if (numOfLives <= 0 || fuel < 0) {
		invincibility = true;
		DeathAnimation(deltaTimeSeconds);
		return;
	}

	//generare si randare lifeUp

	if (startGame) {
		GenerateLifeUp(deltaTimeSeconds);
	}

	DisplayLifeUp(deltaTimeSeconds);

	glm::vec3 color1;
	glm::vec3 color2;
	//setarea culorii apei
	switch (seaNumber) 
	{
		case 0:
			color1 = glm::vec3(0.671f, 0.851f, 0.89f);
			color2 = glm::vec3(0, 0.467f, 0.745f);
			break;

		case 1:
			color1 = glm::vec3(1, 0, 0);
			color2 = glm::vec3(1, 1, 0);
			break;

		case 2:
			color1 = glm::vec3(0, 0.694, 0);
			color2 = glm::vec3(0, 1, 0);
			break;

		case 3:
			color1 = glm::vec3(0.85f);
			color2 = glm::vec3(0.75f);
			break;

		case 4:
			color1 = glm::vec3(1, 0.757f, 0.8f);
			color2 = glm::vec3(1, 0.6f, 0.675f);
			break;

		case 5:
			color1 = glm::vec3(0.1f);
			color2 = glm::vec3(1, 0, 0);
			break;

		default:
			color1 = glm::vec3(1);
			color2 = glm::vec3(1);
			break;
	}

	//
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(0, -(30 + distanceBetweenVertexes * 100), -600);
		modelMatrix *= Transform3D::Scale(300, 300, 20);
		modelMatrix *= Transform3D::Translate(0, -1, 0);
		modelMatrix *= Transform3D::RotateOZ(gameSpeed * seaAngle);
		RenderSimpleMesh(meshes["sea"], shaders["SeaShader"], modelMatrix, color1, color2);
	}
	cylinderCenter.y -= 300 + 30 + distanceBetweenVertexes * 100;
	RenderSimpleMesh(meshes["cube"], shaders["VertexColor"], glm::mat4(1) * Transform3D::Translate(0, cylinderCenter.y, 0));
	if(!paused)
		seaAngle += 0.2f * deltaTimeSeconds;

}

void Tema2::FrameEnd()
{

}

void Tema2::CreateCloud(int numberOfCubes, int numberOfClouds) {
	
	for (int j = 0; j < numberOfClouds; j++) {

		float cloudHeight = (float) rand() / RAND_MAX * 25 + 5;
		float cloudDepth = (float) rand() / RAND_MAX * 200 - 100;
		float cloudAngle = rand();

		for (int i = 0; i < numberOfCubes; i++) {
			float randomScale = (float)rand() / RAND_MAX / 2 + 1;
			float x = (float)rand() / RAND_MAX / 2 - 0.25;
			float y = (float)rand() / RAND_MAX * 2 - 1;
			float z = (float)rand() / RAND_MAX / 2 - 0.25;
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, - (330 + distanceBetweenVertexes * 100), 0);
			modelMatrix *= Transform3D::RotateOZ(cloudAngle);
			modelMatrix *= Transform3D::Translate(0, 330 + distanceBetweenVertexes * 100, 0);
			modelMatrix *= Transform3D::Translate(i, cloudHeight , cloudDepth);
			modelMatrix *= Transform3D::Translate(x, y, z);
			modelMatrix *= Transform3D::Scale(randomScale, randomScale, randomScale);
			modelMatrix *= Transform3D::RotateOZ(rand());
			modelMatrix *= Transform3D::RotateOY(rand());
			modelMatrix *= Transform3D::RotateOX(rand());
			
			cloudSegments.push_back(modelMatrix);
			cloudDepths.push_back(cloudDepth);
		}
	}
}

void Tema2::DisplayCloud(float deltaTimeSeconds) {
	for (int i = 0; i < cloudSegments.size(); i++) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(0, -(330 + distanceBetweenVertexes * 100), 0);
		modelMatrix *= Transform3D::RotateOZ(gameSpeed*seaAngle/4);
		modelMatrix *= Transform3D::Translate(0, 330 + distanceBetweenVertexes * 100, 0);
		modelMatrix *= cloudSegments[i];
		if (cloudDepths[i] > -33) {
			RenderSimpleMesh(meshes["cube"], shaders["CloudShader"], modelMatrix, glm::vec3(1,1,1));
		}
		else if (cloudDepths[i] <= -33 && cloudDepths[i] > -66) {
			RenderSimpleMesh(meshes["cube"], shaders["CloudShader"], modelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));
		}
		else if (cloudDepths[i] <= -66) {
			RenderSimpleMesh(meshes["cube"], shaders["CloudShader"], modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
		}
		
	}
}

void Tema2::CreateSmoke(float deltaTimeSeconds) {
	if (!paused) {
		if (timeSmoke < 0) {
			Smoke s;
			s.position = glm::vec3(planePositionX - 2, planePositionY, 0);
			s.scale = 0.2f * gameSpeed;
			smokeTrail.push_back(s);
			timeSmoke = (float)startTimeSmoke / gameSpeed;
		}
		timeSmoke -= deltaTimeSeconds;
	}
}

void Tema2::DisplaySmoke(float deltaTimeSeconds) {
	glm::vec3 color;
	switch (skinNumber)
	{
	case 0:
		color = glm::vec3(0.9f);
		break;

	case 1:
		color = glm::vec3(1);
		break;

	case 2:
		color = glm::vec3(1, 0.737f, 0.851f);
		break;

	case 3:
		color = glm::vec3(0.52f, 0, 0.52f);
		break;

	case 4:
		color = glm::vec3(0);
		break;

	case 5:
		color = glm::vec3(1, 0, 0);
		break;

	default:
		color = glm::vec3(1);
		break;
	}
	for (int i = 0; i < smokeTrail.size(); i++) {
		if (!paused) {
			smokeTrail[i].position.x -= 4 * deltaTimeSeconds * gameSpeed;
			smokeTrail[i].position.y += 2 * deltaTimeSeconds * gameSpeed;
			smokeTrail[i].scale -= gameSpeed * deltaTimeSeconds / 10;
		}

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(smokeTrail[i].position.x, smokeTrail[i].position.y, 0);
		modelMatrix *= Transform3D::Scale(smokeTrail[i].scale, smokeTrail[i].scale, smokeTrail[i].scale);
		RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color);
	}
	if (!smokeTrail.empty() && smokeTrail[0].scale < 0) {
		smokeTrail.erase(smokeTrail.begin());
	}
}


void Tema2::GenerateObstacles(float deltaTimeSeconds) {
	//if (rand() % 40 == 0 && obstacles.size() < obstacleLimit) {
	if (!paused) {
		if (rand() % 10 == 0 && obstacleTime < 0) {
			Obstacol o;
			o.center = glm::vec3(50, (float)rand() / RAND_MAX * 50 - 25, 0);
			o.radius = 2;
			float angle = (float)rand() / RAND_MAX * RADIANS(20) + RADIANS(260);
			o.direction = glm::vec2(sin(angle), cos(angle));
			obstacles.push_back(o);
			obstacleTime = 1 / gameSpeed;
			//cout << obstacleTime << endl;
		}
		obstacleTime -= deltaTimeSeconds;
	}
}

void Tema2::DisplayObstacles(float deltaTimeSeconds) {
	for (int i = 0; i < obstacles.size(); i++) {
		if (!paused) {
			obstacles[i].center.x += gameSpeed * obstacles[i].direction.x * 15 * deltaTimeSeconds;
			obstacles[i].center.y += gameSpeed * obstacles[i].direction.y * 15 * deltaTimeSeconds;
		}
		Obstacol o = obstacles[i];
		
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(o.center.x, o.center.y, o.center.z);
		modelMatrix *= Transform3D::RotateOZ(2 * Engine::GetElapsedTime());
		modelMatrix *= Transform3D::Scale(o.radius, o.radius, o.radius);
		RenderSimpleMesh(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
		if(!invincibility && !o.hit && CollisionCheck(glm::vec3(planePositionX + 0.5f, planePositionY, 0), 1.7f, o.center, o.radius)){
			obstacles[i].hit = true;
			cout << "DEAD" << endl;
			numOfLives--;
			if (numOfLives == 0) {
				cout << "GAME OVER" << endl;
			}
			hit = true;
			invincibility = true;
			hitTime = startHitTime;
			invincibilityTime = startInvincibilityTime;
			
		}
	}
	if (!obstacles.empty() && obstacles[0].center.x < -50) {
		obstacles.erase(obstacles.begin());
	}
}

void Tema2::GenerateLifeUp(float deltaTimeSeconds) {
	if (!paused) {
		if (timeLifeUp < 0) {
			life.center = glm::vec3(50, (float)rand() / RAND_MAX * 50 - 25, 0);
			life.radius = 3;
			life.collected = false;
			timeLifeUp = startTimeLifeUp;
		}
		timeLifeUp -= deltaTimeSeconds;
	}
}

void Tema2::DisplayLifeUp(float deltaTimeSeconds) {

	if (!life.collected && startGame) {
		if(!paused)
			life.center.x -= gameSpeed*30*deltaTimeSeconds;

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(life.center.x, life.center.y, life.center.z);
		modelMatrix *= Transform3D::RotateOZ(2 * Engine::GetElapsedTime());
		modelMatrix *= Transform3D::Scale(life.radius, life.radius, life.radius);
		RenderSimpleMesh(meshes["lifeUp"], shaders["VertexColor"], modelMatrix);
	}
	
	if (!life.collected && CollisionCheck(glm::vec3(planePositionX + 0.5f, planePositionY, 0), 1.7f, life.center, life.radius)) {
		life.collected = true;
		if (numOfLives > 0 && numOfLives < 3) {
			numOfLives++;
		}
	}
}

bool Tema2::CollisionCheck(glm::vec3 planePosition, float planeRadius, glm::vec3 objPosition, float objRadius) {
	float distance = sqrt((planePosition.x - objPosition.x) * (planePosition.x - objPosition.x) +
		(planePosition.y - objPosition.y) * (planePosition.y - objPosition.y) +
		(planePosition.z - objPosition.z) * (planePosition.z - objPosition.z));
	return distance < (planeRadius + objRadius);
}

void Tema2::DisplayPilot() {

	glm::vec3 color1, color2, color3, color4;

	switch (skinNumber)
	{
	case 0:
		color1 = glm::vec3(0.408f, 0.102f, 0.102f);
		color2 = glm::vec3(1, 0.89f, 0.62f);
		color3 = glm::vec3(1);
		color4 = glm::vec3(0.855f, 0.569f, 0);
		break;

	case 1:
		color1 = glm::vec3(0.2f);
		color2 = glm::vec3(0.8f);
		color3 = glm::vec3(1);
		color4 = glm::vec3(0);
		break;

	case 2:
		color1 = glm::vec3(1, 0, 1);
		color2 = glm::vec3(1, 0.89f, 0.62f);
		color3 = glm::vec3(1);
		color4 = glm::vec3(1, 1, 0);
		break;

	case 3:
		color1 = glm::vec3(0, 0.75, 0);
		color2 = glm::vec3(1, 0.89f, 0.62f);
		color3 = glm::vec3(0, 1, 0);
		color4 = glm::vec3(0.52f, 0, 0.52f);
		break;

	case 4:
		color1 = glm::vec3(0, 0, 0);
		color2 = glm::vec3(1, 0.89f, 0.62f);
		color3 = glm::vec3(0);
		color4 = glm::vec3(1, (float)165 / 256, 0);
		break;

	case 5:
		color1 = glm::vec3(0, 0, 1);
		color2 = glm::vec3(1, 0.89f, 0.62f);
		color3 = glm::vec3(0, 0, 1);
		color4 = glm::vec3(1, 0, 0);
		break;

	default:
		color1 = glm::vec3(1);
		color2 = glm::vec3(1);
		color3 = glm::vec3(1);
		color4 = glm::vec3(1);
		break;
	}

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(-0.3f, 1.5f, 0);
	modelMatrix *= Transform3D::Scale(0.25f, 0.25f, 0.3f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color4);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(-0.3f, 1.2f, 0);
	modelMatrix *= Transform3D::Scale(0.1f, 0.3f, 0.1f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color4);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(0, 1.6f, 0);
	modelMatrix *= Transform3D::Scale(0.3f, 0.1f, 0.1f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color4);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(0.2f, 1.25f, -0.1f);
	modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.07f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color3);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(0.2f, 1.25f, 0.1f);
	modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.07f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color3);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(0, 1.25f, 0);
	modelMatrix *= Transform3D::Scale(0.25f, 0.25f, 0.25f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color2);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(0, 1.1f, 0);
	modelMatrix *= Transform3D::Scale(0.2f, 0.2f, 0.2f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color2);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY + 0.3f, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Scale(0.3f, 0.95f, 0.3f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color1);
}


void Tema2::DisplayPlane(float deltaTimeSeconds) {

	glm::vec3 color1;
	glm::vec3 color2;
	glm::vec3 color3;
	glm::vec3 color4;

	switch (skinNumber)
	{
	case 0:
		color1 = glm::vec3(1, 0, 0);
		color2 = glm::vec3(1, 1, 0);
		color3 = glm::vec3(0, 0, 0);
		color4 = glm::vec3(1, 1, 1);
		break;

	case 1:
		color1 = glm::vec3(0.25f);
		color2 = glm::vec3(0.75f);
		color3 = glm::vec3(0, 0, 0);
		color4 = glm::vec3(1, 1, 1);
		break;

	case 2:
		color1 = glm::vec3(1, 0.945f, 0.953f);
		color2 = glm::vec3(1, 0.792f,0.824f);
		color3 = glm::vec3(0, 0, 0);
		color4 = glm::vec3(1, 1, 1);
		break;

	case 3:
		color1 = glm::vec3(0.5, 0, 0.5);
		color2 = glm::vec3(0, 0.694, 0);
		color3 = glm::vec3(0.7f, 0, 0.7f);
		color4 = glm::vec3(0, 1, 0);
		break;

	case 4:
		color1 = glm::vec3(0, 0, 0);
		color2 = glm::vec3(0, 0, 0);
		color3 = glm::vec3(1, (float)165 / 256, 0);
		color4 = glm::vec3(1, 1, 1);
		break;

	case 5:
		color1 = glm::vec3(1, 1, 1);
		color2 = glm::vec3(0, 0, 1);
		color3 = glm::vec3(1, 0, 0);
		color4 = glm::vec3(0, 0, 1);
		break;

	default:
		color1 = glm::vec3(1);
		color2 = glm::vec3(1);
		color3 = glm::vec3(1);
		color4 = glm::vec3(1);
		break;
	}

	if (skinNumber == 9) {
		//pozitionare ochi
		modelMatrix = glm::mat4(1);
		
		RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

		//pozitionare aripi
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
		modelMatrix *= Transform3D::RotateOZ(planeAngle);
		modelMatrix *= Transform3D::Scale(1.0f / 160.0f, 1.0f / 160.0f, 0);
		modelMatrix *= Transform3D::Translate(-birdCenter.x, -birdCenter.y, 0);
		//modelMatrix *= Transform3D::Translate(0, 36);
		//modelMatrix *= Transform3D::Scale(1, wingAngle);
		//modelMatrix *= Transform3D::Translate(0, -36);

		RenderMesh2D(meshes["wings"], shaders["VertexColor"], modelMatrix);

		//implementare pentru animatie aripi
		/*if (startGame && !gameOver) {
			if (wingAngle < -1) {
				sign = 1;
			}
			else if (wingAngle > 1) {
				sign = -1;
			}
			wingAngle += sign * 5 * deltaTimeSeconds;
		}*/

		//pozitionarea resputului pieselor pasarii
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
		modelMatrix *= Transform3D::RotateOZ(planeAngle);
		modelMatrix *= Transform3D::Scale(1.0f / 160.0f, 1.0f / 160.0f, 1);
		modelMatrix *= Transform3D::Translate(-birdCenter.x, -birdCenter.y, 0);
		RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["bird"], shaders["VertexColor"], modelMatrix);
	}

	//wings

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Scale(0.8f, 0.2f, 4);

	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color2);

	//tail

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(-1.8f, 1, 0);

	modelMatrix *= Transform3D::Scale(0.5f, 0.7f, 0.2f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color1);

	//tail2

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(-1.7f, 0.3f, 0);

	modelMatrix *= Transform3D::Scale(0.5f, 0.2f, 1.6f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color2);

	//propeller1

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(2.15f, 0, 0);
	modelMatrix *= Transform3D::RotateOX(gameSpeed*propellerAngle);

	modelMatrix *= Transform3D::Scale(0.1f, 2.5f, 0.3f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color3);

	//propeller2

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(2.15f, 0, 0);
	modelMatrix *= Transform3D::RotateOX(gameSpeed*propellerAngle);

	modelMatrix *= Transform3D::Scale(0.1f, 0.3f, 2.5f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color3);

	

	//rotor

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(2.2f, 0, 0);

	modelMatrix *= Transform3D::Scale(0.2f, 0.2f, 0.2f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color1);

	//head

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Translate(1.5f, 0, 0);

	modelMatrix *= Transform3D::Scale(0.5f, 1.1f, 1.1f);
	RenderSimpleMesh(meshes["cube"], shaders["PlaneShader"], modelMatrix, color4);

	//body

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(planePositionX, planePositionY, 0);
	modelMatrix *= Transform3D::RotateOZ(planeAngle);
	modelMatrix *= Transform3D::RotateOX(planeInclination);
	modelMatrix *= Transform3D::Scale(2, 1, 1);
	RenderSimpleMesh(meshes["body"], shaders["PlaneShader"], modelMatrix, color1);

}

void Tema2::PlaneMotion(float deltaTimeSeconds) {

	propellerAngle += 4 * deltaTimeSeconds;

	if (!paused) {

		if (numOfLives <= 0 || fuel < 0) {
			return;
		}

		if (!hit) {
			prevPlanePositionX = planePositionX;
			prevPlanePositionY = planePositionY;
			prevPlaneAngle = planeAngle;
		}

		if (invincibility) {
			if (invincibilityTime < 0) {
				invincibility = false;
			}
			invincibilityTime -= deltaTimeSeconds;
		}

		if (hit) {
			if (hitTime > startHitTime / 2) {
				planePositionX -= 20 * deltaTimeSeconds;
				planePositionY -= 20 * deltaTimeSeconds;
				planeAngle += 5 * deltaTimeSeconds;
				if (fpMode) {
					camera->TranslateUpword(-20 * deltaTimeSeconds);
					camera->TranslateForward(-20 * deltaTimeSeconds);
				}
			}
			else if (hitTime > 0) {
				planePositionX += 20 * deltaTimeSeconds;
				planePositionY += 20 * deltaTimeSeconds;
				planeAngle -= 5 * deltaTimeSeconds;
				if (fpMode) {
					camera->TranslateUpword(20 * deltaTimeSeconds);
					camera->TranslateForward(-20 * deltaTimeSeconds);
				}
			}
			else {
				planePositionX = prevPlanePositionX;
				planePositionY = prevPlanePositionY;
				planeAngle = prevPlaneAngle;
				hit = false;
				if (fpMode)
					camera->Set(glm::vec3(planePositionX, planePositionY + 3, 0), glm::vec3(0, planePositionY + 3, 0), glm::vec3(0, 1, 0));

			}
			hitTime -= deltaTimeSeconds;
		}
		if (!hit && startGame) {
			if (planeMotionStatus) {
				if (abs(planePositionY - planeDestination) < 1) {

					if (abs(planeAngle) < RADIANS(5)) {
						planeAngle = 0;
					}
					else if (planeAngle > 0) {
						planeAngle -= 5 * deltaTimeSeconds;
					}
					else if (planeAngle < 0) {
						planeAngle += 5 * deltaTimeSeconds;
					}
					if (planeInclination > RADIANS(10)) {
						inclinationSide = -1;
					}
					else if (planeInclination < RADIANS(-10)) {
						inclinationSide = 1;
					}
					planeInclination += 0.5f * (1 + (gameSpeed - 1) / 4) * inclinationSide * deltaTimeSeconds;
					planePositionY = planeDestination;
					if (fpMode)
						camera->Set(glm::vec3(planePositionX, planePositionY + 3, 0), glm::vec3(0, planePositionY + 3, 0), glm::vec3(0, 1, 0));
					//else
					//	camera->Set(glm::vec3(0, planePositionY * 0.9f / 25, 70), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				}
				else if (planePositionY < planeDestination) {
					planePositionY += (1 + (gameSpeed - 1) / 4) * 30 * deltaTimeSeconds;
					planeAngle += 5 * deltaTimeSeconds;
					planeInclination = 0;
					if (fpMode) {
						camera->MoveUpward((1 + (gameSpeed - 1) / 4) * 30 * deltaTimeSeconds);
					}
					else
						camera->TranslateUpword((1 + (gameSpeed - 1) / 4) * deltaTimeSeconds);
				}
				else if (planePositionY > planeDestination) {
					planePositionY -= (1 + (gameSpeed - 1) / 4) * 30 * deltaTimeSeconds;
					planeAngle -= 5 * deltaTimeSeconds;
					planeInclination = 0;
					if (fpMode) {
						camera->MoveUpward((1 + (gameSpeed - 1) / 4) * -30 * deltaTimeSeconds);
					}
					else
						camera->TranslateUpword((1 + (gameSpeed - 1) / 4) * -deltaTimeSeconds);
				}
			}
			planeAngle = glm::clamp(planeAngle, RADIANS(-45), RADIANS(45));
		}
	}
}

void Tema2::DeathAnimation(float deltaTimeSeconds) {
	planePositionY -= 15 * deltaTimeSeconds;
	planeAngle -= 2*deltaTimeSeconds;
	planeInclination += 5*deltaTimeSeconds;
	planeAngle = glm::clamp(planeAngle, RADIANS(-45), RADIANS(45));
	if(fpMode)
		camera->TranslateUpword(-15 * deltaTimeSeconds);
}

void Tema2::CreateCurbPoints(int numberOfPoints, float scaleX, float scaleY, float start, float end, float verticalOffset) {
	for (int i = 0; i < numberOfPoints+1; i++) {
		float x = scaleX * (end - start) / (numberOfPoints+1) * i;
		float y = scaleY * sinf((end - start) / (numberOfPoints+1) * i);
		Fuel f;
		f.radius = sqrtf(4 - powf(2 * sqrtf(3) / 3, 2))/4;
		f.position = glm::vec3(x, y+verticalOffset, 0);
		fuelList.push_back(f);
	}
}

void Tema2::GenerateFuel(float deltaTimeSeconds) {
	fuelList.clear();
	int numOfFuel = rand() % 8 + 3;
	float scaleX = (float)rand()/RAND_MAX * 9 + 1;
	float scaleY = (float)rand() / RAND_MAX * 9 + 1;
	scaleY = min(scaleY, scaleX);
	float start = 0;
	float end = (float)rand() / RAND_MAX * 5+1;
	float offset = (float)rand() / RAND_MAX * (40 - scaleY) - (40 - scaleY)/2;
	CreateCurbPoints(numOfFuel, scaleX, scaleY, start, end, offset);
}

void Tema2::DisplayFuel(float startPoint, float deltaTimeSeconds) {

	for (int i = 0; i < fuelList.size(); i++) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(startPoint + fuelList[i].position.x, fuelList[i].position.y, fuelList[i].position.z);
		modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
		modelMatrix *= Transform3D::RotateOY(gameSpeed*fuelAngle);
		if(!fuelList[i].collected)
			RenderSimpleMesh(meshes["fuel"], shaders["VertexColor"], modelMatrix);
		if(!paused)
			fuelList[i].position.x -= gameSpeed*fuelSpeed * deltaTimeSeconds;

		Fuel f = fuelList[i];
		if (fuel > 0 && !f.collected && CollisionCheck(glm::vec3(planePositionX + 0.5f, planePositionY, 0), 1.7f, f.position+glm::vec3(startPoint,0,0), f.radius)) {
			fuelList[i].collected = true;
			fuel += fuelValue * startFuel / 100;
			fuel = glm::clamp(fuel, 0.0f, startFuel);
		}
	}
	fuelAngle += 3*deltaTimeSeconds;
}

void Tema2::UserInterface(float deltaTimeSeconds) {
	modelMatrix = glm::mat4(1);
	glm::vec3 tempRadiusz = glm::vec3(barPosition.z) * camera->forward;
	glm::vec3 tempRadiusx = glm::vec3(barPosition.x) * camera->right;
	glm::vec3 tempRadiusy = glm::vec3(barPosition.y) * camera->up;

	modelMatrix *= Transform3D::Translate(tempRadiusz.x, tempRadiusz.y, tempRadiusz.z);
	modelMatrix *= Transform3D::Translate(tempRadiusx.x, tempRadiusx.y, tempRadiusx.z);
	modelMatrix *= Transform3D::Translate(tempRadiusy.x, tempRadiusy.y, tempRadiusy.z);

	modelMatrix *= Transform3D::Translate(camera->position.x, camera->position.y, camera->position.z);
	modelMatrix *= Transform3D::Translate(camera->position.x, camera->position.y, camera->position.z);


	modelMatrix *= Transform3D::Translate(-camera->position.x, -camera->position.y, -camera->position.z);

	modelMatrix *= Transform3D::RotateOY(barAngle.y);
	modelMatrix *= Transform3D::RotateOX(barAngle.x);

	modelMatrix *= Transform3D::Scale((float)4 * fuel / startFuel, 1, 1);
	modelMatrix *= Transform3D::Translate(1, 0, 0);

	RenderSimpleMesh(meshes["whitebar"], shaders["VertexColor"], modelMatrix);

	modelMatrix *= Transform3D::Translate(-1, 0, 0);
	modelMatrix *= Transform3D::Scale(4.2f / (4 * fuel / startFuel), 1.2f, 1);
	modelMatrix *= Transform3D::Translate(1, 0, 0);
	modelMatrix *= Transform3D::Translate(0, 0, -0.1f);

	RenderSimpleMesh(meshes["blackbar"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat4(1);
	tempRadiusz = glm::vec3(heartPosition.z) * camera->forward;
	tempRadiusx = glm::vec3(heartPosition.x) * camera->right;
	tempRadiusy = glm::vec3(heartPosition.y) * camera->up;

	modelMatrix *= Transform3D::Translate(tempRadiusz.x, tempRadiusz.y, tempRadiusz.z);
	modelMatrix *= Transform3D::Translate(tempRadiusx.x, tempRadiusx.y, tempRadiusx.z);
	modelMatrix *= Transform3D::Translate(tempRadiusy.x, tempRadiusy.y, tempRadiusy.z);

	modelMatrix *= Transform3D::Translate(camera->position.x, camera->position.y, camera->position.z);
	modelMatrix *= Transform3D::Translate(camera->position.x, camera->position.y, camera->position.z);


	modelMatrix *= Transform3D::Translate(-camera->position.x, -camera->position.y, -camera->position.z);

	modelMatrix *= Transform3D::RotateOY(barAngle.y);
	modelMatrix *= Transform3D::RotateOX(barAngle.x);

	if (numOfLives > 0) {
		modelMatrix *= Transform3D::Scale(1.5f, 1.5f, 1);
		RenderSimpleMesh(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}

	if (numOfLives > 1) {
		modelMatrix *= Transform3D::Translate(2, 0, 0);
		RenderSimpleMesh(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}

	if (numOfLives > 2) {
		modelMatrix *= Transform3D::Translate(2, 0, 0);
		RenderSimpleMesh(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema2::Restart() {
	numOfLives = 3;
	planePositionX = - 10;
	planePositionY = 0;
	planeAngle = 0;
	planeInclination = 0;
	fuel = startFuel;
	obstacles.clear();
	smokeTrail.clear();
	fuelList.clear();
	life.collected = true;
	score = 0;
	gameSpeed = 1;
	startGame = false;
	camera->Set(glm::vec3(0, 0, 70), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	barPosition = glm::vec3(20, 15, 50);
	barAngle = glm::vec3(0);
	heartPosition = glm::vec3(-25, 15, 50);
	fov = 45.0;
	fpMode = false;
	paused = false;
	printed = false;
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const glm::vec3& color2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->GetProgramID(), "Model");

	glm::mat4 temp = modelMatrix;

	// TODO : set shader uniform "Model" to modelMatrix

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(temp));

	// TODO : get shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->GetProgramID(), "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();//GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, Engine::GetElapsedTime());

	location = glGetUniformLocation(shader->GetProgramID(), "Random");
	glUniform1f(location, distanceBetweenVertexes / 2);

	location = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(shader->program, "second_object_color");
	glUniform3fv(location, 1, glm::value_ptr(color2));

	location = glGetUniformLocation(shader->program, "object_center");
	glUniform3fv(location, 1, glm::value_ptr(cylinderCenter));

	location = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location, 1, glm::value_ptr(lightPosition));



	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;
	location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1f(location, materialShininess);

	location = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location, materialKd);

	location = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location, materialKs);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 20.0f;

		if (window->KeyHold(GLFW_KEY_I)) {
			camera->RotateFirstPerson_OX(+deltaTime);
			barAngle.x += deltaTime;
			cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << endl;
			camera->MoveUpward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_O)) {
			camera->RotateFirstPerson_OX(-deltaTime);
			barAngle.x -= deltaTime;
			cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << endl;
			camera->MoveUpward(-cameraSpeed * deltaTime);
		}



		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_LEFT)) {
			// TODO : translate the camera forward
			camera->RotateFirstPerson_OZ(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_RIGHT)) {
			// TODO : translate the camera to the left
			camera->RotateFirstPerson_OZ(cameraSpeed * deltaTime);
		}

		/*if (window->KeyHold(GLFW_KEY_1)) {
			fov += 10 * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_2)) {
			fov -= 10 * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_3)) {
			width += 10 * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_4)) {
			width -= 10 * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_5)) {
			height += 10 * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_6)) {
			height -= 10 * deltaTime;
		}*/
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_R) {
		Restart();
	}

	if (key == GLFW_KEY_P) {
		paused = !paused;
	}

	if (key == GLFW_KEY_V) {
		skinNumber = (skinNumber + 1) % totalSkins;
	}

	if (key == GLFW_KEY_B) {
		cout << seaNumber << endl;
		seaNumber = (seaNumber + 1) % totalSeas;
	}

	if (key == GLFW_KEY_C) {
		if (!fpMode) {
			camera->Set(glm::vec3(planePositionX, planePositionY+3, 0), glm::vec3(0, planePositionY+3, 0), glm::vec3(0, 1, 0));
			barAngle.y -= RADIANS(90);
			fpMode = true;
			fov = 75.0f;
			barPosition = glm::vec3(25, 13, 25);
			heartPosition = glm::vec3(-25, 13, 25);
		}
		else {
			camera->Set(glm::vec3(0, planePositionY *0.9f/25 , 70), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			barAngle.y += RADIANS(90);
			fpMode = false;
			fov = 45.0f;
			barPosition = glm::vec3(25, 15, 50);
			heartPosition = glm::vec3(-25, 15, 50);
		}
		
	}

}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

	{
		float sensivityOX = 0.01f;
		float sensivityOY = 0.001f;
		glm::ivec2 resolution = window->GetResolution();
		if (window->GetSpecialKeyState() == 0 && numOfLives > 0) {
			planeDestination = (float)(resolution.y/2 - mouseY)/(resolution.y/2)*30;
			planeDestination = glm::clamp((float)(resolution.y / 2 - mouseY) / (resolution.y / 2) * 30, -25.0f, 25.0f);

		}

	}

	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			//renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OY(sensivityOX * -deltaX);
			camera->RotateFirstPerson_OX(sensivityOY * -deltaY);

			barAngle.x += sensivityOY * -deltaY;
			barAngle.y += sensivityOX * -deltaX;
		}

		/*if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OY(sensivityOX * -deltaX);
			camera->RotateThirdPerson_OX(sensivityOY * -deltaY);
		}*/

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
		startGame = true;
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
