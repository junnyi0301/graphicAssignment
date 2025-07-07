
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

int qNo = 1;
bool dead = false;
float x = 0, y = 0;		//origin or circle
float r = 1;			//radius
float x2 = 0, y2 = 0;	//point on circle
float angle = 0;		//angle
float PI = 3.142;		//PI
int noOfTri = 30;		//Number of triangle

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
		else if (wParam == 0x31)
			qNo = 1;
		else if (wParam == 0x32)
			qNo = 2;
		else if (wParam == 0x33)
			qNo = 3;
		else if (wParam == 0x34)
			qNo = 4;
		else if (wParam == 0x35)
			qNo = 5;
		else if (wParam == 0x36)
			qNo = 6;
		else if (wParam == 0x37)
			qNo = 7;
		else if (wParam == 0x38)
			if (dead == false) {
				dead = true;
			}
			else {
				dead = false;
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

void pahang() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(-0.8, 0);
	glVertex2f(-0.8, 0.5);
	glVertex2f(0.8, 0.5);
	glVertex2f(0.8, 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8, -0.5);
	glVertex2f(-0.8, 0);
	glVertex2f(0.8, 0);
	glVertex2f(0.8, -0.5);
	glEnd();
}

void negeriSembilan() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Background
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 1, 0);
	glVertex2f(-0.8, -0.5);
	glVertex2f(-0.8, 0);
	glVertex2f(0, -0.5);
	glVertex2f(0, 0.5);
	glVertex2f(0.8, -0.5);
	glVertex2f(0.8, 0.5);
	glEnd();

	//Logo
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8, 0);
	glVertex2f(-0.8, 0.5);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 0.5);
	glVertex2f(-0.8, 0.5);
	glEnd();
}

void england() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Red
	glBegin(GL_QUADS);
	glColor3f(0.6, 0, 0);
	glVertex2f(-0.8, -0.1);
	glVertex2f(0.8, -0.1);
	glVertex2f(0.8, 0.1);
	glVertex2f(-0.8, 0.1);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6, 0, 0);
	glVertex2f(-0.08, -0.5);
	glVertex2f(0.08, -0.5);
	glVertex2f(0.08, 0.5);
	glVertex2f(-0.08, 0.5);
	glEnd();

	//White
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(-0.8, 0.1);
	glColor4f(0.6, 0.6, 0.6, 0);
	glVertex2f(-0.8, 0.5);
	glColor3f(1, 1, 1);
	glVertex2f(-0.08, 0.5);
	glVertex2f(-0.08, 0.1);
	glEnd();

	//White
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(0.08, 0.1);
	glColor3f(1, 1, 1);
	glVertex2f(0.08, 0.5);
	glColor4f(0.6, 0.6, 0.6, 0);
	glVertex2f(0.8, 0.5);
	glColor3f(1, 1, 1);
	glVertex2f(0.8, 0.1);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(-0.8, -0.1);
	glColor4f(0.6, 0.6, 0.6, 0);
	glVertex2f(-0.8, -0.5);
	glColor3f(1, 1, 1);
	glVertex2f(-0.08, -0.5);
	glColor3f(1, 1, 1);
	glVertex2f(-0.08, -0.1);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(0.08, -0.1);
	glColor3f(1, 1, 1);
	glVertex2f(0.08, -0.5);
	glColor4f(0.6, 0.6, 0.6, 0);
	glVertex2f(0.8, -0.5);
	glColor3f(1, 1, 1);
	glVertex2f(0.8, -0.1);
	glEnd();
}

void scotland() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	glVertex2f(-0.8, -0.5);
	glVertex2f(-0.8, 0.5);
	glVertex2f(0.8, 0.5);
	glVertex2f(0.8, -0.5);
	glEnd();

	glColor3f(0.2, 0.4, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0.1);
	glVertex2f(-0.7, 0.5);
	glVertex2f(0.7, 0.5);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0, -0.1);
	glVertex2f(-0.7, -0.5);
	glVertex2f(0.7, -0.5);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.2, 0);
	glVertex2f(0.8, 0.35);
	glVertex2f(0.8, -0.35);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.2, 0);
	glVertex2f(-0.8, 0.35);
	glVertex2f(-0.8, -0.35);
	glEnd();
}

void japan() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(-0.8, -0.5);
	glVertex2f(-0.8, 0.5);
	glVertex2f(0.8, 0.5);
	glVertex2f(0.8, -0.5);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / noOfTri) {
		x2 = x + (r-0.8) * cos(angle);
		y2 = y + (r-0.8) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();
}

void smile() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / noOfTri) {
		x2 = x + (r-0.5) * cos(angle);
		y2 = y + (r-0.5) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	glColor3f(0, 0, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(-0.2, 0.2);
	glVertex2f(0.2, 0.2);
	glEnd();

	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	//glVertex2f(x, y-0.1);
	for (angle = 0; angle <= PI; angle += PI / noOfTri) {
		x2 = x - (r - 0.85) * cos(angle);
		y2 = y-0.1 - (r - 0.85) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();
}

void emoji() {
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / noOfTri) {
		x2 = x + (r-0.5) * cos(angle);
		y2 = y + (r - 0.5) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	//Eyes
	glLineWidth(7);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-0.2, 0.2);
	glVertex2f(-0.1, 0.1);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-0.1, 0.2);
	glVertex2f(-0.2, 0.1);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0.2, 0.2);
	glVertex2f(0.1, 0.1);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0.1, 0.2);
	glVertex2f(0.2, 0.1);
	glEnd();

	//Nose
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y-0.05);
	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / noOfTri) {
		x2 = x + (r - 0.95) * cos(angle);
		y2 = y-0.05 + (r - 0.95) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	//Halo
	glColor3f(0, 1, 1);
	glBegin(GL_LINE_STRIP);
	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / noOfTri) {
		x2 = x + (r - 0.6) * cos(angle);
		y2 = y + 0.45 + (r - 0.9) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	//Mouth
	glColor3f(1,0,0);
	glLineWidth(8);
	glBegin(GL_LINE_STRIP);
	for (angle = 0; angle <= PI; angle += PI / noOfTri) {
		x2 = x - (r - 0.79) * cos(angle);
		y2 = y-0.1 - (r - 0.87) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	for (angle = 0; angle <= 2*PI; angle += (2*PI) / noOfTri) {
		x2 = x+0.15 - (r - 0.95) * cos(angle);
		y2 = y-0.2 - 0.1 - (r - 0.95) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();

	//Tongue
	glBegin(GL_QUADS);
	glVertex2f(0.15,-0.2);
	glVertex2f(0.2,-0.15);
	glVertex2f(0, 0);
	glVertex2f(0.02, 0.02);
	glEnd();

}

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------

	switch (qNo) {
	case 1:
		pahang();
		break;
	case 2:
		negeriSembilan();
		break;
	case 3:
		england();
		break;
	case 4:
		scotland();
		break;
	case 5:
		japan();
		break;
	case 6:
		smile();
		break;
	case 7:
		emoji();
		break;
	}

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
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