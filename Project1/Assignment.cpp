#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

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
		else if (wParam == 'L')
			leftArmMovement = !leftArmMovement;
		else if (wParam == 'M')
			isWalking = !isWalking;
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
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.2, 0.2, 0.1, 10, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();
	glPopMatrix();
}
void fingerTip() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glColor3f(0, 1, 0);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	glLineWidth(2);
	gluSphere(quadric, 0.02, 30, 30);
	gluDeleteQuadric(quadric);
}
void finger() {

	glColor3f(1, 0, 0);
	fingerPart();

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	fingerPart();
	glPopMatrix();

	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslatef(0, -0.1, 0);
	fingerPart();
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.15, 0);
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	fingerTip();
	glPopMatrix();
	glPopMatrix();
}
void thumb() {

	glPushMatrix();
	glScalef(0.92, 0.65, 0.92);
	glColor3f(1, 0, 0);
	fingerPart();

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	fingerPart();
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.1, 0);
	fingerTip();
	glPopMatrix();
	glPopMatrix();
}
void pinky() {
	glPushMatrix();
	glScalef(0.78, 0.7, 0.78); //Adjust the size of the finger
	finger();
	glPopMatrix();
}
void ring() {
	glPushMatrix();
	glScalef(0.95, 0.95, 0.95);
	finger();
	glPopMatrix();
}
void index() {
	glPushMatrix();
	glScalef(0.92, 0.9, 0.92);
	finger();
	glPopMatrix();
}
void cyclinder() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.15, 0.2, 0.2, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}
void palm() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glScalef(0.5, 0.7, 0.16);
	glRotatef(90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.15, 0.2, 0.2, 10, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();
	
}

void fingers(bool left) {

	//Pinky finger
	if (left) {
		glPushMatrix();
		glTranslatef(-0.08, 0.03, 0.01);
	}
	else {
		glPushMatrix();
		glTranslatef(0.08, 0.03, 0.01);
	}

	pinky();
	glPopMatrix();

	//Ring finger
	glPushMatrix();
	if (left) {
		glTranslatef(-0.04, 0.01, 0);
	}
	else {
		glTranslatef(0.04, 0.01, 0);
	}
	ring();
	glPopMatrix();

	//Middle finger
	finger();

	//Index finger
	glPushMatrix();
	if (left) {
		glTranslatef(0.04, 0.01, 0);
	}
	else {
		glTranslatef(-0.04, 0.01, 0);
	}
	index();
	glPopMatrix();

	//Thumb
	glPushMatrix();
	if (left) {
		glTranslatef(0.08, 0.02, 0.03);
	}
	else {
		glTranslatef(-0.08, 0.02, 0.03);
	}
	thumb();
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
	glPushMatrix();
	glRotatef(left ? -15 : 15, 0, 0, 1);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.15, 0.5);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	glColor3f(1, 0, 0);
	gluCylinder(quadric, 0.4, 0.7, 0.8, 10, 3);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void arm(bool left) {
	sleeve(left);
	
	glPushMatrix();
	glTranslatef(left ? -0.08 : 0.08,0.02,0);
	glRotatef(left ? -15 : 15, 0, 0, 1);
	glRotatef(left ? 50 : -50, 0, 1, 0);
	glScalef(0.7,0.7,0.7);
	hand(left);
	glPopMatrix();
	glPopMatrix();
}


void arms() {
	glPushMatrix();
	glTranslatef(-0.3, 0, 0);
	glPushMatrix();
	glRotatef(leftArmAngle, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	arm(true);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0, 0);
	glPushMatrix();
	glRotatef(rightArmAngle, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	arm(false);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

//Body
void torso() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//Chest
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1,0.7,1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.25, 0.25, 0.25, 10, 1);
	glPopMatrix();

	//Waits
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.25, 0.25, 0.05, 10, 1);
	glPopMatrix();

	//Lower
	glPushMatrix();
	glTranslatef(0,-0.35,0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.25, 0.35, 0.45, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void hoodie() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.3, 0.8, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
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
	GLenum drawstyle = GLU_LINE;

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

void weapon() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//Handle
	glPushMatrix();
	glTranslatef(-1, 0.25, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.01, 0.01, 1.15, 30, 1);
	glPopMatrix();

	//Connector
	glColor3f(0, 1, 1);
	glPushMatrix();
	glTranslatef(-1, 0.18, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.02, 0.03, 0.12, 10, 1);
	glPopMatrix();

	//Tip
	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslatef(-1, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.01, 0.05, 0.15, 4, 1);
	glPopMatrix();

	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslatef(-1, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.05, 0, 0.3, 4, 1);
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

	if(isWalking)
		updateWalk();
	else {
		leftArmAngle = 0;
		leftLegAngle = 0;
		rightArmAngle = 0;
		rightLegAngle = 0;
	}
	arms();
	body();
	feet();
	weapon();
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