#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<gl/glut.h>
#include<gl\GLU.h>
#include<gl/GL.h>
#include<cassert>

typedef GLint V3I[3];
typedef GLint V4I[4];
typedef GLdouble V3D[3];
enum POLYGON{ TRIANGLE = 3, QUADS = 4 };
using namespace std;
class Mesh{
private:
	bool fileLoading;
	V3D* vertex;
	V4I* polygon_quad;
	V3I* polygon_tri;
	int prop;
	int vertexSize;
	int normalSize;
	int polygonSize;
	int polygonShape;//poly, quads?

	string fileName;
	bool randColor;
	int drawMode;
public:
	Mesh();
	Mesh(const char * filename);
	void Init();
	void dataPrinter();
	void loadFile(string filename);
	void render();
	void modeChange();
	void materialColor();
	void setRandColor(bool value);
	string objPolisher(string filename);
	void changeProp();
	void setProp(int number);
	int getDrawMode();
};