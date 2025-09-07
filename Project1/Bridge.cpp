#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Camera movement
float ptx = 0, pty = 0, pRy = 0, pRx = 0, pRz = 1;
float bridgeHeight = 0, bridgeSpeed = 0.5;
bool up = false;
bool carVisible = false;
float carZ = 7;
bool perspective = true;

// Step 1
BITMAP BMP;				// bitmap structure
HBITMAP hBMP = NULL;	//
GLuint texture;

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
		else if (wParam == 'B') {
			if(!carVisible)
				up = !up;
		}
		else if (wParam == 'C') {
			if (bridgeHeight == 0) {
				carVisible = !carVisible;
				carZ = 7;
			}
		}
		else if (wParam == 'P')
			perspective = !perspective;
		else if (wParam == VK_SPACE) {
			pRx = 0;
			pRy = 0;
			pRz = 1;
			bridgeHeight = 0;
			up = false;
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

GLuint loadTexture(LPCSTR filename) {

	// Take from step 1
	GLuint texture = 0;		// texture name

	// Step3: Initialize texture info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//Step4: Assign texture to polygon.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);	// take from Step 5
	return texture;
}

void cyclinder(float radius, float height) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluCylinder(quadric, radius, radius, height, 30, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void cone(float radius, float height) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	gluQuadricTexture(quadric, GL_TRUE);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluCylinder(quadric, radius, 0, height, 30, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void window(float radius, bool front) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glLineWidth(3);
	glColor3f(0.961, 0.961, 0.941);
	glPushMatrix();
	glRotatef(front ? 180 : 0, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluCylinder(quadric, radius, radius, 0.01, 30, 1);
	glColor3f(0.678, 0.847, 0.902);
	glTranslatef(0,0,0.001);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(30);
		float x = 0.079 * cosf(theta);
		float y = 0.079 * sinf(theta);
		glVertex2f(x + 0, y + 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0,-0.01);
	gluDisk(quadric, radius, radius + 0.01, 30, 1);
	glPopMatrix();

	glColor3f(0.961, 0.961, 0.941);
	glBegin(GL_QUADS);
	glVertex3f(radius, -0.005, front ? -0.01 : 0.01);
	glVertex3f(-radius, -0.005, front ? -0.01 : 0.01);
	glVertex3f(-radius, 0.005, front ? -0.01 : 0.01);
	glVertex3f(radius, 0.005, front ? -0.01 : 0.01);
	glEnd();

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glColor3f(0.961, 0.961, 0.941);
	glBegin(GL_QUADS);
	glVertex3f(radius, -0.005, front ? -0.01 : 0.01);
	glVertex3f(-radius, -0.005, front ? -0.01 : 0.01);
	glVertex3f(-radius, 0.005, front ? -0.01 : 0.01);
	glVertex3f(radius, 0.005, front ? -0.01 : 0.01);
	glEnd();
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void corner() {
	glPushMatrix();
	glScalef(0.2,1,0.2);
	glColor3f(0.784, 0.784, 0.745);
	cyclinder(0.2, 1.7);
	glTranslatef(0,1.7,0);
	texture = loadTexture("roof.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1,1,1);
	//glColor3f(0.274, 0.314, 0.392);
	cone(0.25, 0.2);
	glPopMatrix();
	glDeleteTextures(1, &texture);

}

void layer() {
	float height = 0.3;
	texture = loadTexture("stoneWall.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1,1,1);
	//glColor3f(0.627, 0.627, 0.588);
	glBegin(GL_QUADS);

	// Bottom face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0, -0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0, -0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, 0, 0.2);

	// Top face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, height, -0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, height, -0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, height, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, height, 0.2);

	// Front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0, -0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0, -0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, height, -0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, height, -0.2);

	// Back face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0, 0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, height, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, height, 0.2);

	// Left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, height, 0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, height, -0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, 0, -0.2);

	// Right face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, height, 0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, height, -0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0, -0.2);
	glEnd();
	glDeleteTextures(1, &texture);


	glPushMatrix();
	glTranslatef(0, height / 2, -0.2);
	glColor3f(0.961, 0.961, 0.941);
	window(0.08, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, height / 2, 0.2);
	glColor3f(0.961, 0.961, 0.941);
	window(0.08, false);
	glPopMatrix();
}

void cube(float width, float y) {
	glBegin(GL_QUADS);

	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.1, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 0.1, -width);

	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, y, width);

	// Left side
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, y, width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, y, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 0, -width);

	// Right side
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, 0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, y, width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, 0, -width);

	// Bottom face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 0, width);

	// Top face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, y, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, y, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, y, width);

	glEnd();
}

void quad(float width, float y, float length) {
	glBegin(GL_QUADS);

	// Front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, -length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, -length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, -length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, y, -length);

	// Back face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, y, length);

	// Left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, y, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, y, -length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 0, -length);

	// Right face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, 0, length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, y, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, -length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, 0, -length);

	// Bottom face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 0, -length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, -length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 0, length);

	// Top face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, y, -length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, y, -length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, y, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, y, length);

	glEnd();

}

void pyramid(float width, float height) {
	glBegin(GL_TRIANGLE_FAN);

	// Apex (mapped to center-top of texture)
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, height, 0);

	// Base corners
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-width, 0, -width);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(width, 0, -width);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(width, 0, width);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-width, 0, width);

	// Close the loop back to first base corner
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-width, 0, -width);

	glEnd();
}

void tower() {
	glColor3f(0.784, 0.784, 0.745);
	glPushMatrix();
	glTranslatef(-0.2, 0, -0.2);
	corner();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0, -0.2);
	corner();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0, 0.2);
	corner();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, 0, 0.2);
	corner();
	glPopMatrix();

	glPushMatrix();
	layer();
	glTranslatef(0, 0.3, 0);
	glColor3f(0.824, 0.824, 0.784);
	cube(0.22, 0.1);
	glTranslatef(0, 0.1, 0);
	layer();
	glTranslatef(0, 0.3, 0);
	glColor3f(0.824, 0.824, 0.784);
	cube(0.22, 0.1);
	glTranslatef(0, 0.1, 0);
	layer();
	glTranslatef(0, 0.3, 0);
	glColor3f(0.824, 0.824, 0.784);
	cube(0.22, 0.1);
	glTranslatef(0, 0.1, 0);
	layer();
	glTranslatef(0, 0.3, 0);
	glColor3f(0.824, 0.824, 0.784);
	cube(0.22, 0.1);
	glTranslatef(0, 0.1, 0);
	glColor3f(0.549, 0.549, 0.510);
	texture = loadTexture("stoneWall.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1, 1, 1);
	cube(0.18,0.15);
	glDeleteTextures(1, &texture);
	glTranslatef(0, 0.15, 0);
	glColor3f(0.824, 0.824, 0.784);
	cube(0.13,0.15);
	glTranslatef(0, 0.15, 0);
	glColor3f(0.274, 0.314, 0.392);

	texture = loadTexture("roof.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	pyramid(0.15, 0.2);
	glDeleteTextures(1, &texture);
	glPopMatrix();
}

void road() {
	glPushMatrix();
	glTranslatef(0, 0.2, -2.8);
	quad(0.3,0.1, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.2, 2.8);
	quad(0.3,0.1, 2);
	glPopMatrix();
}

void bridgePart(float width, float y, float length, bool left) {
	glPushMatrix();
	if (left) {
		glRotatef(180, 0, 1, 0);
	}
	glBegin(GL_QUADS);
	// Front
	glVertex3f(-width, 0, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, y, 0);
	glVertex3f(-width, y, 0);

	// Back
	glVertex3f(-width, 0, length);
	glVertex3f(width, 0, length);
	glVertex3f(width, y, length);
	glVertex3f(-width, y, length);

	// Left side
	glVertex3f(-width, 0, length);
	glVertex3f(-width, y, length);
	glVertex3f(-width, y, 0);
	glVertex3f(-width, 0, 0);

	// Right side
	glVertex3f(width, 0, length);
	glVertex3f(width, y, length);
	glVertex3f(width, y, 0);
	glVertex3f(width, 0, 0);

	// Bottom
	glVertex3f(-width, 0, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, 0, length);
	glVertex3f(-width, 0, length);

	// Top
	glVertex3f(-width, y, 0);
	glVertex3f(width, y, 0);
	glVertex3f(width, y, length);
	glVertex3f(-width, y, length);
	glEnd();

	glPopMatrix();
}

void bridge() {
	if (up) {
		bridgeHeight < 60 ? bridgeHeight += bridgeSpeed : bridgeHeight = 60;
	}
	else {
		bridgeHeight > 0 ? bridgeHeight -= bridgeSpeed : bridgeHeight = 0;
	}


	glPushMatrix();
	glTranslatef(0, 0.2, -0.8);
	glRotatef(-bridgeHeight, 1, 0, 0);
	bridgePart(0.3, 0.1, 0.8, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.2, 0.8);
	glRotatef(bridgeHeight, 1, 0, 0);
	bridgePart(0.3, 0.1, 0.8, true);
	glPopMatrix();
}

void foundation() {
	glColor3f(0.2,0.1,0.1);
	glPushMatrix();
	glTranslatef(0.5,0,1);
	quad(0.3, 0.3, 0.3);
	glTranslatef(-1,0,0);
	quad(0.3, 0.3, 0.3);
	glTranslatef(0,0,-2);
	quad(0.3, 0.3, 0.3);
	glTranslatef(1,0,0);
	quad(0.3, 0.3, 0.3);
	glPopMatrix();
}

void pillars() {
	glPushMatrix();
	glTranslatef(0.5, 0, 1);
	tower();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0, 1);
	tower();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0, -1);
	tower();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0, -1);
	tower();
	glPopMatrix();
}

void car() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(0,0.58,0);
	glRotatef(90,0,1,0);
	glScalef(0.5,0.5,0.5);
	glBegin(GL_POLYGON);
	glVertex3f(-0.3, 0.1, -0.05);
	glVertex3f(0.3, 0.1, -0.05);
	glVertex3f(0.3, 0.3, -0.05);
	glVertex3f(0.1, 0.3, -0.05);
	glVertex3f(0.1, 0.4, -0.05);
	glVertex3f(-0.1, 0.4, -0.05);
	glVertex3f(-0.1, 0.3, -0.05);
	glVertex3f(-0.2, 0.3, -0.05);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.3, 0.1, 0.05);
	glVertex3f(0.3, 0.1, 0.05);
	glVertex3f(0.3, 0.3, 0.05);
	glVertex3f(0.1, 0.3, 0.05);
	glVertex3f(0.1, 0.4, 0.05);
	glVertex3f(-0.1, 0.4, 0.05);
	glVertex3f(-0.1, 0.3, 0.05);
	glVertex3f(-0.2, 0.3, 0.05);
	glEnd();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-0.15,0.1,-0.06);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluCylinder(quadric, 0.05, 0.05, 0.12, 30, 1);

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(30);
		float x = 0.05 * cosf(theta);
		float y = 0.05 * sinf(theta);
		glVertex2f(x + 0, y + 0);
	}
	glEnd();

	glTranslatef(0,0,0.12);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(30);
		float x = 0.05 * cosf(theta);
		float y = 0.05 * sinf(theta);
		glVertex2f(x + 0, y + 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.1, -0.06);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluCylinder(quadric, 0.05, 0.05, 0.12, 30, 1);

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(30);
		float x = 0.05 * cosf(theta);
		float y = 0.05 * sinf(theta);
		glVertex2f(x + 0, y + 0);
	}
	glEnd();

	glTranslatef(0, 0, 0.12);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 30; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(30);
		float x = 0.05 * cosf(theta);
		float y = 0.05 * sinf(theta);
		glVertex2f(x + 0, y + 0);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void cars() {
	glPushMatrix();
	glColor3f(1, 0, 0);
	car();
	glTranslatef(0,0,-0.5);
	glColor3f(0, 1, 0);
	car();
	glTranslatef(0,0,-0.5);
	glColor3f(0, 0, 1);
	car();
	glPopMatrix();
}

void carAnimation() {
	carZ -= 0.1;
	glPushMatrix();
	glTranslatef(-0.15, 0, carZ);
	cars();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0, -carZ);
	glRotatef(180, 0, 1, 0);
	cars();
	glPopMatrix();
}

void project() {
	foundation();
	glPushMatrix();
	glTranslatef(0,0.3,0);
	pillars();
	glColor3f(0.196, 0.196, 0.196);
	road();
	bridge();
	glPopMatrix();
	if (carVisible) {
		carAnimation();
		if (carZ < -6) {
			carVisible = false;
			carZ = 7;
		}
	}
}

void projection() {
	if (perspective) {
		gluPerspective(100.0, 1 / 1, 0.1, 10.0);
		gluLookAt(pRy, pRx, -pRz,   // camera position
			0.0, 0.0, 0.0,     // look at origin
			0.0, 1.0, 0.0);
	}
	else {
		glOrtho(-3.0, 3.0, -3.0, 3.0, -10.0, 10.0);
		gluLookAt(-1, 1.0, 1,   // eye position (further away)
			0.0, 0.0, 0.0,   // look at the origin
			0.0, 1.0, 0.0);  // up vector
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projection();

	project();
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