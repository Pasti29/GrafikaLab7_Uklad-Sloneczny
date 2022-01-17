#include "Planet.h"

Planet::Planet(double radius, std::string path) {
	this->radius = radius;
	texture.path = path;
}

Planet::Planet(double radius, std::string path, int distanceFromCenter, int daysInYear) {
	this->radius = radius;
	texture.path = path;
	this->distanceFromCenter = distanceFromCenter;
	this->daysInYear = daysInYear;

	loadTexture();
	setAngle(0);
	setX();
	setZ();
}

void Planet::loadTexture() {
	texture.pBytes = Textures::LoadTGAImage(texture.path.c_str(), &texture.ImWidth, &texture.ImHeight, &texture.ImComponents, &texture.ImFormat);
}

void Planet::showTexture() {
	glTexImage2D(GL_TEXTURE_2D, 0, texture.ImComponents, texture.ImWidth, texture.ImHeight, 0, texture.ImFormat, GL_UNSIGNED_BYTE, texture.pBytes);
}

void Planet::setAngle(int day) {
	angle = (double) day / (double )daysInYear;
}

void Planet::setX() {
	x = cos(2 * angle * M_PI) * distanceFromCenter;
}

void Planet::setZ() {
	z = -1 * sin(2 * angle * M_PI) * distanceFromCenter;
}

double Planet::getRadius() {
	return radius;
}

double Planet::getX() {
	return x;
}

double Planet::getZ() {
	return z;
}
