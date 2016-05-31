#include"BenGL.h"
///////////////////////////////////////////////////////////////////////////////////
//Point
Point::Point(){ this->x = this->y = 0; }

///////////////////////////////////////////////////////////////////////////////////
//Screen
Screen::Screen(){
	this->height = 1080;
	this->width = 1380;
}
Screen::Screen(int width, int height){
	this->width= width;
	this->height= height;
}
///////////////////////////////////////////////////////////////////////////////////
//FPS_Counter
FPS_Counter::FPS_Counter(){
	timeFlip[timeindex] = clock();
}
void FPS_Counter::update(){
	timeFlip[timeindex] = clock();
	title = to_string(60 - ((timeFlip[timeindex] - timeFlip[(timeindex + 1) & 1]) * 60) / CLOCKS_PER_SEC).c_str();
	timeindex = (timeindex + 1) & 1;
}
string FPS_Counter::getTitle(){ 
	return "frame: "+title; 
}
///////////////////////////////////////////////////////////////////////////////////
//Flag
Flag::Flag(){
	isOrtho = 0;
	isMove = 0;
	isRotate = 0;
	isRight = 0;
}

///////////////////////////////////////////////////////////////////////////////////
//Camera_Deprecated

#ifdef _USE_CAMERA_DEPRECATED
CameraDeprecated::CameraDeprecated(){
	this->mouse[0] = 0;
	this->mouse[1] = 0;
	this->zoom = 1.0;
	this->move[0] = 0;
	this->move[1] = 0;
}
void CameraDeprecated::cameraWork(int mode){
	if (mode==ZOOM||mode==MOVE_AND_ZOOM||mode==ROTATE_AND_ZOOM||mode==CAMERA_ALL)
		glScalef(this->zoom, this->zoom, this->zoom);
	if (mode == MOVE || mode == MOVE_AND_ZOOM || mode == MOVE_AND_ROTATE || mode == CAMERA_ALL)
		glTranslatef(this->move[0], this->move[1], 0);
	if (mode == ROTATE || mode == MOVE_AND_ROTATE || mode == ROTATE_AND_ZOOM || mode == CAMERA_ALL){
		glRotatef(this->rotation[0], 0, 1, 0);
		glRotatef(this->rotation[1], 1, 0, 0);
	}
}
void CameraDeprecated::cameraMoveInit(Screen* screen, int x, int y){
	this->move[0] = (x / (screen->width / 2.0)) - 1;
	this->move[1] = ((screen->height - y) / (screen->height / 2.0)) - 1;
}

void CameraDeprecated::cameraRotateInit(int x, int y){
	this->mouse[0] = x;
	this->mouse[1] = y;
	this->prevMouse[0] = this->mouse[0];
	this->prevMouse[1] = this->mouse[1];
}

void CameraDeprecated::cameraMoveAction(Screen* screen,int x, int y){
	this->move[0] = (x / (screen->width / 2.0)) - 1;
	this->move[1] = ((screen->height - y) / (screen->height / 2.0)) - 1;
}

void CameraDeprecated::cameraRotateAction(int x, int y){
	this->mouse[0] = x;
	this->mouse[1] = y;
	this->rotation[0] -= (GLdouble)(this->mouse[0] - this->prevMouse[0]);
	this->rotation[1] += (GLdouble)(this->mouse[1] - this->prevMouse[1]);
	this->prevMouse[0] = this->mouse[0];
	this->prevMouse[1] = this->mouse[1];
}

void CameraDeprecated::cameraZoomIn(){
	this->zoom*=1.2;
}
void CameraDeprecated::cameraZoomOut(){
	this->zoom/=1.2;
}

void CameraDeprecated::cameraInitiate(){
	this->mouse[0] = 0;
	this->mouse[1] = 0;
	this->zoom = 1.0;
	this->rotation[0] = 0;
	this->rotation[1] = 0;
	this->move[0] = 0;
	this->move[1] = 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////////
//Camera

Camera::Camera(){
	this->cameraReset();
}
void Camera::cameraReset(){
	this->zoom = 1;
	this->dist = 0.2;
	for (int i = 0; i < 3; i++){
		this->centerPos[i] = 0;
		this->upVec[i] = 0;
		this->origEyePos[i] = 0;
		this->eyePos[i] = 0;
		this->origUpVec[i] = 0;
	}
	this->origEyePos[2] = dist;
	this->origUpVec[1] = 1;

	this->prevPoint.x = 0;
	this->prevPoint.x = 0;
}
void Camera::cameraInit(int x, int y){
	this->prevPoint.x = x;
	this->prevPoint.x = y;
}
void Camera::cameraUpdate(int x, int y){
	this->swing += (double)( prevPoint.x-x);
	this->elevation += (double)(prevPoint.y-y);
	double temp[3] = { 0 };
	double rot_elev[3][3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };//elev 만큼 x축으로 회전한 행렬
	double rot_swing[3][3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };//swing 만큼 -y축으로 회전한 행렬
	rot_elev[1][1] = cos(RADIAN(elevation));    rot_elev[1][2] = -sin(RADIAN(elevation));
	rot_elev[2][1] = sin(RADIAN(elevation));    rot_elev[2][2] = cos(RADIAN(elevation));

	rot_swing[0][0] = cos(RADIAN(swing));    rot_swing[0][2] = sin(RADIAN(swing));
	rot_swing[2][0] = -sin(RADIAN(swing));   rot_swing[2][2] = cos(RADIAN(swing));
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
			temp[i] += rot_elev[i][j] * origEyePos[j];
	}
	for (int i = 0; i < 3; i++){
		eyePos[i] = 0;
		for (int j = 0; j < 3; j++)
			eyePos[i] += rot_swing[i][j] * temp[j];
	}
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
			temp[i] += rot_elev[i][j] * origUpVec[j];
	}
	for (int i = 0; i < 3; i++){
		upVec[i] = 0;
		for (int j = 0; j < 3; j++)
			upVec[i] += rot_swing[i][j] * temp[j];
	}
	this->prevPoint.x = x;
	this->prevPoint.y = y;
}

void Camera::cameraWork(){
	glScalef(zoom, zoom, zoom);
	gluLookAt(eyePos[0], eyePos[1], eyePos[2],
		centerPos[0], centerPos[1], centerPos[2],
		upVec[0],upVec[1],upVec[2]);
}
void Camera::cameraZoomIn(){
	this->zoom *= 1.2;
	
}
void Camera::cameraZoomOut(){
	this->zoom /= 1.2;
}
///////////////////////////////////////////////////////////////////////////////////
//Button

Button::Button(){
	positionX=0;
	positionY=0;
	Text="";
	width=1;
	height=1;
	isPressed=0;
	for (int i = 0; i < 3;i++)
		color[i] = 0.9;
	
}
Button::Button(int x, int y, double width, double height){
	this->positionX = x;
	this->positionY = y;
	this->Text = "";
	this->width = width;
	this->height = height;
	this->isPressed = 0;
	for (int i = 0; i < 3; i++)
		color[i] = 0.9;
}
Button::Button(int x, int y, double width, double height, string Text){
	positionX = x;
	positionY = y;
	this->Text = "";
	this->width = width;
	this->height = height;
	isPressed = 0;
	for (int i = 0; i < 3; i++)
		color[i] = 0.9;
}
void Button::Press(){
	this->isPressed = 2;
}
bool Button::isHover(Screen* screen,int x, int y){
	if ((x > positionX&&x<positionX + width) && (((screen->height - y)<positionY) && ((screen->height - y)>positionY - height))){
		isPressed = 1;
		return true;
	}
	isPressed = 0;
	return false;	
}
void Button::drawButton(){
	
	if (isPressed == 2){
		for (int i = 0; i < 3; i++){
			color[i] = color[i] - 0.015 < 0.8 ? 0.8 : color[i] - 0.015;
		}
	}
	else if (isPressed == 1){
		for (int i = 0; i < 3; i++){
			color[i] = color[i] + 0.015 > 1 ? 1 : color[i] + 0.015;
		}
	}
	else{
		for (int i = 0; i < 3; i++){
			color[i] = color[i] + 0.015 >= 0.915 ? color[i] - 0.015 : color[i] + 0.015;
		}		
	}
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix(); {
		glTranslatef(positionX, positionY, 0);
		glutSolidRectangle(width, height, SQUARE_LEFT_TOP);
		glColor3f(0, 0, 0);
		glutDrawText(Text, 1);
	}
	glPopMatrix();
}
void Button::action(function<void(void)>function){
	function();
}

///////////////////////////////////////////////////////////////////////////////////
//text
void glutDrawText(string text, double size){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);
	glPushMatrix(); {
		glScalef(size / 300.0, size / 300.0, size / 300.0);
		for (int i = 0; i < text.length(); i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		}
	}
	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
	glBlendFunc(NULL, NULL);
}

void glutDrawBitmapText(string input, int x, int y){
	glRasterPos2d(x, y);
	for (int i = 0; i<input.length();i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,input.c_str()[i]);

}
//arc
void glutDrawArc(double radius, double angle, int slice){
	if (angle>360){
		glutWireCircle(radius,slice);
	}
	else{
		slice = (slice > 50) ? 50 : slice;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= slice; i++)
			glVertex3f(0 * cos((double)((angle / slice)*i / (180 / M_PI))) - radius*sin((double)((angle / slice)*i / (180 / M_PI))),
			0 * sin((double)((angle / slice)*i / (180 / M_PI))) + radius*cos((double)((angle/ slice)*i / (180 / M_PI))),
			0);
		glEnd();
	}
	
}

//pie
void glutDrawPie(double radius, double angle, int slice){
	if (angle>360){
		glutSolidCircle(radius, slice);
	}
	else{
		slice = (slice > 50) ? 50 : slice;
		glBegin(GL_POLYGON); {
			glVertex3f(0, 0, 0);
			for (int i = 0; i <= slice; i++)
				glVertex3f(0 * cos((double)((angle / slice)*i / (180 / M_PI))) - radius*sin((double)((angle / slice)*i / (180 / M_PI))),
				0 * sin((double)((angle / slice)*i / (180 / M_PI))) + radius*cos((double)((angle / slice)*i / (180 / M_PI))),
				0);
		}glEnd();
	}
}

//circle
void glutSolidCircle(double radius, int slice){
	int i;
	slice = (slice>50) ? 50 : slice;
	glBegin(GL_POLYGON);
		for (i = 0; i <= slice * 2; i++)
			glVertex3f(0 * cos((double)((360 / slice)*i / (180 / M_PI))) - radius*sin((double)((360 / slice)*i / (180 / M_PI))),
				0 * sin((double)((360 / slice)*i / (180 / M_PI))) + radius*cos((double)((360 / slice)*i / (180 / M_PI))),
				0);
	glEnd();
}
void glutWireCircle(double radius, int slice){
	int i;
	slice = (slice>50) ? 50 : slice;
	glBegin(GL_LINE_LOOP); {
		for (i = 0; i <= slice; i++)
			glVertex3f(0 * cos((double)((360 / slice)*i / (180 / M_PI))) - radius*sin((double)((360 / slice)*i / (180 / M_PI))),
				0 * sin((double)((360 / slice)*i / (180 / M_PI))) + radius*cos((double)((360 / slice)*i / (180 / M_PI))),
				0);
	}glEnd();
}
//cylinder
void glutSolidCylinder(double topRadius, double height, double bottomRadius, int slice){
	int i, j, step = 0;
	glPushMatrix(); {
		glutSolidCircle(topRadius, slice);
		glPointSize(2);
		glBegin(GL_QUAD_STRIP); {
			for (i = 0; i <= ((slice)* 2); i++){
				for (j = 0; j < 2; j++){
					step = (step + 1) & 1;
					if (step)
						glVertex3f(0 * cos((double)((360 / slice)*i / (180 / M_PI))) - topRadius*sin((double)((360 / slice)*i / (180 / M_PI))),
							0 * sin((double)((360 / slice)*i / (180 / M_PI))) + topRadius*cos((double)((360 / slice)*i / (180 / M_PI))),
							0);
					else
						glVertex3f(0 * cos((double)((360 / slice)*i / (180 / M_PI))) - bottomRadius*sin((double)((360 / slice)*i / (180 / M_PI))),
							0 * sin((double)((360 / slice)*i / (180 / M_PI))) + bottomRadius*cos((double)((360 / slice)*i / (180 / M_PI))),
							height);
					i--;
				}
				i += 2;
			}
		}glEnd();
		glTranslatef(0, 0, height);
		glutSolidCircle(bottomRadius, slice);
	}
	glPopMatrix();
}


//rectangle
void glutWireRectangle(double width, double height){
	glBegin(GL_LINE_LOOP); {
		glVertex3f(-(width/2),-(height/2),0);
		glVertex3f((width / 2), -(height / 2), 0);
		glVertex3f((width / 2), (height / 2), 0);
		glVertex3f(-(width / 2), (height / 2), 0);
	}
	glEnd();
}
void glutWireRectangle(double width, double height,int offset){
	if (offset == SQUARE_CENTER){
		glBegin(GL_LINE_LOOP); {
			glVertex3f(-(width / 2), -(height / 2), 0);
			glVertex3f((width / 2), -(height / 2), 0);
			glVertex3f((width / 2), (height / 2), 0);
			glVertex3f(-(width / 2), (height / 2), 0);
		}
		glEnd();
	}
	else if (offset == SQUARE_LEFT_TOP){
		glBegin(GL_QUADS); {
			glVertex3f(0, -height, 0);
			glVertex3f(width, -height, 0);
			glVertex3f(width, 0, 0);
			glVertex3f(0, 0, 0);
		}
		glEnd();
	}
}
void glutSolidRectangle(double width, double height){
	glBegin(GL_QUADS); {
		glVertex3f(-(width / 2), -(height / 2), 0);
		glVertex3f((width / 2), -(height / 2), 0);
		glVertex3f((width / 2), (height / 2), 0);
		glVertex3f(-(width / 2), (height / 2), 0);
	}
	glEnd();
}
void glutSolidRectangle(double width, double height,int offset){
	if (offset == SQUARE_CENTER){
		glBegin(GL_QUADS); {
			glVertex3f(-(width / 2), -(height / 2), 0);
			glVertex3f((width / 2), -(height / 2), 0);
			glVertex3f((width / 2), (height / 2), 0);
			glVertex3f(-(width / 2), (height / 2), 0);
		}
		glEnd();
	}
	else if(offset==SQUARE_LEFT_TOP){
		glBegin(GL_QUADS); {
			glVertex3f(0, -height, 0);
			glVertex3f(width, -height, 0);
			glVertex3f(width, 0, 0);
			glVertex3f(0, 0, 0);
		}
		glEnd();
	}
}
//square
void glutWireSquare(double side){
	glBegin(GL_LINE_LOOP); {
		glVertex3f(-(side / 2), -(side/ 2), 0);
		glVertex3f((side/ 2), -(side / 2), 0);
		glVertex3f((side / 2), (side / 2), 0);
		glVertex3f(-(side / 2), (side/ 2), 0);
	}
	glEnd();
}
void glutSolidSquare(double side){
	glBegin(GL_QUADS); {
		glVertex3f(-(side / 2), -(side / 2), 0);
		glVertex3f((side / 2), -(side / 2), 0);
		glVertex3f((side / 2), (side / 2), 0);
		glVertex3f(-(side / 2), (side / 2), 0);
	}
	glEnd();
}

//roundSquare
void glutWireRoundRectangle(double width, double height, double rad){
	glBegin(GL_LINES); {
		glVertex3f(-((width / 2) - rad), -(height / 2), 0);
		glVertex3f((width / 2) - rad, -(height / 2), 0);

		glVertex3f((width / 2), -((height / 2)-rad), 0);
		glVertex3f((width / 2), ((height / 2) - rad), 0);

		glVertex3f((width / 2) - rad, (height / 2), 0);
		glVertex3f(-((width / 2) - rad), (height / 2), 0);

		glVertex3f(-(width / 2), ((height / 2) - rad), 0);
		glVertex3f(-(width / 2), -((height / 2) - rad), 0);
	}
	glEnd();
	glPushMatrix(); {
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width/4,height/4,1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		
		
	}glPopMatrix();

}
void glutSolidRoundRectangle(double width, double height, double rad){
	rad = (rad > (width / 2)) ? width/2: rad;
	rad = (rad > (height/ 2)) ? width / 2 : rad;
	glPushMatrix(); {
		glTranslatef(-((width / 2) - rad), -((height / 2) - rad), 0);
		glutSolidCircle(rad,50);
		glTranslatef(width-(2*rad),0,0);
		glutSolidCircle(rad, 50);
		glTranslatef(0, height-(2*rad), 0);
		glutSolidCircle(rad, 50);
		glTranslatef(-(width - (2 * rad)), 0, 0);
		glutSolidCircle(rad, 50);
	}glPopMatrix();
	glutSolidRectangle(width,height-(2*rad));
	glutSolidRectangle(width-(2*rad), height);
}

//roundSquare
void glutWireRoundSquare(double side, double rad){
	glutWireRoundRectangle(side, side, rad);
}
void glutSolidRoundSquare(double side, double rad){
	glutSolidRoundRectangle(side, side, rad);
}


//roundSquareColumn
void glutSolidRoundRectangleColumn(double topWidth, double topHeight, double bottomWidth, double bottomHeight, double elevation, double rad){
	glPushMatrix(); {
		glutSolidRoundRectangle(topWidth,topHeight,rad);
		
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topWidth / 2) - rad, 0);
				//glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad,elevation,rad,50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth/ 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();


		}glPopMatrix();
		glTranslatef(0,0,elevation);
		glutSolidRoundRectangle(bottomWidth, bottomHeight, rad);
	}glPopMatrix();

}

void glutSolidRoundSquareColumn(double topSide, double bottomSide, double elevation, double rad){

	glPushMatrix(); {
#ifdef DEBUG
		GLUT_PAINT_RED
#endif
		glutSolidRoundSquare(topSide, rad);
#ifdef DEBUG
		GLUT_PAINT_BLUE
#endif
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((topSide / 2) - rad), (topSide/ 2) - rad, 0);
				//glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide / 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide/ 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide/ 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
		}glPopMatrix();
		glTranslatef(0, 0, elevation);
#ifdef DEBUG
		GLUT_PAINT_RED
#endif
		glutSolidRoundSquare(bottomSide,rad);
	}glPopMatrix();
}

//axis
void glutDrawAxisMini(){
	double fontsize = 0.1;
	glLineWidth(2);
	GLUT_PAINT_RED
	glBegin(GL_LINES); {
		glVertex3f(0, 0, 0);
		glVertex3f(0.15, 0, 0);
	}
	glEnd();
	glPushMatrix(); {
		glTranslatef(0.2,0,0);
		glutDrawText("X",fontsize);
	}glPopMatrix();
	/////////////
	GLUT_PAINT_BLUE
	glBegin(GL_LINES); {
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0.15, 0);
	}
	glEnd();
	glPushMatrix(); {
		glTranslatef(0, 0.2, 0);
		glutDrawText("Y", fontsize);
	}glPopMatrix();
	/////////////
	GLUT_PAINT_GREEN
	glBegin(GL_LINES); {
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0.15);
	}
	glEnd();
	glPushMatrix(); {
		glTranslatef(0, 0, 0.2);
		glutDrawText("Z", fontsize);
	}glPopMatrix();
}

void glutDrawGrid(double size){
	glPushMatrix(); {
		glLineWidth(0.25);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glColor4f(0.5, 0.5, 0.5, 0.9);
		glScalef(size, size, size);
		for (int k = 0; k < 3; k++){
			if (k==1)
				glRotatef(90, 1, 0, 0);
			else if (k==2)
				glRotatef(90, 0, 1, 0);
			glPushMatrix(); {
				for (int j = 0; j < 2; j++){
					glBegin(GL_LINES); {
						for (int i = 0; i < 30; i++){
							if (-1 + (2 / 30.0)*i != 0){
								glVertex3f(-1+(2/30.0)*i,1,0);
								glVertex3f(-1 + (2 / 30.0)*i, -1, 0);
							}
						}
					}glEnd();
					glRotatef(90, 0, 0, 1);
				}
			
			}glPopMatrix();
		}
		glDisable(GL_LINE_SMOOTH);
		glBlendFunc(NULL, NULL);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}
void glutDrawAxis(){
	glLineWidth(5);
	for (int i = 0; i < 3; i++)
	{
		glColor3f(1, 0, 0);
		if (i == 1){
			glColor3f(0, 1, 0);
			glRotatef(90, 0, 0, 1);
		}
		else if (i == 2){
			glColor3f(0, 0, 1);
			glRotatef(90, 0, 1, 0);
		}
		glPushMatrix(); {
			glBegin(GL_LINES); {
				glVertex3f(-2, 0, 0);
				glVertex3f(2, 0, 0);
			}
			glEnd();
		}
		glPopMatrix();
	}
}