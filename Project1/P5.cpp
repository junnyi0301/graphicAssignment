#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

int page = 1;
double rotateX = 0, rotateY = 0, rotateZ = 0;

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
		if (wParam == 0x31)
			page = 1;
		if (wParam == 0x32)
			page = 2;
		if (wParam == 0x33) {
			page = 3;
			rotateX = 0;
			rotateY = 0;
			rotateZ = 0;
		}
		if (wParam == VK_LEFT)
			rotateY += 5;
		if (wParam == VK_RIGHT)
			rotateY -= 5;
		if (wParam == VK_UP)
			rotateZ += 5;
		if (wParam == VK_DOWN)
			rotateZ -= 5;

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

void sphere() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_LINE);
	glRotatef(0.03, 1, 0, 0);
	glColor3f(1, 0, 0);
	gluSphere(quadric, 0.6, 30, 8);

}

void cyclinder() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_LINE);
	glRotatef(0.03, 1, 0, 0);
	glColor3f(1, 0, 0);
	gluCylinder(quadric, 0.6, 0.6, 0.5, 30, 8);
}

void iceCreamCone() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glRotatef(90,1,0,0);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	glColor3f(0.72, 0.47, 0.34);
	gluCylinder(quadric, 0.2, 0, 0.6, 30, 8);
	glLineWidth(2);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	glColor3f(0.25, 0.15, 0.10);
	gluCylinder(quadric, 0.21, 0, 0.6, 10, 5);
	glPopMatrix();
}

void iceCream(double radius) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, radius, 30, 8);
}

void topping1() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	glColor3f(0.25, 0.15, 0.10);
	gluCylinder(quadric, 0.02, 0.02, 0.5, 30, 8);
}

void topping2() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	glColor3f(1, 1, 1);
	gluCylinder(quadric, 0.12, 0.12, 0.005, 30, 8);
}

void cherry() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	glColor3f(1, 0, 0);
	gluSphere(quadric, 0.06, 30, 8);
}

void iceCreamComplete() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glLoadIdentity();
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateZ, 0, 0, 1);
	glColor3f(0.8,0.8,0);
	iceCream(0.18);
	glColor3f(0, 0.8, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.2, 0);
	iceCream(0.17);
	glPopMatrix();
	iceCreamCone();

	glPushMatrix();
	glTranslatef(0, 0.38, 0);
	cherry();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glPushMatrix();
	glRotatef(-60, 1, 0, 0);
	topping1();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.35, -0.1);
	glPushMatrix();
	glRotatef(-35, 1, 0, 0);
	topping2();
	glPopMatrix();
	glPopMatrix();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (page == 1)
		sphere();
	else if (page == 2)
		cyclinder();
	else if (page == 3)
		iceCreamComplete();
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
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
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