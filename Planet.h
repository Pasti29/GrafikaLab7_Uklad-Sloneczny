#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include "Textures.h"

class Planet {
public:
	// Zmienne dla obrazu tekstury
	struct TEXTURE {
		std::string path;
		GLbyte* pBytes;
		GLint ImWidth, ImHeight, ImComponents;
		GLenum ImFormat;
	};

private:
	double angle;
	int distanceFromCenter;
	double radius = 0;
	double x = 0;
	const double y = 0;
	double z = 0;
	int daysInYear;
	TEXTURE texture;

public:
	Planet(double radius, std::string path);
	Planet(double radius, std::string path, int distanceFromCenter, int daysInYear);

private:
	void loadTexture();

public:
	void showTexture();
	void setAngle(int currentDay);
	void setX();
	void setZ();
	double getRadius();
	double getX();
	double getZ();
};

