#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

class Planet {
private:
	double angle;
	int distanceFromCenter;
	double radius = 0;
	double x = 0;
	const double y = 0;
	double z = 0;
	int daysInYear;
	// byte* texture;
	// TODO Tekstury

public:
	Planet(double radius);
	Planet(double radius, int distanceFromCenter, int daysInYear);

	void setAngle(int currentDay);
	void setX();
	void setZ();
	double getRadius();
	double getX();
	double getZ();
};

