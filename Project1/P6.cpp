#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>


#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float x = 0.8, y = 0, z = 0;
float ambL[3] = { 1.0, 1.0, 1.0 }; // White color amb light
float posA[3] = { x, y, z };  //amb light position { 0.0, 0.8, 0.0 }
float difL[3] = { 1.0, 1.0, 1.0 }; // white color diffuse light
float posD[3] = { x, y, z };  //dif light position { 0.8, 0.0, 0.0 }
float ambM[3] = { 0.0, 0.0, 1.0 }; //blue color amb material
float difM[3] = { 0.0, 0.0, 1.0 }; //blue color dif material

bool isLightOn = true;
bool clockwise = true;

int page = 1;

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
		else if (wParam == VK_SPACE)
			isLightOn = !isLightOn;
		else if (wParam == 'W')
			posD[1] += 0.5;
		else if (wParam == 'S')
			posD[1] -= 0.5;
		else if (wParam == 'A')
			posD[0] -= 0.5;
		else if (wParam == 'D')
			posD[0] += 0.5;
		else if (wParam == 'E')
			posD[2] -= 0.5;
		else if (wParam == 'Q')
			posD[2] += 0.5;
		else if (wParam == VK_UP)
			clockwise = true;
		else if (wParam == VK_DOWN)
			clockwise = false;
		else if (wParam == 'O')
			page = 1;
		else if (wParam == 'P')
			page = 2;
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

void sphere(double r) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//gluQuadricDrawStyle(quadric, GLU_LINE);
	//glRotatef(0.03, 1, 0, 0);
	glColor3f(0.2, 0.4, 0.8);
	gluSphere(quadric, r, 30, 30);

}

void drawPyramid(float size)
{
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0.0);
	glVertex3f(0, size / 2, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(0, size / 2, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(0, size / 2, 0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glVertex3f(0, size / 2, 0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, size/2, 0);
	glVertex3f(-size/2, -size/2, -size/2);
	glVertex3f(size/2, -size / 2, -size/2);
	glVertex3f(size/2, -size / 2, size/2);
	glVertex3f(-size/2, -size / 2, size/2);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glVertex3f(size / 2, -size / 2, size / 2);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glEnd();
}

void lighting() {
	if (isLightOn) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}

	//Light 0: White color amb light at posA above sphere (0.0, 0.8, 0.0)
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambL); //red color amb light
	glLightfv(GL_LIGHT0, GL_POSITION, posA);
	//glEnable(GL_LIGHT0); // off when light 1 on

	//Light 1: White color dif light at pos (0.8, 0.0, 0.0) right sphere
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difL); //red color amb light
	glLightfv(GL_LIGHT1, GL_POSITION, posD);
	glEnable(GL_LIGHT1);
}

void display()
{
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	lighting();

	glMatrixMode(GL_MODELVIEW);
	if (clockwise)
		glRotatef(-0.1, 1.0, 1.0, 1.0);
	else
		glRotatef(0.1, 1.0, 1.0, 1.0);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambM); // red color amb material
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);

	if (page == 1)
		sphere(0.5);
	else if (page == 2)
		drawPyramid(0.8);


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
		700, 10, 500, 500,
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