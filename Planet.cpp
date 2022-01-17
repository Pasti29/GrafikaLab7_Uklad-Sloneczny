#include "Planet.h"

Planet::Planet(double radius) {
	this->radius = radius;
}

Planet::Planet(double radius, int distanceFromCenter, int daysInYear) {
	this->radius = radius;
	this->distanceFromCenter = distanceFromCenter;
	this->daysInYear = daysInYear;

	setAngle(0);
	setX();
	setZ();
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
