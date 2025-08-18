#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

double tx = 0, ty = 0;
float rotateSpeed, sunSpeed = 0.05, moonSpeed = 0.05, sunPosition, moonPosition = 260, rotate = 0;
bool night, day;
float r = 0.2, g = 0.8, b = 0.8;

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
		if (wParam == VK_LEFT) {
			rotateSpeed += 0.01;
			tx -= 0.05;
		}
		if (wParam == VK_RIGHT) {
			rotateSpeed -= 0.01;
			tx += 0.05;
		}
		if (wParam == VK_UP)
			ty += 0.05;
		if (wParam == VK_DOWN)
			ty -= 0.05;
		if (wParam == VK_SPACE) {
			rotateSpeed = 0;
		}
		if (wParam == 0x31) {
			night = true;
			day = false;
		}
		if (wParam == 0x32) {
			day = true;
			night = false;
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

void practice1() {
	glClear(GL_COLOR_BUFFER_BIT);

	//Block 4
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);

	//Block 5
	glPushMatrix();
	glRotatef(0.1, 0, 0, 1);

	//Block 1
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(-0.5, 0);
	glVertex2f(0, 0.5);
	glVertex2f(0.5, 0);
	glEnd();
	glPopMatrix();

	//Block 2
	glPushMatrix();
	glTranslatef(-0.5, 0, 0);
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glEnd();
	glPopMatrix();
}

void q1() {

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(tx, ty, 0);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, 0.2);
	glVertex2f(0, 0.2);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-tx, -ty, 0);
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex2f(0.5, 0);
	glVertex2f(0.5, 0.2);
	glVertex2f(0, 0.2);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
}

void windmill() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(-1, -0.3);
	glVertex2f(1, -0.3);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);
	glEnd();

	//Sun
	if (night && sunPosition <= 180) {
		sunPosition += sunSpeed;
		if (r > 0.2) {
			r -= 0.00009;
		}
		if (g > 0.3) {
			g -= 0.00009;
		}
		if (b > 0.3) {
			b -= 0.00009;
		}
	}

	glPushMatrix();
	glRotatef(sunPosition, 0, 0, 1);
	glColor3f(1, 0.5, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.6, 0.6);
	for (double angle = 0; angle <= 2 * 3.142; angle += (2 * 3.142) / 30) {
		double x2 = 0.6 + (0.2) * cos(angle);
		double y2 = 0.6 + (0.2) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();
	glPopMatrix();

	
	//Moon
	if (night && sunPosition >= 160 && moonPosition < 360) {
		moonPosition += moonSpeed;

	}

	glPushMatrix();
	glRotatef(moonPosition, 0, 0, 1);
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.6, 0.6);
	for (double angle = 0; angle <= 2 * 3.142; angle += (2 * 3.142) / 30) {
		double x2 = 0.6 + (0.2) * cos(angle);
		double y2 = 0.6 + (0.2) * sin(angle);
		glVertex2f(x2, y2);
	}
	glEnd();
	glPopMatrix();

	glColor3f(0, 0.6, 0);
	glBegin(GL_QUADS);
	glVertex2f(-1, -0.3);
	glVertex2f(1, -0.3);
	glVertex2f(1, -1);
	glVertex2f(-1, -1);
	glEnd();

	glColor3f(0.2, 0, 0.09);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.12, 0.05);
	glVertex2f(0.12, 0.05);
	glVertex2f(0, 0.3);
	glEnd();

	glColor3f(0.93, 0.89, 0.69);
	glBegin(GL_QUADS);
	glVertex2f(-0.1, -0.1);
	glVertex2f(-0.1, 0.05);
	glVertex2f(0.1, 0.05);
	glVertex2f(0.1, -0.1);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(-0.1, -0.1);
	glVertex2f(0.1, -0.1);
	glVertex2f(0.15, -0.3);
	glVertex2f(-0.15, -0.3);
	glEnd();

	glColor3f(0.93, 0.89, 0.69);
	glBegin(GL_QUADS);
	glVertex2f(-0.15, -0.3);
	glVertex2f(0.15, -0.3);
	glVertex2f(0.15, -0.5);
	glVertex2f(-0.15, -0.5);
	glEnd();

	glColor3f(0.2, 0, 0.09);
	glBegin(GL_QUADS);
	glVertex2f(-0.15, -0.5);
	glVertex2f(0.15, -0.5);
	glVertex2f(0.3, -0.6);
	glVertex2f(-0.3, -0.6);
	glEnd();

	glColor3f(0.93,0.89,0.69);
	glBegin(GL_QUADS);
	glVertex2f(-0.25, -0.6);
	glVertex2f(0.25, -0.6);
	glVertex2f(0.25, -0.8);
	glVertex2f(-0.25, -0.8);
	glEnd();

	rotate += rotateSpeed;
	glPushMatrix();
	glRotatef(rotate, 0, 0, 1);
	glColor3f(1, 1, 1);
	glLineWidth(40);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0.4, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, -0.4);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(-0.4, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, 0.4);
	glEnd();
	glPopMatrix();
}

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	windmill();
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