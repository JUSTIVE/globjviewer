#pragma once
#define _USE_MATH_DEFINES
#include<gl/glut.h>
#include<gl/GL.h>
#include<string>
#include<iostream>
#include<functional>
#include<math.h>
#include<time.h>

enum SQUARE_OFFSET{ SQUARE_CENTER, SQUARE_LEFT_TOP };
#define RADIAN(x) x*M_PI/180.0
#define GLUT_PAINT_RED glColor3f(1,0,0);
#define GLUT_PAINT_GREEN glColor3f(0,1,0);
#define GLUT_PAINT_BLUE glColor3f(0,0,1);
using namespace std;
class Point{
public:
	Point();
	long double x, y;
};
class Screen {
public:
	Screen();
	Screen(int width,int height);
	int width;
	int height;
};
class FPS_Counter{
private:
	int timeindex = 0;
	time_t timeFlip[2];
	string title;
public:
	FPS_Counter();
	string getTitle();
	void update();
};
class Flag{
private:
	static Flag* var;
public:
	Flag();
	Flag*getFlag();
	int isOrtho;
	int isMove;
	int isRotate;
	int isRight;
};
#ifdef _USE_CAMERA_DEPRECATED
class Camera {
	GLint mouse[2];
	GLint prevMouse[2];
	GLdouble zoom;
	GLdouble rotation[2];
	GLdouble move[2];
public:
	enum CAMERA_MODE{ MOVE, ROTATE, ZOOM, MOVE_AND_ZOOM, ROTATE_AND_ZOOM, MOVE_AND_ROTATE, CAMERA_ALL };
	CameraDeprecated();
	void cameraWork(int mode);
	void cameraMoveInit(Screen* screen,int x, int y);
	void cameraRotateInit(int x, int y);
	void cameraMoveAction(Screen* screen,int x, int y);
	void cameraRotateAction(int x, int y);
	void cameraZoomIn();
	void cameraZoomOut();
	void cameraInitiate();
};
#endif
class Camera{
private:
	double zoom;
	double origEyePos[3],origUpVec[3];
	double swing, elevation,dist;
	double eyePos[3], centerPos[3], upVec[3];
	Point prevPoint;
public:
	enum CAMERA_MODE{ MOVE, ROTATE, ZOOM, MOVE_AND_ZOOM, ROTATE_AND_ZOOM, MOVE_AND_ROTATE, CAMERA_ALL };
	Camera();
	void cameraReset();
	void cameraInit(int x, int y);
	void cameraWork();
	void cameraUpdate(int x, int y);
	void cameraZoomIn();
	void cameraZoomOut();
	
};
class Button{
private:
	int positionX;
	int positionY;
	string Text;
	double width;
	double height;
	int isPressed;
	double color[3];
public:
	Button();
	Button(int x, int y, double width, double height);
	Button(int x, int y, double width, double height,string Text);
	bool isHover(Screen* Screen,int x, int y);
	void Press();
	void drawButton();
	void action(function<void(void)>function);
};
//text
void glutDrawText(string input, double size);
void glutDrawBitmapText(string input,int x, int y);

//arc
void glutDrawArc(double radius, double angle, int slice);

//pie
void glutDrawPie(double radius, double angle, int slice);

//circle
void glutSolidCircle(double radius, int slice);
void glutWireCircle(double radius, int slice);

//cylinder
void glutSolidCylinder(double topRadius, double height, double bottomRadius, int slice);

//rectangle
void glutWireRectangle(double width,double height);
void glutWireRectangle(double width, double height, int offset);
void glutSolidRectangle(double width, double height);
void glutSolidRectangle(double width, double height, int offset);

//square
void glutWireSquare(double side);
void glutSolidSquare(double side);

//roundRectangle
void glutWireRoundRectangle(double width, double height,double rad);
void glutSolidRoundRectangle(double width, double height, double rad);

//roundSquare
void glutWireRoundSquare(double side, double rad);
void glutSolidRoundSquare(double side, double rad);

//round Rectangle/SquareColumn
void glutSolidRoundRectangleColumn(double topWidth, double topHeight, double bottomWidth, double bottomHeight, double elevation, double rad);
void glutSolidRoundSquareColumn(double topSide, double bottomSide, double elevation, double rad);

//axis
void glutDrawAxisMini();
void glutDrawGrid(double size);
void glutDrawAxis();