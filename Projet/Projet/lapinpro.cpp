/********************************************************/
/*                     lapinpro.cpp                                                 */
/********************************************************/
/*                Affiche a l'ecran un lapin en 3D                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <cmath>

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
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0}};

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
Point sp[8*2];
float zoomf =1.0;

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
void poils();
void Corps();
void Cou();
void Museau();
void Yeux();
void Oreilles();
void dents();
void mange();


int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("cube");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}
void Cube()
{

   for (int i=0;i<6;i++)
    {
      glBegin(GL_POLYGON);
      for (int j=0;j<3;j++){
          glColor3f(pCube[fCube[i][j]].r,pCube[fCube[i][j]].g,pCube[fCube[i][j]].b);
          glVertex3f(pCube[fCube[i][j]].x,pCube[fCube[i][j]].y,pCube[fCube[i][j]].z);
      }
      glEnd();
    }

}

void Ellipsoïde(float x , float y , float z, int n)
{
   glBegin(GL_QUADS);

    for (int i = 0; i < n; i++) {
        float t1 = 2.0f * M_PI * i / n;
        float t1_next = 2.0f * M_PI * (i + 1) / n;

        for (int j = 0; j < n; j++) {
            float t2 = M_PI * j / n;
            float t2_next = M_PI * (j + 1) / n;

            // Define the vertices of each quad
            float x1 = x * cos(t1) * sin(t2);
            float y1 = y * sin(t1) * sin(t2);
            float z1 = z * cos(t2);

            float x2 = x * cos(t1_next) * sin(t2);
            float y2 = y * sin(t1_next) * sin(t2);
            float z2 = z * cos(t2);

            float x3 = x * cos(t1_next) * sin(t2_next);
            float y3 = y * sin(t1_next) * sin(t2_next);
            float z3 = z * cos(t2_next);

            float x4 = x * cos(t1) * sin(t2_next);
            float y4 = y * sin(t1) * sin(t2_next);
            float z4 = z * cos(t2_next);

            // Define the quad by specifying its vertices
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glVertex3f(x4, y4, z4);
        }
    }

    glEnd();
}

void DemiEllipsoide(float a, float b,float c,int n,int s) {
    for (int i = 0; i < n; ++i) {
        double phi1 = M_PI * (-0.5 + (double)(i) / n);
        double phi2 = M_PI * (-0.5 + (double)(i + 1) / n);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= n; ++j) {
            double theta = 2.0 * M_PI * (double)(j) / n;

            double x1 = a * cos(theta) * cos(phi1);
            double y1 = b * sin(theta) * cos(phi1);
            double z1 = c * sin(phi1);

            double x2 = a * cos(theta) * cos(phi2);
            double y2 = b * sin(theta) * cos(phi2);
            double z2 = c * sin(phi2);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}

void Cylindre(float x, float y, float z ,int n){
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
    //v1=(0.5*i)/n;
    //v2=(0.5*(i+1))/n;
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
    glTexCoord2f(0.5,0);
    glVertex3f(sp[(i+1)%n].x,sp[(i+1)%n].y,sp[(i+1)%n].z);
    glTexCoord2f(0.5,0.5);
    glVertex3f(sp[n+(i+1)%n].x,sp[n+(i+1)%n].y,sp[n+(i+1)%n].z);
    glTexCoord2f(0,0.5);
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
    double v1,v2;
    //v1=(0.5*i)/n;
    //v2=(0.5*(i+1))/n;
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
    glTexCoord2f(0.5,0);
    glVertex3f(sp[(i+1)%n].x,sp[(i+1)%n].y,sp[(i+1)%n].z);
    glTexCoord2f(0.5,0.5);
    glVertex3f(sp[n+(i+1)%n].x,sp[n+(i+1)%n].y,sp[n+(i+1)%n].z);
    glTexCoord2f(0,0.5);
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

void conetronc(float rsup, float rinf, float h,int n){


    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; ++i) {
        double theta = 2.0 * M_PI * (double)(i)/n;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        double x1 = rsup * cosTheta;
        double y1 = rsup * sinTheta;
        double x2 = rinf * cosTheta;
        double y2 = rinf * sinTheta;

        // Sommet supérieur
        glVertex3f(x1, y1, h * 0.5f);
        // Sommet inférieur
        glVertex3f(x2, y2, -h * 0.5f);
    }
    glEnd();

}




void Corps()
{

  glPushMatrix();
     glColor3f(0.345,0.16,0);
    glTranslatef(0,0.8,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(1.2,0.8,0.9,20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.345,0.16,0);
    glTranslatef(0.4,1,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(1.4,0.8,0.7,20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.345,0.16,0);
    glTranslatef(0.5,0.9,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(0.9,0.8,0.7,20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.345,0.16,0);
    glTranslatef(0,1.2,0);
    glRotatef(45,0,0,1);
    Ellipsoïde(1,0.6,0.8,20);
    glPopMatrix();
    glPushMatrix();
     glColor3f(0.345,0.16,0);
     glTranslatef(-0.3,0.8,0);
     glRotatef(35,0,0,1);
     Ellipsoïde(1.2,1,0.9,20);
    glPopMatrix();


}

void Cou()
{
    glPushMatrix();
     glTranslatef(0.4*cos((2*M_PI)),0.2,0);
     glTranslatef(0.9,0.75+1,0);
     glRotatef(-45,0,0,1);
     Cylindre(0.3,0.2,0.3,16);
    glPopMatrix();

}

void Museau()
{
  glPushMatrix();
      glColor3f(0.5,0,1);
      glTranslatef(0.4+1.4,0.8+1+0.4,0);
      glRotatef(-45,0,0,1);
      Ellipsoïde(0.7,0.5,0.6,16);
    glPopMatrix();
    glPushMatrix();
      glColor3f(0.5,0,1);
     glTranslatef(0.4+1.4,0.8+1+0.25,0);
     glTranslatef(0.5,-0.2,0);
     glRotatef(90,0,1,0);
     glRotatef(35,1,0,0);
       conetronc(0.15,0.43,0.5,6);
    glPopMatrix();
    glPushMatrix();//b
     glTranslatef(0.4+1.4+0.7,0.8+1,0);
     Ellipsoïde(0.2,0.1,0.1,8);
     glPopMatrix();
     glPushMatrix();
     glColor3f(1,1,0);
     glTranslatef(0.3+1.4+0.7,0.7+1,0.1);
     glRotatef(-45,0,0,1);
     Ellipsoïde(0.2,0.1,0.3,8);
     glPopMatrix();
     glPushMatrix();
     glColor3f(1,1,0);
     glTranslatef(0.3+1.4+0.7,0.7+1,-0.1);
     glRotatef(-45,0,0,1);
     Ellipsoïde(0.2,0.1,0.3,8);
     glPopMatrix();
     //machoire inf
     glPushMatrix();
     glColor3f(1,0,0);
     glTranslatef(0.05+1.4+0.7,0.55+1,0);
     glRotatef(-10,0,0,1);
     glRotatef(-25,0,0,1);
     Ellipsoïde(0.2,0.1,0.3,8);
     glPopMatrix();
     //nez
     glPushMatrix();
     glTranslatef(0.5+1.4+0.7,0.7+1,0);
     Ellipsoïde(0.1,0.1,0.1,8);
     glPopMatrix();
     //joux
     glPushMatrix();
     glColor3f(0.5,0,1);
     glTranslatef(0.4+1.4+0.3,0.8+1+0.2,0.3);
     glRotatef(-45,0,0,1);
     Ellipsoïde(0.4,0.3,0.3,8);
     glPopMatrix();
     glPushMatrix();
     glColor3f(0.5,0,1);
     glTranslatef(0.4+1.4+0.3,0.8+1+0.2,-0.3);
     glRotatef(-45,0,0,1);
     Ellipsoïde(0.4,0.3,0.3,8);
     glPopMatrix();

}
void Yeux()
{
 glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.2+0.4+1.4,0.8+1+0.6,-0.3);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
     glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.2+0.4+1.4,0.8+1+0.4+0.2,0.3);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
}

void Oreilles()
{




  glPushMatrix();
    glColor3f(1,0.16,0.8);
    glTranslatef(0.2+1.4,0.8+1+0.4,-0.4);
    glTranslatef(0,0.3+0.6,0);
    glRotatef(40,0,1,0);
    glRotatef(-20,0,0,1);//last modification
    glRotatef(-10,1,0,0);
    Ellipsoïde(0.1,1,0.3,20);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,0.16,0.8);
    glTranslatef(0.2+1.4,0.8+1+0.4,0.4);
    glTranslatef(0,0.3+0.6,0);
    glRotatef(-40,0,1,0);
    glRotatef(-20,0,0,1);//same
    glRotatef(10,1,0,0);
    Ellipsoïde(0.1,1,0.3,20);
    glPopMatrix();
}
void dents()
{
   glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.3+1.4+0.7,0.55+1,-0.12);
      glRotatef(-45,0,0,1);
      Cylindre(0.05,0.1,0.1,8);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(0.3+1.4+0.7,0.55+1,0.12);
      glRotatef(-45,0,0,1);
      Cylindre(0.05,0.1,0.1,8);
    glPopMatrix();
}

void Carotte()
{
  glPushMatrix();
  glColor3f(0.9569,0.40,0.1059);
  conetronc(0.1,0.001,1,5);
    glPushMatrix();
      glRotatef(90,1,0,0);
      glTranslatef(0,0.5,0);
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
//***********************end functions*******************


void affichage()
{
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  glOrtho(-5,5,-5,5,-5,5);
  glScalef(zoomf,zoomf,1);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);




     //corps
     Corps();
    //cou
    Cou();
    //Museau
    Museau();
    //Les yeux
    Yeux();
    //oreilles
    Oreilles();
    //dents
    dents();
    //Carotte
    Carotte();
    //Carotte dans la bouche
    mange();



for (int i=1;i<20;i++) {
float a=0.01;
float angle = 10;
float b=0.02;
glPushMatrix();
if(i<10){
 glTranslatef(0.3+1.4+0.7+(i*a)    ,2  +(i*a)   ,-(0.2+(i*a)));
     glRotatef(-50,0,0,1);
     glRotatef(-45,1,0,0);
 Cylindre(0.02,0.02,0.02,8);
 }

else{
glTranslatef(0.3+1.4+0.7+(i*a)    ,2  +(i*a)-1/i   ,-(0.2+(i*a)));
     glRotatef(-50,0,0,1);
 Cylindre(0.02,0.02,0.02,8);
angle+=10;
     glRotatef(angle,1,0,0);
}
glPopMatrix();}

/*

     /*glPushMatrix();
     glColor3f(0,1,0.5);
    glTranslatef(0,0.8,0);
    glRotatef(25,0,0,1);
    Ellipsoïde(1.2,0.6,0.9,20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,0);
    Ellipsoïde(1,1,1,10);
    glPopMatrix();*/

    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

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
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
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
