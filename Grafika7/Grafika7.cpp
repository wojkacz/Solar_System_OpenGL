#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>
#include <string>
#include <iostream>
#include <gl/gl.h>
#include <gl/glut.h>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat lookPos[] = { 0.0, 0.0, 0.0 };

static GLfloat pixX2angle = 0;
static GLfloat pixY2angle = 0;

static GLfloat thetaX = 6.2;
static GLfloat thetaY = 0.5;

int status = 0;
int lookAt = 0;

static int x_pos_old = 0;
static int y_pos_old = 0;

static int delta_y = 0;
static int delta_x = 0;

static GLfloat R = 30;

GLUquadric *sunQ, *mercuryQ, *venusQ, *earthQ, *marsQ, *jupiterQ, *saturnQ, *uranusQ, *neptuneQ;
GLuint sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture;

int a = 50;


GLdouble earthRadius = 0.1;
GLdouble sunRadius = earthRadius * 109;
GLdouble mercuryRadius = earthRadius * 0.3825;
GLdouble venusRadius = earthRadius * 0.9489;
GLdouble marsRadius = earthRadius * 0.5335;
GLdouble jupiterRadius = earthRadius * 9.4494;
GLdouble saturnRadius = earthRadius * 11.2092;
GLdouble uranusRadius = earthRadius * 4.0074;
GLdouble neptuneRadius = earthRadius * 3.8827;

float earthDistanceFromSun = 5;
float scaleLast4Planets = 4;
float additionalDistance = 6;

float mercuryDistance = sunRadius + (earthDistanceFromSun * 0.3871);
float venusDistance = sunRadius + (earthDistanceFromSun * 0.7233);
float earthDistance = sunRadius + earthDistanceFromSun;
float marsDisctance = sunRadius + (earthDistanceFromSun * 1.5237);
float jupiterDistance = sunRadius + (earthDistanceFromSun / scaleLast4Planets * 5.2034) + additionalDistance;
float saturnDistance = sunRadius + (earthDistanceFromSun / scaleLast4Planets * 9.5371) + additionalDistance;
float uranusDistance = sunRadius + (earthDistanceFromSun / scaleLast4Planets * 19.1913) + additionalDistance;
float neptuneDistance = sunRadius + (earthDistanceFromSun / scaleLast4Planets * 30.0690) + additionalDistance;

point3 mercuryPos = { 0, 0, mercuryDistance };
point3 venusPos = { 0, 0, venusDistance };
point3 earthPos = { 0, 0, earthDistance };
point3 marsPos = { 0, 0, marsDisctance };
point3 jupiterPos = { 0, 0, jupiterDistance };
point3 saturnPos = { 0, 0, saturnDistance };
point3 uranusPos = { 0, 0, uranusDistance };
point3 neptunePos = { 0, 0, neptuneDistance };

float mercuryRotation = 0;
float venusRotation = 0;
float earthRotation = 0;
float marsRotation = 0;
float jupiterRotation = 0;
float saturnRotation = 0;
float uranusRotation = 0;
float neptuneRotation = 0;

float earthSpeed = 2 * M_PI / 5400.0;
float mercurySpeed = earthSpeed * (1 / 0.24);
float venusSpeed = earthSpeed * (1 / 0.62);
float marsSpeed = earthSpeed * (1 / 1.88);
float jupiterSpeed = earthSpeed * (1 / 11.87);
float saturnSpeed = earthSpeed * (1 / 29.48);
float uranusSpeed = earthSpeed * (1 / 84.07);
float neptuneSpeed = earthSpeed * (1 / 164.90);

float sunSelfRotation = 0;
float mercurySelfRotation = 0;
float venusSelfRotation = 0;
float earthSelfRotation = 0;
float marsSelfRotation = 0;
float jupiterSelfRotation = 0;
float saturnSelfRotation = 0;
float uranusSelfRotation = 0;
float neptuneSelfRotation = 0;

void countPlanetSpeed() {
    mercurySpeed = earthSpeed * (1 / 0.24);
    venusSpeed = earthSpeed * (1 / 0.62);
    marsSpeed = earthSpeed * (1 / 1.88);
    jupiterSpeed = earthSpeed * (1 / 11.87);
    saturnSpeed = earthSpeed * (1 / 29.48);
    uranusSpeed = earthSpeed * (1 / 84.07);
    neptuneSpeed = earthSpeed * (1 / 164.90);
}

void countPlanetSelfRotation() {
    float s = ((earthSpeed * 365.0) * 57.3);
    sunSelfRotation += s / 27.0;
    mercurySelfRotation += s / 58.0;
    venusSelfRotation += s / 243.0;
    earthSelfRotation += s;
    marsSelfRotation += s / 1.025;
    jupiterSelfRotation += s / 0.408;
    saturnSelfRotation += s / 0.446;
    uranusSelfRotation += s / 0.708;
    neptuneSelfRotation += s * 3.0 / 2.0;

    if (sunSelfRotation >= 360) sunSelfRotation -= 360;
    if (mercurySelfRotation >= 360) mercurySelfRotation -= 360;
    if (venusSelfRotation >= 360) venusSelfRotation -= 360;
    if (earthSelfRotation >= 360) earthSelfRotation -= 360;
    if (marsSelfRotation >= 360) marsSelfRotation -= 360;
    if (jupiterSelfRotation >= 360) jupiterSelfRotation -= 360;
    if (saturnSelfRotation >= 360) saturnSelfRotation -= 360;
    if (uranusSelfRotation >= 360) uranusSelfRotation -= 360;
    if (neptuneSelfRotation >= 360) neptuneSelfRotation -= 360;
}

void countPlanetOrbitAngle() {
    mercuryRotation -= mercurySpeed;
    venusRotation -= venusSpeed;
    earthRotation -= earthSpeed;
    marsRotation -= marsSpeed;
    jupiterRotation -= jupiterSpeed;
    saturnRotation -= saturnSpeed;
    uranusRotation -= uranusSpeed;
    neptuneRotation -= neptuneSpeed;

    if (mercuryRotation < 0) mercuryRotation += 2 * M_PI;
    if (venusRotation < 0) venusRotation += 2 * M_PI;
    if (earthRotation < 0) earthRotation += 2 * M_PI;
    if (marsRotation < 0) marsRotation += 2 * M_PI;
    if (jupiterRotation < 0) jupiterRotation += 2 * M_PI;
    if (saturnRotation < 0) saturnRotation += 2 * M_PI;
    if (uranusRotation < 0) uranusRotation += 2 * M_PI;
    if (neptuneRotation < 0) neptuneRotation += 2 * M_PI;
}

void timeLoop(int t) {
    countPlanetOrbitAngle();
    countPlanetSelfRotation();
}

void drawCircle(float r) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < 75; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(75);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex3f(x, 0, y);
    }
    glEnd();
}

GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{

    /*************************************************************************************/

    // Struktura dla nagłówka pliku  TGA

#pragma warning(disable : 4996)
#pragma pack(1)           
    typedef struct
    {
        GLbyte    idlength;
        GLbyte    colormaptype;
        GLbyte    datatypecode;
        unsigned short    colormapstart;
        unsigned short    colormaplength;
        unsigned char     colormapdepth;
        unsigned short    x_orgin;
        unsigned short    y_orgin;
        unsigned short    width;
        unsigned short    height;
        GLbyte    bitsperpixel;
        GLbyte    descriptor;
    }TGAHEADER;
#pragma pack(8)

    FILE* pFile;
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    short sDepth;
    GLbyte* pbitsperpixel = NULL;


    /*************************************************************************************/

    // Wartości domyślne zwracane w przypadku błędu

    *ImWidth = 0;
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;

    pFile = fopen(FileName, "rb");
    if (pFile == NULL)
        return NULL;

    /*************************************************************************************/
    // Przeczytanie nagłówka pliku 


    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


    /*************************************************************************************/

    // Odczytanie szerokości, wysokości i głębi obrazu

    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;


    /*************************************************************************************/
    // Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)

    if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
        return NULL;

    /*************************************************************************************/

    // Obliczenie rozmiaru bufora w pamięci


    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


    /*************************************************************************************/

    // Alokacja pamięci dla danych obrazu


    pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

    if (pbitsperpixel == NULL)
        return NULL;

    if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
    {
        free(pbitsperpixel);
        return NULL;
    }


    /*************************************************************************************/

    // Ustawienie formatu OpenGL
    switch (sDepth) {
    case 3:
        *ImFormat = GL_BGR_EXT;
        *ImComponents = GL_RGB8;
        break;

    case 4:
        *ImFormat = GL_BGRA_EXT;
        *ImComponents = GL_RGBA8;
        break;

    case 1:
        *ImFormat = GL_LUMINANCE;
        *ImComponents = GL_LUMINANCE8;
        break;
    };

    fclose(pFile);
    return pbitsperpixel;
}

GLuint loadTexture(std::string fileName) {
    GLuint temp = 0;
    glGenTextures(1, &temp);

    GLint image_width = 0;
    GLint image_height = 0;
    GLint image_components = 0;
    GLenum image_format = 0;

    GLbyte* image_bytes = LoadTGAImage(fileName.c_str(),
        &image_width,
        &image_height,
        &image_components,
        &image_format);
    glBindTexture(GL_TEXTURE_2D, temp);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        image_components,
        image_width,
        image_height,
        0,
        image_format,
        GL_UNSIGNED_BYTE,
        image_bytes);

    free(image_bytes);
    return temp;
}

void countPositions() {
    mercuryPos[0] = mercuryDistance * cos(mercuryRotation);
    mercuryPos[2] = mercuryDistance * sin(mercuryRotation);
    venusPos[0] = venusDistance * cos(venusRotation);
    venusPos[2] = venusDistance * sin(venusRotation);
    earthPos[0] = earthDistance * cos(earthRotation);
    earthPos[2] = earthDistance * sin(earthRotation);
    marsPos[0] = marsDisctance * cos(marsRotation);
    marsPos[2] = marsDisctance * sin(marsRotation);
    jupiterPos[0] = jupiterDistance * cos(jupiterRotation);
    jupiterPos[2] = jupiterDistance * sin(jupiterRotation);
    saturnPos[0] = saturnDistance * cos(saturnRotation);
    saturnPos[2] = saturnDistance * sin(saturnRotation);
    uranusPos[0] = uranusDistance * cos(uranusRotation);
    uranusPos[2] = uranusDistance * sin(uranusRotation);
    neptunePos[0] = neptuneDistance * cos(neptuneRotation);
    neptunePos[2] = neptuneDistance * sin(neptuneRotation);

    switch (lookAt) {
    case 0:
        lookPos[0] = 0.0;
        lookPos[1] = 0.0;
        lookPos[2] = 0.0;
        break;

    case 1:
        lookPos[0] = mercuryPos[0];
        lookPos[1] = mercuryPos[1];
        lookPos[2] = mercuryPos[2];
        break;

    case 2:
        lookPos[0] = venusPos[0];
        lookPos[1] = venusPos[1];
        lookPos[2] = venusPos[2];
        break;

    case 3:
        lookPos[0] = earthPos[0];
        lookPos[1] = earthPos[1];
        lookPos[2] = earthPos[2];
        break;

    case 4:
        lookPos[0] = marsPos[0];
        lookPos[1] = marsPos[1];
        lookPos[2] = marsPos[2];
        break;

    case 5:
        lookPos[0] = jupiterPos[0];
        lookPos[1] = jupiterPos[1];
        lookPos[2] = jupiterPos[2];
        break;

    case 6:
        lookPos[0] = saturnPos[0];
        lookPos[1] = saturnPos[1];
        lookPos[2] = saturnPos[2];
        break;

    case 7:
        lookPos[0] = uranusPos[0];
        lookPos[1] = uranusPos[1];
        lookPos[2] = uranusPos[2];
        break;

    case 8:
        lookPos[0] = neptunePos[0];
        lookPos[1] = neptunePos[1];
        lookPos[2] = neptunePos[2];
        break;
    }
}

void drawOrbits() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
    drawCircle(mercuryDistance);
    drawCircle(venusDistance);
    drawCircle(earthDistance);
    drawCircle(marsDisctance);
    drawCircle(jupiterDistance);
    drawCircle(saturnDistance);
    drawCircle(uranusDistance);
    drawCircle(neptuneDistance);
    glEnable(GL_TEXTURE_2D);
}

void drawPlanets() {
        // Merkury
        glPushMatrix();
        glTranslatef(mercuryPos[0], mercuryPos[1], mercuryPos[2]);
        glBindTexture(GL_TEXTURE_2D, mercuryTexture);
        gluQuadricTexture(mercuryQ, 1);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glRotatef(0.1, 1.0f, 0.0f, 0.0f);
        glPushMatrix();
        glRotatef(-mercurySelfRotation, 0.0f, 0.0f, 1.0f);
        gluSphere(mercuryQ, mercuryRadius, a, a);
        glPopMatrix();
        glPopMatrix();

    // Wenus
    glPushMatrix();
    glTranslatef(venusPos[0], venusPos[1], venusPos[2]);
    glBindTexture(GL_TEXTURE_2D, venusTexture);
    gluQuadricTexture(venusQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(177, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(venusSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(venusQ, venusRadius, a, a);
    //glRotatef(90, 1.0f, 0.0f, 0.0f);
    //glTranslatef(-venusPos[0], -venusPos[1], -venusPos[2]);
    glPopMatrix();
    glPopMatrix();

    // Ziemia
    glPushMatrix();
    glTranslatef(earthPos[0], earthPos[1], earthPos[2]);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    gluQuadricTexture(earthQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(23, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-earthSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(earthQ, earthRadius, a, a);
    glPopMatrix();
    glPopMatrix();

    // Mars 
    glPushMatrix();
    glTranslatef(marsPos[0], marsPos[1], marsPos[2]);
    glBindTexture(GL_TEXTURE_2D, marsTexture);
    gluQuadricTexture(marsQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(25, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-marsSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(marsQ, marsRadius, a, a);
    //glRotatef(90, 1.0f, 0.0f, 0.0f);
    //glTranslatef(-marsPos[0], -marsPos[1], -marsPos[2]);
    glPopMatrix();
    glPopMatrix();

    // Jowisz
    glPushMatrix();
    glTranslatef(jupiterPos[0], jupiterPos[1], jupiterPos[2]);
    glBindTexture(GL_TEXTURE_2D, jupiterTexture);
    gluQuadricTexture(jupiterQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(3, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-jupiterSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(jupiterQ, jupiterRadius, a, a);
    //glTranslatef(-jupiterPos[0], -jupiterPos[1], -jupiterPos[2]);
    glPopMatrix();
    glPopMatrix();

    // Saturn
    glPushMatrix();
    glTranslatef(saturnPos[0], saturnPos[1], saturnPos[2]);
    glBindTexture(GL_TEXTURE_2D, saturnTexture);
    gluQuadricTexture(saturnQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(27, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-saturnSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(saturnQ, saturnRadius, a, a);
    glPopMatrix();
    glPopMatrix();

    // Uran
    glPushMatrix();
    glTranslatef(uranusPos[0], uranusPos[1], uranusPos[2]);
    glBindTexture(GL_TEXTURE_2D, uranusTexture);
    gluQuadricTexture(uranusQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(98, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-uranusSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(uranusQ, uranusRadius, a, a);
    //glTranslatef(-uranusPos[0], -uranusPos[1], -uranusPos[2]);
    glPopMatrix();
    glPopMatrix();

    // Neptun
    glPushMatrix();
    glTranslatef(neptunePos[0], neptunePos[1], neptunePos[2]);
    glBindTexture(GL_TEXTURE_2D, neptuneTexture);
    gluQuadricTexture(neptuneQ, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(30, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(-neptuneSelfRotation, 0.0f, 0.0f, 1.0f);
    gluSphere(neptuneQ, neptuneRadius, a, a);
    //glTranslatef(-neptunePos[0], -neptunePos[1], -neptunePos[2]);
    glPopMatrix();
    glPopMatrix();
}

void drawSun() {
    GLfloat lightAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shinieness[] = { 128 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glMaterialfv(GL_FRONT, GL_AMBIENT, lightAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lightDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shinieness);

    glPushMatrix();
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(-sunSelfRotation, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    gluQuadricTexture(sunQ, 1);

    glDisable(GL_LIGHTING);
    gluSphere(sunQ, sunRadius, a, a);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void setCameraPos() {
    if (status == 1) {
        if(thetaY + (delta_y * pixY2angle * 0.01) > (M_PI / -2.0) && thetaY + (delta_y * pixY2angle * 0.01) < (M_PI / 2.0))
            thetaY += delta_y * pixY2angle * 0.01;

        thetaX += delta_x * pixX2angle * 0.01;
        if (thetaX > 2 * M_PI) thetaX -= 2 * M_PI;
        else if (thetaX < 0) thetaX += 2 * M_PI;
    }
    else if (status == 2 && R + (delta_y * 0.05) > 0) R += delta_y * 0.02;

    viewer[0] = R * cos(thetaX) * cos(thetaY) + lookPos[0];
    viewer[1] = R * sin(thetaY) + lookPos[1];
    viewer[2] = R * sin(thetaX) * cos(thetaY) + lookPos[2];
    gluLookAt(viewer[0], viewer[1], viewer[2], lookPos[0], lookPos[1], lookPos[2], 0.0, 1.0, 0.0);
}

void RenderScene(void){
    glColor3f(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    setCameraPos();
    drawSun();
    countPositions();
    drawOrbits();
    drawPlanets();
    
    glutTimerFunc(10, timeLoop, 1);
    glutSwapBuffers();
}

void MyInit(void)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    sunQ = gluNewQuadric();
    sunTexture = loadTexture("sun.tga");

    mercuryQ = gluNewQuadric();
    mercuryTexture = loadTexture("mercury.tga");

    venusQ = gluNewQuadric();
    venusTexture = loadTexture("venus.tga");
    
    earthQ = gluNewQuadric();
    earthTexture = loadTexture("earth.tga");

    marsQ = gluNewQuadric();
    marsTexture = loadTexture("mars.tga");
    
    jupiterQ = gluNewQuadric();
    jupiterTexture = loadTexture("jupiter.tga");

    saturnQ = gluNewQuadric();
    saturnTexture = loadTexture("saturn.tga");

    uranusQ = gluNewQuadric();
    uranusTexture = loadTexture("uranus.tga");
    
    neptuneQ = gluNewQuadric();
    neptuneTexture = loadTexture("neptune.tga");
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pixX2angle = 360.0 / (float)horizontal;
    pixY2angle = 360.0 / (float)vertical;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1.0, 0.1, 200.0);

    if (horizontal <= vertical) glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
    else glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keys(unsigned char key, int x, int y)
{
    switch (key) {
    case '0':
        lookAt = 0;
        break;

    case '1':
        lookAt = 1;
        break;

    case '2':
        lookAt = 2;
        break;

    case '3':
        lookAt = 3;
        break;

    case '4':
        lookAt = 4;
        break;

    case '5':
        lookAt = 5;
        break;

    case '6':
        lookAt = 6;
        break;

    case '7':
        lookAt = 7;
        break;

    case '8':
        lookAt = 8;
        break;

    case 'f':
        earthSpeed *= 1.5;
        countPlanetSpeed();
        break;

    case 's':
        earthSpeed /= 1.5;
        countPlanetSpeed();
        break;
    }
}

void Mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        x_pos_old = x;
        y_pos_old = y;

        // wcięnięty został lewy klawisz myszy
        status = 1;
    }
    else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        y_pos_old = y;

        // wcięnięty został prawy klawisz myszy
        status = 2;
    }
    else status = 0;          // nie został wcięnięty żaden klawisz
}

void Motion(GLsizei x, GLsizei y)
{   
    if (x_pos_old == x && y_pos_old == y) return;
    // obliczenie różnicy położenia kursora myszy
    delta_x = x - x_pos_old;
    delta_y = y - y_pos_old;

    // podstawienie bieżącego położenia jako poprzednie
    x_pos_old = x;
    y_pos_old = y;
}

void idle(void) {
    glutPostRedisplay();
}

void main(void) {
    std::cout << "Wojciech Kaczmarek, 252813 - Uklad Sloneczny\nSterowanie:\n";
    std::cout << "lewy przycisk myszy [pionowo i poziomo] - obrot wokol obiektu\nprawy przycisk myszy [pionowo] - przyblizanie i oddalanie\n";
    std::cout << "f - przyspieszenie czasu\ns - spowolnienie czasu\n";
    std::cout << "[0-8] - ustawienie kamery na ogladanie innego obiektu\n";
    std::cout << "0 - slonce\n1 - merkury\n2 - wenus\n3 - ziemia\n4 - mars\n5 - jowisz\n6 - saturn\n7 - uran\n8 - neptun\n";


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Uklad Sloneczny");
    glutDisplayFunc(RenderScene);
    glutIdleFunc(idle);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(keys);
    glutMotionFunc(Motion);
    MyInit();
    glutMainLoop();
}