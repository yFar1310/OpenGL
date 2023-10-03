/****************************************************************************************/
/*                     BrasRobot.cpp                    */
/****************************************************************************************/
/*         Affiche a le bras du robot en 3D         */
/****************************************************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>



char presse;
int anglex,angley,x,y,xold,yold;

GLUquadric* quad = gluNewQuadric();
double d = 0.05+((2*0.1)+(0.0625)+0.5+0.1)/2;
double angle = 0;
double angle1 = 0;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void socle();
void rotule1();
void bras();
void rotule2();
void avantBras();
void pince();
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutInitWindowSize(500,500);
  glutCreateWindow("BrasRobot");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

  /* enregistrement des fonctions de rappel */
  glutIdleFunc(idle);
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void socle()
{
   glPushMatrix();
     glScalef((0.7),(0.125),(0.2));
     glutSolidCube(1);
   glPopMatrix();
}

void rotule1()
{
   glPushMatrix();
     glColor3f(0.3,0.4,0.8);
     glTranslatef(0,(0.1625),0);// y=r+(y(socle)/2)
     glutSolidSphere((0.1),16,16);
   glPopMatrix();
}
void bras()
{
   glPushMatrix();
     glColor3f(0.9,0.5,0.2);
     glTranslatef(0,0,0);
     glRotatef(-90,1,0,0);
     glTranslatef(0,0,(0.2625));
     //glutSolidCone(0.1,4,4,4);
     gluCylinder(quad,(0.1),(0.1),(0.5),16,1);
   glPopMatrix();
}
void rotule2()
{
 glPushMatrix();
     glColor3f(0.3,0.4,0.8);
     glTranslatef(0,  ((2*0.1)+(0.0625)+0.5+0.1),0);// y=2*r+(y(socle)/2)+cylindreHeight//+r
     glutSolidSphere((0.1),16,16);
   glPopMatrix();
}
void avantBras()
{
glPushMatrix();
     glColor3f(0,1,1);
     glTranslatef(0,0,0);
     glRotatef(-90,1,0,0);
     glTranslatef(0,0,0.1+((2*0.1)+(0.0625)+0.5+0.1)); //r + (2*r+(y(socle)/2)+cylindreHeight//+r)
     //glutSolidCone(0.1,4,4,4);
     gluCylinder(quad,(0.1),(0.1),(0.5),16,1);
   glPopMatrix();
}

void pince()
{
   glPushMatrix();
     glColor3f(0,1,0);
     glTranslatef(0,0.1+((2*0.1)+(0.0625)+0.5+0.1)+0.5+(0.125)/4,0);
     glScalef((0.3),(0.125)/2,(0.1));
     glutSolidCube(1);
   glPopMatrix();
   glPushMatrix();
      glColor3f(1,1,0);
      glTranslatef(-(0.3)/2 + 0.06/2,(0.125)/4+(0.3/2)+0.1+((2*0.1)+(0.0625)+0.5+0.1)+0.5+(0.125)/4,0);
      glScalef(0.06,0.3,(0.1));
      glutSolidCube(1);
   glPopMatrix();
   glPushMatrix();
      glColor3f(1,0,0);
      glTranslatef((0.3)/2 - 0.06/2,(0.125)/4+(0.3/2)+0.1+((2*0.1)+(0.0625)+0.5+0.1)+0.5+(0.125)/4,0);
      glScalef(0.06,0.3,(0.1));
      glutSolidCube(1);
   glPopMatrix();
}

void idle()
{
              angle++;
              glutPostRedisplay();
}

void affichage()
{

  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-3,3,-3,2,-2,2);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  socle();
  rotule1();
  bras();
  rotule2();
  //glRotatef(angle,0,1,0);
  glPushMatrix();
  glTranslatef(0,(0.1+((2*0.1)+(0.0625)+0.5+0.1))-0.1,0);
  glRotatef(angle1,1,0,0);
  glTranslatef(0,-(0.1+((2*0.1)+(0.0625)+0.5+0.1))+0.1,0);
  avantBras();
  pince();
  glPopMatrix();


   // Dessin du cube
  /*for (int i=0;i<6;i++)
    {
      glBegin(GL_POLYGON);
      for (int j=0;j<4;j++){
          glColor3f(pCube[fCube[i][j]].r,pCube[fCube[i][j]].g,pCube[fCube[i][j]].b);
          glVertex3f(pCube[fCube[i][j]].x,pCube[fCube[i][j]].y,pCube[fCube[i][j]].z);
      }
      glEnd();
    }*/




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
    case 'a'://Les faces à l'envers s'affichent en fil de fer
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_FRONT,GL_LINE);
      glutPostRedisplay();
    case 'R':
       if(angle1<=90){
       angle1++;
       glutPostRedisplay();
       }
       break;
    case 'r':
       if(angle1>=-90)
       {
        angle1--;
        glutPostOverlayRedisplay();
       }
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
