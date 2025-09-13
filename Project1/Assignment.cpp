#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <GL/glu.h>
#include <chrono>
#include <string>
#include <chrono>


#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Time
auto lastTime = std::chrono::high_resolution_clock::now();

//Menu
int menu = 1;

//Style
GLenum drawstyle = GLU_FILL;

//Camera movement
float radius = 1.0f;
float theta = 0.0f;
float phi = 0.0f;
float camX = 0, camY = 0, camZ = 0;
float ptx = 0, pty = 0, pRy = 0, pRx = 0, pRz = 0;
float oRx = 0;
bool perspective = true;

//Object movement
float tRx = 0, tRy = 0;

//Arm movement
bool leftArmMovement = true;
bool smallArmMovement = false;
float forearmMaxHeight = 135, forearmMinHeight = 0;
float bicepMaxHeight = 180, bicepMinHeight = -45;
float leftBicep = 0, rightBicep = 0, rightForearm = 45, leftForearm = 45;
float leftHandRotation = 0, rightHandRotation = 0;
float handRotationMax = 70, handRotationMin = -70;

//Finger Movement
float part = -10;
bool separateMovement = false;
float pinkyAngle = -10, ringAngle = -10, indexAngle = -10, middleAngle = -10;

//Walking Animation
float leftLegAngle = 0.0f;
float rightLegAngle = 0.0f;
float leftArmAngle = 0.0f;
float rightArmAngle = 0.0f;
float bodyPositionZ = 0;
float bodyPositionX = 0;
float bodyRotation = 0;

bool isWalking = false;
bool leftLegForward = true;
bool rightLegForward = false; // opposite phase
bool leftArmForward = false;
bool rightArmForward = true;

float maxLegAngle = 5.0f; // max swing
float maxArmAngle = 20.0f;

//Light
float x = -1, y = 0.3, z = -1.2;
bool isLightOn = true;

//Weapon Animation
float weaponPositionY = 0, weaponRotation = 0;
bool fighting = false, fightingCharging = true, fightingDone = false;
std::chrono::high_resolution_clock::time_point fightingStartTime;

//Flying Animation
bool isFlying = false, floating = false, flyingDone = false;
std::chrono::high_resolution_clock::time_point flyingStartTime;
float bodyPositionY = 0, upperBodyPositionY = 0;
float shardsRotation = 0, shardsPosition = 0, shardsSize;

//Expression
bool angry = false;

//Texture
LPCSTR clothesTextureList[] = {"", "Fabric.bmp", "Chainmail.bmp", "Banana.bmp" };
LPCSTR weaponTextureList[] = {"", "IceFrost.bmp" , "HotLava.bmp" };
LPCSTR clothesTexture = "";
LPCSTR weaponTexture = "";
LPCSTR hairTexture = "Hair.bmp";
int clothesTextureIndex = 0, weaponTextureIndex = 0;

//Animation Speed
float animationSpeed = 10; //Smaller number faster animation
float stepSpeed = 2; //Walking animation speed larger number faster animation
float walkingSpeed = 0.5; //Larger number faster speed
float armSpeed = 5; //Larger number faster speed
float fingerSpeed = 5; //Larger number faster speed


float getDeltaTime() {
	static auto lastTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - lastTime;

	float deltaTime = elapsed.count();
	lastTime = currentTime;

	// Clamp to avoid lag spikes
	if (deltaTime > 0.05f) // ~20 FPS minimum
		deltaTime = 0.05f;

	return deltaTime;
}

void update() {
	float dt = getDeltaTime();

	// Speeds tuned for visible effect
	animationSpeed = 0.5 / dt;   // larger number slower animation
	stepSpeed = 40 * dt;   // larger number faster animation
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
				perspective ? theta += walkingSpeed : oRx += walkingSpeed;
			else if (wParam == 'D')
				perspective ? theta -= walkingSpeed : oRx -= walkingSpeed;
			else if (wParam == 'W')
				phi += walkingSpeed;
			else if (wParam == 'S')
				phi -= walkingSpeed;
			else if (wParam == 'E')
				radius -= walkingSpeed;
			else if (wParam == 'Q')
				radius += walkingSpeed;
			else if (wParam == 'R')
				camY += walkingSpeed;
			else if (wParam == 'F')
				camY -= walkingSpeed;
			break;
		case 2:
			if (wParam == 'A')
				x += walkingSpeed;
			else if (wParam == 'D')
				x -= walkingSpeed;
			else if (wParam == 'W')
				y += walkingSpeed;
			else if (wParam == 'S')
				y -= walkingSpeed;
			else if (wParam == 'E')
				z += walkingSpeed;
			else if (wParam == 'Q')
				z -= walkingSpeed;
			break;
		case 3:
			if (wParam == 'W') {
				if (bodyRotation == 0) {
					bodyPositionZ -= walkingSpeed;
					camZ -= walkingSpeed;
				}
				else if (bodyRotation == -90) {
					bodyPositionX += walkingSpeed;
					camX += walkingSpeed;
				}
				else if (bodyRotation == 180) {
					bodyPositionZ += walkingSpeed;
					camZ += walkingSpeed;
				}
				else if (bodyRotation == 90) {
					bodyPositionX -= walkingSpeed;
					camX -= walkingSpeed;
				}
			}
			else if (wParam == 'D') {
				bodyRotation -= 90;
				if (bodyRotation == -180)
					bodyRotation = 180;
			}
			else if (wParam == 'A') {
				bodyRotation += 90;
				if (bodyRotation == 270)
					bodyRotation = -90;
			}
			else if (wParam == 'S') {
				if (bodyRotation == 0) {
					bodyPositionZ += walkingSpeed;
					camZ += walkingSpeed;
				}
				else if (bodyRotation == -90) {
					bodyPositionX -= walkingSpeed;
					camX -= walkingSpeed;
				}
				else if (bodyRotation == 180) {
					bodyPositionZ -= walkingSpeed;
					camZ -= walkingSpeed;
				}
				else if (bodyRotation == 90) {
					bodyPositionX += walkingSpeed;
					camX += walkingSpeed;
				}
			}
			break;
		default:
			break;
		}

		//Light
		if (wParam == 'L') {
			isLightOn = !isLightOn;
		}

		//Menu selection
		if (wParam == 0x31)
			menu = 1;
		else if (wParam == 0x32)
			menu = 2;
		else if (wParam == 0x33)
			menu = 3;
		
		//View
		if (wParam == 0x34)
			perspective = !perspective;

		//Texture
		if (wParam == 0x39) {
			clothesTextureIndex < 3 ? clothesTextureIndex += 1 : clothesTextureIndex = 0;
			clothesTexture = clothesTextureList[clothesTextureIndex];
			update();
		}
		else if (wParam == 0x38) {
			weaponTextureIndex < 2 ? weaponTextureIndex += 1 : weaponTextureIndex = 0;
			weaponTexture = weaponTextureList[weaponTextureIndex];
			update();
		}

			
		//Body movement
		if (wParam == 'P')
			leftArmMovement = !leftArmMovement;
		else if (wParam == 'O') {
			smallArmMovement = !smallArmMovement;
		}
		
		//Animation
		if (wParam == 'B') { //Flying animation
			isFlying = !isFlying;
			flyingDone = false;
			floating = false;
			flyingStartTime = std::chrono::high_resolution_clock::now();
		}
		else if (wParam == 'N') { //Weapon animation
			weaponRotation = 0;
			weaponPositionY = 0;

			fighting = !fighting;

			if(fighting)
				fightingStartTime = std::chrono::high_resolution_clock::now();
		}
		else if (wParam == 'M') { //Walking animation
			isWalking = !isWalking;
			leftLegAngle = 0;
			rightLegAngle = 0;
			leftForearm = 45;
			rightForearm = 45;
			leftBicep = 0;
			rightBicep = 0;
			leftArmAngle = 0;
			rightArmAngle = 0;
		}

		//Finger movement
		if (wParam == 0x30) {
			separateMovement = !separateMovement;
			indexAngle = -10;
			middleAngle = -10;
			ringAngle = -10;
			pinkyAngle = -10;
			part = -10;
		}

		if (!separateMovement) {
			if (wParam == 'U')
				part > -80 ? part -= fingerSpeed : part = -80;
			else if (wParam == 'I')
				part < 0 ? part += fingerSpeed : part = 0;
		}
		else {
			if(wParam == 'G')
				indexAngle > -80 ? indexAngle -= fingerSpeed : indexAngle = -80;
			if(wParam == 'H')
				middleAngle > -80 ? middleAngle -= fingerSpeed : middleAngle = -80;
			if(wParam == 'J')
				ringAngle > -80 ? ringAngle -= fingerSpeed : ringAngle = -80;
			if(wParam == 'K')
				pinkyAngle > -80 ? pinkyAngle -= fingerSpeed : pinkyAngle = -80;

			if(wParam == 'T')
				indexAngle < 0 ? indexAngle += fingerSpeed : indexAngle = 0;
			if(wParam == 'Y')
				middleAngle < 0 ? middleAngle += fingerSpeed : middleAngle = 0;
			if(wParam == 'U')
				ringAngle < 0 ? ringAngle += fingerSpeed : ringAngle = 0;
			if(wParam == 'I')
				pinkyAngle < 0 ? pinkyAngle += fingerSpeed : pinkyAngle = 0;
		}

		//Arm movement
		if (wParam == VK_UP)
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
		else if (wParam == VK_LEFT)
			leftArmMovement ? leftHandRotation > handRotationMin ? leftHandRotation -= armSpeed : leftHandRotation = handRotationMin : rightHandRotation > handRotationMin ? rightHandRotation -= armSpeed : rightHandRotation = handRotationMin;
		else if (wParam == VK_RIGHT)
			leftArmMovement ? leftHandRotation < handRotationMax ? leftHandRotation += armSpeed : leftHandRotation = handRotationMax : rightHandRotation < handRotationMax ? rightHandRotation += armSpeed : rightHandRotation = handRotationMax;
		
		if (wParam == VK_SPACE) {
			theta = 0;
			radius = 1;
			phi = 0;
			camY = 0;
			leftLegAngle = 0;
			rightLegAngle = 0;
			leftForearm = 45;
			rightForearm = 45;
			leftBicep = 0;
			rightBicep = 0;
			leftArmAngle = 0;
			rightArmAngle = 0;
			part = -10;
			x = -1;
			y = 0.3;
			z = -1.2;
			leftHandRotation = 0;
			rightHandRotation = 0;
			weaponRotation = 0;
			weaponPositionY = 0;
			fighting = false;
			isFlying = false;
			bodyPositionY = 0;
			upperBodyPositionY = 0;
			bodyPositionZ = 0;
			bodyPositionX = 0;
			bodyRotation = 0;
			camZ = 0;
			camX = 0;
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

void drawHexagon(float radius) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 6; i++) {
		float angle = i * (2.0f * 3.142 / 6.0f); // 60° each step
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, 0.0f);  // Z = 0 (flat in XY plane)
	}
	glEnd();
}


//Arm Parts
void fingerPart() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(0.1, 0.1, 0.5);
	drawCyclinder(0.2, 0.2, 0.1);
	glPopMatrix();
}
// 1 Cyclinder

void fingerTip() {
	

	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	drawSphere(0.02);
	glPopMatrix();
	
}
//1 Sphere

void finger(bool left, float fingerAngle) {

	glPushMatrix();
	if (!separateMovement) {
		left ? glRotatef(part > -45 ? part : -45, 1, 0, 0) : glRotatef(-45, 1, 0, 0);
	}

	if (separateMovement) {
		left ? glRotatef(fingerAngle > -45 ? fingerAngle : -45, 1, 0, 0) : glRotatef(-45, 1, 0, 0);
	}
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	if (!separateMovement) {
		left ? glRotatef(part > -70 ? part : -70, 1, 0, 0) : glRotatef(-70, 1, 0, 0);
	}
	if (separateMovement) {
		left ? glRotatef(fingerAngle > -45 ? fingerAngle : -45, 1, 0, 0) : glRotatef(-70, 1, 0, 0);
	}
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();

	if (!separateMovement) {
		left ? glRotatef(part > -80 ? part : -80, 1, 0, 0) : glRotatef(-80, 1, 0, 0);
	}
	if (separateMovement) {
		left ? glRotatef(fingerAngle > -45 ? fingerAngle : -45, 1, 0, 0) : glRotatef(-80, 1, 0, 0);
	}
	fingerPart();
	glTranslatef(0, -0.05, 0);
	fingerTip();
	glPopMatrix();

	

	
}
// 3 FingerPart, 3 FingerTip

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
// 2 FingerPart, 2 FingerTip

void pinky(bool left) {
	glPushMatrix();
	glScalef(0.78, 0.7, 0.78); //Adjust the size of the finger
	finger(left, pinkyAngle);
	glPopMatrix();
}
// 1 Finger

void ring(bool left) {
	glPushMatrix();
	glScalef(0.95, 0.95, 0.95);
	finger(left, ringAngle);
	glPopMatrix();
}
// 1 Finger

void index(bool left) {
	glPushMatrix();
	glScalef(0.92, 0.9, 0.92);
	finger(left, indexAngle);
	glPopMatrix();
}
// 1 Finger

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
// 3 Cyclinder

void fingers(bool left) {

	glPushMatrix();
	glScalef(0.8,0.6,1);
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
	finger(left, middleAngle);

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
// 4 Finger 1 Thumb

void hand(bool left) {
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	palm();
	glPopMatrix();

	fingers(left);

}
// 1 Palm 1 Fingers


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
	drawCyclinder(0.4, 0.7, 0.4, clothesTexture);
	glRotatef(180, 1, 0, 0);
	drawCyclinder(0.4, 0.4, 0.05, clothesTexture);
	glTranslatef(0,0,0.05);
	drawCyclinder(0.4, 0.3, 0.05, clothesTexture);
	glTranslatef(0,0,0.05);
	drawCyclinder(0.3, 0.2, 0.025, clothesTexture);
	glTranslatef(0,0,0.025);
	drawCyclinder(0.2, 0.0, 0.02, clothesTexture);
	glPopMatrix();
}
// 5 Cyclinders

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
	drawCyclinder(0.01, 0.01, 1.15);
	glPopMatrix();

	//Connector
	glColor3f(0.8, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.17, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 1, 1);
	drawCyclinder(0.015, 0.025, 0.09);

	glPopMatrix();

	//Tip
	weaponTextureIndex == 0 ? glColor3f(0, 0.77, 1) : glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	drawCyclinder(0.01, 0.05, 0.15, weaponTexture);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1.7, 0.7, 1);
	drawCyclinder(0.05, 0, 0.3, weaponTexture);
	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);

	//Outline
	//Tip
	glLineWidth(3);
	weaponTextureIndex == 0 ? glColor3f(0, 0.67, 0.8) : glColor3f(0, 0, 0);
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
// 6 Cyclinders

void fightingAnimation() {
	auto now = std::chrono::high_resolution_clock::now();
	float elapsed = std::chrono::duration<float>(now - fightingStartTime).count();

	if (elapsed >= 1)
		fightingCharging = false;

	if (elapsed > 0.5)
		angry = true;
	
	if (!fightingCharging && elapsed >= 3)
		fightingDone = true;

	if (fightingCharging) {
		rightBicep < 140 ? rightBicep += 140 / animationSpeed : rightBicep = 140;
		weaponRotation < 180 ? weaponRotation += 180 / animationSpeed : weaponRotation = 180;
		weaponPositionY < 0.7 ? weaponPositionY += 0.7 / animationSpeed : weaponPositionY = 0.7;
	}

	if (!fightingDone && !fightingCharging) {
		rightBicep > 120 ? rightBicep -= 120 / animationSpeed : rightBicep = 120;
		weaponPositionY < 5 ? weaponPositionY += 5 / animationSpeed : weaponPositionY = 5;
	}

	if (fightingDone) {
		fightingCharging = true;
		fightingDone = false;
		fighting = false;
		angry = false;
		weaponPositionY = 0;
		weaponRotation = 0;
		rightBicep = 0;
	}
}

void arm(bool left) {

	if(fighting)
		fightingAnimation();

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
	glRotatef(left ? leftHandRotation : rightHandRotation, 0, 1, 0);
	glScalef(0.7, 0.7, 0.7);
	hand(left);

	if (!left) {
		glTranslatef(-0.45, 0.01, 0.03);
		glScalef(1.3, 1.3, 1.3);
		glRotatef(92, 0, 0, 1);
		glRotatef(weaponRotation, 0, 0, 1);
		glTranslatef(0, weaponPositionY, 0);
		weapon();
	}
	glPopMatrix();
}
// 1 Sleeve, 2 Cyclinder, 2 Sphere, 1 Hand, 1 Weapon

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
	glScalef(1,1,0.9);
	drawSphere(0.05);
	glPopMatrix();
	glRotatef(rightArmAngle, 1, 0, 0);
	glTranslatef(0, -0.5, 0);
	arm(false);
	glPopMatrix();
}
// 2 Sphere, 2 Arm

//Body
void torso() {
	//Chest
	glColor3f(0.9, 0.9, 0.9);
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1,0.7,1);
	drawCyclinder(0.25, 0.23, 0.25, clothesTexture);
	glPopMatrix();

	//Waits
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.23, 0.23, 0.05, clothesTexture);
	glPopMatrix();

	//Lower
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(0,-0.35,0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.23, 0.35, 0.45, clothesTexture);
	glPopMatrix();
}

void hoodie() {
	glColor3f(0, 0.77, 1);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 0.7, 1);
	drawCyclinder(0.25, 0.26, 0.04, clothesTexture);
	glTranslatef(0, 0, 0.04);
	drawCyclinder(0.26, 0.28, 0.01, clothesTexture);
	glTranslatef(0, 0, 0.01);
	drawCyclinder(0.28, 0.3, 0.02, clothesTexture);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.3, 0.3, 0.02, clothesTexture);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.3, 0.28, 0.02, clothesTexture);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 0, -0.02);
	drawCyclinder(0.28, 0.2, 0.02, clothesTexture);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.2, 0.15, 0.02, clothesTexture);
	glPopMatrix();
}

void body() {
	torso();
	hoodie();
}

//Leg
void foot() {
	//Leg
	glPushMatrix();
	glTranslatef(0, -0.6, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	drawCyclinder(0.08, 0.08, 0.3);
	glPopMatrix();

	//Shoe sole
	glPushMatrix();
	glTranslatef(0, -0.87, -0.05);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1.4, 1);
	drawCyclinder(0.08, 0.08, 0.04);
	glPopMatrix();


	//Shoe Tip
	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(0, 1, 0, 0);
	glScalef(1.2, 0.5, 1);
	drawCyclinder(0.05, 0.07, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.87, -0.15);
	glRotatef(180, 1, 0, 0);
	glScalef(1, 0.5, 1);
	drawCyclinder(0.06, 0, 0.02);
	glPopMatrix();
}
// 4 Cyclinder

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
// 2 Foot

//Head
void neck() {
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glRotatef(90,1,0,0);
	glScalef(1.2,0.9,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-0.02,0);
	glRotatef(-90, 1, 0, 0);
	glPushMatrix();
	glScalef(1.2, 0.7, 1);
	drawCircle(0.2);
	glRotatef(180, 1, 0, 0);
	drawCyclinder(0.21, 0.2, 0.02);
	glPopMatrix();
	drawCyclinder(0.18,0.1,0.02);
	glTranslatef(0, 0, 0.02);
	drawCyclinder(0.1,0.08,0.02);
	glTranslatef(0,0,0.02);
	drawCyclinder(0.08,0.08,0.02);
	glTranslatef(0,0,0.02);
	drawCyclinder(0.08,0.1,0.02);
	glPopMatrix();
}
// 5 Cyclinder

void eye() {
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0.085,0.25,-0.3);
	glRotatef(180, 0, 0, 1);
	glRotatef(17, 0, 1, 0);
	glScalef(0.2,0.25,1);
	drawCircle(0.2);
	drawCyclinder(0.2, 0.2, 0.1);
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.05, -0.001);
	glScalef(0.7, 0.7, 1);
	drawCircle(0.2);
	drawCyclinder(0.2, 0.2, 0.1);
	glLineWidth(5);
	glTranslatef(0.2, -0.15, -0.01);
	glRotatef(angry ? -60 : -80, 0, 0, 1);
	glScalef(1.5, 3, 1);
	drawCircle(0.2, -45, false);
	glPopMatrix();

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(-0.085,0.25,-0.3);
	glRotatef(180, 0, 0, 1);
	glRotatef(-17, 0, 1, 0);
	glScalef(0.2,0.25,1);
	drawCircle(0.2);
	drawCyclinder(0.2, 0.2, 0.1);
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.05, -0.001);
	glScalef(0.7, 0.7, 1);
	drawCircle(0.2);
	drawCyclinder(0.2, 0.2, 0.1);
	glLineWidth(5);
	glTranslatef(-0.2, -0.15, -0.01);
	glRotatef(angry ? 60 : 80, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glScalef(1.5, 3, 1);
	drawCircle(0.2, -45, false);
	glPopMatrix();
}
// 4 Triangle fan, 4 Cyclinders, 2 Line strip

void cheeks() {
	glColor3f(0.8, 0.4, 0.6);
	glPushMatrix();
	glTranslatef(0.12, 0.18, -0.28);
	glRotatef(-20, 0, 1, 0);
	glScalef(0.5,0.3,1);
	drawCircle(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.12, 0.18, -0.28);
	glRotatef(20, 0, 1, 0);
	glScalef(0.5,0.3,1);
	drawCircle(0.1);
	glPopMatrix();
}
// 2 Triangle fan

void mouth() {
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(0, 0.14, -0.31);
	glScalef(0.6,0.1,1);
	glRotatef(angry ? 180 : 0,0,0,1);
	drawCircle(0.05, -180, false);
	glPopMatrix();
}
// 1 Line strip

void hairMiddlePart() {

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.05f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.05f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.05f, 0.0f, -0.05f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.05f, 0.0f, -0.05f);
	glEnd();

}
// 1 Quads

void hairStrand(double length) {
	glPushMatrix();
	glScalef(0.2, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.05, 0, -0.05);
	glVertex3f(-0.05, 0, -0.05);
	glEnd();

	glPushMatrix();
	glTranslatef(0,0,-0.05);
	glScalef(1,1,0.73);
	hairMiddlePart();
	for (int x = 0; x < length; x++) {
		glTranslatef(0, 0, -0.05);
		glRotatef(x < 11 ? -4 - x : -2, 1, 0, 0);
		hairMiddlePart();
	}
	glTranslatef(0, 0, -0.05);
	glRotatef(-10, 1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.05, 0, 0);
	glVertex3f(-0.05, 0, 0);
	glVertex3f(0, 0, -0.05);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}
// 2 Triangles, X+1 hairMiddlePart

void backHair() {
	for (int x = 0; x < 8; x++) {
		hairStrand(16);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 1; x++) {
		hairStrand(16 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(16 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 1; x++) {
		hairStrand(13 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(13 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}
}
// 12 hairstrand 16*8 middlepart, 15*2 middlepart, 14*2 middlepart

void hair() {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.67, 0);
	//Front
	for (int x = 0; x < 10; x++) {
		hairStrand(11);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 5; x++) {
		hairStrand(10 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(10 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(14 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(12 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(12 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 4; x++) {
		hairStrand(11 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(11 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	backHair();
	backHair();
	backHair();
	backHair();
	backHair();
	backHair();
	backHair();

	for (int x = 0; x < 2; x++) {
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(14 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 4; x++) {
		hairStrand(11 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(11 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(12 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(12 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(14 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(14);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(15 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(12 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(12 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 3; x++) {
		hairStrand(11);
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(12 - (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(12 - (x + 1));
		glRotatef(2, 0, 1, 0);
	}

	for (int x = 0; x < 2; x++) {
		hairStrand(10 + (x + 1));
		glRotatef(2, 0, 1, 0);
		hairStrand(10 + (x + 1));
		glRotatef(2, 0, 1, 0);
	}


	glPopMatrix();
}
// 7 back hair

void ear(bool left) { //Neko ears
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glTranslatef(0,0.5,-0.25);
	glRotatef(left ? 40 : -40,0,0,1);
	glScalef(0.9, 0.9, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0,0.2,0);
	glVertex3f(0.1,0,0);
	glVertex3f(-0.1,0,0);
	glEnd();

	glColor3f(0.8, 0.4, 0.4);
	glPushMatrix();
	glTranslatef(0, 0, -0.001);
	glScalef(0.9, 0.7, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0,0.2,0);
	glVertex3f(0.1,0,0);
	glVertex3f(-0.1,0,0);
	glEnd();
	glPopMatrix();

	glColor3f(0.97, 0.97, 0.8);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0.2,0);
	glVertex3f(0.1,0,0);
	glVertex3f(0,0,0.4);
	glVertex3f(-0.1, 0, 0);
	glEnd();
	glPopMatrix();
}
// 2 Triangles 1 Triangle fan

void ears() {
	glPushMatrix();
	glTranslatef(-0.15,0,0);
	ear(true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15,0,0);
	ear(false);
	glPopMatrix();
}
// 2 Ears

void head() {
	glColor3f(0.97, 0.97, 0.8);
	glPushMatrix();
	glTranslatef(0, 0.05, 0);
	glScalef(1, 1, 1);
	glRotatef(-90, 1, 0, 0);
	drawCyclinder(0, 0.1, 0.001);
	drawCyclinder(0.1, 0.22, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.22, 0.24, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.24, 0.26, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.26, 0.27, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.27, 0.28, 0.1);
	glTranslatef(0, 0, 0.1);
	drawCyclinder(0.28, 0.3, 0.05);
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

	glPushMatrix();
	glTranslatef(0, 0.05, -0.04);
	glScalef(1, 1, 1);
	glRotatef(-90, 1, 0, 0);
	drawCyclinder(0, 0.1, 0.001);
	drawCyclinder(0.1, 0.22, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.22, 0.24, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.24, 0.26, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.26, 0.27, 0.025);
	glTranslatef(0, 0, 0.025);
	drawCyclinder(0.27, 0.27, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.27, 0.28, 0.05);
	glTranslatef(0, 0, 0.05);
	drawCyclinder(0.28, 0.28, 0.1);
	glTranslatef(0, 0, 0.1);
	//drawCyclinder(0.28, 0.25, 0.05);
	glPopMatrix();

	eye();

	cheeks();

	mouth();

	hair();

	ears();
}

//Decorations
void diamond() {
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0, -0.1);
	glVertex3f(0, -0.3, 0);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0, 0, -0.1);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0, 0.1);
	glVertex3f(0, -0.3, 0);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0, 0, 0.1);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0, -0.1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, -0.1);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, -0.3, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, -0.1);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0, -0.3, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0.3, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0, 0.1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0.1);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, -0.3, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0.1);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0, -0.3, 0);
	glEnd();

	
	glPopMatrix();
}
// 2 Triangle strip, 6 line loop

void iceCore() {
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glPushMatrix();
	glTranslatef(0,0,-0.02);
	drawHexagon(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0.02);
	drawHexagon(0.1);
	glPopMatrix();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(-0.05,0.085,0.02);
	glVertex3f(-0.05,0.085,-0.02);
	glVertex3f(0.05, 0.085, 0.02);
	glVertex3f(0.05,0.085,-0.02);
	glVertex3f(0.1, 0, 0.02);
	glVertex3f(0.1, 0, -0.02);
	glVertex3f(0.05, -0.085, 0.02);
	glVertex3f(0.05, -0.085, -0.02);
	glVertex3f(-0.05, -0.085, 0.02);
	glVertex3f(-0.05, -0.085, -0.02);
	glVertex3f(-0.1, 0, 0.02);
	glVertex3f(-0.1, 0, -0.02);
	glVertex3f(-0.05, 0.085, 0.02);
	glVertex3f(-0.05, 0.085, -0.02);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.05, 0.085, 0.02);
	glVertex3f(0.05, 0.085, 0.02);
	glVertex3f(0.1, 0, 0.02);
	glVertex3f(0.05, -0.085, 0.02);
	glVertex3f(-0.05, -0.085, 0.02);
	glVertex3f(-0.1, 0, 0.02);
	glVertex3f(-0.05, 0.085, 0.02);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.05, 0.085, -0.02);
	glVertex3f(0.05, 0.085, -0.02);
	glVertex3f(0.1, 0, -0.02);
	glVertex3f(0.05, -0.085, -0.02);
	glVertex3f(-0.05, -0.085, -0.02);
	glVertex3f(-0.1, 0, -0.02);
	glVertex3f(-0.05, 0.085, -0.02);
	glEnd();


}
// 2 Polygon, 2 Quad Strip, 2 Line loop

void iceEntity(int num) {
	glColor3f(0.8f, 0.95f, 1.0f);
	glPushMatrix();
	glScalef(0.6, 0.6, 0.6);
	iceCore();
	glPopMatrix();
	
	for (int x = 0; x < num; x++) {
		glColor3f(0.8f, 0.95f, 1.0f);
		glPushMatrix();
		glRotatef((360/num) * x, 0, 0, 1);
		glTranslatef(0, 0.15, 0);
		glRotatef(shardsRotation, 1, 0, 0);
		glTranslatef(0, shardsPosition, 0);
		glScalef(0.3, 0.3, 0.3);
		diamond();
		glPopMatrix();
	}
}
// 1 core, X diamond

void iceEntityAnimation() {
	glPushMatrix();
	glTranslatef(0, 0, 0.2);
	glPushMatrix();
	glRotatef(0, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glScalef(shardsSize, shardsSize, shardsSize);
	iceEntity(3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(60, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glScalef(shardsSize, shardsSize, shardsSize);
	iceEntity(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(60 * 2, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glScalef(shardsSize, shardsSize, shardsSize);
	iceEntity(4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(60 * 3, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glScalef(shardsSize, shardsSize, shardsSize);
	iceEntity(3);
	glPopMatrix();
	glPopMatrix();
}

void flyAnimation() {
	auto now = std::chrono::high_resolution_clock::now();
	float elapsed = std::chrono::duration<float>(now - flyingStartTime).count();

	if (isFlying) {
		if (elapsed < 0.5)
			upperBodyPositionY > -0.05 ? upperBodyPositionY -= 0.05 / animationSpeed : upperBodyPositionY = -0.05;
		else if (!floating) {
			upperBodyPositionY = 0;
			bodyPositionY < 1 ? bodyPositionY += 1 / animationSpeed : bodyPositionY = 1;
			leftBicep < 180 ? leftBicep += 180 / animationSpeed : leftBicep = 180;
			rightBicep < 180 ? rightBicep += 180 / animationSpeed : rightBicep = 180;

			if (bodyPositionY == 1) {
				floating = true;
			}
		}

		if (floating && !flyingDone) {
			leftBicep > 0 ? leftBicep -= 180 / animationSpeed : leftBicep = 0;
			rightBicep > 0 ? rightBicep -= 180 / animationSpeed : rightBicep = 0;
			shardsSize < 1 ? shardsSize += 1 / animationSpeed : shardsSize = 1;

			if (shardsSize == 1)
				flyingDone = true;
		}

		if (flyingDone) {
			if(elapsed > 2 && elapsed < 4)
				leftBicep < 180 ? leftBicep += 180 / animationSpeed : leftBicep = 180;
			if(shardsSize == 1 && elapsed > 3)
				shardsRotation < 90 ? shardsRotation += 90 / animationSpeed : shardsRotation = 90;

			if (elapsed > 4) {
				shardsPosition > -5 ? shardsPosition -= 5 / animationSpeed : shardsPosition = -5;
				leftBicep > 90 ? leftBicep -= 90 / animationSpeed : leftBicep = 90;
			}
		}
	}
	else if (!isFlying && !flyingDone) {
		bodyPositionY > 0 ? bodyPositionY -= 1 / animationSpeed : bodyPositionY = 0;
		shardsSize > 0 ? shardsSize -= 1 / animationSpeed : shardsSize = 0;
		leftBicep > 0 ? leftBicep -= 90 / animationSpeed : leftBicep = 0;
		if (shardsSize == 0) {
			shardsPosition = 0;
			shardsRotation = 0;
			flyingDone = true;
		}
	}
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
	GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightSpecular[] = { 0.3f, 0.3f, 0.3f, 0.3f };
	GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat lightPos[] = { x, y, z, 0};       // point light

	GLfloat matSpecular[] = { 0.3, 0.3, 0.3, 1 }; // white highlight
	GLfloat matShininess[] = { 5 };  // much softer highlight


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
	/*glColor3f(0.9, 0.6, 0.3);
	glPushMatrix();
	glTranslatef(x, y, z);
	drawSphere(0.1);
	glPopMatrix();*/
}

void projection() {
	pRx = radius * cos(phi) * sin(theta);
	pRy = radius * sin(phi);
	pRz = radius * cos(phi) * cos(theta);

	if (perspective) {
		gluPerspective(100.0, 1920.0 / 1080.0, 0.1, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(pRx, pRy, -pRz,   // camera position
			camX, camY, camZ,     // look at origin
			0.0, 1.0, 0.0);
	}
	else {
		glOrtho(-4.445, 4.445, -2.5, 2.5, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(oRx, 0, -1,   // camera position
			0.0, 0.0, 0.0,     // look at origin
			0.0, 1.0, 0.0);
	}
}

void background() {
	glColor3f(0, 0.7, 0);
	glBegin(GL_QUADS);
	glVertex3f(8, 0, -8);
	glVertex3f(-8, 0, -8);
	glVertex3f(-8, 0, 8);
	glVertex3f(8, 0, 8);
	glEnd();
}

void display()
{
	update();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.33f, 0.61f, 0.82f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	projection();
	lighting();

	if(isWalking)
		updateWalk();

	flyAnimation();

	glPushMatrix();
	glTranslatef(bodyPositionX, bodyPositionY, bodyPositionZ);
	glRotatef(bodyRotation, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0,upperBodyPositionY,0);
	neck();
	head();
	arms();
	body();
	glPopMatrix();
	feet();
	iceEntityAnimation();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-0.91,0);
	background();
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