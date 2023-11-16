/********************************************************/
/*                     lapinpro.cpp                     */
/********************************************************/
/*                Affiche a l'ecran un lapin en 3D      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>


#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif
using namespace std;

/*class Point*/
class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8]={
    {-0.5,-0.5, 0.5,1,1,1},
    { 0.5, -0.5, 0.5,1,1,1},
    { 0.5, -0.5, -0.5,1,1,1},
    { -0.5, -0.5, -0.5,11,1,1},
    { -0.5,0.5, 0.5,1,1,1},
    { 0.5, 0.5, 0.5,1,1,1},
    { 0.5, 0.5, -0.5,1,1,1},
    { -0.5, 0.5, -0.5,1,1,1}};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]={
  {0,3,2,1},
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7}};

char presse;
int anglex,angley,x,y,xold,yold;
int rayonInit2=0;
Point sp[8*2];
float zoomf =1.0;
const int largimg = 299;
const int hautimg = 299;
//unsigned char image[largimg*hautimg*3];
//unsigned char image2[largimg*hautimg*3];
unsigned char texture[largimg][hautimg][3];
int rayonInit = 0;
int incre=1;
int angleBouche = 0;
int increB=1;
const int maxR = 10;
bool tombe=false;
float carotteY=0;//pour gérer l'animation de la carotte
float lightAngle = 0.0f;
bool laissef = false;
bool anim = false;
int incre2=1;
GLuint tex1;//corps pieds ...
GLuint tex2;//cou et museau
GLuint tex3; //carotte
GLuint tex4; //tige
GLuint tex5;//sol
GLuint tex6;//ciel

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void Ellipsoïde(float x,float y,float z,int n);
void Cube();
void DemiEllipsoide(float x, float y, float z, int n,int s);
void conetronc(float rsup,float rinf,float h,int n);
void Cylindre(float x,float y,float z,int n);
void CylindreD(float x,float y,float z,int n);
void poils();//fonction pour les poils
void Corps();//fonction pour le corps
void Cou();//fonction pour le cou
void Museau();//fonction pour le museau
void Yeux();//fonction pour les yeux
void Oreilles();//fonction pour les oreilles
void dents();//fonction pour les dents
void mange();//fonction pour mettre la carotte dans la bouche du lapin
void pattes();//fonction pour les pattes de devant
void Queue();//fonction pour dessiner le queue
void Ground();//fonction pour dessiner la base
void Ciel();//fonction pour dessiner le ciel
void loadJpegImage(char * fichier);
void Manu(int key,int x,int y);
void animer();//fonction qui va animer les oreilles pour l'animation automatique

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("Lapin");
/* Initialisation d'OpenGL */
  glClearColor(0,0,0,0.0);
  //glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);
  /*Géneration des id pour les textures*/
  glGenTextures(1, &tex1);
glBindTexture(GL_TEXTURE_2D, tex1);
loadJpegImage("./finalTex.jpg"); // Chargez l'image pour tex1
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hautimg, largimg, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);


glGenTextures(1, &tex2);
glBindTexture(GL_TEXTURE_2D, tex2);
loadJpegImage("./texture_lapin.jpg"); // Chargez l'image pour tex2
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 299, 299, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);


glGenTextures(1, &tex3);
glBindTexture(GL_TEXTURE_2D, tex3);
loadJpegImage("./texCarotte.jpg"); // Chargez l'image pour tex3
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 299, 299, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

glGenTextures(1, &tex4);
glBindTexture(GL_TEXTURE_2D, tex4);
loadJpegImage("./texTige.jpg"); // Chargez l'image pour tex4
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 299, 299, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

glGenTextures(1, &tex5);
glBindTexture(GL_TEXTURE_2D, tex5);
loadJpegImage("./TextureGround.jpg"); // Chargez l'image pour tex5
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 299, 299, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);


glGenTextures(1, &tex6);
glBindTexture(GL_TEXTURE_2D, tex6);
loadJpegImage("./sky-blue.jpg"); // Chargez l'image pour tex6
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 299, 299, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);




  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutIdleFunc(idle);
  glutSpecialFunc(Manu);
  glutReshapeFunc(reshape);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void idle() {

    rayonInit += incre;
    if (rayonInit == 0)
        incre = 1;
    if (rayonInit == maxR)
        incre = -1;
    if (angleBouche == 25) {
        carotteY -= 0.05;// Ajustez le coefficient pour contrôler la vitesse de la chute

        // Vérifiez si la carotte a atteint la limite inférieure
        if (carotteY<-0.5) laissef = true;

    }
    if(laissef == true)
    {
    angleBouche-=0.1;
    carotteY -= 0.05;
    if (carotteY < -2.25)
     {
            carotteY = -2.25; // Réinitialisez la position de la carotte à la limite inférieure
     }
    }
animer();
   lightAngle += 0.06f;  // Ajustez la vitesse de rotation si nécessaire
GLfloat lightX = 1.0f * cos(lightAngle);  // Ajustez la distance si nécessaire
GLfloat lightZ = -1.0f * sin(lightAngle);  // Ajustez la distance si nécessaire

GLfloat lightPosition[] = {lightX, 0.0f, lightZ, 0.0f};
glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

// Configuration de la lumière directionnelle
GLfloat lightDirection[] = {lightX, 0.0f, lightZ, 0.0f};  // Direction de la lumière (vers le bas)
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Couleur de la lumière (blanche)
GLfloat lightAmbient[] = {1.0f, 0.8f, 0.0f, 1.0f};  // Couleur ambiante dorée
//GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Couleur spéculaire (blanche)

glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);  // Angle d'éclairage de 360 degrés pour une lumière directionnelle


glEnable(GL_LIGHT0);  // Activez la lumière GL_LIGHT0


    glutPostRedisplay();
}

void animer()
{
if(anim==true){
  rayonInit2 += incre2;
    if (rayonInit2 == 0)
        incre2 = 1;
    else if (rayonInit2 == maxR/4)
        incre2 = -1;
}
}

void Cube()
{
    glBindTexture(GL_TEXTURE_2D, tex5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Répéter horizontalement (U)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for (int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);
        int vertexIndex = fCube[i][0];

glTexCoord2f(0,0);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][1];

glTexCoord2f(0,1);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][2];

glTexCoord2f(1, 1);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][3];

glTexCoord2f(0, 1);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void CubeC()
{
    glBindTexture(GL_TEXTURE_2D, tex6);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Répéter horizontalement (U)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for (int i = 2; i < 6; i++) {
        glBegin(GL_POLYGON);
        int vertexIndex = fCube[i][0];

glTexCoord2f(0,0);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][1];

glTexCoord2f(0.5,0);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][2];

glTexCoord2f(0.5,0.5);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

vertexIndex = fCube[i][3];

glTexCoord2f(0, 0.5);
glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
void Ellipsoïde(int NP, int NM, float r,float a,float b,float c)
{


    Point * points = new Point[NP*NM];
    int faces[NM*(NP-1)][4];

    for(int j=0;j<NP;j++)
    {
        for(int i=0;i<NM;i++)
        {
            points[NM*j+i].x=a*r*cos(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].y=b*r*sin(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].z=c*r*sin((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].r= 0.5;
            points[NM*j+i].g= 0.5;
            points[NM*j+i].b= 0.5;
        }
    }

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            faces[j*NM+i][0]=j*NM+i;
            faces[j*NM+i][1]=NM*j+(i+1)%NM;
            faces[j*NM+i][2]=NM*(j+1)+(i+1)%NM;
            faces[j*NM+i][3]=NM*(j+1)+i;
        }
    }
    glBindTexture(GL_TEXTURE_2D,tex1);

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            glBegin(GL_POLYGON);
            // Calcul de la normale pour la face courante
            float normalX = points[faces[j*NM+i][0]].x;
            float normalY = points[faces[j*NM+i][0]].y;
            float normalZ = points[faces[j*NM+i][0]].z;

            // Calcul de la longueur de la normale
            float length = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);

            // Normalisation de la normale
            if (length > 0)
            {
                normalX /= length;
                normalY /= length;
                normalZ /= length;
            }

            glNormal3f(normalX, normalY, normalZ);

            glTexCoord2f(double(i)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][0]].x,points[faces[j*NM+i][0]].y,points[faces[j*NM+i][0]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][1]].x,points[faces[j*NM+i][1]].y,points[faces[j*NM+i][1]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][2]].x,points[faces[j*NM+i][2]].y,points[faces[j*NM+i][2]].z);
            glTexCoord2f(double(i)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][3]].x,points[faces[j*NM+i][3]].y,points[faces[j*NM+i][3]].z);
            glEnd();
        }
    }
    glBindTexture(GL_TEXTURE_2D,0);



}

void Cylindre(float x, float y, float z ,int n){
glBindTexture(GL_TEXTURE_2D,tex2);
for(int i=0; i<n; i++)
{
    double px =x*cos((2*i*M_PI)/n);
    double pz =-z*sin((2*i*M_PI)/n);
    Point p1;
    p1.x = px;
    p1.y = (y/2);
    p1.z=pz;
    Point p2;
    p2.x = px;
    p2.y = -(y/2);
    p2.z=pz;
    sp[i]=p1;
    sp[i+n]=p2;
}

for(int i=0; i<n;i++)
{
    double v1,v2;
    v1=(0.5*i)/n;
    v2=(0.5*(i+1))/n;
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
    glTexCoord2f(v1,0);
    glVertex3f(sp[(i+1)%n].x,sp[(i+1)%n].y,sp[(i+1)%n].z);
    glTexCoord2f(v1,v2);
    glVertex3f(sp[n+(i+1)%n].x,sp[n+(i+1)%n].y,sp[n+(i+1)%n].z);
    glTexCoord2f(0,v2);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
    glEnd();
}


glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
}
glEnd();
glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
}
glEnd();
glBindTexture(GL_TEXTURE_2D,0);
}
void CylindreD(float x, float y, float z ,int n){
for(int i=0; i<n; i++)
{
    double px =x*cos((2*i*M_PI)/n);
    double pz =-z*sin((2*i*M_PI)/n);
    Point p1;
    p1.x = px;
    p1.y = (y/2);
    p1.z=pz;
    Point p2;
    p2.x = px;
    p2.y = -(y/2);
    p2.z=pz;
    sp[i]=p1;
    sp[i+n]=p2;
}

for(int i=0; i<n;i++)
{
    double v1,v2;
    v1=(0.5*i)/n;
    v2=(0.5*(i+1))/n;
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
    glTexCoord2f(v1,0);
    glVertex3f(sp[(i+1)%n].x,sp[(i+1)%n].y,sp[(i+1)%n].z);
    glTexCoord2f(v1,v2);
    glVertex3f(sp[n+(i+1)%n].x,sp[n+(i+1)%n].y,sp[n+(i+1)%n].z);
    glTexCoord2f(0,v2);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
    glEnd();
}


glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
}
glEnd();
glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
}
glEnd();
}
void CylindreCarotte(float x, float y , float z , int n)
{
glBindTexture(GL_TEXTURE_2D,tex4);
for(int i=0; i<n; i++)
{
    double px =x*cos((2*i*M_PI)/n);
    double pz =-z*sin((2*i*M_PI)/n);
    Point p1;
    p1.x = px;
    p1.y = (y/2);
    p1.z=pz;
    Point p2;
    p2.x = px;
    p2.y = -(y/8);
    p2.z=pz;
    sp[i]=p1;
    sp[i+n]=p2;
}
for(int i=0; i<n;i++)
{

    glBegin(GL_POLYGON);

    glVertex3f(sp[i].x,sp[i].y,sp[i].z);

    glVertex3f(sp[(i+1)%n].x,sp[(i+1)%n].y,sp[(i+1)%n].z);

    glVertex3f(sp[n+(i+1)%n].x,sp[n+(i+1)%n].y,sp[n+(i+1)%n].z);

    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
    glEnd();
}
glBindTexture(GL_TEXTURE_2D,0);
glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
}
glEnd();
glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(1,1,1);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
}
glEnd();
}

void conetronc(float rsup, float rinf, float h, int n) {
glBindTexture(GL_TEXTURE_2D,tex1);
    // Stocker les sommets dans un tableau
    Point * points = new Point[2 * (n + 1)];

    // Définir les coordonnées de texture pour chaque sommet
    float textureStep = 1.0 / n;
    float v = 0.0;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; ++i) {
        double theta = 2.0 * M_PI * (double)(i) / n;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        double x1 = rsup * cosTheta;
        double y1 = rsup * sinTheta;
        double x2 = rinf * cosTheta;
        double y2 = rinf * sinTheta;

        // Sommet supérieur avec coordonnées de texture
        glTexCoord2f(i * textureStep, 1.0);
        glVertex3f(x1, y1, h * 0.5f);
        points[i].x = x1;
        points[i].y = y1;
        points[i].z = h * 0.5f;

        // Sommet inférieur avec coordonnées de texture
        glTexCoord2f(i * textureStep, 0.0);
        glVertex3f(x2, y2, -h * 0.5f);
        points[n + i].x = x2;
        points[n + i].y = y2;
        points[n + i].z = -h * 0.5f;

        v += textureStep;
    }
    glEnd();
glBindTexture(GL_TEXTURE_2D,0);
}
void conetroncC(float rsup, float rinf, float h, int n) {
glBindTexture(GL_TEXTURE_2D,tex3);
    // Stocker les sommets dans un tableau
    Point * points = new Point[2 * (n + 1)];

    // Définir les coordonnées de texture pour chaque sommet
    float textureStep = 1.0 / n;
    float v = 0.0;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; ++i) {
        double theta = 2.0 * M_PI * (double)(i) / n;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        double x1 = rsup * cosTheta;
        double y1 = rsup * sinTheta;
        double x2 = rinf * cosTheta;
        double y2 = rinf * sinTheta;

        // Sommet supérieur avec coordonnées de texture
        glTexCoord2f(i * textureStep, 1.0);
        glVertex3f(x1, y1, h * 0.5f);
        points[i].x = x1;
        points[i].y = y1;
        points[i].z = h * 0.5f;

        // Sommet inférieur avec coordonnées de texture
        glTexCoord2f(i * textureStep, 0.0);
        glVertex3f(x2, y2, -h * 0.5f);
        points[n + i].x = x2;
        points[n + i].y = y2;
        points[n + i].z = -h * 0.5f;

        v += textureStep;
    }
    glEnd();
glBindTexture(GL_TEXTURE_2D,0);
}
void Corps()
{

  glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0,0.8,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(40,40,1,1.2,0.8,0.9);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.4,1,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(40,40,1,1.4,0.8,0.7);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.5,0.9,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(40,40,1,0.9,0.8,0.7);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0,1.2,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(40,40,1,1,0.6,0.8);
    glPopMatrix();

    glPushMatrix();
     glColor3f(1,1,1);
     glTranslatef(-0.3,0.8,0);
     glRotatef(35,0,0,1);
     Ellipsoïde(50,50,1,1.2,1,0.9);
    glPopMatrix();

    //final part
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(-0.4,0.3,0.4);
      glRotatef(25,0,-1,0);
      glRotatef(-35,0,0,1);
      Ellipsoïde(50,50,1,1,0.5,0.4);
    glPopMatrix();
     glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(-0.4,0.3,-0.4);
      glRotatef(25,0,1,0);
      glRotatef(-35,0,0,1);
      Ellipsoïde(50,50,1,1,0.5,0.4);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.35-0.4,-0.4,-0.7);
      glRotatef(90,0,0,1);
      glRotatef(-60,0,0,1);
      Ellipsoïde(50,50,1,0.5,0.2,0.2);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.35-0.4,-0.4,0.7);
      glRotatef(90,0,0,1);
      glRotatef(-60,0,0,1);
      Ellipsoïde(50,50,1,0.5,0.2,0.2);
    glPopMatrix();
        glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.1,-0.7,0.7);
      Ellipsoïde(50,50,1,0.6,0.15,0.2);
    glPopMatrix();
    glPopMatrix();
        glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.1,-0.7,-0.7);
      Ellipsoïde(50,50,1,0.6,0.15,0.2);
    glPopMatrix();

}

void Cou()
{
    glPushMatrix();
    glColor3f(1,1,1);
     glTranslatef(0.4*cos((2*M_PI)),0.2,0);
     glTranslatef(0.9,0.75+1,0);
     glRotatef(-45,0,0,1);
     Cylindre(0.3,0.6,0.3,16);
    glPopMatrix();

}
void Museau()
{
  glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.4+1.4,0.8+1+0.4,0);
      glRotatef(-45,0,0,1);
      Ellipsoïde(20,20,1,0.7,0.5,0.6);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1,1,1);
     glTranslatef(0.4+1.4,0.8+1+0.25,0);
     glTranslatef(0.5,-0.2,0);
     glRotatef(90,0,1,0);
     glRotatef(35,1,0,0);
       conetronc(0.15,0.43,0.5,6);
    glPopMatrix();
    glPushMatrix();//b
     glTranslatef(0.4+1.4+0.7,0.8+1,0);
     Ellipsoïde(20,20,1,0.2,0.1,0.1);
     glPopMatrix();
     glPushMatrix();
     glColor3f(1,1,1);
     glTranslatef(0.3+1.4+0.7,0.7+1,0.1);
     glRotatef(-45,0,0,1);
     Ellipsoïde(20,20,1,0.2,0.1,0.3);
     glPopMatrix();
     glPushMatrix();
     glColor3f(1,1,1);
     glTranslatef(0.3+1.4+0.7,0.7+1,-0.1);
     glRotatef(-45,0,0,1);
     Ellipsoïde(20,20,1,0.2,0.1,0.3);
     glPopMatrix();
     //machoire inf
     glPushMatrix();
     glColor3f(1,1,1);
     glPushMatrix();
     glTranslatef(-0.05+1.4+0.7,0.55+1,0);
     glRotatef(-angleBouche,0,0,1);
     glTranslatef(0.05,-0.02,0);
     glRotatef(-10,0,0,1);
     glRotatef(-25,0,0,1);
     Ellipsoïde(20,20,1,0.3,0.1,0.3);
     glPopMatrix();
     glPopMatrix();
     //nez
     glPushMatrix();
     glTranslatef(0.5+1.4+0.7,0.7+1,0);
     Ellipsoïde(20,20,1,0.1,0.1,0.1);
     glPopMatrix();
     //joux
     glPushMatrix();
     glColor3f(1,1,1);
     glTranslatef(0.4+1.4+0.3,0.8+1+0.2,0.3);
     glRotatef(-45,0,0,1);
     Ellipsoïde(20,20,1,0.4,0.3,0.3);
     glPopMatrix();
     glPushMatrix();
     glColor3f(1,1,1);
     glTranslatef(0.4+1.4+0.3,0.8+1+0.2,-0.3);
     glRotatef(-45,0,0,1);
     Ellipsoïde(20,20,1,0.4,0.3,0.3);
     glPopMatrix();

}
void Yeux()
{
 glPushMatrix();
      glColor3f(0,0,0);
      glTranslatef(0.2+0.4+1.4,0.8+1+0.6,-0.3);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
     glPushMatrix();
      glColor3f(0,0,0);
      glTranslatef(0.2+0.4+1.4,0.8+1+0.4+0.2,0.3);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
}
void Oreilles()
{
glPushMatrix();
  glRotatef(-rayonInit2,1,1,0);
  glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.2+1.4,0.8+1+0.4+0.2,-0.4);
    glTranslatef(0,0.3+0.6,0);
    glRotatef(40,0,1,0);
    glRotatef(-20,0,0,1);//last modification
    glRotatef(-10,1,0,0);
    Ellipsoïde(20,20,1,0.1,1,0.3);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glRotatef(rayonInit2,1,1,0);
    glColor3f(1,1,1);
    glTranslatef(0.2+1.4,0.8+1+0.4+0.2,0.4);
    glTranslatef(0,0.3+0.6,0);
    glRotatef(-40,0,1,0);
    glRotatef(-20,0,0,1);//same
    glRotatef(10,1,0,0);
    Ellipsoïde(20,20,1,0.1,1,0.3);
    glPopMatrix();
glPopMatrix();
}
void dents()
{
   glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.3+1.4+0.7,0.55+1,-0.12);
      glRotatef(-45,0,0,1);
      CylindreD(0.05,0.1,0.1,8);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.3+1.4+0.7,0.55+1,0.12);
      glRotatef(-45,0,0,1);
      CylindreD(0.05,0.1,0.1,8);
    glPopMatrix();
}
void Carotte()
{
  glPushMatrix();
  glTranslatef(0,carotteY,0);
  glColor3f(0.9569,0.40,0.1059);
  conetroncC(0.15,0.001,1.5,5);
    glPushMatrix();
      glRotatef(90,1,0,0);
      glTranslatef(0,0.7,0);
      for (int i = 0; i < 8; i++)
    {
        glColor3f(0.3, 1, 0.2);
        glRotatef(-i , 1, 1, 1);  // Applique une rotation individuelle
        CylindreCarotte(0.01, 0.3, 0.01, 8);
    }
    for (int i = 0; i < 8; i++)
    {
        glColor3f(0.3, 1, 0.2);
        glRotatef(i , 1, 0, 1);
        glRotatef(i , 0, 0, 1);  // Applique une rotation individuelle
        CylindreCarotte(0.01, 0.3, 0.01, 8);
    }
    for (int i = 0; i < 60; i++)
    {
        glColor3f(0.3, 1, 0.2);
        glRotatef(i , 1, 1, 0);
        glRotatef(i , 0, 1, 0);  // Applique une rotation individuelle
        glRotatef(i/2 , 1, 0, 0);  // Applique une rotation individuelle
        CylindreCarotte(0.01, 0.5, 0.01, 8);
    }
     glPopMatrix();
  glPopMatrix();
}
void mange()
{
   glPushMatrix();
     glTranslatef(0.2+1.4+0.7,0.55+1,0.2);
    Carotte();
   glPopMatrix();
}
void poils()
{
    float a = 0.01;

    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.36 + 1.4 + 0.7 + (i * a), 2 + (i * a), -(0.15 + (i * a)));
        glRotatef(-45, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, -a - asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }
    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.4 + 1.4 + 0.7 + (i * a), 1.9 + (i * a), -(0.17 + (i * a)));
        glRotatef(-45, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, -a - asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }
    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.5 + 1.4 + 0.7 + (i * a), 1.8 + (i * a), -(0.2 + (i * a)));
        glRotatef(-45, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, -a - asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }

    //SYMETRIE
    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.36 + 1.4 + 0.7 + (i * a), 2 + (i * a), (0.15 + (i * a)));
        glRotatef(130, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, -a +asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }
    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.4 + 1.4 + 0.7 + (i * a), 1.9 + (i * a), (0.17 + (i * a)));
        glRotatef(130, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, a + asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }
    for (int i = 1; i < 50; i++) {
        glPushMatrix();
        glTranslatef(0.5 + 1.4 + 0.7 + (i * a), 1.8 + (i * a), (0.2 + (i * a)));
        glRotatef(135, 0, 0, 1);
        glRotatef(-50, 1, 0, 0);
        glTranslatef(0, 0, a + asin(i * 0.05) * 0.2);
        glColor3f(1, 1, 1);
        CylindreD(0.02, 0.04, 0.02, 8);
        glPopMatrix();
    }
}
void pattes()
{

  glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(1,0.7,-0.35);
      glRotatef(25,0,-1,0);
      glRotatef(-15,0,0,1);
      Ellipsoïde(20,20,1,0.3,0.7,0.3);
    glPopMatrix();
 glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.9,0.7-0.9,-0.4);
    glRotatef(10,0,0,1);
    Ellipsoïde(20,20,1,0.2,0.5,0.2);
 glPopMatrix();
 glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(-5,0,0,1);
   glTranslatef(1,0.7-1.3,-0.4);
   Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(-5,0,0,1);
   glTranslatef(1,0.7-1.3,-0.4);
   glRotatef(40,0,1,0);
  Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(-5,0,0,1);
   glTranslatef(1,0.7-1.3,-0.4);
   glRotatef(-40,0,1,0);
   Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();




glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(1,0.7,0.35);
      glRotatef(25,0,1,0);
      glRotatef(-15,0,0,1);
      Ellipsoïde(20,20,1,0.3,0.7,0.3);
    glPopMatrix();
 glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.9,0.7-0.9,0.4);
    glRotatef(25,0,0,1);
    Ellipsoïde(20,20,1,0.2,0.5,0.2);
 glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(25,0,0,1);
   glTranslatef(0.8,0.7-0.9-0.89,0.4);
   glRotatef(40,0,1,0);
  Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();
 glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(25,0,0,1);
   glTranslatef(0.8,0.7-0.9-0.89,0.4);
   glRotatef(-40,0,1,0);
   Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0.9);
   glRotatef(25,0,0,1);
   glTranslatef(0.8,0.7-0.9-0.89,0.4);
   Ellipsoïde(16,16,1,0.2,0.1,0.1);
glPopMatrix();



/*
glPushMatrix();
   glColor3f(1,0,0);
   glTranslatef(-0.3,0.8-1,0.3);
   glRotatef(40,0,1,0);
   DemiEllipsoide(0.2,0.4,0.2,16,16);
glPopMatrix();
glPushMatrix();
   glColor3f(1,0,0);
   glTranslatef(-0.3,0.8-1,-0.3);
   glRotatef(-40,0,1,0);
   DemiEllipsoide(0.2,0.4,0.2,16,16);
glPopMatrix();


glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,0.3);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,0.3);
   glRotatef(40,0,1,0);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,0.3);
   glRotatef(-40,0,1,0);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();


glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,-0.3);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,-0.3);
   glRotatef(40,0,1,0);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();
glPushMatrix();
   glColor3f(1,1,0);
   glTranslatef(-0.3,0.8-1-0.4,-0.3);
   glRotatef(-40,0,1,0);
   DemiEllipsoide(0.2,0.1,0.1,16,16);
glPopMatrix();
*/
}
void Queue()
{
glPushMatrix();
glRotatef(-rayonInit,0,0,1);
glPushMatrix();
  glColor3f(1,1,1);
  glTranslatef(-0.4-1.2,0.3,0);
  Ellipsoïde(20,20,1,0.4,0.4,0.4);
glPopMatrix();
glPushMatrix();
  glColor3f(1,1,1);
  glTranslatef(-0.6-1.2,0.2,0);
  Ellipsoïde(20,20,1,0.5,0.3,0.4);
glPopMatrix();
glPushMatrix();
  glColor3f(1,1,1);
  glTranslatef(-0.6-1.2,0.3,0);
  Ellipsoïde(20,20,1,0.4,0.3,0.4);
glPopMatrix();
glPopMatrix();
}
void Ground()
{

 glPushMatrix();
 glTranslatef(0,-1.1,0);
 glScalef(10,0.5,10);
 Cube();
 glPopMatrix();
}
void Ciel()
{

 glPushMatrix();
 glTranslatef(0,3.5,-1.5);
 glScalef(10,10,6);
 CubeC();
 glPopMatrix();
}
//***********************end functions*******************


void affichage()
{
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glShadeModel(GL_SMOOTH);
   glEnable(GL_TEXTURE_2D);
  glLoadIdentity();

  //glMatrixMode(GL_PROJECTION);

  glOrtho(-5,5,-5,5,-5,5);
  glScalef(zoomf,zoomf,1);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //GLfloat lightPosition[] = {2.0f, 0.0f, -2.0f, 1.0f}; // Coordonnées de la lumière
 //GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Couleur diffuse de la lumière

//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
//GLfloat objectDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Couleur diffuse de l'objet
//glMaterialfv(GL_FRONT, GL_DIFFUSE, objectDiffuse);

  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);


     //corps
    Corps();
    //cou
    Cou();
    //Museau
    Museau();
    //poils
    poils();
    //Les yeux
    Yeux();
    //oreilles
    Oreilles();
    //dents
    dents();
    //pattes
    pattes();
    //Queue
    Queue();
    mange();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    Ground();
    Ciel();

  glFlush();

  //On echange les buffers
  glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'z':
        zoomf-=0.2f;
        glutPostRedisplay();
        break;
    case 'Z':
        zoomf+=0.2f;
        glutPostRedisplay();
        break;
    case 'b':
        if(angleBouche<25) {
        angleBouche++;
        }
        glutPostRedisplay();
        break;
    case 'B':
         if(angleBouche>-30){
         angleBouche--;
         }
        glutPostRedisplay();
        break;
    case 'i':
         anim = true;
    break;
    case 'I':
         anim = false;
    break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    }
}
void Manu(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:
        angley+=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_UP:
        angley-=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_LEFT:
        anglex-=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_RIGHT:
        anglex+=5;
        glutPostRedisplay();
        break;
        default:
	  break;
    }
}


void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
void loadJpegImage(char *fichier)
{
    unsigned char* image = (unsigned char*) (malloc(sizeof(char) * 3 * 299 * 299));

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *file;
    unsigned char *ligne;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    #ifdef _WIN32
    if (fopen_s(&file, fichier, "rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
    #elif __GNUC__
    if ((file = fopen(fichier,"rb")) == 0)
    {
        fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
        exit(1);
    }
    #endif
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);


    if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
        fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
        exit(1);
    }

    jpeg_start_decompress(&cinfo);
    ligne=image;
    while (cinfo.output_scanline<cinfo.output_height)
    {
        ligne=image+3*299*cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo,&ligne,1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    //boucle qui permet de faire la texture
    for(int i = 0; i<299 ; i++)
    {
        for(int j = 0; j<299; j++)
        {
            texture[i][j][0] = image[i*299*3+j*3];
            texture[i][j][1] = image[i*299*3+j*3+1];
            texture[i][j][2] = image[i*299*3+j*3+2];
        }
    }

    free(image);
}

