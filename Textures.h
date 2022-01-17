#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdio>

class Textures {
public:
	static GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat);
};
