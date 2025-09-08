#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "Practical 6"

float ambL[3] = { 1.0, 1.0, 1.0 };		//red color amb light
float posA[3] = { 0.0, 0.8, 0.0 };		//amb light position UP (0.0, 0.8, 0.0)
float difL[3] = { 1.0, 1.0, 1.0 };		//green color diffuse light 
float posB[3] = { 0.8, 0.0, 0.0 };		//dif light position RIGHT (0.8, 0.0, 0.0)
float ambM[3] = { 0.0, 0.0, 1.0 };		//blue color amb material
float difM[3] = { 0.0, 0.0, 1.0 };		//blue color dif material
float x = 0, y = 0, z = 0, moveSpeed = 0.1; //light move speed
float posD[3] = { x, y, z };
bool isLightOn = false;					//is light on?
float rotAngle = 0.0f, rotSpeed = 0.0f;
const float kSpin = 0.6f;

bool drawSphereNow = true;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == 'W')
			y += moveSpeed;
		else if (wParam == 'S')
			y -= moveSpeed;
		else if (wParam == 'A')
			x -= moveSpeed;
		else if (wParam == 'D')
			x += moveSpeed;
		else if (wParam == 'E')
			z += moveSpeed;
		else if (wParam == 'Q')
			z -= moveSpeed;
		else if (wParam == VK_UP) {
			rotSpeed += 0.1f;
		}
		else if (wParam == VK_DOWN) {
			rotSpeed -= 0.1f;
		}
		else if (wParam == VK_SPACE)
			isLightOn = !isLightOn;
		else if (wParam == 'O')
			drawSphereNow = true;
		else if (wParam == 'P')
			drawSphereNow = false;
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

void drawSphere(double r) {
	GLUquadricObj* sphere = NULL;			//create quadric obj pointer
	sphere = gluNewQuadric();				//create the quadric obj in the memory
	gluQuadricDrawStyle(sphere, GLU_FILL);	//set to line draw style
	gluSphere(sphere, r, 30, 30);			//draw sphere
	gluDeleteQuadric(sphere);				//delete the quardric
}

void  drawPyramid() {
	//Front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.2f, 0.8f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glVertex3f(0.0f, 0.25f, -0.25f);
	glEnd();

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.2f, 0.8f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, -0.5f);
	glVertex3f(0.0f, 0.25f, -0.25f);
	glEnd();

	// Back
	glNormal3f(0.0f, 0.0f, -1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.2f, 0.8f);
	glVertex3f(0.25f, -0.25f, -0.5f);   // right-back base
	glVertex3f(-0.25f, -0.25f, -0.5f);   // left-back base
	glVertex3f(0.0f, 0.25f, -0.25f);  // apex
	glEnd();

	// Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.2f, 0.8f);
	glVertex3f(-0.25f, -0.25f, -0.5f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glVertex3f(0.0f, 0.25f, -0.25f);
	glEnd();

	// Base (quad)
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glColor3f(1.0f, 0.2f, 0.8f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, -0.5f);
	glVertex3f(-0.25f, -0.25f, -0.5f);
	glEnd();
}

void lightning() {
	posD[0] = x; posD[1] = y; posD[2] = z;

	if (isLightOn) glEnable(GL_LIGHTING);
	else           glDisable(GL_LIGHTING);

	GLfloat amb4[] = { ambL[0], ambL[1], ambL[2], 1.0f };
	GLfloat dif4[] = { difL[0], difL[1], difL[2], 1.0f };
	GLfloat pos4[] = { posD[0], posD[1], posD[2], 1.0f }; // ? uses W/A/S/D/E/Q

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb4);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif4);
	glLightfv(GL_LIGHT0, GL_POSITION, pos4);
	glEnable(GL_LIGHT0);
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      // reset so last frame’s rotation doesn’t affect the light
	rotAngle += rotSpeed;
	glPushMatrix();
	glRotatef(rotAngle, 1.0f, 1.0f, 1.0f);
	lightning();           // place the (moving) light using x,y,z
	glPopMatrix();

	// REMOVE these 6 lines from your original display() (they overwrite the moving light):
	// glLightfv(GL_LIGHT0, GL_AMBIENT, ambL);
	// glLightfv(GL_LIGHT0, GL_POSITION, posA);
	// glEnable(GL_LIGHT0);
	// glLightfv(GL_LIGHT1, GL_DIFFUSE, difL);
	// glLightfv(GL_LIGHT1, GL_POSITION, posB);
	// glEnable(GL_LIGHT1);

	// Make Up/Down effective (fixed speed, direction set by keys)
	 // rotate about all axes

	// Material (ensure diffuse is set so light is visible)
	GLfloat ambMat4[] = { ambM[0], ambM[1], ambM[2], 1.0f };
	GLfloat difMat4[] = { 1.0f,    1.0f,    1.0f,    1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambMat4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difMat4);

	if (drawSphereNow) drawSphere(0.5);
	else               drawPyramid();
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
		900, 10, 300, 300,
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