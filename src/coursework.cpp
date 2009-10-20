#include <iostream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif /* __APPLE__ */

float angle=0.0;
float x=0.0f,y=1.75f,z=5.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
float ratio=1.0;
int frame,mytime,timebase=0;
char s[30];

GLuint DLid;

GLuint createDL(void);

void changeSize(int w, int h)	{

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(h == 0)
    h = 1;

  ratio = 1.0f * w / h;
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the clipping volume
  gluPerspective(45,ratio,1,1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, 
            x + lx,y + ly,z + lz,
            0.0f,1.0f,0.0f);
}

void initScene() {

  glEnable(GL_DEPTH_TEST);
  DLid = createDL();
}


void drawSnowMan() {


  glColor3f(1.0f, 1.0f, 1.0f);

  // Draw Body	
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);


  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);

  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();

  // Draw Nose
  glColor3f(1.0f, 0.5f , 0.5f);
  glRotatef(0.0f,1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f,0.5f,10,2);
}



GLuint createDL() {
  GLuint snowManDL,loopDL;

  snowManDL = glGenLists(1);
  loopDL = glGenLists(1);

  glNewList(snowManDL,GL_COMPILE);
  drawSnowMan();
  glEndList();

  glNewList(loopDL,GL_COMPILE);
  for(int i = -3; i < 3; i++)
    for(int j=-3; j < 3; j++) {
      glPushMatrix();
      glTranslatef(i*10.0,0,j * 10.0);
      glCallList(snowManDL);
      glPopMatrix();
    }
  glEndList();

  return(loopDL);
}


void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw ground

  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f, 0.0f, -100.0f);
  glVertex3f(-100.0f, 0.0f,  100.0f);
  glVertex3f( 100.0f, 0.0f,  100.0f);
  glVertex3f( 100.0f, 0.0f, -100.0f);
  glEnd();

  // Draw 36 SnowMen

  glCallList(DLid);
  frame++;
  mytime=glutGet(GLUT_ELAPSED_TIME);
  if (mytime - timebase > 1000) {
    sprintf(s,"FPS:%4.2f",frame*1000.0/(mytime-timebase));
    glutSetWindowTitle(s);
    timebase = mytime;		
    frame = 0;
  }
  glutSwapBuffers();
}

void orientMe(float ang) {


  lx = sin(ang);
  lz = -cos(ang);
  glLoadIdentity();
  gluLookAt(x, y, z, 
            x + lx,y + ly,z + lz,
            0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
  x = x + i*(lx)*0.3;
  z = z + i*(lz)*0.3;

  glLoadIdentity();
  gluLookAt(x, y, z, 
            x + lx,y + ly,z + lz,
            0.0f,1.0f,0.0f);
}


void inputKey(unsigned char c, int x, int y) {

  switch (c) {
    case 'a' : angle -= 0.03f;orientMe(angle);break;
    case 's' : angle +=0.03f;orientMe(angle);break;
    case 't' : moveMeFlat(1);break;
    case 'g' : moveMeFlat(-1);break;
  }
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(300,300);
  glutCreateWindow("SnowMen");

  initScene();

  glutKeyboardFunc(inputKey);

  glutDisplayFunc(renderScene);
  glutIdleFunc(renderScene);

  glutReshapeFunc(changeSize);

  glutMainLoop();

  return(0);
}
