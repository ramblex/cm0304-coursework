#include <iostream>

#ifndef __apple__
#define __apple__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif /* __apple__ */

using namespace std;

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutMainLoop();
}
