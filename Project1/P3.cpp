#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

int page = 1, x, y, z;

float rotateSpeedX = 0, rotateSpeedY = 0, rotateSpeedZ = 0;

bool move = false;

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
			page = 1;
		else if (wParam == 0x32)
			page = 2;
		else if (wParam == 0x33)
			page = 3;
		else if (wParam == 0x58) {
			x = 1;
			y = 0;
			z = 0;
		}
		else if (wParam == 0x59) {
			x = 0;
			y = 1;
			z = 0;
		}
		else if (wParam == 0x5A) {
			x = 0;
			y = 0;
			z = 1;
		}
		else if (wParam == VK_LEFT)
			rotateSpeedY -= 2;
		else if (wParam == VK_RIGHT)
			rotateSpeedY += 2;
		else if (wParam == VK_UP) {
			if (move) {
				rotateSpeedX += 1;
			}
			else if (!move) {
				rotateSpeedZ -= 1;
			}
		}
		else if (wParam == VK_DOWN) {
			if (move && rotateSpeedX > 0) {
				rotateSpeedX -= 1;
			}
			else if (!move) {
				rotateSpeedZ += 1;
			}
		}
		else if (wParam == 0x34) {
			if (move == false) {
				move = true;
			}
			else {
				move = false;
			}
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

void drawCube(float size)
{
	glBegin(GL_QUADS);
	// Face 1
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	// Face 2
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, 0.0f, size);
	//Face 3
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);
	//Face 4
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	//Face 5
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);
	//Face 6
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(size, size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glEnd();
}

void drawPyramid(float size)
{
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	// Face 1
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}

void drawArm(float size) {
	glLineWidth(5);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
	glVertex3f(size, 0.2, 0);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0, 0.2, 0.2);
	glVertex3f(size, 0.2, 0.2);
	glVertex3f(size, 0, 0.2);
	glVertex3f(0, 0, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0, 0.2, 0.2);
	glVertex3f(0, 0, 0.2);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(size, 0, 0);
	glVertex3f(size, 0.2, 0);
	glVertex3f(size, 0.2, 0.2);
	glVertex3f(size, 0, 0.2);
	glEnd();
}

void robotArm() {
	glPushMatrix();
	glRotatef(rotateSpeedY, 0, 1, 0);
	glPushMatrix();
	glRotatef(rotateSpeedZ,1,0,0);
	glPushMatrix();
	glRotatef(rotateSpeedX, 0, 0, 1);
	drawArm(0.4);
	glPopMatrix();
	drawArm(-0.4);
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (page == 1) {
		glPushMatrix();
		glRotatef(0.01, 1, 1, 1);
		drawCube(0.3);
	}
	else if (page == 2) {
		glPushMatrix();
		glRotatef(0.1, x, y, z);
		drawPyramid(0.2);
	}
	else if (page == 3) {
		glPopMatrix();
		robotArm();
	}
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