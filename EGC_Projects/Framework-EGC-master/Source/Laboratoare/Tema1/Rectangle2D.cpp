#include "Rectangle2D.h"
#include "Tema1.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>

Rectangle2D::Rectangle2D()
{
}

//se creaza informatia pentru mesha unui nou dreptunghi
/*
	givenName = numele obiectului
	leftBottomCorner = coordonata coltului din stanga-jos
	width	= latimea dreptunghiului
	height	= inaltimea dreptunghiului
	color = culoarea obiectului
*/
Rectangle2D::Rectangle2D(std::string givenName, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color)
{
	name = givenName;
	corner = leftBottomCorner;
	this->width = width;
	this->height = height;
	vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	indices = { 0, 1, 2, 3 ,0 ,2};
	
}

//se creaza informatia pentru mecanica unui dreptunghi
/*
	newPoint = coordonata punctului prin care se identifica pozitia obstacolului
	newColor = culoarea asociata obstacolului
	newSign = folosit pentru a decida daca gaura e mobila sau nu (0 = imobila, 1 = mobila)
*/
Rectangle2D::Rectangle2D(glm::vec2 newPoint, std::string newColor, int newSign)
{
	point = newPoint;
	sign = newSign;
	color = newColor;
}

Rectangle2D::~Rectangle2D()
{
}

//metoda care verifica coliziunea cercului primit cu cele doua dreptunghiuri care formeaza obstacolul
/*
	circleCenter = pozitia centrului cercului
	circleRadius = raza cercului
	hole = largimea gaurii
	width = latimea dreptunghiului
	height = inaltimea dreptunghiului
*/
bool Rectangle2D::checkCollision(glm::vec2 circleCenter, float circleRadius, float hole, float width, float height) {
	float xmax, xmin, ymax1, ymin1, ymax2, ymin2;
	float x, y1, y2, d1, d2;
	xmax = point.x + width;
	xmin = point.x;
	ymax1 = point.y;
	ymin1 = 0;
	ymax2 = height;
	ymin2 = point.y + hole;
	x = glm::clamp(circleCenter.x, xmin, xmax);
	y1 = glm::clamp(circleCenter.y, ymin1, ymax1);
	y2 = glm::clamp(circleCenter.y, ymin2, ymax2);
	d1 = sqrt(pow((x - circleCenter.x),2) + pow((y1 - circleCenter.y),2));
	d2 = sqrt(pow((x - circleCenter.x), 2) + pow((y2 - circleCenter.y), 2));
	return d1 < circleRadius || d2 < circleRadius;
}