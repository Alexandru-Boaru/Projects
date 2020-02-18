#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}

void Laborator2::Init()
{
	cullFace = GL_BACK;
	polygonMode = GL_FILL;

	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a mesh box using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-1, -1,  -1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-1, 1,  1), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(-1, 1,  -1), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1, -1,  -1), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(1, 1,  -1), glm::vec3(0, 1, 0)),
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices =
		{
			0, 2, 1,	// indices for first triangle
			1, 2, 3,	// indices for second triangle
			0, 1, 4,
			4, 1, 5,
			4, 5, 6,
			5, 7, 6,
			0, 4, 2,
			4, 6, 2,
			2, 6, 3,
			6, 7, 3,
			1, 7, 5,
			1, 3, 7,
			// TODO: Complete indices data
		};

		meshes["cube1"] = new Mesh("generated cube 1");
		//meshes["cube1"]->InitFromData(vertices, indices);
		//CreateMesh("cube1", vertices,indices);
		// Create a new mesh from buffer data
		Mesh *cube = CreateMesh("cube3", vertices, indices);

		vector<VertexFormat> vertices1
		{
			VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(0, 1,  2), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(1, 0,  2), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(1, 1,  2), glm::vec3(0, 1, 1)),
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices1 =
		{
			0, 1, 2,
			2, 3, 1,
		};

		meshes["square"] = new Mesh("generated square");
		Mesh* square = CreateMesh("square", vertices1, indices1);

		vector<VertexFormat> vertices2
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(0, 1,  0), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(0, 0,  1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(0, 1,  0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0, 0,  1), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0, 1,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0, 0,  1), glm::vec3(1, 0, 1)),
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices2 =
		{
			1, 2, 3,
			7, 6, 4,
			5, 11, 8,
			9, 0, 10,
		};

		meshes["pyramid"] = new Mesh("generated pyramid");
		Mesh* pyramid = CreateMesh("pyramid", vertices2, indices2);

	}
}

Mesh* Laborator2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// TODO: Create the VAO and bind it
	CheckOpenGLError();

	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Send vertices data into the VBO buffer
	CheckOpenGLError();

	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// TODO: Send indices data into the IBO buffer

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader
	CheckOpenGLError();

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	CheckOpenGLError();
	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// TODO: Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(cullFace);
	// TODO: Set face custom culling. Use the "cullFace" variable

	// render an object using face normals for color
	RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

	// render an object using colors from vertex
	RenderMesh(meshes["cube3"], shaders["VertexColor"], glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0.25f));

	RenderMesh(meshes["square"], shaders["VertexColor"], glm::vec3(2, 2, 0), glm::vec3(0.25f));

	RenderMesh(meshes["pyramid"], shaders["VertexColor"], glm::vec3(0, 1, -2), glm::vec3(0.5f));

	// TODO: Disable face culling
	glEnable(GL_CULL_FACE);
}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	// TODO: switch between GL_FRONT and GL_BACK culling
	// Save the state in "cullFace" variable and apply it in the Update() method not here

	if (key == GLFW_KEY_1) {
		cullFace = cullFace == GL_BACK ? GL_FRONT : GL_BACK;
	}

	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator2::OnWindowResize(int width, int height)
{
}
