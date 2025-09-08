#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "Practical 7"

// =================== Globals ===================
int qNo = 1;

// Fixed indices for textures
enum { TEX_BOX = 0, TEX_METAL = 1, TEX_BRICK = 2, TEX_COUNT = 3 };
GLuint textureArr[TEX_COUNT] = { 0,0,0 };
GLuint texture = 0;
int currentTex = TEX_BOX;

const char* TEX_FILE[TEX_COUNT] = { "Box.bmp", "Metal.bmp", "Brick.bmp" };

// Forward declare
GLuint loadTexture(LPCSTR filename);

// Load-once helper using fixed slots
inline void ensureTex(int idx) {
    if (idx < 0 || idx >= TEX_COUNT) return;
    if (textureArr[idx] == 0) {
        textureArr[idx] = loadTexture(TEX_FILE[idx]);
    }
}

// ------------------------------------------------

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) PostQuitMessage(0);
        else if (wParam == '1') qNo = 1;
        else if (wParam == '2') qNo = 2;
        else if (wParam == '3') qNo = 3;
        else if (wParam == 'A') { currentTex = TEX_BOX;  ensureTex(TEX_BOX); }
        else if (wParam == 'S') { currentTex = TEX_METAL; ensureTex(TEX_METAL); }
        else if (wParam == 'D') { currentTex = TEX_BRICK; ensureTex(TEX_BRICK); }
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ------------------------------------------------

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

    int n = ChoosePixelFormat(hdc, &pfd);
    if (!n) return false;
    return SetPixelFormat(hdc, n, &pfd) ? true : false;
}

// ------------------------------------------------

void drawCube(float size) {
    glBegin(GL_QUADS);
    // Front
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, 0.0f, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, size, size);
    // Left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, size, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, size);
    // Top
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, size, 0.0f);
    // Right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, 0.0f, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0f, 0.0f);
    // Bottom
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, size, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, 0.0f);
    // Back
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, size, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0f, 0.0f);
    glEnd();
}

void P7Q1() {
    // Use BRICK in its own slot (no collision with BOX)
    ensureTex(TEX_BRICK);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureArr[currentTex]);
    glColor3f(1, 1, 1);

    // Simple textured pyramid (same as before, using the bound brick texture)
    glBegin(GL_TRIANGLES);
    // Front
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, -0.25f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.25f, -0.25f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.25f, -0.25f);
    glEnd();

    glBegin(GL_TRIANGLES);
    // Right
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.25f, -0.25f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.25f, -0.25f, -0.5f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.25f, -0.25f);
    glEnd();

    glBegin(GL_TRIANGLES);
    // Back
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.25f, -0.25f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.25f, -0.25f, -0.5f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.25f, -0.25f);
    glEnd();

    glBegin(GL_TRIANGLES);
    // Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, -0.25f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.25f, -0.25f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.25f, -0.25f);
    glEnd();

    glBegin(GL_QUADS);
    // Base
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, -0.25f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.25f, -0.25f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.25f, -0.25f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.25f, -0.25f, -0.5f);
    glEnd();
}

void P7Q2() {
    // ensure selected texture exists and bind it
    ensureTex(currentTex);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureArr[currentTex]);
    glColor3f(1, 1, 1);

    glPushMatrix();
    // center a 0.5 cube at origin
    glTranslatef(-0.25f, -0.25f, -0.25f);
    drawCube(0.5f);
    glPopMatrix();
}

void drawCyclinder() {
    GLUquadricObj* quadric = NULL;
    quadric = gluNewQuadric();

    glColor3f(1, 1, 0);
    texture = loadTexture("stoneWall.bmp");
    gluQuadricTexture(quadric, texture);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluSphere(quadric, 0.5, 30, 10);
    glDeleteTextures(1, &texture);
}

void drawSphere() {
    GLUquadricObj* quadric = NULL;
    quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, true);
    gluSphere(quadric, 0.5, 30, 10);
    gluDeleteQuadric(quadric);
}

void P7Q3() {
    textureArr[0] = loadTexture("Box.bmp");
    drawSphere();
    glDeleteTextures(1, &textureArr[0]);

}

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

// ------------------------------------------------

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    static float angle = 0.f;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    angle += 0.05f;

    switch (qNo) {
    case 1: P7Q1(); break;
    case 2: P7Q2(); break;
    case 3: P7Q3(); break;
    default: P7Q1(); break;
    }

    glDisable(GL_TEXTURE_2D);
}

// ------------------------------------------------

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
        900, 10, 300, 300, NULL, NULL, wc.hInstance, NULL);

    HDC hdc = GetDC(hWnd);
    if (!initPixelFormat(hdc)) return false;

    HGLRC hglrc = wglCreateContext(hdc);
    if (!wglMakeCurrent(hdc, hglrc)) return false;

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

    // Cleanup textures
    for (int i = 0; i < TEX_COUNT; ++i) {
        if (textureArr[i]) glDeleteTextures(1, &textureArr[i]);
        textureArr[i] = 0;
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hWnd, hdc);
    UnregisterClass(WINDOW_TITLE, wc.hInstance);
    return true;
}