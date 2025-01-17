#include <string>
#include <iostream>
#include <thread>
using namespace std;

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <Math.h>     // Needed for sin, cos
#define PI 3.14159265f


int windowPosX = 50;      // Windowed mode's top-left corner x
int windowPosY = 50;      // Windowed mode's top-left corner y

/// window stuff
int window; // main window id
const int BALL_SIZE = 7;

//const int ANIMATION_TIME = 20;

// physics variables
float g = .00002;
float f = .00001;
float vx = 0;
float vy = 0;

// mouse variables
int dragging = 0;
int mousePressed = 0;
int mouseX = 0;
int mouseY = 0;

// Misc.
int ballCounter = 5;
float acc = 0;
float launch = 0;
int score = 0;

// colour struct for rgba colours
struct Color {
	float r;
	float g;
	float b;
	float a;
};

// point struct
struct Point {
	float x;
	float y;
	float z;
};

// Vector struct
struct Vector {
	float angle;
	float speed;
};

// Points
Point origin;

Point upperWall;
Point bottomWall;
Point leftWall1;
Point leftWall2;
Point rightWall1;
Point rightWall2;

Point fieldMidLine;

Point bumper1;
Point bumper2;
Point bumper3;
Point bumper4;
Point bumper5;
Point bumper6;
Point bumper7;
Point bumper8;
Point bumper9;
Point bumper10;
Point bumper11;
Point bumper12;
Point bumper13;
Point bumper14;
Point bumper15;
Point bumper16;
Point bumper17;
Point bumper18;
Point bumper19;
Point bumper20;
Point bumper21;
Point bumper22;
Point bumper23;
Point bumper24;

Point leftLFlipperState;
Point leftRFlipperState;
Point rightLFlipperState;
Point rightRFlipperState;

// Ball Points
Point currentPosition;
Point initialPosition;
Point lastPosition;

// flipper variables
float angle = 0.0;
const float ANGLE_CHANGE = 45;

// draw the Pinball
void drawCircle(Point& p, float radius, Color& c) {
	float delta_theta = 0.01;
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 2 * PI; angle += delta_theta)
		glVertex3f(radius * cos(angle) + p.x, radius * sin(angle) + p.y, 0);
	glEnd();
		
	lastPosition = p;
	if ((p.y < 1 || p.x > 0 || p.y > 440) && ballCounter > 0)
	{
		ballCounter--;
		p.x = 150;
		p.y = 250;		
		vx = -vx;
		vy = -vy;
	}
	if (p.x > 0)
	{
		if (p.y >= 440)
		{
			vx = vy;
			p.x -= vx;
		}
		else
		{
			vy = 0;
			vy += (launch / 2000);
			p.y += vy;
		}
	}
	else
	{
		acc = 0;
		p.x -= vx;
		if (vy > 0) {
			vy += g;
			vy -= f;
		}
		else if (vy < 0) {
			vy -= g;
			vy += f;
		}
		else if (vx > 0) {
			vx -= f;
		}
		else if (vx < 0) {
			vx += f;
		}
		p.y -= vy;
	}
}

// draws a bumper obstacle
void drawBumper(Point& p, float radius, Color& c) {
	// Collision code
	float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));
	if (d <= 22)
	{
		// do this when there is a collision

		vx = -vx;
		vy = -vy;
	}

	// draw circle
	float delta_theta = 0.05;
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 2 * PI; angle += delta_theta)
		glVertex3f(radius * cos(angle) + p.x, radius * sin(angle) + p.y, 0);
	glEnd();
}

// draws a rectangle
void drawRectangle(Point& p, float height, float width, Color& c) {
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POLYGON);
	glVertex3f(p.x, p.y, 0);
	glVertex3f(p.x + width, p.y, 0);
	glVertex3f(p.x + width, p.y + height, 0);
	glVertex3f(p.x, p.y + height, 0);
	glVertex3f(p.x, p.y, 0);
	glEnd();
}

// Outer side walls
void drawWallS(Point& p, float height, float width, Color& c) {
	// handle collision
	if (p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7)
	{
		// do this when there is collision with side of wall
		vx = -vx;
	}
	else if (p.y + 440 < currentPosition.y - 7 || p.y > currentPosition.y + 7)
	{
		// do this when there is collision with top of wall
		vy = -vy;
	}

	// draw rectangle
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POLYGON);
	glVertex3f(p.x, p.y, 0);
	glVertex3f(p.x + width, p.y, 0);
	glVertex3f(p.x + width, p.y + height, 0);
	glVertex3f(p.x, p.y + height, 0);
	glVertex3f(p.x, p.y, 0);
	glEnd();
}

// display function
void display(void)
{
	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT);
	float ld;
	float rd;

	// POINT LOCATIONS
	origin.x = 0; origin.y = 0;
		
	// walls
	upperWall.x = 0;		upperWall.y = 460;
	bottomWall.x = 0;		bottomWall.y = 0;
	leftWall1.x = 0;		leftWall1.y = 0;
	leftWall2.x = 0;		leftWall2.y = 300;
	rightWall1.x = 295;		rightWall1.y = 0;
	rightWall2.x = 295;		rightWall2.y = 300;

	// lines 
	fieldMidLine.x = 150; fieldMidLine.y = 0;

	// obstacles
	bumper1.x = 30;		bumper1.y = 100;
	bumper2.x = 30;		bumper2.y = 200;
	bumper3.x = 30;		bumper3.y = 300;
	bumper4.x = 30;		bumper4.y = 400;
	bumper5.x = 80;		bumper5.y = 40;
	bumper6.x = 80;		bumper6.y = 140;
	bumper7.x = 80;		bumper7.y = 240;
	bumper8.x = 80;		bumper8.y = 340;
	bumper9.x = 80;		bumper9.y = 440;
	bumper10.x = 120;	bumper10.y = 200;
	bumper11.x = 120;	bumper11.y = 300;
	bumper12.x = 180;	bumper12.y = 200;
	bumper13.x = 180;	bumper13.y = 300;
	bumper14.x = 220;	bumper14.y = 40;
	bumper15.x = 220;	bumper15.y = 140;
	bumper16.x = 220;	bumper16.y = 240;
	bumper17.x = 220;	bumper17.y = 340;
	bumper18.x = 220;	bumper18.y = 440;
	bumper19.x = 270;	bumper19.y = 100;
	bumper20.x = 270;	bumper20.y = 200;
	bumper21.x = 270;	bumper21.y = 300;
	bumper22.x = 270;	bumper22.y = 400;

	// COLORS
	// grey
	Color grey;
	grey.r = 0.827451; grey.g = 0.827451; grey.b = 0.827451;

	// gold
	Color gold;
	gold.r = 0.721569; gold.g = 0.52549; gold.b = 0.0431373;

	// black
	Color black;
	black.r = 0.0; black.g = 0.0; black.b = 0.0;

	// green
	Color green;
	green.r = 0.180392; green.g = 0.545098; green.b = 0.341176;

	// DRAWING ALL OBJECTS
	// background
	drawRectangle(origin, 1300, 500, green);

	// Left Flipper
	glPushMatrix();
	glTranslatef(40, 55, 0);
	glRotatef(-25, 0, 0, 1);

	if (GetAsyncKeyState(0x5A) & 0x8000 && angle < 45.0)
	{
		glRotatef(ANGLE_CHANGE, 0, 0, 1);  // rotate n degrees around z axis

		leftRFlipperState.x = 105;
		leftRFlipperState.y = 95;

		leftLFlipperState.x = 50;
		leftLFlipperState.y = 75;

		ld = abs((4 * currentPosition.x) + (-11 * currentPosition.y) + 625) / 11.70469991;
	}
	else
	{
		if (angle > 0.0) {
			glRotatef(-ANGLE_CHANGE, 0, 0, 1);  // rotate n degrees around z axis
		}

		leftRFlipperState.x = 112;
		leftRFlipperState.y = 37;

		leftLFlipperState.x = 50;
		leftLFlipperState.y = 65;

		ld = abs((14 * currentPosition.x) + (31 * currentPosition.y) - 2715) / 34.0147027;
	}

	glPopMatrix();

	if (ld <= 15 && (currentPosition.x >= 50 && currentPosition.x <= 112))
	{
		vy = -vy;
	}

	if (GetAsyncKeyState(0xBF) & 0x8000 && angle < 45.0)
	{
		glRotatef(-ANGLE_CHANGE, 0, 0, 1);  // rotate n degrees around z axis

		rightLFlipperState.x = 170;
		rightLFlipperState.y = 85;

		rightRFlipperState.x = 220;
		rightRFlipperState.y = 67;

		rd = abs((9 * currentPosition.x) + (25 * currentPosition.y) - 3655) / 26.57066051;
	}
	else
	{
		if (angle > 0.0) {
			glRotatef(ANGLE_CHANGE, 0, 0, 1);  // rotate n degrees around z axis
		}
		rightLFlipperState.x = 160;
		rightLFlipperState.y = 37;

		rightRFlipperState.x = 220;
		rightRFlipperState.y = 67;

		rd = abs((1 * currentPosition.x) + (-2 * currentPosition.y) - 86) / 2.236067977;
	}

	//drawFlipper(rightFlipper, -60, 15, blue);
	glPopMatrix();

	if (rd <= 15 && (currentPosition.x >= 160 && currentPosition.x <= 220))
	{
		vy = -vy;
	}

	// obstacles
	drawBumper(bumper1, 6, gold); // bumper1
	drawBumper(bumper2, 6, gold); // bumper2
	drawBumper(bumper3, 6, gold); // bumper3
	drawBumper(bumper4, 6, gold); // bumper4
	drawBumper(bumper5, 6, gold); // bumper5
	drawBumper(bumper6, 6, gold); // bumper6
	drawBumper(bumper7, 6, gold); // bumper7
	drawBumper(bumper8, 6, gold); // bumper8
	drawBumper(bumper9, 6, gold); // bumper9
	drawBumper(bumper10, 6, gold); // bumper10
	drawBumper(bumper11, 6, gold); // bumper11
	drawBumper(bumper12, 6, gold); // bumper12
	drawBumper(bumper13, 6, gold); // bumper13
	drawBumper(bumper14, 6, gold); // bumper14
	drawBumper(bumper15, 6, gold); // bumper15
	drawBumper(bumper16, 6, gold); // bumper16
	drawBumper(bumper17, 6, gold); // bumper17
	drawBumper(bumper18, 6, gold); // bumper18
	drawBumper(bumper19, 6, gold); // bumper19
	drawBumper(bumper20, 6, gold); // bumper20
	drawBumper(bumper21, 6, gold); // bumper21
	drawBumper(bumper22, 6, gold); // bumper22
	drawBumper(bumper23, 6, gold); // bumper23
	drawBumper(bumper24, 6, gold); // bumper24


	//Line mid field 
	drawRectangle(fieldMidLine, 1000, 1, grey);

	//ball
	drawCircle(currentPosition, 8, black); // ball

	// walls
	drawWallS(upperWall, 20, 400, black); // top wall
	drawWallS(bottomWall, 20, 400, black); // bottom wall
	drawWallS(leftWall1, 200, 5, black); // left wall1
	drawWallS(leftWall2, 200, 5, black); //left wall2
	drawWallS(rightWall1, 200, 5, black); //right wall1
	drawWallS(rightWall2, 200, 5, black); //right wall2

	glutSwapBuffers();

}





void idle(void)
{
	//  Call display function (draw the current frame)
	glutPostRedisplay();
}

// initializes openGL, glut, and glui
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 300, 0, 500, 0, 1);
	initialPosition.x = 150;	initialPosition.y = 250;
	currentPosition = initialPosition;	   
}

// checks mouse button press
void mousePress(int button, int state, int x, int y) {
	if (button != 0) {  // left mouse button pressed?
		mousePressed = 1;

	}
	else {
		mousePressed = 0;
		dragging = 0;
	}
}

// says whether the mouse is dragging or not
void mouseDrag(int x, int y) {

	mousePressed = 1;
	dragging = 1;
	mouseX = x;
	mouseY = y;
}

// this sets the mouse position in window 
void mouseMovement(int x, int y) {

	mousePressed = 0;
	dragging = 0;
	mouseX = x;
	mouseY = y;
}

// main method
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 500);
	window = glutCreateWindow("Football Soccer");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseDrag);
	glutPassiveMotionFunc(mouseMovement);

	init();
	glutMainLoop();
	return 0;
}