
/****************************************************************************************/
/*                     isocede.cpp                    */
/****************************************************************************************/
/*         Affiche a l'ecran un cube en 3D         */
/****************************************************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <string>

// Déclaration de cubeActif comme variable globale
bool cubeActif = true; // Par défaut, le cube est actif

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
	//Constructeur
	Point(double x,double y, double z, float r, float g , float b) : x(x) , y(y) , z(z) , r(r) , g(g) , b(b) {}


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

  //Fonction pour calculer le point milieu d'une arete
	Point milieu(Point& A, Point& B, float r, float g, float b)
{
      double x = (B.x+A.x)/2;
      double y= (B.y+A.y)/2;
      double z = (B.z+A.z)/2;
      float r1=r;
      float g1=g;
      float b1=b;
      return Point(x,y,z,r1,g1,b1);

}

//Tableau pour stocker les sommets de l'icosaèdre
Point pico[12]={
   {milieu(pCube[0],pCube[1],1,1,1)},//m0
   {milieu(pCube[1],pCube[2],1,1,1)},//m1
   {milieu(pCube[2],pCube[3],1,1,1)},//m2
   {milieu(pCube[0],pCube[3],1,1,1)},//m3
   {milieu(pCube[0],pCube[4],1,1,1)},//m4
   {milieu(pCube[1],pCube[5],1,1,1)},//m5
   {milieu(pCube[2],pCube[6],1,1,1)},//m6
   {milieu(pCube[3],pCube[7],1,1,1)},//m7
   {milieu(pCube[4],pCube[5],1,1,1)},//m8
   {milieu(pCube[5],pCube[6],1,1,1)},//m9
   {milieu(pCube[6],pCube[7],1,1,1)},//m10
   {milieu(pCube[7],pCube[4],1,1,1)},//m11
};

int fico[20][3]={
    {0,2,1},//i0
    {2,0,3},//i1
    {0,5,4},//i2
    {4,5,8},//i3
    {1,9,5},//i4
    {1,6,9},//i5
    {2,7,6},//i6
    {7,10,6},//i7
    {3,4,11},//i8
    {11,7,3},//i9
    {8,9,10},//i10
    {10,11,8},//i11 //??
    {0,1,5},//i12(Deuxième Partie)
    {8,5,9},//i13
    {11,8,4},//i14
    {3,0,4},//i15
    {1,2,6},//i16
    {6,9,10},//i17
    {7,10,11},//i18
    {2,3,7},//i19
};

Point sommetIco(Point A,Point B,int axeX, int axeY,int axeZ , double c)
{
      Point p = milieu(A,B,1,0,0);
      if(axeX!=0)
      {
          if(axeX == 1){p.x = axeX*c;}
          else p.x = axeX*(-c);
      }
      if(axeY!=0)
      {
          if(axeY == 1){p.y = axeY*c;}
          else p.y = axeY*(-c);
      }
      if(axeZ!=0)
      {
          if(axeZ == 1){p.z = axeZ*c;}
          else p.z = axeZ*(-c);
      }

      return p;

};



Point Sico[12]={
   {sommetIco(pCube[0],pCube[1],0,1,0,1.5)},
   {sommetIco(pCube[1],pCube[2],0,1,0,1.5)},
   {sommetIco(pCube[2],pCube[3],0,1,0,1.5)},
   {sommetIco(pCube[3],pCube[0],0,1,0,1.5)},
   {sommetIco(pCube[0],pCube[4],0,1,0,1.5)},
   {sommetIco(pCube[1],pCube[5],0,1,0,1.5)},
   {sommetIco(pCube[2],pCube[6],0,1,0,1.5)},
   {sommetIco(pCube[3],pCube[7],0,1,0,1.5)},
   {sommetIco(pCube[4],pCube[5],0,1,0,1.5)},
   {sommetIco(pCube[5],pCube[6],0,1,0,1.5)},
   {sommetIco(pCube[6],pCube[7],0,1,0,1.5)},
   {sommetIco(pCube[7],pCube[4],0,1,0,1.5)},

};


char presse;
int anglex,angley,x,y,xold,yold;
int modeAffichageIco = 0;// 0 fil de fer , 1 plein , 2 sommet
char modeAffichageCube = 'f';


/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
Point milieu(Point A,Point B,float r, float g, float b);
void NotrreIcosaedre();
Point sommetIco(Point A,Point B,int axeX, int axeY,int axeZ , double c);
void changerModeAffichage(int nouveauMode);
void clavierCube(unsigned char touche, int x, int y);
void clavierIcosa(unsigned char touche, int x, int y);



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

void NotreIcosaedre()
{


   if (modeAffichageIco == 0) {
        // Mode fil de fer
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (modeAffichageIco == 1) {
        // Mode plein
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (modeAffichageIco == 2) {
        // Mode sommet
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glPointSize(5.0); // Ajustez la taille des sommets si nécessaire
    }
   for (int i = 0; i < 20; i++) {
    glBegin(GL_POLYGON);
    for (int j = 0; j < 3; j++) {
        int vertexIndex = fico[i][j];
        glColor3f(pico[vertexIndex].r, pico[vertexIndex].g, pico[vertexIndex].b);
        glVertex3f(pico[vertexIndex].x, pico[vertexIndex].y, pico[vertexIndex].z);
    }
    glEnd();

}

}
void NotreCube()
{

    if (modeAffichageCube == 'f') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Mode fil de fer pour le cube
    } else if (modeAffichageCube == 'p') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Mode plein pour le cube
    } else if (modeAffichageCube == 's') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // Mode sommet pour le cube
        glPointSize(5.0); // Ajustez la taille des sommets si nécessaire
    }
   // Dessin du cube
   int i,j;
  for (i=0;i<6;i++)
    {
      glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor3f(pCube[fCube[i][j]].r,pCube[fCube[i][j]].g,pCube[fCube[i][j]].b);
          glVertex3f(pCube[fCube[i][j]].x,pCube[fCube[i][j]].y,pCube[fCube[i][j]].z);
      }
      glEnd();


    }


}
/*void changerModeAffichage(int nouveauMode)
{
    modeAffichage = nouveauMode;
    glutPostRedisplay();//Demande d'une mise à jour de l'affichage
}
*/
void affichage()
{
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);





    NotreCube();

    //Appel de la fonction Notre Icosaedre pour dessiner l'icosaèdre
   NotreIcosaedre();
    // Vérifier l'orientation des faces de l'icosaèdre
   //glutSolidIcosahedron();

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

void clavier(unsigned char touche, int x, int y)
{
    switch (touche)
    {
    case '0': /* affichage du carre plein pour le cube */
        modeAffichageIco = 0;
        glutPostRedisplay();
        break;
    case '1': /* affichage en mode fil de fer pour le cube */
        modeAffichageIco = 1;
        glutPostRedisplay();
        break;
    case '2': /* affichage en mode sommets seuls pour le cube */
        modeAffichageIco = 2;
        glutPostRedisplay();
        break;
    case 'f':
         modeAffichageCube='f';
         glutPostRedisplay();
         break;
    case 'p':
         modeAffichageCube='p';
         glutPostRedisplay();
         break;
    case 's':
         modeAffichageCube='s';
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
    case 'a': // Les faces à l'envers du cube s'affichent en fil de fer
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_FRONT, GL_LINE);
        glutPostRedisplay();
        break;
    case 'q':
          exit(0);
    }
}


/* void clavierIcosa(unsigned char touche, int x, int y)
{
    switch (touche)
    {
    case '0': //Affichage de l'icosaèdre en fil de fer
        changerModeAffichage(0);
        glutPostRedisplay();
        break;
    case '1': // Affichage de l'icosaèdre en plein
        changerModeAffichage(1);
        glutPostRedisplay();
        break;
    case '2': // Affichage de l'icosaèdre en mode sommets
        changerModeAffichage(2);
        glutPostRedisplay();
        break;
    case 'q':
          exit(0);
    }
}
*/

/*void clavier(unsigned char touche, int x, int y)
{
    // Gestion du clavier pour le cube
    if (cubeActif) {
        clavierCube(touche, x, y);
    }
    // Gestion du clavier pour l'icosaèdre
    else {
        clavierIcosa(touche, x, y);
    }

    // Touche 'c' pour basculer entre le cube et l'icosaèdre
    if (touche == 'c') {
        cubeActif = !cubeActif;
    }
}*/

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
