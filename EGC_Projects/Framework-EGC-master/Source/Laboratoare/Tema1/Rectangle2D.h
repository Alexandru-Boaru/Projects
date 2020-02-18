#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

//fisier header pentru clasa Rectangle2D

class Rectangle2D
{
public:
	std::string name;	//numele obiectului
	glm::vec3 corner;	//coordonata coltului din care porneste dreptunghiul
	float width = 0;	//latimea dreptunghiului
	float height = 0;	//inaltimea dreptunghiului
	std::string color;	//culoarea dreptunghiului
	std::vector<VertexFormat> vertices;		//vertecsi care definesc dreptunghiul
	std::vector<unsigned short> indices;	//indicii care definesc dreptunghiul

	int sign = 0;		//decide daca gaura este mobila (0 - imobila, 1 - mobila in sus, -1 - mobila in jos)
	glm::vec2 point;			//coordonata folosita in identificarea pozitiei obstacolului
	bool counted = false;		//bool care verifica daca obstacolul a punctat jucatorul
	bool checked = false;		//bool care verifica daca obstacolul a generat un nou obbstacol

	Rectangle2D();
	Rectangle2D(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color);
	Rectangle2D::Rectangle2D(glm::vec2 newPoint, std::string color, int newSign = 0);
	~Rectangle2D();

	//metoda pentru verificare de coliziune
	bool checkCollision(glm::vec2 circleCenter, float circleRadius, float hole, float width, float height);

};