#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>


#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

double translateZ = -5;
double tSpeed = 0.2;
double position = 5;
float hue = 0.0f;
int view = 2;
bool isOrtho = false;
double ONear = -10, OFar = 10, PNear = 1, PFar = 10;
double ptx = 0, pty = 0;		//translate x and y for projection with tSpeed
double tx = 0;
double pRy = 0, prSpeed = 2;	//Rotate Y with prSpeed for projection


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
		else if (wParam == VK_UP) {
			if (isOrtho) {
				if (translateZ < OFar - 0.6)
					translateZ += tSpeed;
			}else {
				if (position > PNear + 0.4 /*tz < PNear - radius*/) {
					position -= tSpeed;
					translateZ += tSpeed;
				}
			}
		}
		else if (wParam == VK_DOWN) {
			if (isOrtho) {
				if (translateZ > ONear - 0.6) {
					translateZ -= tSpeed;
				}
			} else {
				if (position < PFar + 0.2 /*tz > PNear - PFar*/) {
					position += tSpeed;
					translateZ -= tSpeed;
				}
			}
		}
		else if (wParam == VK_LEFT) {
			tx -= tSpeed;
		}
		else if (wParam == VK_RIGHT) {
			tx += tSpeed;
		}
		else if (wParam == 'O') {
			isOrtho = true;
			translateZ = 0;
		}
		else if (wParam == 'P') {
			isOrtho = false;
			translateZ = -5;
			position = 5;
		}
		else if (wParam == 'A') {
			ptx -= tSpeed;
		}
		else if (wParam == 'D') {
			ptx += tSpeed;
		}
		else if (wParam == 'W') {
			pty += tSpeed;
		}
		else if (wParam == 'S') {
			pty += tSpeed;
		}
		else if (wParam == 'Q') {
			pRy += prSpeed;
		}
		else if (wParam == 'E') {
			pRy -= prSpeed;
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
	hue += 0.0001f; // Speed of hue change
	if (hue > 1.0f) hue -= 1.0f;

	float r, g, b;
	HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
	glColor3f(r, g, b);
}

void sphere() {

	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_LINE);
	glLineWidth(2);
	updateColor();
	gluSphere(quadric, 0.6, 30, 30);
	gluDeleteQuadric(quadric);
}

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(ptx, pty, 0);	//Translate x and y for projection
	glRotatef(pRy, 0, 1, 0);

	if(isOrtho)
		glOrtho(-4, 4, -2.25, 2.25, ONear, OFar);
	else
		gluPerspective(20, 1.77, PNear, PFar);

	//glFrustum(-4,4,-2.25,2.25,1,10);

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	projection();

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx, 0, translateZ);*/

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glPushMatrix();
	glTranslatef(tx, translateZ, 0);
	sphere();
	glPopMatrix();
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
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
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