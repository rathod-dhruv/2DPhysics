#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius) : radius(radius) {
	this->radius = radius;
	std::cout << "CircleShape constructor called!" << std::endl;
};
Shape* CircleShape::Clone() const {
	return new CircleShape(radius);
}
CircleShape::~CircleShape() {
	std::cout << "CircleShape destructor called!" << std::endl;
};

ShapeType CircleShape::GetType() const {
	return CIRCLE;
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) : vertices(vertices) {
	// TODO::...
	std::cout << "PolygonShape constructor called!" << std::endl;
};

PolygonShape::~PolygonShape() {
	// TODO::...
};

Shape* PolygonShape::Clone() const {
	return new PolygonShape(vertices);
	//this->vertices and vertices are same
}

ShapeType PolygonShape::GetType() const {
	return POLYGON;
}

BoxShape::BoxShape(float width, float height){
	// TODO::...
};

BoxShape::~BoxShape() {
	// TODO::...
};

ShapeType BoxShape::GetType() const {
	return BOX;
}

Shape* BoxShape::Clone() const {
	return new BoxShape(width, height);
}