#include <GL/glut.h>
#include <iostream>


int main(int argc, char **argv)
{

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(320,320);
  glutCreateWindow("hello world");



  char moo;

  std::cin >> moo;

  return 1;

}
