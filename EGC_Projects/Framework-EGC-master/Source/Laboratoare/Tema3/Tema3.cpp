#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"
#include <stb/stb_image.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

//functie pentru crearea unui teren plat

void Tema3::CreateFlatGround(int width, int height, const char* name)
{
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float a, b;
			a = (float)i/(height - 1);
			b = (float)j/(width - 1);
			vertices.push_back(VertexFormat(glm::vec3((float)i,0,(float)j),
											glm::vec3(1), 
											glm::vec3(0,1,0), 
											glm::vec2(a,b)));
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

//functie pentru crearea unei sfere
/*
	name = numele meshei
	sectorCount = numarul de sectoare
	stackCount = numarul de stack-uri
*/

void Tema3::CreateSphere(const char* name, int sectorCount, int stackCount, vector<glm::vec3> colors) {
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
			vertices.push_back(VertexFormat(glm::vec3(x, y, z), colors[rand() % colors.size()], glm::vec3(1), glm::vec2((float)j / sectorCount, (float)i / stackCount)));
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
				indices.push_back(k1 + 1);
				indices.push_back(k2);

			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2 + 1);
				indices.push_back(k2);

			}
		}
	}
	meshes[name] = new Mesh(name);
	Mesh* sphere = CreateMesh(name, vertices, indices);
}

//functie care seteaza camera la o pozitie anterioara a urmatorului jucator

void Tema3::SetCamera() 
{
	camera->Set(oldCameraPosition[currentWorm], worms[currentWorm].position + glm::vec3(0.0, 0.5, 0.0), oldCameraUp[currentWorm]);
	lastAlt[currentWorm] = worms[currentWorm].position.y;

	cameraSet = true;
}

void Tema3::Init()
{

	
	worms[0].position = glm::vec3(-12.5,0,0);
	worms[0].angle = glm::vec2(0);
	worms[1].position = glm::vec3(glm::vec3(12.5, 0, 0));
	worms[1].angle = glm::vec2(0);

	spotPositions[0] = glm::vec3(-5, 10, 0);
	spotPositions[1] = glm::vec3(5, 10, 0);

	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";
	const string meshLoc = "Source/Laboratoare/Tema3/Meshes/";

	// Load textures

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wormcolor_v2.png").c_str(), GL_REPEAT);
		mapTextures["worm_skin"] = texture;
	}

	{
		mapTextures["random"] = CreateRandomTexture(32, 32);
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "night_sky.png").c_str(), GL_REPEAT);
		mapTextures["sky_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "heightmap.png").c_str(), GL_REPEAT);
		mapTextures["heightmap"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "dry_earth.png").c_str(), GL_REPEAT);
		mapTextures["dry_earth"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "red.png").c_str(), GL_REPEAT);
		mapTextures["red"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "green.png").c_str(), GL_REPEAT);
		mapTextures["green"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "blue.png").c_str(), GL_REPEAT);
		mapTextures["blue"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "yellow.png").c_str(), GL_REPEAT);
		mapTextures["yellow"] = texture;
	}
	
	// incarcare meshe
	{
		Mesh* mesh = new Mesh("worm");
		mesh->LoadMesh(meshLoc, "newworm_v2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("arrow");
		mesh->LoadMesh(meshLoc, "arrow.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	// Creare a unui quad simplu
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)	
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	CreateFlatGround(251, 251, "plane");
	CreateSphere("sky", 200, 200, { glm::vec3(1) });

	// Create a shader program for drawing face polygon with the color of the normal

	//shader pt vierme si alte obiecte
	{
		Shader *shader = new Shader("WormShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//shader pt teren
	{
		Shader* shader = new Shader("HeightMap");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/HMVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/HMFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	//proprietati lumina
	{
		lightPosition = glm::vec3(0, 10, 1);
		lightDirection = glm::vec3(0, -1, 0);
		startLightDirection = glm::vec3(0, -1, 0);
		materialShininess = 30;
		materialKd = 0.6;
		materialKs = 0.7;
		cut_off = RADIANS(30.0f);
	}

	//buffer de inaltimi
	heightPixels = stbi_load((textureLoc + "heightmap.png").c_str(), &width, &height, &channels, STBI_grey);

	camera = new Tema_3::Camera();
	fov = 45;

	oldCameraPosition[0] = glm::vec3(worms[0].position.x + 0.4, (heightPixels[(int)(floor((worms[0].position.z + 25) / 50 * height) * width + floor((worms[0].position.x + 25) / 50 * width))] / 255.0) * terrainScale * 0.2 + 0.5, worms[0].position.z - 2);
	oldCameraPosition[1] = glm::vec3(worms[1].position.x + 0.4, (heightPixels[(int)(floor((worms[1].position.z + 25) / 50 * height) * width + floor((worms[1].position.x + 25) / 50 * width))] / 255.0) * terrainScale * 0.2 + 0.5, worms[1].position.z - 2);

	oldCameraUp[0] = glm::vec3(0, 1, 0);
	oldCameraUp[1] = glm::vec3(0, 1, 0);

	lastAlt[0] = oldCameraPosition[0].y - 0.5;
	lastAlt[1] = oldCameraPosition[1].y - 0.5;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

}

void Tema3::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 1000.0f);
}

void Tema3::Update(float deltaTimeSeconds)
{
	spotPositions[0].x = worms[0].position.x;
	spotPositions[0].z = worms[0].position.z;
	spotPositions[1].x = worms[1].position.x;
	spotPositions[1].z = worms[1].position.z;
	
	//randare cer
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(30, 30, 30);
		modelMatrix *= Transform3D::RotateOX(RADIANS(90.0f));
		RenderSimpleMesh(meshes["sky"], shaders["WormShader"], modelMatrix, 3, mapTextures["sky_texture"]);
	}

	//randare teren deformat
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-25.0f, 0, -25.0f);
		modelMatrix *= Transform3D::Scale(0.2f,0.2f,0.2f);
		RenderSimpleMesh(meshes["plane"], shaders["HeightMap"], modelMatrix, 0, mapTextures["heightmap"], mapTextures["dry_earth"]);
	}

	//randare viermi
	for(int i = 0 ; i < 2; i++)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		worms[i].position.y = (heightPixels[(int)(floor((worms[i].position.z+25)/50 * height) * width + floor((worms[i].position.x + 25) / 50 * width))] / 255.0) * terrainScale * 0.2;
		if (lastAlt[currentWorm] != worms[currentWorm].position.y) {
			camera->position.y += worms[currentWorm].position.y-lastAlt[currentWorm];
		}
		oldCameraPosition[i].y -= lastAlt[i] - worms[i].position.y;
		lastAlt[i] = worms[i].position.y;

		modelMatrix *= Transform3D::Translate(worms[i].position.x, worms[i].position.y, worms[i].position.z);
		
		modelMatrix *= Transform3D::RotateOY(worms[i].angle.y);
		modelMatrix *= Transform3D::RotateOX(worms[i].angle.x);
		modelMatrix *= Transform3D::Translate(0.0,0.5,0.0);
		modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
		RenderSimpleMesh(meshes["worm"], shaders["WormShader"], modelMatrix, 1, mapTextures["worm_skin"]);
	}

	//randare proiectil
	DisplayProjectiles(deltaTimeSeconds);
	if (!cameraSet) {
		SetCamera();
	}

	if (projectiles.size() == 0 && alreadyLaunched == true) {
		if (delayTime > 0) {
			delayTime -= deltaTimeSeconds;
			if (delayTime < 0) {
				alreadyLaunched = false;
				locked = false;
				maxSteps = startMaxSteps;
				SetCamera();
				
			}
		}
	}

	//randare componente de UI
	if(!alreadyLaunched)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 barPosition = camera->position + camera->forward * 0.5f;
		modelMatrix *= Transform3D::Translate(barPosition.x, barPosition.y,barPosition.z);
		modelMatrix *= Transform3D::Translate(camera->up.x * 0.15f, camera->up.y * 0.15f, camera->up.z * 0.15f);
		modelMatrix *= Transform3D::Translate(camera->right.x*0.2f, camera->right.y * 0.2f, camera->right.z * 0.2f);
		modelMatrix *= Transform3D::RotateOY(-glm::asin(0.4f/camera->distanceToTarget));
		modelMatrix *= Transform3D::RotateOY(barAngle[currentWorm].y);
		modelMatrix *= Transform3D::RotateOX(barAngle[currentWorm].x);
		modelMatrix *= Transform3D::Scale(0.2f, 0.03f, 0.2f);
		
		
		RenderSimpleMesh(meshes["square"], shaders["WormShader"], modelMatrix, 0, NULL, NULL, glm::vec4(1));

		modelMatrix = Transform3D::Translate(camera->up.x * -0.05f, camera->up.y * -0.05f, camera->up.z * -0.05f)*modelMatrix;
		RenderSimpleMesh(meshes["square"], shaders["WormShader"], modelMatrix, 0, NULL, NULL, glm::vec4(1));


		modelMatrix = glm::mat4(1);
		barPosition = camera->position + camera->forward * 0.499f;
		modelMatrix *= Transform3D::Translate(barPosition.x, barPosition.y, barPosition.z);
		modelMatrix *= Transform3D::Translate(camera->up.x * 0.15f, camera->up.y * 0.15f, camera->up.z * 0.15f);
		modelMatrix *= Transform3D::Translate(camera->right.x * 0.2f, camera->right.y * 0.2f, camera->right.z * 0.2f);
		modelMatrix *= Transform3D::RotateOY(-glm::asin(0.4f / camera->distanceToTarget));
		modelMatrix *= Transform3D::RotateOY(barAngle[currentWorm].y);
		modelMatrix *= Transform3D::RotateOX(barAngle[currentWorm].x);
		modelMatrix *= Transform3D::Scale(0.19f, 0.02f, 0.19f);
		modelMatrix *= Transform3D::Translate(0.5, -0.5, 0.0);
		modelMatrix *= Transform3D::Scale(worms[currentWorm].hp / 100.0f, 1.0, 1.0);
		modelMatrix *= Transform3D::Translate(-0.5, 0.5, 0.0);

		RenderSimpleMesh(meshes["square"], shaders["WormShader"], modelMatrix, 0, NULL, NULL,  currentWorm==0?glm::vec4(1.0,0.0,0.0,1.0): glm::vec4(0.0, 0.0, 1.0, 1.0));
	
		modelMatrix = glm::mat4(1);

		barPosition = camera->position + camera->forward * 0.499f;
		modelMatrix *= Transform3D::Translate(camera->up.x * -0.05f, camera->up.y * -0.05f, camera->up.z * -0.05f);
		modelMatrix *= Transform3D::Translate(barPosition.x, barPosition.y, barPosition.z);
		modelMatrix *= Transform3D::Translate(camera->up.x * 0.15f, camera->up.y * 0.15f, camera->up.z * 0.15f);
		modelMatrix *= Transform3D::Translate(camera->right.x * 0.2f, camera->right.y * 0.2f, camera->right.z * 0.2f);
		modelMatrix *= Transform3D::RotateOY(-glm::asin(0.4f / camera->distanceToTarget));
		modelMatrix *= Transform3D::RotateOY(barAngle[currentWorm].y);
		modelMatrix *= Transform3D::RotateOX(barAngle[currentWorm].x);
		modelMatrix *= Transform3D::Scale(0.19f, 0.02f, 0.19f);
		modelMatrix *= Transform3D::Translate(0.5, -0.5, 0.0);
		modelMatrix *= Transform3D::Scale(maxSteps / 10.0f, 1.0, 1.0);
		modelMatrix *= Transform3D::Translate(-0.5, 0.5, 0.0);

		RenderSimpleMesh(meshes["square"], shaders["WormShader"], modelMatrix, 0, NULL, NULL, glm::vec4(0.5,0.5,0.5,1.0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(worms[currentWorm].position.x, worms[currentWorm].position.y, worms[currentWorm].position.z);
		modelMatrix *= Transform3D::RotateOY(worms[currentWorm].angle.y);
		modelMatrix *= Transform3D::RotateOX(worms[currentWorm].angle.x+asin(0.4));
		modelMatrix *= Transform3D::Translate(0.0,0.5,0.0);
		modelMatrix *= Transform3D::Scale(0.3f, 0.3f, 0.3f);

		RenderSimpleMesh(meshes["arrow"], shaders["WormShader"], modelMatrix, 0, NULL, NULL, glm::vec4(1.0,1.0,1.0,0.5));

	}

	
}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

//functie care paseaza informatiile catre shader

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int mode, Texture2D* texture1, Texture2D* texture2, glm::vec4 tempColBar)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int use_second = glGetUniformLocation(shader->program, "mode");
	glUniform1i(use_second, mode);

	int time = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(time, Engine::GetElapsedTime()/2);
	

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	light_position = glGetUniformLocation(shader->program, "spot_positions[0]");
	glUniform3fv(light_position, 1, glm::value_ptr(spotPositions[0]));

	light_position = glGetUniformLocation(shader->program, "spot_positions[1]");
	glUniform3fv(light_position, 1, glm::value_ptr(spotPositions[1]));

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	light_direction = glGetUniformLocation(shader->program, "spot_directions[0]");
	glUniform3fv(light_direction, 1, glm::value_ptr(spotDirections[0]));

	light_direction = glGetUniformLocation(shader->program, "spot_directions[1]");
	glUniform3fv(light_direction, 1, glm::value_ptr(spotDirections[1]));

	int co = glGetUniformLocation(shader->program, "cut_off");
	glUniform1f(co, cut_off);

	int is_spot_light = glGetUniformLocation(shader->program, "is_spot_light");
	glUniform1i(is_spot_light, isSpotLight);

	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	glm::vec3 tempCol1 = glm::vec3(1.0,0.0,0.0);
	glm::vec3 tempCol2 = glm::vec3(0.0, 0.0, 1.0);

	
	int light_color = glGetUniformLocation(shader->program, "light_colors[0]");
	glUniform3fv(light_color, 1, glm::value_ptr(tempCol1));

	light_color = glGetUniformLocation(shader->program, "light_colors[1]");
	glUniform3fv(light_color, 1, glm::value_ptr(tempCol2));
	

	int ver_color = glGetUniformLocation(shader->program, "ver_color");
	glUniform4fv(ver_color, 1, glm::value_ptr(tempColBar));

	int size = glGetUniformLocation(shader->program, "size");
	glUniform2f(size, 250.0f, 250.f);

	int t_scale = glGetUniformLocation(shader->program, "terrain_scale");
	glUniform1f(t_scale, terrainScale);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

//functie pentru creare de meshe

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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

//functie care creaza crater pornind din punctul definit de z si x

void Tema3::CreateHole(float z, float x)
{
	
	for (int i = -4; i < 5; i++) {
		for (int j = -4; j < 5; j++) {
			if (((i == -4 || i == 4) && (j == -4 || j == 4)) ||
				((i == -3) && (j == -4 || j == 4)) || 
				((i == 3) && (j == -4 || j == 4)) ||
				((i == -4) && (j == -3 || j == 3)) ||
				((i == 4) && (j == -3 || j == 3))) {
				continue;
			}
			if (heightPixels[(int)((z+i) * width + (x+j))] > 25) {
				heightPixels[(int)((z + i) * width + (x + j))] -= 25*(1-((float)abs(i)+(float)abs(j))/8.0);
			}
			else {
				heightPixels[(int)((z + i) * width + (x + j))] = 0;
			}
		}
	}

	//se verifica daca viermele a fost atins de impactul proiectilului
	for (int i = 0; i < 2; i++) {
		
		if ((worms[i].position.z + 25) / 50 * height > z - 4 &&
			(worms[i].position.z + 25) / 50 * height < z + 4 &&
			(worms[i].position.x + 25) / 50 * width > x - 4 &&
			(worms[i].position.x + 25) / 50 * width < x + 4) {
			worms[i].hp -= 10;
			cout << "Player " << i+1  << " was hit. Current HP: " << worms[i].hp << endl;
			if (worms[i].hp <= 0) {
				gameOver = true;
				cout << "Player " << 2 - i << " wins" << endl;
			}
		}
	}

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mapTextures["heightmap"]->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, (void*)heightPixels);
	CheckOpenGLError();

}


//randarea si miscarea proiectilului in scena

void Tema3::DisplayProjectiles(float deltaTimeSeconds) {
	float projSpeed = 7.0f;
	for (int i = 0; i < projectiles.size(); i++) {
		if (!projectiles[i].hit) {
			if (projectiles[i].direction.y > -1) {
				projectiles[i].direction.y -= deltaTimeSeconds * 0.5;
			}
			projectiles[i].center += projSpeed * deltaTimeSeconds * projectiles[i].direction;
			camera->position.x = projectiles[i].center.x;
			camera->position.z = projectiles[i].center.z;
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(projectiles[i].center.x, projectiles[i].center.y, projectiles[i].center.z);
			modelMatrix *= Transform3D::Scale(0.1, 0.1, 0.1);
			RenderSimpleMesh(meshes["sphere"], shaders["WormShader"], modelMatrix, 1,mapTextures["random"]);
		}
		if (!projectiles.empty() && (projectiles[i].center.x < -25 ||
			projectiles[i].center.x > 25 ||
			projectiles[i].center.z < -25 ||
			projectiles[i].center.z > 25 ||
			projectiles[i].center.y < (heightPixels[(int)(floor((projectiles[0].center.z + 25) / 50 * height) * width + floor((projectiles[0].center.x + 25) / 50 * width))] / 255.0) * terrainScale * 0.2)) {
			projectiles[i].hit = true;
			if (projectiles[i].center.y < (heightPixels[(int)(floor((projectiles[0].center.z + 25) / 50 * height) * width + floor((projectiles[0].center.x + 25) / 50 * width))] / 255.0) * terrainScale * 0.2){
				CreateHole(floor((projectiles[0].center.z + 25) / 50 * height), floor((projectiles[0].center.x + 25) / 50 * width));
			}
		}
	}
	
	if (!projectiles.empty() && projectiles[0].hit){
		projectiles.erase(projectiles.begin());
		delayTime = 2.0f;
		
	}
}



Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	//genereaza informatii random pentru textura

	for (int i = 0; i < size; i++) {
		data[i] = rand()%255;
	}


	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Genereaza si da bind la texture id

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	//setez datele
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//genereaza mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	CheckOpenGLError();

	// salveaza textura intr-un wrapper Texture2D
	Texture2D* texture = new Texture2D();
	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	return texture;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{

	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = camera->right;
		glm::vec3 forward = camera->forward;
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		//deplasare vierme

		if (!alreadyLaunched && !gameOver && maxSteps > 0 &&
				(window->KeyHold(GLFW_KEY_UP) ||
				window->KeyHold(GLFW_KEY_DOWN) ||
				window->KeyHold(GLFW_KEY_LEFT) ||
				window->KeyHold(GLFW_KEY_RIGHT))) {
			maxSteps -= deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_UP) && !alreadyLaunched && !gameOver && maxSteps > 0) {
			if (abs((worms[currentWorm].position + forward * deltaTime * speed).x) < 25 &&
				abs((worms[currentWorm].position + forward * deltaTime * speed).z) < 25) {
				worms[currentWorm].position += forward * deltaTime * speed; altChanged = true;
				camera->position += forward * deltaTime * speed;
			}
		}
		if (window->KeyHold(GLFW_KEY_DOWN) && !alreadyLaunched && !gameOver && maxSteps > 0) {
			if (abs((worms[currentWorm].position - forward * deltaTime * speed).x) < 25 &&
				abs((worms[currentWorm].position - forward * deltaTime * speed).z) < 25) {
				worms[currentWorm].position -= forward * deltaTime * speed; altChanged = true;
				camera->position -= forward * deltaTime * speed;
			}
		}
		if (window->KeyHold(GLFW_KEY_LEFT) && !alreadyLaunched && !gameOver && maxSteps > 0) {
			if (abs((worms[currentWorm].position - right * deltaTime * speed).x) < 25 &&
				abs((worms[currentWorm].position - right * deltaTime * speed).z) < 25) {
				worms[currentWorm].position -= right * deltaTime * speed; altChanged = true;
				camera->position -= right * deltaTime * speed;
			}
		}
		if (window->KeyHold(GLFW_KEY_RIGHT) && !alreadyLaunched && !gameOver && maxSteps > 0) {
			if (abs((worms[currentWorm].position + right * deltaTime * speed).x) < 25 &&
				abs((worms[currentWorm].position + right * deltaTime * speed).z) < 25) {
				worms[currentWorm].position += right * deltaTime * speed; altChanged = true;
				camera->position += right * deltaTime * speed;
			}
		}

		

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, lightAngleX, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, lightAngleY, glm::vec3(0, 0, 1));
		glm::vec4 temp = glm::vec4(startLightDirection.x, startLightDirection.y, startLightDirection.z, 1);
		glm::vec4 temp1 = modelMatrix * temp;
		lightDirection.x = temp1.x;
		lightDirection.y = temp1.y;
		lightDirection.z = temp1.z;

	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

	float sensivityOX = 0.01f;
	float sensivityOY = 0.01f;

	//rotire camera third person
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !alreadyLaunched){
		renderCameraTarget = true;

		camera->RotateThirdPerson_OY(sensivityOX * -deltaX);
		camera->RotateThirdPerson_OX(sensivityOY * -deltaY);
		worms[currentWorm].angle += glm::vec2(sensivityOY * deltaY, sensivityOX * -deltaX);
		barAngle[currentWorm].x -= sensivityOY * -deltaY;
		barAngle[currentWorm].y += sensivityOX * -deltaX;
	}

}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	//lansare proiectil
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !alreadyLaunched && !gameOver) {
		Projectile p;
		p.center = worms[currentWorm].position +glm::vec3(0.0,0.5,0.0);
		p.direction = camera->forward+glm::vec3(0,0.4,0);
		p.radius = 0.2f;
		projectiles.push_back(p);
		alreadyLaunched = true;
		oldCameraPosition[currentWorm] = camera->position;
		oldCameraUp[currentWorm] = camera->up;
		currentWorm = 1 - currentWorm;

		glm::vec3 newCamPos = glm::vec3(p.center.x, 10, p.center.z);

		camera->Set(newCamPos, p.center, glm::vec3(p.direction.x, 0, p.direction.z));

	}
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
