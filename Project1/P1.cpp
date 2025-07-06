
#include <Windows.h>
#include <gl/GL.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

int qNo = 1;

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
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0);
	glVertex2f(-0.8, -0.5);
	glVertex2f(-0.8, 0);
	glVertex2f(0, 0);
	glVertex2f(0, -0.5);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0, -0.5);
	glVertex2f(0, 0.5);
	glVertex2f(0.8, 0.5);
	glVertex2f(0.8, -0.5);
	glEnd();

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
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
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