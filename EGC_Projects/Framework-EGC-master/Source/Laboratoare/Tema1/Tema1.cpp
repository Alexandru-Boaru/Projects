#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Rectangle2D.h"

using namespace std;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}
//functia de creare a meshelor
/*
	name = numele pentru mesha
	vertices = lista de vertecsi a meshei
	indices = lista de indici a meshei
*/
Mesh* Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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
	return meshes[name];
}

//functie pentru crearea meshelor cercurilor
/*	name = numele pentru mesha
	numOfVertex = numarul de veretecsi din care e format cercul (fara centru)
	z = pozitia pe coordonata z (pentru ordonarea obiectelor in scena)
	color = culoarea cercului
*/
void Tema1::CreateCircleMesh(const char* name,int numOfVertex, int z, glm::vec3 color) {

	vector<VertexFormat> vertices;
	vector<unsigned short> indices;
	vertices.push_back(VertexFormat(glm::vec3(0, 0, z),color));
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

void Tema1::Init()
{
	//setarea camerei
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, resolution.x, 0, resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	//setarea spatiului logic
	screenH = 720;
	screenW = 1280;

	windowH = resolution.y;
	windowW = resolution.x;

	height = screenH;				//inaltimea maxima a obstacolului
	width = screenW / 20;			//latimea unui dreptunghi

	//crearea meshelor pentru obstacole (culori diferite)

	Rectangle2D rect = Rectangle2D::Rectangle2D("rect",glm::vec3(0,0,0), width, screenH,glm::vec3(1,0,0));
	meshes["rect"] = new Mesh("generated rect");
	Mesh* shape = CreateMesh("rect", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("rect1", glm::vec3(0, 0, 0), width, height, glm::vec3((float)250/255, (float)128/255, (float)114/255));
	meshes["rect1"] = new Mesh("generated rect1");
	shape = CreateMesh("rect1", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("rect2", glm::vec3(0, 0, 0), width, height, glm::vec3((float)220/255, (float)20/255, (float)60/255));
	meshes["rect2"] = new Mesh("generated rect2");
	shape = CreateMesh("rect2", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("rect3", glm::vec3(0, 0, 0), width, height, glm::vec3((float)178/255, (float)34/255, (float)34/255));
	meshes["rect3"] = new Mesh("generated rect3");
	shape = CreateMesh("rect3", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("rect4", glm::vec3(0, 0, 0), width, height, glm::vec3((float)219/255, (float)112/255, (float)147/255));
	meshes["rect4"] = new Mesh("generated rect4");
	shape = CreateMesh("rect4", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("rect5", glm::vec3(0, 0, 0), width, height, glm::vec3(1, (float)69/255, 0));
	meshes["rect5"] = new Mesh("generated rect5");
	shape = CreateMesh("rect5", rect.vertices, rect.indices);

	rectangleColor = { "rect", "rect1", "rect2", "rect3", "rect4", "rect5" };

	rect = Rectangle2D::Rectangle2D("ground", glm::vec3(0, 0, 0), screenW, screenH *2/100 , glm::vec3(0.486f, 0.988f, 0));
	meshes["ground"] = new Mesh("generated ground");
	shape = CreateMesh("ground", rect.vertices, rect.indices);

	rect = Rectangle2D::Rectangle2D("blackScreen", glm::vec3(0, 0, 1), 1, 1, glm::vec3(0, 0, 0));
	meshes["blackScreen"] = new Mesh("generated blackScreen");
	shape = CreateMesh("blackScreen", rect.vertices, rect.indices);

	//definirea catorva culori

	glm::vec3 cyan = { 0, 1, 1 };
	glm::vec3 black = { 0.1f,0.1f,0.1f };
	glm::vec3 white = { 1,1,1 };
	glm::vec3 orange = { 1,(float)165 / 256,0 };
	glm::vec3 yellow = { 1, 1, 0 };

	//se creaza informatia meshei pasarii


	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	//piesa pentru corpul pasarii

	vertices =
	{
		VertexFormat(glm::vec3(10, 26,  0), black),
		VertexFormat(glm::vec3(0, 16,  0), black),
		VertexFormat(glm::vec3(3, 9,  0), black),
		VertexFormat(glm::vec3(9, 2,  0), black),
		VertexFormat(glm::vec3(15, 0,  0), black),
		VertexFormat(glm::vec3(23, 5,  0), black),
		VertexFormat(glm::vec3(26, 20,  0), black),
		VertexFormat(glm::vec3(30, 8,  0), black),
		VertexFormat(glm::vec3(38, 24,  0), black),
		VertexFormat(glm::vec3(58, 15,  0), black),
		VertexFormat(glm::vec3(78, 44,  0), black),
		VertexFormat(glm::vec3(75, 18,  0), black),
		VertexFormat(glm::vec3(92, 19,  0), black),
		VertexFormat(glm::vec3(89, 46,  0), black),
		
	};

	indices =
	{
		0, 2, 1,
		0, 3, 2,
		0, 4, 3,
		0, 5, 4,
		0, 6, 5,
		6, 7, 5,
		8, 7, 6,
		8, 9, 7,
		8, 10, 9,
		10, 11, 9,
		10, 13, 12,
		10, 12, 11
	};
	

	meshes["bird"] = new Mesh("generated bird");
	shape = CreateMesh("bird", vertices, indices);

	vertices.clear();
	indices.clear();

	//piesa pentru capul pasarii

	vertices =
	{
		VertexFormat(glm::vec3(105, 35,  0), white),
		VertexFormat(glm::vec3(92, 19,  0), white),
		VertexFormat(glm::vec3(89, 46,  0), white),
		VertexFormat(glm::vec3(104, 57,  0), white),
		VertexFormat(glm::vec3(116, 56,  0), white),
		VertexFormat(glm::vec3(131, 56,  0), white)
	};

	indices =
	{

		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5
	};

	meshes["head"] = new Mesh("generated head");
	shape = CreateMesh("head", vertices, indices);

	vertices.clear();
	indices.clear();

	//piesa pentru ciocul pasarii

	vertices =
	{
		VertexFormat(glm::vec3(153, 37,  0), orange),
		VertexFormat(glm::vec3(105, 35,  0), orange),
		VertexFormat(glm::vec3(131, 56,  0), orange),
		VertexFormat(glm::vec3(146, 52,  0), orange),
		VertexFormat(glm::vec3(155, 46,  0), orange),
		VertexFormat(glm::vec3(159, 40,  0), orange),
		VertexFormat(glm::vec3(160, 33,  0), orange)
	};

	indices =
	{

		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6
	};

	meshes["beak"] = new Mesh("generated beak");
	shape = CreateMesh("beak", vertices, indices);

	vertices.clear();
	indices.clear();

	//piesa pentru aripile pasarii

	vertices =
	{
		VertexFormat(glm::vec3(54, 73,  0), black),
		VertexFormat(glm::vec3(47, 28,  0), black),
		VertexFormat(glm::vec3(31, 48,  0), black),
		VertexFormat(glm::vec3(25, 61,  0), black),
		VertexFormat(glm::vec3(26, 81,  0), black),
		VertexFormat(glm::vec3(33, 92,  0), black),
		VertexFormat(glm::vec3(46, 90,  0), black),
		VertexFormat(glm::vec3(65, 96,  0), black),
		VertexFormat(glm::vec3(74, 94,  0), black),
		VertexFormat(glm::vec3(82, 78,  0), black),
		VertexFormat(glm::vec3(84, 67,  0), black),
		VertexFormat(glm::vec3(80, 55,  0), black),
		VertexFormat(glm::vec3(77, 42,  0), black),
	};

	indices =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 1
	};

	meshes["wings"] = new Mesh("generated wings");
	shape = CreateMesh("wings", vertices, indices);

	//crearea unor cercuri

	CreateCircleMesh("circle", 100, -1, glm::vec3(0, 1, 0));
	CreateCircleMesh("sun", 200, -2, glm::vec3(1, 1, 0));
	CreateCircleMesh("eye", 8, 1, glm::vec3(0.502, 0, 0.502));

	//initilizarea unor variabile

	startTimeForJump = 0.2;		//durata saltului
	timeForJump = -1;			//contor pentru salt
	startGravity = 1.05;		//gravitatia de inceput
	gravity = 1.05;				//gravitatia curenta
	startGame = false;			//verifica daca jocul este in stare play/pause

	//informatii pentru pozitionarea pasarii
	birdCenter.x = 80;				//coordonata x a centrului pasarii
	birdCenter.y = 45;				//coordonata y a centrului pasarii
	birdPosX = screenW / 3;			//coordonata x a plasarii pasarii in scena 
	birdPosY = 2* screenH / 3;		//coordonata y a plasarii pasarii in scena 
	birdAngle = 0;					//unghiul inclinarii pasarii
	birdRadius = (160 + 96) / 8;	//raza cercului care inconjoara pasarea
	wingAngle = 1;					//factor care scaleza aripile
	sign = -1;

	//informatii pentru obstacole
	hole = screenH / 3;				//largimea gaurii dintre dreptunghiuri
	offset = 3 * screenW / 4;		//punctul fata de origine de care trebuie sa treaca un obstacol pentru a genera altul
	obstacleSpeed = 300;			//viteza obstacolelor

	//mesaj de start
	cout << endl <<endl<< "NEW GAME" << endl;
	score = 0;						//scorul curent
	highScore = 0;					//scorul cel mai bun obtinut
	cout << score << endl;
}

void Tema1::FrameStart()
{
	//setarea culorii de fundal
	glClearColor((float) 135/255, (float) 206/255, (float) 235/255, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	//seteaza zona in care se deseneaza
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	//se adauga spatii negre de padding
	AddBlackScreen();

	//pozitionare soare
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
	modelMatrix *= Transform2D::Translate(screenW/2, 0);
	modelMatrix *= Transform2D::Scale(screenW / 4, screenW / 4);
	RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);

	//pozitionare sol
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
	RenderMesh2D(meshes["ground"], shaders["VertexColor"], modelMatrix);

	//pozitionare cerc ajutator
	if (circleEnabled)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
		modelMatrix *= Transform2D::Translate(birdPosX, birdPosY);
		modelMatrix *= Transform2D::Scale(birdRadius, birdRadius);
		RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

	}

	//pozitionare ochi
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
	modelMatrix *= Transform2D::Translate(birdPosX, birdPosY);
	modelMatrix *= Transform2D::Rotate(birdAngle);
	modelMatrix *= Transform2D::Translate(-birdCenter.x / 2, -birdCenter.y / 2);
	modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
	modelMatrix *= Transform2D::Translate(112, 45);
	modelMatrix *= Transform2D::Scale(5, 5);
	RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

	//pozitionare aripi
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
	modelMatrix *= Transform2D::Translate(birdPosX, birdPosY);
	modelMatrix *= Transform2D::Rotate(birdAngle);
	modelMatrix *= Transform2D::Translate(-birdCenter.x / 2, -birdCenter.y / 2);
	modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
	modelMatrix *= Transform2D::Translate(0, 36);
	modelMatrix *= Transform2D::Scale(1, wingAngle);
	modelMatrix *= Transform2D::Translate(0, -36);
	
	RenderMesh2D(meshes["wings"], shaders["VertexColor"], modelMatrix);

	//implementare pentru animatie aripi
	if (startGame && !gameOver) {
		if (wingAngle < -1) {
			sign = 1;
		}
		else if (wingAngle > 1) {
			sign = -1;
		}
		wingAngle += sign * 5 *deltaTimeSeconds;
	}
	
	//pozitionarea resputului pieselor pasarii
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
	modelMatrix *= Transform2D::Translate(birdPosX, birdPosY);
	modelMatrix *= Transform2D::Rotate(birdAngle);
	modelMatrix *= Transform2D::Translate(-birdCenter.x / 2, -birdCenter.y / 2);
	modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
	RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["bird"], shaders["VertexColor"], modelMatrix);

	//miscarea pasarii
	BirdMotion(deltaTimeSeconds);

	//miscarea obstacolelor
	PipeMotion(deltaTimeSeconds);
}

void Tema1::FrameEnd()
{

}

//metoda care adauga dreptunghiuri negre pe margini pe post de padding (in cazurile in care dimensiunea ferestrei se schimba)
void Tema1::AddBlackScreen() {

	if ((float)windowW / screenW > (float)windowH / screenH) {
		float w = (float)((windowW * screenH / windowH) - screenW) / 2;
		float h = screenH;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
		modelMatrix *= Transform2D::Scale(-w, h);
		RenderMesh2D(meshes["blackScreen"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
		modelMatrix *= Transform2D::Translate(screenW, 0);
		modelMatrix *= Transform2D::Scale(w, h);
		RenderMesh2D(meshes["blackScreen"], shaders["VertexColor"], modelMatrix);
	}

	if ((float)windowW / screenW < (float)windowH / screenH) {
		float w = screenW;
		float h = (float)((windowH * screenW / windowW) - screenH) / 2;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
		modelMatrix *= Transform2D::Scale(w, -h);
		RenderMesh2D(meshes["blackScreen"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
		modelMatrix *= Transform2D::Translate(0, screenH);
		modelMatrix *= Transform2D::Scale(w, h);
		RenderMesh2D(meshes["blackScreen"], shaders["VertexColor"], modelMatrix);
	}
}

//metoda care se ocupa cu mecanica obstacolelor
void Tema1::PipeMotion(float deltaTimeSeconds) {

	//se lucreaza cu o lista de obstacole
	//daca lista e goala se adauga un obstacol
	if (rectList.empty()) {
		glm::vec2 newPoint = { screenW + width , NewRandomPipe()};
		Rectangle2D r = Rectangle2D::Rectangle2D(newPoint, rectangleColor[rand() % rectangleColor.size()]);
		rectList.push_back(r);
	}

	//se parcurge lista de obstacole
		for (int i = 0; i < rectList.size(); i++) {

			//daca se identifica o coliziune intre pasare si vreunul din obstacole, atunci se incheie jocul
			if (rectList[i].checkCollision({ birdPosX, birdPosY }, birdRadius, hole, width, height) && !gameOver) {
				cout << "GAME OVER" << endl << "FINAL SCORE: " << score << endl;
				gameOver = true;
				if (highScore < score) {
					highScore = score;
					cout << "NEW HIGHSCORE: " << highScore << endl;
				}
			}

			//cat timp obstacolele sunt vizibile, acestea se vor misca de la dreapta la stanga
			if (rectList[i].point.x + width > 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
				modelMatrix *= Transform2D::Translate(rectList[i].point.x, 0) * Transform2D::Scale(1, rectList[i].point.y / height);
				RenderMesh2D(meshes[rectList[i].color], shaders["VertexColor"], modelMatrix);

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Visualization(windowW, windowH, screenW, screenH);
				modelMatrix *= Transform2D::Translate(rectList[i].point.x, rectList[i].point.y + hole)
					* Transform2D::Scale(1, (screenH - rectList[i].point.y - hole) / height);
				RenderMesh2D(meshes[rectList[i].color], shaders["VertexColor"], modelMatrix);

				if (startGame && !gameOver) {
					rectList[i].point.x -= obstacleSpeed * deltaTimeSeconds;
					
					//in cazul in care gaura dintre obstacole este mobila, 
					//sa se schimba directia miscarii in momentul in care se ating limitele date
					if (rectList[i].point.y < screenH * 5 / 100) {
						rectList[i].sign = 1;
					}
					else if (rectList[i].point.y > screenH * 95 / 100 - hole) {
						rectList[i].sign = -1;
					}
					rectList[i].point.y += rectList[i].sign * 100 * deltaTimeSeconds;
				}		
			}

			//daca un obstacol a trecut de un punct situat la distanta offset fata de origine,
			//atunci se creaza un nou obstacol
			if (rectList[i].point.x < offset && !rectList[i].checked) {
				rectList[i].checked = true;
				glm::vec2 newPoint = { screenW + width , NewRandomPipe() };
				int chance = rand() % 5;
				Rectangle2D r;
				if (chance == 0) {
					r = Rectangle2D::Rectangle2D(newPoint, rectangleColor[rand()%rectangleColor.size()], 1);
				}
				else {
					r = Rectangle2D::Rectangle2D(newPoint, rectangleColor[rand() % rectangleColor.size()], 0);

				}
				rectList.push_back(r);
			}

			//in momentul in care pasarea depaseste un obstacol se incrementeaza punctajul
			//dupa un anumit numar de puncte, viteza obstacolelor creste
			if (rectList[i].point.x + width < birdPosX - birdRadius && !rectList[i].counted) {
				rectList[i].counted = true;
				score++;
				if (score % 10 == 0) {
					obstacleSpeed += 50;
				}
				cout << score << endl;
			}
			
		}

		//daca obstacolul cel mai din stanga a iesit din scena, atunci acesta e sters din lista
		if (rectList[0].point.x + width< 0) {
			rectList.erase(rectList.begin());
		}
}

//metoda care se ocupa cu mecanica pasarii
void Tema1::BirdMotion(float deltaTimeSeconds) {
	if (startGame && !gameOver) {

		//pasarea este in picaj
		if (timeForJump < 0) {
			birdPosY -= 400 * gravity * deltaTimeSeconds;
			gravity *= startGravity;
			if (birdAngle > -1) {
				//se inclina in jos
				birdAngle -= 3.5f * deltaTimeSeconds;
			}
		}

		//pasarea se ridica
		else {
			birdPosY += 500 * timeForJump * 10 * deltaTimeSeconds;
			timeForJump -= deltaTimeSeconds;
			gravity = startGravity;
			if (birdAngle < 0.75) {
				//se inclina in sus
				birdAngle +=  4* deltaTimeSeconds;
			}
		}
	}

	//daca pasarea atinge vreuna din marginile scenei, atunci se incheie jocul
	if ((birdPosY - birdRadius< 0 || birdPosY + birdRadius > screenH) && !gameOver) {
		cout << "GAME OVER" << endl << "FINAL SCORE: " << score << endl;
		gameOver = true;
		if (highScore < score) {
			highScore = score;
			cout << "NEW HIGHSCORE: " << highScore << endl;
		}
	}
}

//metoda folosita pentru a da o inaltime random a gaurii dintre obstacole
int Tema1::NewRandomPipe() {
	int res = rand() % (screenH * 90 / 100 - hole) + screenH * 5 / 100;
	return res;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Tema1::OnKeyPress(int key, int mods)
{
	//daca se apasa tasta space, atunci pasarea se ridica pentru o perioada foarte scurta de timp
	if (key == GLFW_KEY_SPACE && !gameOver) {
		if (!startGame) {
			startGame = true;
		}
		if (timeForJump < startTimeForJump/2) {
			timeForJump = startTimeForJump;
		}
	}

	//buton de pauza al jocului
	if (key == GLFW_KEY_P && !gameOver) {
		startGame = !startGame;
	}

	//buton de restart al jocului
	if (key == GLFW_KEY_R) {
		startGame = false;
		gameOver = false;
		birdPosX = screenW / 3;
		birdPosY = 2 * screenH / 3;
		birdAngle = 0;
		wingAngle = 1;

		timeForJump = -1;
		gravity = 1.05;
		score = 0;
		obstacleSpeed = 300;
		rectList.clear();
		cout << endl << "NEW GAME" << endl;
		cout <<score << endl;
	}

	//buton de activare/dezactivare a cercului ajutator
	if (key == GLFW_KEY_E) {
		circleEnabled = !circleEnabled;
	}

}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
	windowH = height;
	windowW = width;
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)windowW, 0, (float)windowH, 0.01f, 400);
}
