#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>
#include <chrono>
#include <string>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

auto lastTime = std::chrono::high_resolution_clock::now();

//Menu
int menu = 1;

//Style
GLenum drawstyle = GLU_FILL;

//Camera movement
float ptx = 0, pty = 0, pRy = 0, pRx = 0, pRz = 1;
bool perspective = true;

//Object movement
float tRx = 0, tRy = 0;

//Arm movement
bool leftArmMovement = true;
bool smallArmMovement = false;
float forearmMaxHeight = 135, forearmMinHeight = 0;
float bicepMaxHeight = 180, bicepMinHeight = -45;
float leftBicep = 0, rightBicep = 0, rightForearm = 45, leftForearm = 45;
float armSpeed = 5;

//Finger Movement
float part = -10;
float fingerSpeed = 1;

//Walking Animation
float leftLegAngle = 0.0f;
float rightLegAngle = 0.0f;
float leftArmAngle = 0.0f;
float rightArmAngle = 0.0f;

bool isWalking = false;
bool leftLegForward = true;
bool rightLegForward = false; // opposite phase
bool leftArmForward = false;
bool rightArmForward = true;

float stepSpeed = 0.1f;   // how fast the angle changes
float maxLegAngle = 10.0f; // max swing
float maxArmAngle = 20.0f;

//eyes


//Light
float x = 0.7, y = -0.5, z = -1;
bool isLightOn = false;

float getDeltaTime() {
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = currentTime - lastTime;
	lastTime = currentTime;
	return delta.count(); // seconds
}

float update(float angle, float speed) {
	float dt = getDeltaTime();
	return angle + speed * dt;
}

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

		//Camera and light movement
		switch (menu) {
		case 1:
			if (wParam == 'A')
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
			break;
		case 2:
			if (wParam == 'A')
				x += 0.1;
			else if (wParam == 'D')
				x -= 0.1;
			else if (wParam == 'W')
				y += 0.1;
			else if (wParam == 'S')
				y -= 0.1;
			else if (wParam == 'E')
				z += 0.1;
			else if (wParam == 'Q')
				z -= 0.1;
			break;
		default:
			break;
		}

		//Menu selection
		if (wParam == 0x31)
			menu = 1;
		else if (wParam == 0x32)
			menu = 2;
		else if (wParam == 0x33)
			menu = 3;

		//Body movement
		if (wParam == 'P')
			leftArmMovement = !leftArmMovement;
		else if (wParam == 'M') {
			isWalking = !isWalking;
			leftForearm = 45;
			rightForearm = 45;
			leftBicep = 0;
			rightBicep = 0;
		}

		//Finger movement
		if (wParam == 'U')
			part > -80 ? part -= fingerSpeed : part = -80;
		else if (wParam == 'I')
			part < 0 ? part += fingerSpeed : part = 0;

		else if (wParam == VK_UP)
			switch (smallArmMovement) {
			case false:
				leftArmMovement ? leftBicep < bicepMaxHeight ? leftBicep += armSpeed : leftBicep = bicepMaxHeight : rightBicep < bicepMaxHeight ? rightBicep += armSpeed : rightBicep = bicepMaxHeight;
				break;
			case true:
				leftArmMovement ? leftForearm < forearmMaxHeight ? leftForearm += armSpeed : leftForearm = forearmMaxHeight : rightForearm < forearmMaxHeight ? rightForearm += armSpeed : rightForearm = forearmMaxHeight;
				break;
			}
		else if (wParam == VK_DOWN)
			switch (smallArmMovement) {
			case false:
				leftArmMovement ? leftBicep > bicepMinHeight ? leftBicep -= armSpeed : leftBicep = bicepMinHeight : rightBicep > bicepMinHeight ? rightBicep -= armSpeed : rightBicep = bicepMinHeight;
				break;
			case true:
				leftArmMovement ? leftForearm > forearmMinHeight ? leftForearm -= armSpeed : leftForearm = forearmMinHeight : rightForearm > forearmMinHeight ? rightForearm -= armSpeed : rightForearm = forearmMinHeight;
				break;
			}
		else if (wParam == 'O') {
			smallArmMovement = !smallArmMovement;
		}
		else if (wParam == 'L') {
			isLightOn = !isLightOn;
		}
		else if (wParam == VK_SPACE) {
			pRx = 0;
			pRy = 0;
			pRz = 1;
			leftForearm = 45;
			rightForearm = 45;
			leftBicep = 0;
			rightBicep = 0;
			part = -10;
			x = 0.7;
			y = -0.5;
			z = -1;
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
	// Load BMP via GDI
	HBITMAP hBMP = (HBITMAP)LoadImageA(GetModuleHandleA(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	BITMAP bmp;
	GetObject(hBMP, sizeof(BITMAP), &bmp);

	const int bpp = bmp.bmBitsPixel; // bits per pixel (24 or 32 expected)
	GLenum srcFormat = (bpp == 32) ? GL_BGRA_EXT : GL_BGR_EXT;
	GLint  internal = (bpp == 32) ? GL_RGBA : GL_RGB;

	// Safest alignment for arbitrary row sizes
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Good defaults
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internal,
		bmp.bmWidth, bmp.bmHeight, 0,
		srcFormat, GL_UNSIGNED_BYTE, bmp.bmBits);

	DeleteObject(hBMP);
	return tex;
}

void drawSphere(float radius) {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, drawstyle);
	gluSphere(quadric, radius, 30, 30);
	gluDeleteQuadric(quadric);
}

void drawCyclinder(float topRadius, float bottomRadius, float height, LPCSTR texture = "") {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	GLuint loadedTexture = loadTexture(texture);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, loadedTexture);
	gluQuadricTexture(quadric, loadedTexture);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, topRadius, bottomRadius, height, 10, 3);
	glDeleteTextures(1, &loadedTexture);
}

void drawCircle(float radius, float angleDeg = 360.0f, bool fill = true) {
	int segments = 30; // higher value = smoother circle

	if (fill) {
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f, 0.0f); // center point of fan
		for (int i = 0; i <= segments; i++) {
			// convert to radians
			float theta = (angleDeg * i / segments) * 3.1415926f / 180.0f;
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			glVertex2f(x, y);
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= segments; i++) {
			float theta = (angleDeg * i / segments) * 3.1415926f / 180.0f;
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			glVertex2f(x, y);
		}
		glEnd();
	}
}


//Arm Parts
void fingerPart() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(0.1, 0.1, 0.5);
	drawCyclinder(0.2, 0.2, 0.1);
	glPopMatrix();
}
void fingerTip() {


	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	drawSphere(0.02);
	glPopMatrix();

}
void finger(bool left) {

	glPushMatrix();
	left ? glRotatef(part > -45 ? part : -45, 1, 0, 0) : glRotatef(-45, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(part > -70 ? part : -70, 1, 0, 0) : glRotatef(-70, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(part > -80 ? part : -80, 1, 0, 0) : glRotatef(-80, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();
	glPopMatrix();




}
void thumb(bool left) {

	glPushMatrix();
	left ? glRotatef(-20, 1, 0, 0) : glRotatef(-70, 1, 0, 0);
	glScalef(0.92, 0.9, 0.92);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	left ? glRotatef(-10, 0, 0, 1) : glRotatef(30, 1, 0, 0);
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();
	glPopMatrix();
}
void pinky(bool left) {
	glPushMatrix();
	glScalef(0.78, 0.7, 0.78); //Adjust the size of the finger
	finger(left);
	glPopMatrix();
}
void ring(bool left) {
	glPushMatrix();
	glScalef(0.95, 0.95, 0.95);
	finger(left);
	glPopMatrix();
}
void index(bool left) {
	glPushMatrix();
	glScalef(0.92, 0.9, 0.92);
	finger(left);
	glPopMatrix();
}
void palm() {
	glPushMatrix();
	glTranslatef(0, -0.15, 0);
	glScalef(0.4, 0.8, 0.16);
	glRotatef(-90, 1, 0, 0);
	drawCyclinder(0.18, 0.18, 0.07);
	glTranslatef(0, 0, 0.07);
	drawCyclinder(0.18, 0.13, 0.08);
	glTranslatef(0, 0, -0.07);
	drawCyclinder(0.18, 0, 0.1);
	glPopMatrix();
}
void fingers(bool left) {

	glPushMatrix();
	glScalef(0.8, 0.6, 1);
	//Pinky finger
	glPushMatrix();
	if (left) {
		glTranslatef(-0.08, 0.03, 0.01);
	}
	else {
		glTranslatef(0.08, 0.03, 0.01);
	}

	pinky(left);
	glPopMatrix();

	//Ring finger
	glPushMatrix();
	if (left) {
		glTranslatef(-0.04, 0.01, 0);
	}
	else {
		glTranslatef(0.04, 0.01, 0);
	}
	ring(left);
	glPopMatrix();

	//Middle finger
	finger(left);

	//Index finger
	glPushMatrix();
	if (left) {
		glTranslatef(0.04, 0.01, 0);
	}
	else {
		glTranslatef(-0.04, 0.01, 0);
	}
	index(left);
	glPopMatrix();

	//Thumb
	glPushMatrix();
	if (left) {
		glTranslatef(0.08, 0.05, 0.02);
	}
	else {
		glTranslatef(-0.08, 0.06, 0.02);
	}
	thumb(left);
	glPopMatrix();
	glPopMatrix();

}

void weapon() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	//Handle
	glColor3f(0.25, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.01, 0.01, 1.15, 30, 1);
	glPopMatrix();

	//Connector
	glColor3f(0.8, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.18, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.02, 0.03, 0.1, 10, 1);
	glPopMatrix();

	//Tip
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.01, 0.05, 0.15, 4, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0, 0.3, 4, 1);
	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);

	//Outline
	//Tip
	glLineWidth(3);
	glColor3f(0, 0.67, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.01, 0.05, 0.15, 4, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluCylinder(quadric, 0.05, 0, 0.3, 4, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void hand(bool left) {
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	palm();
	glPopMatrix();

	fingers(left);

}
void sleeve(bool left) {
	glPushMatrix();
	glTranslatef(0, 0.5, 0.01);
	if (!left) {
		glRotatef(rightBicep, 1, 0, 0); //Bicep movement
	}
	else {
		glRotatef(leftBicep, 1, 0, 0); //Bicep movement
	}
	glRotatef(90, 1, 0, 0);
	glRotatef(left ? -15 : 15, 0, 1, 0);
	glScalef(0.2, 0.15, 0.5);
	drawCyclinder(0.4, 0.7, 0.4);
	glRotatef(180, 1, 0, 0);
	drawCyclinder(0.4, 0.4, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.4, 0.3, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.3, 0.2, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.2, 0.0, 0.02);
	glPopMatrix();
}


void arm(bool left) {

	glColor3f(0.9, 0.9, 0.9);
	sleeve(left);

	//Arm
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.5, 0.01);
	glRotatef(90, 1, 0, 0);

	if (!left) {
		glRotatef(rightBicep, 1, 0, 0); //Bicep movement
	}
	else {
		glRotatef(leftBicep, 1, 0, 0); //Bicep movement
	}
	glRotatef(left ? -10 : 10, 0, 1, 0);
	drawCyclinder(0.06, 0.06, 0.2);
	glTranslatef(0, 0, 0.2);
	drawSphere(0.06);
	if (!left) {
		glRotatef(rightForearm, 1, 0, 0); //Forearm movement
	}
	else {
		glRotatef(leftForearm, 1, 0, 0); //Forearm movement
	}
	glRotatef(left ? 8 : -8, 0, 1, 0);
	drawCyclinder(0.06, 0.03, 0.18);

	glTranslatef(0, 0, 0.18);
	drawSphere(0.03);
	glTranslatef(0, 0, 0.08);
	//glRotatef(left ? -15 : 15, 0, 0, 1);
	//glRotatef(left ? 70 : -90, 0, 1, 0);
	glRotatef(left ? -90 : 90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	hand(left);

	if (!left) {
		glTranslatef(-0.45, 0.01, 0.03);
		glScalef(1.3, 1.3, 1.3);
		glRotatef(92, 0, 0, 1);
		weapon();
	}
	glPopMatrix();
}


void arms() {
	glPushMatrix();
	glTranslatef(-0.3, 0, 0);
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glScalef(1, 1, 0.9);
	drawSphere(0.05);
	glPopMatrix();
	glRotatef(leftArmAngle, 1, 0, 0);
	glTranslatef(0, -0.5, 0);
	arm(true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0, 0);
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glScalef(1, 1, 0.9);
	drawSphere(0.05);
	glPopMatrix();
	glRotatef(rightArmAngle, 1, 0, 0);
	glTranslatef(0, -0.5, 0);
	arm(false);
	glPopMatrix();
}

//Body
void torso() {
	//Chest
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.25, 0.23, 0.25);
	glPopMatrix();

	//Waits
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.23, 0.23, 0.05);
	glPopMatrix();

	//Lower
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0, -0.35, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.23, 0.35, 0.45, "Fabric.bmp");
	glPopMatrix();
}

void hoodie() {
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.25, 0.26, 0.04);
	glTranslatef(0, 0, 0.04);
	drawCyclinder(0.26, 0.28, 0.01);
	glTranslatef(0, 0, 0.01);
	drawCyclinder(0.28, 0.3, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.3, 0.3, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.3, 0.28, 0.02);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 0, -0.02);
	drawCyclinder(0.28, 0.2, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.2, 0.15, 0.02);
	glPopMatrix();
}

void body() {
	torso();
	hoodie();
}

void foot() {
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();

	//Leg
	glPushMatrix();
	glTranslatef(0, -0.6, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.08, 0.08, 0.3, 10, 1);
	glPopMatrix();

	//Shoe sole
	glPushMatrix();
	glTranslatef(0, -0.87, -0.05);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1.4, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.08, 0.08, 0.04, 10, 1);
	glPopMatrix();


	//Shoe Tip
	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(0, 1, 0, 0);
	glScalef(1.2, 0.5, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0.07, 0.1, 10, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(180, 1, 0, 0);
	glScalef(1, 0.5, 1);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.06, 0, 0.02, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void updateWalk() {
	rightForearm = 45;
	leftForearm = 45;

	// ---- Left leg ----
	if (leftLegForward) {
		leftLegAngle += stepSpeed;
		if (leftLegAngle >= maxLegAngle) leftLegForward = false;
	}
	else {
		leftLegAngle -= stepSpeed;
		if (leftLegAngle <= -maxLegAngle) leftLegForward = true;
	}

	// ---- Right leg (opposite) ----
	if (rightLegForward) {
		rightLegAngle += stepSpeed;
		if (rightLegAngle >= maxLegAngle) rightLegForward = false;
	}
	else {
		rightLegAngle -= stepSpeed;
		if (rightLegAngle <= -maxLegAngle) rightLegForward = true;
	}

	// ---- Left arm (opposite to left leg) ----
	if (leftArmForward) {
		leftArmAngle += stepSpeed;
		if (leftArmAngle >= maxArmAngle) leftArmForward = false;
	}
	else {
		leftArmAngle -= stepSpeed;
		if (leftArmAngle <= -maxArmAngle) leftArmForward = true;
	}

	// ---- Right arm (opposite to right leg) ----
	if (rightArmForward) {
		rightArmAngle += stepSpeed;
		if (rightArmAngle >= maxArmAngle) rightArmForward = false;
	}
	else {
		rightArmAngle -= stepSpeed;
		if (rightArmAngle <= -maxArmAngle) rightArmForward = true;
	}
}

void feet() {
	glColor3f(0.3, 0.1, 0.1);
	glPushMatrix();
	glTranslatef(-0.1, 0, 0);
	glRotatef(leftLegAngle, 1, 0, 0);
	foot();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0, 0);
	glRotatef(rightLegAngle, 1, 0, 0);
	foot();
	glPopMatrix();
}

void neck() {
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(1.2, 0.9, 1);
	//drawCircle(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.02, 0);
	glRotatef(-90, 1, 0, 0);
	glPushMatrix();
	glScalef(1.3, 0.7, 1);
	drawCircle(0.2);
	glRotatef(180, 1, 0, 0);
	drawCyclinder(0.21, 0.2, 0.02);
	glPopMatrix();
	drawCyclinder(0.18, 0.1, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.1, 0.08, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.08, 0.08, 0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.08, 0.1, 0.02);
	glPopMatrix();
}

void eye() {
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0.1, 0.25, -0.26);
	glRotatef(180, 0, 0, 1);
	glRotatef(12, 0, 1, 0);
	glScalef(0.2, 0.25, 1);
	drawCircle(0.2);
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.05, -0.01);
	glScalef(0.7, 0.7, 1);
	drawCircle(0.2);
	glLineWidth(5);
	glTranslatef(0.2, -0.15, -0.01);
	glRotatef(-80, 0, 0, 1);
	glScalef(1.5, 3, 1);
	drawCircle(0.2, -45, false);
	glPopMatrix();

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(-0.1, 0.25, -0.26);
	glRotatef(180, 0, 0, 1);
	glRotatef(-12, 0, 1, 0);
	glScalef(0.2, 0.25, 1);
	drawCircle(0.2);
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.05, -0.01);
	glScalef(0.7, 0.7, 1);
	drawCircle(0.2);
	glLineWidth(5);
	glTranslatef(-0.2, -0.15, -0.01);
	glRotatef(80, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glScalef(1.5, 3, 1);
	drawCircle(0.2, -45, false);
	glPopMatrix();
}

void cheeks() {
	glColor3f(0.8, 0.4, 0.6);
	glPushMatrix();
	glTranslatef(0.12, 0.18, -0.2);
	glRotatef(-15, 0, 1, 0);
	glScalef(0.5, 0.3, 1);
	drawCircle(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.12, 0.18, -0.2);
	glRotatef(15, 0, 1, 0);
	glScalef(0.5, 0.3, 1);
	drawCircle(0.1);
	glPopMatrix();
}

void mouth() {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.14, -0.23);
	glScalef(0.6, 0.1, 1);
	drawCircle(0.05, -180, false);
	glPopMatrix();
}

///
/// 
///

void drawEllipsoid(GLUquadric* q, float width, float height, float depth) {
	if (!q) return;
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);

	glScalef(width * 0.5f, height * 0.5f, depth * 0.5f);
	glColor3f(0.98f, 0.98f, 0.98f);        // white sclera
	gluSphere(q, 1.0, 32, 24);
	glPopMatrix();
}

void drawEye(GLUquadric* q, float width, float height, float depth) {
	if (!q) return;

	// --- 1) Eyeball (ellipsoid sclera) ---
	drawEllipsoid(q, width, height, depth);

	// scale into unit-sphere space for the flat layers
	glPushMatrix();
	glScalef(width * 0.5f, height * 0.5f, depth * 0.5f);
	glTranslatef(0, -0.4, 0);

	// constants for layering
	const float irisR = 0.45f;  // relative to unit radius
	const float pupilR = 0.18f;
	const float epsIris = 0.002f;
	const float epsPup = 0.003f;
	const float epsHi = 0.004f;

	// --- 2) Iris ---
	glPushMatrix();
	glTranslatef(0, 0, 1.0f + epsIris);
	glScalef(1.0f, 1.6f, 1.0f);
	// Outer rim (dark blue ring)
	glColor3f(0.12f, 0.35f, 0.60f);
	gluDisk(q, irisR * 0.85f, irisR, 48, 1);

	// Inner fill (lighter blue)
	glColor3f(0.25f, 0.65f, 0.90f);
	gluDisk(q, 0.0, irisR * 0.85f, 48, 1);
	glPopMatrix();

	// --- 3) Pupil ---
	glPushMatrix();
	glTranslatef(0, 0, 1.0f + epsPup);
	glColor3f(0.05f, 0.05f, 0.07f);        // black
	gluDisk(q, 0.0, pupilR, 36, 1);
	glPopMatrix();

	// --- 4) Highlight ---
	glPushMatrix();
	glTranslatef(irisR * 0.22f, irisR * 0.22f, 1.0f + epsHi);
	glColor3f(1.0f, 1.0f, 1.0f);           // shiny spot
	gluDisk(q, 0.0, irisR * 0.10f, 18, 1);
	glPopMatrix();

	glPopMatrix(); // back from unit-sphere space
}



void eyes2() {
	GLUquadricObj* quadric = gluNewQuadric();

	glPushMatrix();
	glTranslatef(0.1f, 0.28f, -0.2f);
	glRotatef(180, 0, 1, 0);

	// LEFT eye
	glPushMatrix();
	glRotatef(6, 0, 0, 1);
	drawEye(quadric, 0.14f, 0.10f, 0.16f);
	glPopMatrix();

	// RIGHT eye 
	glPushMatrix();
	glTranslatef(0.20f, 0.0f, 0.0f); // adjust spacing
	glRotatef(-6, 0, 0, 1);
	drawEye(quadric, 0.14f, 0.10f, 0.16f);
	glPopMatrix();

	glPopMatrix();
}


///
///
/// 

void drawRectPrism(float halfW, float depth, float thick) {
	float xL = -halfW, xR = halfW, yT = +thick * 0.5f, yB = -thick * 0.5f, z0 = 0.0f, z1 = -depth;
	glBegin(GL_QUADS);
	// top
	glVertex3f(xL, yT, z0); glVertex3f(xR, yT, z0); glVertex3f(xR, yT, z1); glVertex3f(xL, yT, z1);
	// bottom
	glVertex3f(xR, yB, z0); glVertex3f(xL, yB, z0); glVertex3f(xL, yB, z1); glVertex3f(xR, yB, z1);
	// front
	glVertex3f(xL, yB, z0); glVertex3f(xR, yB, z0); glVertex3f(xR, yT, z0); glVertex3f(xL, yT, z0);
	// back
	glVertex3f(xL, yB, z1); glVertex3f(xL, yT, z1); glVertex3f(xR, yT, z1); glVertex3f(xR, yB, z1);
	// left
	glVertex3f(xL, yB, z0); glVertex3f(xL, yT, z0); glVertex3f(xL, yT, z1); glVertex3f(xL, yB, z1);
	// right
	glVertex3f(xR, yB, z0); glVertex3f(xR, yB, z1); glVertex3f(xR, yT, z1); glVertex3f(xR, yT, z0);
	glEnd();
}

// Triangular prism (isosceles in XZ, extruded along Y)
void drawTriPrism(float halfW, float depth, float thick) {
	float yT = +thick * 0.5f, yB = -thick * 0.5f, zBase = 0.0f, zTip = -depth;
	// top & bottom triangles
	glBegin(GL_TRIANGLES);
	glVertex3f(-halfW, yT, zBase); glVertex3f(halfW, yT, zBase); glVertex3f(0, yT, zTip);
	glVertex3f(-halfW, yB, zBase); glVertex3f(0, yB, zTip);      glVertex3f(halfW, yB, zBase);
	glEnd();
	// side faces
	glBegin(GL_QUADS);
	glVertex3f(-halfW, yB, zBase); glVertex3f(halfW, yB, zBase); glVertex3f(halfW, yT, zBase); glVertex3f(-halfW, yT, zBase);
	glVertex3f(-halfW, yB, zBase); glVertex3f(-halfW, yT, zBase); glVertex3f(0, yT, zTip);     glVertex3f(0, yB, zTip);
	glVertex3f(halfW, yB, zBase);  glVertex3f(0, yB, zTip);       glVertex3f(0, yT, zTip);      glVertex3f(halfW, yT, zBase);
	glEnd();
}

void hairMiddlePart3D(float halfW = 0.05f, float depth = 0.05f, float thick = 0.02f) {
	drawRectPrism(halfW, depth, thick);
}

// 3D strand: identical transforms to your hairStrand(), just volumetric
void hairStrand3D(int length,
	float widthScaleX = 0.2f,  // you used glScalef(0.2,1,1)
	float segDepth = 0.05f,
	float thickness = 0.02f)
{
	glPushMatrix();
	glScalef(widthScaleX, 1.0f, 1.0f);

	// front cap wedge (your first triangle)
	drawTriPrism(/*halfW*/0.05f, /*depth*/segDepth, /*thick*/thickness);

	glPushMatrix();
	glTranslatef(0, 0, -segDepth);
	glScalef(1, 1, 0.73f);
	hairMiddlePart3D(0.05f, segDepth, thickness);

	for (int x = 0; x < length; ++x) {
		glTranslatef(0, 0, -segDepth);
		glRotatef((x < 11) ? (-4 - x) : (-2), 1, 0, 0); // same curl logic
		hairMiddlePart3D(0.05f, segDepth, thickness);
	}

	// tip (your last triangle)
	glTranslatef(0, 0, -segDepth);
	glRotatef(-10, 1, 0, 0);
	drawTriPrism(/*halfW*/0.05f, /*depth*/segDepth, /*thick*/thickness);
	glPopMatrix();

	glPopMatrix();
}

/// <summary>
/// 
/// </summary>

void backHair() {
	// 8 pieces
	for (int x = 0; x < 5; ++x) {
		hairStrand3D(18);
		glRotatef(2, 0, 1, 0);
	}

	// 2 pieces (duplicated once, like your original pattern)
	for (int x = 0; x < 1; ++x) {
		hairStrand3D(18);
		glRotatef(2, 0, 1, 0);
		hairStrand3D(18);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 3; ++x) {
		hairStrand3D(18);
		glRotatef(2, 0, 1, 0);
	}

	// another 2 pieces
	for (int x = 0; x < 1; ++x) {
		hairStrand3D(18);
		glRotatef(2, 0, 1, 0);
		hairStrand3D(17);
		glRotatef(2, 0, 1, 0);
	}
}

void hair() {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.67f, 0);        // same anchor as your code
	glScalef(1.0f, 1.0f, 1.0f);

	// FRONT / SIDES
	for (int x = 0; x < 3; ++x) { hairStrand3D(12); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(11); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 3; ++x) { hairStrand3D(10); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(11); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(10); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(11); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(12); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(13); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(11); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(12); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(13); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 3; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 3; ++x) { hairStrand3D(17); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 3; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 5; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 4; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(17); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(17); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(18); glRotatef(2, 0, 1, 0); }
	for (int i = 0; i < 4; ++i) backHair();
	for (int x = 0; x < 2; ++x) { hairStrand3D(18); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(17); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 4; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(13); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(16); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 4; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(13); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(15); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(14); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(13); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 2; ++x) { hairStrand3D(12); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 8; ++x) { hairStrand3D(10); glRotatef(2, 0, 1, 0); }
	for (int x = 0; x < 6; ++x) { hairStrand3D(11); glRotatef(2, 0, 1, 0); }

	glPopMatrix();
}

///
/// 
///

void ear(bool left) { //Neko ears
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0, 0.5, -0.25);
	glRotatef(left ? 40 : -40, 0, 0, 1);
	glScalef(0.9, 0.9, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(-0.1, 0, 0);
	glEnd();

	glColor3f(0.8, 0.4, 0.4);
	glPushMatrix();
	glTranslatef(0, 0, -0.001);
	glScalef(0.9, 0.7, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(-0.1, 0, 0);
	glEnd();
	glPopMatrix();

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0, 0.4);
	glVertex3f(-0.1, 0, 0);
	glEnd();
	glPopMatrix();
}

void ears() {
	glPushMatrix();
	glTranslatef(-0.15, 0, 0);
	ear(true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0, 0);
	ear(false);
	glPopMatrix();
}

void head() {
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	glScalef(1, 1, 0.9);
	glRotatef(-90, 1, 0, 0);
	drawCyclinder(0.25, 0.27, 0.1);
	glTranslatef(0, 0, 0.1);
	drawCyclinder(0.27, 0.3, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.3, 0.32, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.32, 0.33, 0.1);
	glTranslatef(0, 0, 0.1);
	drawCyclinder(0.33, 0.3, 0.1);
	glTranslatef(0, 0, 0.1);
	drawCyclinder(0.3, 0.25, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.25, 0.15, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.15, 0, 0.02);
	glPopMatrix();

	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.13, 0);
	glScalef(1, 0.3, 0.9);
	drawSphere(0.25);
	glPopMatrix();

	eyes2();

	cheeks();

	mouth();

	hair();
	//drawHair();

	ears();
}

void lighting() {

	if (isLightOn) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT1);
	}


	// White diffuse + ambient + specular light
	GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8, 0.8 };
	GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 0.8f }; // soft white ambient
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPos[] = { x, y, z, 1.0f };         // point light

	GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white highlight
	GLfloat matShininess[] = { 32.0f };

	// Let glColor3f() control the material color
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Assign light properties
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	//Light Source
	GLUquadricObj* quadric = NULL;
	quadric = gluNewQuadric();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x, y, z);
	gluQuadricDrawStyle(quadric, drawstyle);
	gluCylinder(quadric, 0.05, 0.05, 0.05, 10, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

void projection() {
	if (perspective) {
		gluPerspective(100.0, 1920.0 / 1080.0, 0.1, 10.0);
	}
	else {
		glOrtho(-5.0, 5.0, -5.0, 5.0, 0.1, 100.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pRy, pRx, -pRz,   // camera position
		0.0, 0.0, 0.0,     // look at origin
		0.0, 1.0, 0.0);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.7, 0.2, 0.2, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	projection();
	lighting();

	if (isWalking)
		updateWalk();
	else {
		leftArmAngle = 0;
		leftLegAngle = 0;
		rightArmAngle = 0;
		rightLegAngle = 0;
	}

	neck();
	head();
	arms();
	body();
	feet();
	glPushMatrix();
	glTranslatef(-1, 0, 0);
	weapon();
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
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
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