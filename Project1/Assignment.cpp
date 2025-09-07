#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Style
GLenum drawstyle = GLU_FILL;

//Camera movement
float ptx = 0, pty = 0, pRy = 0, pRx = 0, pRz = 1;

//Object movement
float tRx = 0, tRy = 0;

//Arm movement
bool leftArmMovement = true;
float leftX = 0, rightX = 0;
float armMaxHeight = 180, armSpeed = 5, armMinHeight = -45;

//Walking Animation
float leftLegAngle = 0.0f;
float rightLegAngle = 0.0f;
float leftArmAngle = 0.0f;
float rightArmAngle = 0.0f;

bool isWalking = false;
bool leftLegForward = true;
bool rightLegForward = false; // opposite phase
bool leftArmForward = false;
bool rightArmForward = true;

float stepSpeed = 0.1f;   // how fast the angle changes
float maxLegAngle = 10.0f; // max swing
float maxArmAngle = 20.0f;

//Light
float x = 0.7, y = -0.5, z = -1;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == 'A')
			pRy += 0.1;
		else if (wParam == 'D')
			pRy -= 0.1;
		else if (wParam == 'W')
			pRx += 0.1;
		else if (wParam == 'S')
			pRx -= 0.1;
		else if (wParam == 'E')
			pRz += 0.1;
		else if (wParam == 'Q')
			pRz -= 0.1;
		else if (wParam == 'I')
			y += 0.1;
		else if (wParam == 'K')
			y -= 0.1;
		else if (wParam == 'J')
			x += 0.1;
		else if (wParam == 'L')
			x -= 0.1;
		else if (wParam == 'U')
			z -= 0.1;
		else if (wParam == 'O')
			z += 0.1;
		else if (wParam == 'P')
			leftArmMovement = !leftArmMovement;
		else if (wParam == 'M') {
			isWalking = !isWalking;
			leftX = 0;
			rightX = 0;
		}
		else if (wParam == VK_UP)
			leftArmMovement ? leftX < armMaxHeight ? leftX += armSpeed : leftX = armMaxHeight : rightX < armMaxHeight ? rightX += armSpeed : rightX = armMaxHeight;
		else if (wParam == VK_DOWN)
			leftArmMovement ? leftX > armMinHeight ? leftX -= armSpeed : leftX = armMinHeight : rightX > armMinHeight ? rightX -= armSpeed : rightX = armMinHeight;
		else if (wParam == VK_LEFT)
			tRx -= 3;
		else if (wParam == VK_RIGHT)
			tRx += 3;
		else if (wParam == VK_SPACE) {
			pRx = 0;
			pRy = 0;
			pRz = 1;
			leftX = 0;
			rightX = 0;
			x = 0.7;
			y = -0.5;
			z = -1;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//Arm Parts
void fingerPart() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glPushMatrix();
	glScalef(0.1, 0.1, 0.5);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.2, 0.2, 0.1, 10, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();
	glPopMatrix();
}
void fingerTip() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	gluQuadricDrawStyle(quadric, drawstyle);
	glLineWidth(2);
	gluSphere(quadric, 0.02, 30, 30);
	glPopMatrix();
	gluDeleteQuadric(quadric);
}
void finger(bool left) {

	glPushMatrix();
	left ? glRotatef(0, 0, 0, 0) : glRotatef(-80, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(0, 0, 0, 0) : glRotatef(-20, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(0, 0, 0, 0) : glRotatef(-80, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();
	glPopMatrix();

	

	
}
void thumb(bool left) {

	glPushMatrix();
	left ? glRotatef(0, 0, 0, 0) : glRotatef(-90, 1, 0, 0);
	glScalef(0.92, 0.9, 0.92);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(0, 0, 0, 0) : glRotatef(20, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();
	glPopMatrix();
}
void pinky(bool left) {
	glPushMatrix();
	glScalef(0.78, 0.7, 0.78); //Adjust the size of the finger
	finger(left);
	glPopMatrix();
}
void ring(bool left) {
	glPushMatrix();
	glScalef(0.95, 0.95, 0.95);
	finger(left);
	glPopMatrix();
}
void index(bool left) {
	glPushMatrix();
	glScalef(0.92, 0.9, 0.92);
	finger(left);
	glPopMatrix();
}
void cyclinder() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.15, 0.2, 0.2, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}
void palm() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(0, -0.15, 0);
	glScalef(0.5, 0.7, 0.16);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.2, 0.15, 0.15, 10, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();
	
}
void fingers(bool left) {

	//Pinky finger
	glPushMatrix();
	if (left) {
		glTranslatef(-0.08, 0.03, 0.01);
	}
	else {
		glTranslatef(0.08, 0.03, 0.01);
	}

	pinky(left);
	glPopMatrix();

	//Ring finger
	glPushMatrix();
	if (left) {
		glTranslatef(-0.04, 0.01, 0);
	}
	else {
		glTranslatef(0.04, 0.01, 0);
	}
	ring(left);
	glPopMatrix();

	//Middle finger
	finger(left);

	//Index finger
	glPushMatrix();
	if (left) {
		glTranslatef(0.04, 0.01, 0);
	}
	else {
		glTranslatef(-0.04, 0.01, 0);
	}
	index(left);
	glPopMatrix();

	//Thumb
	glPushMatrix();
	if (left) {
		glTranslatef(0.08, 0.05, 0.04);
	}
	else {
		glTranslatef(-0.08, 0.06, 0.02);
	}
	thumb(left);
	glPopMatrix();

}

void hand(bool left) {
	glPushMatrix();
	glTranslatef(0, 0.15, 0.015);
	palm();
	glPopMatrix();

	fingers(left);

}
void sleeve(bool left) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(0, 0.5, 0.01);
	glRotatef(left ? -15 : 15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(0.2, 0.15, 0.5);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.4, 0.7, 0.8, 10, 3);
	glPopMatrix();
}

void weapon() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	//Handle
	glColor3f(0.25,0,0);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.01, 0.01, 1.15, 30, 1);
	glPopMatrix();

	//Connector
	glColor3f(0.8, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.18, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.02, 0.03, 0.1, 10, 1);
	glPopMatrix();

	//Tip
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.01, 0.05, 0.15, 4, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0, 0.3, 4, 1);
	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);

	//Outline
	//Tip
	glLineWidth(3);
	glColor3f(0, 0.67, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.01, 0.05, 0.15, 4, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.05, 0, 0.3, 4, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}


void arm(bool left) {

	glColor3f(0.9, 0.9, 0.9);
	sleeve(left);
	
	glPushMatrix();
	glTranslatef(left ? -0.08 : 0.08,0.02,0);
	glRotatef(left ? -15 : 15, 0, 0, 1);
	glRotatef(left ? 70 : -90, 0, 1, 0);
	glScalef(0.7,0.7,0.7);
	glColor3f(0.97, 0.97, 0.8);
	hand(left);
	glPopMatrix();

	if (!left) {
		glPushMatrix();
		glTranslatef(0.04, 0.03, -0.25);
		glRotatef(-90, 1, 0, 0);
		weapon();
		glPopMatrix();
	}
}


void arms() {
	glPushMatrix();
	glTranslatef(-0.3, 0, 0);
	glRotatef(leftArmAngle + leftX, 1, 0, 0);
	glTranslatef(0, -0.5, 0);
	arm(true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0, 0);
	glRotatef(rightArmAngle + rightX, 1, 0, 0);
	glTranslatef(0, -0.5, 0);
	arm(false);
	glPopMatrix();
}

//Body
void torso() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//Chest
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1,0.7,1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.25, 0.25, 0.25, 10, 1);
	glPopMatrix();

	//Waits
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.25, 0.25, 0.05, 10, 1);
	glPopMatrix();

	//Lower
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0,-0.35,0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.25, 0.35, 0.45, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void hoodie() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.3, 0.8, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.19, 0.3, 0.08, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);

}

void body() {
	torso();
	hoodie();
}

void foot() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//Leg
	glPushMatrix();
	glTranslatef(0, -0.6, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.08, 0.08, 0.3, 10, 1);
	glPopMatrix();

	//Shoe sole
	glPushMatrix();
	glTranslatef(0, -0.87, -0.05);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1.4, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.08, 0.08, 0.04, 10, 1);
	glPopMatrix();


	//Shoe Tip
	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(0, 1, 0, 0);
	glScalef(1.2, 0.5, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0.07, 0.1, 10, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(180, 1, 0, 0);
	glScalef(1, 0.5, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.06, 0, 0.02, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void updateWalk() {
	// ---- Left leg ----
	if (leftLegForward) {
		leftLegAngle += stepSpeed;
		if (leftLegAngle >= maxLegAngle) leftLegForward = false;
	}
	else {
		leftLegAngle -= stepSpeed;
		if (leftLegAngle <= -maxLegAngle) leftLegForward = true;
	}

	// ---- Right leg (opposite) ----
	if (rightLegForward) {
		rightLegAngle += stepSpeed;
		if (rightLegAngle >= maxLegAngle) rightLegForward = false;
	}
	else {
		rightLegAngle -= stepSpeed;
		if (rightLegAngle <= -maxLegAngle) rightLegForward = true;
	}

	// ---- Left arm (opposite to left leg) ----
	if (leftArmForward) {
		leftArmAngle += stepSpeed;
		if (leftArmAngle >= maxArmAngle) leftArmForward = false;
	}
	else {
		leftArmAngle -= stepSpeed;
		if (leftArmAngle <= -maxArmAngle) leftArmForward = true;
	}

	// ---- Right arm (opposite to right leg) ----
	if (rightArmForward) {
		rightArmAngle += stepSpeed;
		if (rightArmAngle >= maxArmAngle) rightArmForward = false;
	}
	else {
		rightArmAngle -= stepSpeed;
		if (rightArmAngle <= -maxArmAngle) rightArmForward = true;
	}
}

void feet() {
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(-0.1, 0, 0);
	glRotatef(leftLegAngle, 1, 0, 0);
	foot();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0, 0);
	glRotatef(rightLegAngle, 1, 0, 0);
	foot();
	glPopMatrix();
}

void head() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, drawstyle);
	glPushMatrix();
	glTranslatef(0,0.25,-0.05);
	glScalef(0.7,0.7,0.5);
	gluSphere(quadric, 0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	gluSphere(quadric, 0.2, 30, 30);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void lighting() {

	//glEnable(GL_LIGHTING);

	// White diffuse + ambient + specular light
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // diffuse color
	GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // soft ambient light
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // shiny reflections
	GLfloat lightPos[] = { x, y, z, 1.0f };          // position (1.0f = point light)

	// Blue material with white highlights
	GLfloat matDiffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // base color (blue)
	GLfloat matAmbient[] = { 0.0f, 0.0f, 0.2f, 1.0f }; // darker ambient blue
	GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // shiny white reflection
	GLfloat matShininess[] = { 50.0f };                  // shininess [0–128]

	// Let glColor3f() control the material color
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// Assign light properties
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	// Assign material properties
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	//Light Source
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x, y, z);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0.05, 0.05, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(20, 20, 1, 50);
	gluPerspective(100.0, 1920.0 / 1080.0, 0.1, 10.0);
	gluLookAt(pRy, pRx, -pRz,   // camera position
		0.0, 0.0, 0.0,     // look at origin
		0.0, 1.0, 0.0);

	lighting();

	if(isWalking)
		updateWalk();
	else {
		leftArmAngle = 0;
		leftLegAngle = 0;
		rightArmAngle = 0;
		rightLegAngle = 0;
	}

	head();
	arms();
	body();
	feet();
	glPushMatrix();
	glTranslatef(-1, 0, 0);
	weapon();
	glPopMatrix();
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------