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
	//glColor3f(1, 0, 0);
	gluCylinder(quadric, 0.2, 0.2, 0.1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	gluDeleteQuadric(quadric);
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
	glScalef(0.78, 0.75, 0.78);
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
	//glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.15, 0.2, 0.2, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}
void palm() {
	glPushMatrix();
	glScalef(0.5, 0.7, 0.16);
	cyclinder();
	glPopMatrix();
	
}

void fingers(bool left) {

	glPushMatrix();
	if (left) {
		glTranslatef(0, 0.03, 0.01);
		pinky();
	}
	else {
		glTranslatef(0, 0.02, 0.03);
		thumb();
	}
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.04, 0.01, 0);
	if (left)
		ring();
	else
		index();
	glPopMatrix();

	//Middle finger
	glPushMatrix();
	glTranslatef(0.08, 0, 0);
	finger();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.12, 0.01, 0);
	index();
	glPopMatrix();

	glPushMatrix();
	if (left) {
		glTranslatef(0.16, 0.02, 0.03);
		thumb();
	}
	else {
		glTranslatef(0.16, 0.03, 0.01);
		pinky();
	}
	glPopMatrix();
}
void hand(bool left) {
	glPushMatrix();
	glTranslatef(0.08, 0.15, 0.015);
	palm();
	glPopMatrix();

	fingers(left);
}
void sleeve(bool left) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(left ? 0.15 : 0, 0.5, 0.01);
	glPushMatrix();
	glRotatef(left ? -15 : 15, 0, 0, 1);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.1, 0.5);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	glColor3f(1, 0, 0);
	gluCylinder(quadric, 0.4, 0.8, 0.8, 10, 3);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void arm(bool left) {
	sleeve(left);

	glPushMatrix();
	glRotatef(left ? -5 : 5, 0, 0, 1);
	hand(left);
	glPopMatrix();
}


void arms() {
	glPushMatrix();
	glTranslatef(-0.4, 0, 0);
	glPushMatrix();
	glRotatef(leftX, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	arm(true);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 0, 0);
	glPushMatrix();
	glRotatef(rightX, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	arm(false);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
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

	arms();
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