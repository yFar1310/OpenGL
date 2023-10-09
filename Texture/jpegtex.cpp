#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif
using namespace std;

class Point{
public:
    double x;
    double y;
    double z;
};
const int largimg = 256;
const int hautimg = 256;
const int n=6;
const double h = 1;
unsigned char image[largimg*hautimg*3];
unsigned char texture[largimg * 2][hautimg * 2][3];


char presse;
int anglex=30,angley=20,x,y,xold,yold;
Point sp[n*2];

void affichage();
void clavier(unsigned char touche,int x,int y);
void souris(int boutton, int etat,int x,int y);
void sourismouv(int x,int y);
void redim(int l,int h);
void loadJpegImage(char *fichier);

int main(int argc,char **argv)
{
  /*appel de la fonction loadJpegImage*/
  loadJpegImage("./calimero.jpg");
  /* Creation de la fenetre OpenGL */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(400,400);
  glutCreateWindow("Texture JPEG");
  /* Initialisation de l'etat d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1,1,1);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  /* Mise en place de la projection perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,1,1.0,5.0);
  glMatrixMode(GL_MODELVIEW);
  /* Parametrage du placage de textures */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,largimg*2,hautimg*2,0,
	       GL_RGB,GL_UNSIGNED_BYTE,texture);
  glEnable(GL_TEXTURE_2D);
  /* Mise en place des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutMouseFunc(souris);
  glutMotionFunc(sourismouv);
  glutReshapeFunc(redim);
  /* Entr�e dans la boucle principale glut */
  glutMainLoop();
  return 0;
}



void affichage()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  gluLookAt(0.0,0.0,2.5,0.0,0.0,0.0,0.0,1.0,0.0);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);


for(int i=0; i<n; i++)
{
    double px =0.5*cos((2*i*M_PI)/n);
    double pz =-0.5*sin((2*i*M_PI)/n);
    Point p1;
    p1.x = px;
    p1.y = (h/2);
    p1.z=pz;
    Point p2;
    p2.x = px;
    p2.y = -(h/2);
    p2.z=pz;
    sp[i]=p1;
    sp[i+n]=p2;
}
for(int i=0; i<n;i++)
{
    //double v1,v2;
    //v1=(0.5*i)/n; on ajoute v1 pour gérer le point issu du découpage 
    //v2=(0.5*(i+1))/n; on ajoute v2 pour gérer le point symétriquede v1 issu du découpage 
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
    glColor3f(0,0,1);
    glVertex3f(sp[i].x,sp[i].y,sp[i].z);
}
glEnd();
glBegin(GL_POLYGON);
for(int i=0; i<n;i++)
{
    glColor3f(0,0,1);
    glVertex3f(sp[i+n].x,sp[i+n].y,sp[i+n].z);
}
glEnd();

glColor3f(1,1,1);

  /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glBegin(GL_POLYGON);

  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5, 0.5);
  glEnd();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0,0.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);
  glEnd();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  glBegin(GL_POLYGON);
  glTexCoord2f(0,0);   glVertex3f( 0.5, 0.5,-0.5);
  glTexCoord2f(0,3);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3,3);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,0.5);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0.5);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.0);   glVertex3f(-0.5, 0.5, 0.5);
  glEnd();


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);
  glEnd();
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,1);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(0,1);   glVertex3f( 0.5,-0.5,-0.5);
  glEnd();
  */
  glutSwapBuffers();

}



void clavier(unsigned char touche,int x,int y)
{
  switch(touche) {
  case 'l':
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glutPostRedisplay();
    break;
  case 'n':
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glutPostRedisplay();
  break;

  case 27: /* touche ESC */
    exit(0);
  default:
	  break;
  }
}

void souris(int bouton, int etat,int x,int y)
{
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
  {
    presse = 1;
    xold = x;
    yold=y;
  }
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
    presse=0;
}

void sourismouv(int x,int y)
  {
    if (presse)
    {
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay();
    }

    xold=x;
    yold=y;
  }

void redim(int l,int h)
{
  if (l<h)
    glViewport(0,(h-l)/2,l,l);
  else
    glViewport((l-h)/2,0,h,h);
}



void loadJpegImage(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
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
      ligne=image+3*largimg*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

for(int i=0;i<largimg*2;i++)
{
    for(int j=0;j<hautimg*2;j++)
    {
        if((i<largimg && j<hautimg)||(i>=largimg && j>=hautimg))
        {
            texture[i][j][0]=image[(i%256)*256*3+(j%256)*3];
            texture[i][j][1]=image[(i%256)*256*3+(j%256)*3+1];
            texture[i][j][2]=image[(i%256)*256*3+(j%256)*3+2];
        }
        else
        {
            texture[i][j][0]=255;
            texture[i][j][1]=255;
            texture[i][j][2]=255;
        }
    }
}
}
