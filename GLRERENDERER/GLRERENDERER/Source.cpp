#pragma warning(disable:4996)

#include"BenGL.h"
#include<vector>
#include<Windows.h>
#include<tchar.h>
#include<cassert>
#include<time.h>
#include"Mesh.h"
#include<ctype.h>

#define SIDE_WIDTH 300
#define FILE_DIR "../meshes/"
void loadFileList(const char* folder);
void printFileList();
void displayFunc();
void irine_Idler();
void mouseFunc(int button, int state, int x, int y);
void reshapeFunc(int width, int height);
void createMenu(int value);
void init();
void lightInit();

string DRAWINGMODE[3] = {"POLYGON","WIREFRAME","POINTS"};
Screen* screen;
FPS_Counter fps_counter;
Mesh* mesh;

#ifndef _USE_CAMERA_DEPRECATED
Camera camera;
#endif
Flag flag;
//IU
Button* drawmodeButton;
Button* propButton;
//files
vector<string> FILES;
int currentFile=-1;
//lights
GLfloat AmbientLightValue[] = { 0.0, 0.0, 0.0, 1.0};
GLfloat DiffuseLightValue[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat PositionLightValue[] = { 0.0, 3.0, 2.0, 0.0 };

//materials
GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialSpecular[] = { 0.0f, 0.0f, 1.0f, 1.0f };

//load file from directory
void loadFileList(const char* folder){
	char search_path[200];
	wchar_t search_path_t[260];
	LPCWSTR ptr;
	sprintf(search_path, "%s*.obj", folder);
	mbstowcs(search_path_t, search_path, strlen(search_path)+1);
	ptr = search_path_t;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(ptr, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				wstring ws(fd.cFileName);
				string filename(ws.begin(), ws.end());
				FILES.push_back(filename);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}	
}
//show file list
void printFileList(){
	cout << "file amount : "<<FILES.size()<<endl;
	for (int i = 0; i < FILES.size(); i++){
		cout << FILES[i] << endl;
	}
}

void displayFunc(){
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	//UI
	glOrtho(0, 300, 0, screen->height, -100.0, 100.0);
	glViewport(0, 0, SIDE_WIDTH, screen->height);
		
	glPushMatrix(); {
		drawmodeButton->drawButton();
		//glTranslated(0, -20, 0);
		propButton->drawButton();
	}glPopMatrix();

	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();{
		glTranslatef(0, screen->height,0);
		glutSolidRectangle(300,screen->height,SQUARE_LEFT_TOP);
	}glPopMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();{
		glTranslatef(10, (screen->height)-30,0.2);
		glutDrawBitmapText("FILENAME : ", 0, 0);
		glTranslatef(0, -30, 0);
		if (currentFile != -1){
			glutDrawBitmapText(FILES[currentFile], 0, 0);
		}
		else{
			glutDrawBitmapText("NULL", 0, 0);
		}
		glTranslatef(0, -60, 0);
		glutDrawBitmapText("DRAWMODE : ", 0, 0);
		glTranslatef(0, -30, 0);
		if (currentFile != -1){
			glutDrawBitmapText(DRAWINGMODE[mesh->getDrawMode()], 0, 0);
		}
		else{
			glutDrawBitmapText("NULL", 0, 0);
		}
			
	}
	glPopMatrix();
	
	
	glPushMatrix();{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutInitDisplayMode(GL_PROJECTION);
	glLoadIdentity();
		
		glOrtho(-2, 2, -2, 2, -100, 100);
		//gluPerspective(30.0, (GLfloat)screen->width / (GLfloat)screen->height, -100.0, 100.0);
		glViewport(SIDE_WIDTH, 0, screen->width, screen->height);
		//gluPerspective(30.0, (GLfloat)screen->width / (GLfloat)screen->height, -100.0, 100.0);
		glScalef(0.1, 0.1, 0.1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix(); {
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//MAIN
			glColor3f(0.5, 0.5, 0.5);
			camera.cameraWork();
			mesh->render();
			//glutSolidSphere(0.5,50,50);
			glutSolidTeapot(0.5);
			
		}glPopMatrix();
		glPushMatrix(); {
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
			camera.cameraWork();
			glutDrawGrid(10);
			glutDrawAxis();
			glTranslatef(0.7, -0.7, 0);
			glutDrawAxisMini();
			
			//glutDrawAxisMini();
		}glPopMatrix();
	}glPopMatrix();
	
	glutSwapBuffers();
}

void createMenu(int value){
	currentFile = value - 1;
	string temp = FILE_DIR + FILES[value - 1];
	mesh->loadFile(temp);
}

void irine_Idler(){
	fps_counter.update();
	glutSetWindowTitle(fps_counter.getTitle().c_str());
	glutPostRedisplay();
}
void mouseFunc(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (x > 300){
			cout << "yeah" << endl;
			flag.isRotate = (flag.isRotate + 1) & 1;
			camera.cameraInit(x, y);
		}
		switch (state)
		{
		case GLUT_UP:
			if (currentFile != -1&&(drawmodeButton->isHover(screen,x,y)==true)){
				auto fp = bind(&Mesh::modeChange, mesh);
				drawmodeButton->action(fp);
			}
			if (currentFile != -1 && (propButton->isHover(screen, x, y) == true)){
				mesh->changeProp();				
			}
			break;
		case GLUT_DOWN:
			if (drawmodeButton->isHover(screen, x, y)){
				drawmodeButton->Press();
			}
			if (propButton->isHover(screen, x, y)){
				propButton->Press();
			}
			break;
		default:
			break;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		
		break;
	case GLUT_RIGHT_BUTTON:
		if (flag.isMove == 0 && flag.isRotate == 0)
			flag.isRight = (flag.isRight + 1) & 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void motionFunc(int x,int y){
	if (flag.isMove) {

	}
	else if (flag.isRotate){
	}
	camera.cameraUpdate(x,y);
	
	glutPostRedisplay();
}

void reshapeFunc(int width, int height) {
	screen->width = height;
	//glutReshapeWindow(width + SIDE_WIDTH, height);
	//screen->width = width;
	//screen->height = height;
	//glOrtho(0, 300, 0, screen->height, -100.0, 100.0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	//gluPerspective(30.0, (GLfloat)screen->width / (GLfloat)screen->height, 1.0, 100.0);
	glutPostRedisplay();
}

void init(){
	
	screen = new Screen();
	drawmodeButton = new Button(0,(screen->height-100),SIDE_WIDTH,60);
	propButton = new Button(0, (screen->height - 40), SIDE_WIDTH, 60);
	mesh = new Mesh();
	loadFileList(FILE_DIR);

	
}
void glSetup(){
	//glFrontFace(GL_CCW);
	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glShadeModel(GL_SMOOTH);
	camera.cameraUpdate(0, 0);

}
void keyFunc(unsigned char key, int x, int y){
	if (isupper(key))
		key = tolower(key);
	switch (key){
	case '+':
		camera.cameraZoomIn();
		break;
	case '-':
		camera.cameraZoomOut();
		break;
	case 'i':
#ifdef _USE_CAMERA_DEPRECATED
		camera.cameraInitiate();
#endif
		camera.cameraReset();
		camera.cameraInit(0, 0);
		camera.cameraUpdate(0, 0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void lightInit(){
	GLfloat modelAmb[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat localView[] = { 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_POSITION, PositionLightValue); //조명의 위치(광원)를 설정한다.
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmb);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, localView);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//glClearColor(0.0, 0.1, 0.1, 0);
}
void passiveFunc(int x, int y){
	drawmodeButton->isHover(screen,x, y);
	propButton->isHover(screen, x, y);
}

void main(int argc,char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE|GLUT_DEPTH);
	init();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutInitWindowPosition(0,0);
	glutInitWindowSize(screen->width, screen->height);
	glutCreateWindow("wassup");
	
	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);
	
	glSetup();
	//fileload
	glutCreateMenu(createMenu);
	for (int i = 0; i < FILES.size(); i++)
		glutAddMenuEntry(FILES[i].c_str(),i+1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	lightInit();

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(irine_Idler);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyFunc);
	glutPassiveMotionFunc(passiveFunc);
	glutMainLoop();
}
