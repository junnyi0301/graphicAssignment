#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

double x = 0, y = 0;
bool reset = false;
float hue = 0.0f;
double r = 1, g = 1, b = 1;
double scale = 0;
double PI = 3.14159265358979323846;

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
		else if (wParam == VK_LEFT)
			x -= 0.2;
		else if (wParam == VK_RIGHT)
			x += 0.2;
		else if (wParam == VK_DOWN)
			y -= 0.2;
		else if (wParam == VK_UP)
			y += 0.2;
		else if (wParam == 0x52) {
			r = 1;
			g = 0;
			b = 0;
		}
		else if (wParam == 0x47) {
			r = 0;
			g = 1;
			b = 0;
		}
		else if (wParam == 0x42) {
			r = 0;
			g = 0;
			b = 1;
		}
		else if (wParam == VK_SPACE) {
			r = 1;
			g = 1;
			b = 1;
			x = 0;
			y = 0;
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

void graphic() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();	//Reset the transformation
	glTranslatef(0.5, -0.5, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.5,0.5,0.5);

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5, 0);
	glVertex2f(0, 0.5);
	glVertex2f(0.5, 0);
	glEnd();

	glLoadIdentity();
	glTranslatef(x, y, 0);
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glEnd();
}

void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
	int i = int(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch (i % 6) {
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case 5: r = v; g = p; b = q; break;
	}
}

void updateColor() {
	hue += 0.005f; // Speed of hue change
	if (hue > 1.0f) hue -= 1.0f;

	float r, g, b;
	HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
	glColor3f(r, g, b);
}

void star() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

	glRotatef(0.1, 0, 0, 1);
	updateColor();

	float centerX = 0.0f, centerY = 0.0f;
	float radiusOuter = 0.5f;
	float radiusInner = 0.2f;
	int numPoints = 5; // Five-pointed star

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(centerX, centerY); // Central anchor

	for (int i = 0; i <= numPoints * 2; ++i) {
		float angle = i * PI / numPoints;
		float radius = (i % 2 == 0) ? radiusOuter : radiusInner;
		float x = centerX + radius * cos(angle);
		float y = centerY + radius * sin(angle);
		glVertex2f(x, y);
	}

	glEnd();
}

void rotatePoint() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
	glBegin(GL_TRIANGLE_FAN);
	float angle = 0 * PI / 1;
	float radius = 0.5;
	float x = 0 + radius * cos(angle);
	float y = 0 + radius * sin(angle);
	glVertex2f(x, y);
	glEnd();
}

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	rotatePoint();

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