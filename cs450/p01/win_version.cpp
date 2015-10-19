// CS450 Project 1
// Jonah Brooks
// 10/07/2015

#include "glut.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

# define M_PI           3.14159265358979323846  /* pi */

GLuint ConeList;
GLuint TreeList;
GLuint GrassList;
GLuint FieldList;

const GLfloat fieldRadius = 10.0;

const int numTrees = 100;
const GLfloat treeHeight = 1.00;
const GLfloat treeRadius = .25;
GLfloat treeInfo[numTrees][7];

const int numGrass = 10000;
const GLfloat grassHeight = treeHeight*0.1;
const GLfloat grassWidth = treeRadius*0.01;
const GLfloat grassLength= treeRadius*0.05;
GLfloat grassInfo[numGrass][7];

int mouse_last_x;
int mouse_last_y;
GLfloat screen_scale = 1;
GLfloat scale_increment = .01;

// How many degrees the view should rotate for each pixel of mouse movement
const GLfloat vdegree_change_scale = 1;
GLfloat vdegree_change_x;
GLfloat vdegree_change_y;

// Function for drawing a list lst at (x,y,z), 
// with an up vector of (ux,uy,uz),
// and scaled by a ratio of scale
void drawList(GLuint lst, GLfloat x, GLfloat y, GLfloat z,
	GLfloat ux, GLfloat uy, GLfloat uz,
	GLfloat scale);

void initLists();
void mouseClick(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void displayMe(void);

void drawList(GLuint lst, GLfloat x, GLfloat y, GLfloat z,
	GLfloat ux, GLfloat uy, GLfloat uz,
	GLfloat scale)
{
	glPushMatrix();
	// Move to cone to (x,y,z)
	glTranslatef(x,y,z);
	// Rotate the list so it points in the (ux,uy,uz) direction
	// relative to the global rotation
	glRotatef((180.0/M_PI)*acos(uy/sqrt(ux*ux+uy*uy+uz*uz)),uz,0,-ux);
	// Resize the cone in all dimensions
	glScalef(scale, scale, scale);
	glCallList(lst);

	glPopMatrix();

}

void initLists()
{
	//Make ConeList for drawing cones
	ConeList = glGenLists(1);
	glNewList(ConeList, GL_COMPILE);
		glColor3f(0.0, 8.0, 0.0);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, 0.5*treeHeight, 0.0);
			for (int ang = 0; ang <= 360; ang++)
			{
				if (ang % 7 == 0)
				{
					glColor3f(0.0, 0.7, 0.0);
				}
				else if (ang % 13 == 0)
				{
					glColor3f(0.0, .8, 0.0);
				}
				glVertex3f(treeRadius*cos(M_PI*ang / 180.0), 0, treeRadius*sin(M_PI*ang / 180.0));
			}
		glEnd();
	glEndList();

	// Make TreeList for drawing trees
	TreeList = glGenLists(1);
	glNewList(TreeList, GL_COMPILE);
		// Draw trunk
		glColor3f(0.2, 0.2, 0.0);
		// Draw a cylinder
		glBegin(GL_TRIANGLE_STRIP);
			for (int ang = 0; ang <= 360; ang++)
			{
				if (ang % 7 == 0)
				{
					glColor3f(0.3, 0.2, 0.0);
				}
				else if (ang % 13 == 0)
				{
					glColor3f(0.2, .2, 0.0);
				}
				// Vertex on top edge
				glVertex3f(treeRadius/2.5 * cos(M_PI*ang / 180.0), .2*treeHeight, treeRadius/2.5 * sin(M_PI*ang / 180.0));
				// Vertex on bottom edge
				glVertex3f(treeRadius / 2.5 * cos(M_PI*ang / 180.0), -.1*treeHeight, treeRadius / 2.5 * sin(M_PI*ang / 180.0));
			}
		glEnd();
		// Draw bottom
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, -.1*treeHeight, 0.0);
			for (int ang = 0; ang <= 360; ang++)
			{
				if (ang % 7 == 0)
				{
					glColor3f(0.3, 0.2, 0.0);
				}
				else if (ang % 13 == 0)
				{
					glColor3f(0.2, .2, 0.0);
				}
				// Vertex on bottom edge
				glVertex3f(treeRadius / 2.5 * cos(M_PI*ang / 180.0), -.1*treeHeight, treeRadius / 2.5 * sin(M_PI*ang / 180.0));
			}
		glEnd();

		// Draw branches
		drawList(ConeList, 0.0, 0.2*treeHeight, 0.0, 0, 1, 0, 1);
		drawList(ConeList, 0.0, 0.4*treeHeight, 0.0, 0, 1, 0, .9);
		drawList(ConeList, 0.0, 0.6*treeHeight, 0.0, 0, 1, 0, .8);
	glEndList();

	GrassList = glGenLists(1);
	glNewList(GrassList, GL_COMPILE);
		glColor3f(.0, .9, .0);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, grassHeight, 0.0);
			glVertex3f(grassWidth, 0.0, 0.0);
			glVertex3f(-grassWidth, 0.0, 0.0);
			glVertex3f(0.0, 0.0, grassLength);
			glVertex3f(grassWidth, 0.0, 0.0);
		glEnd();
	glEndList();

	FieldList = glGenLists(1);
	glNewList(FieldList, GL_COMPILE);
		glColor3f(.3, .5, .3);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(fieldRadius, 0, fieldRadius);
		glVertex3f(-fieldRadius, 0, fieldRadius);
		glVertex3f(-fieldRadius, 0, -fieldRadius);
		glVertex3f(fieldRadius, 0, -fieldRadius);
		glVertex3f(fieldRadius, 0, fieldRadius);

		glEnd();
	glEndList();

}

void mouseClick(int button, int state, int x, int y)
{
	// For any button, reset view controls in prep for movement
	mouse_last_x = x;
	mouse_last_y = y;
	vdegree_change_x = 0.0;
	vdegree_change_y = 0.0;

	if (button == 4) // Scrollwheel down
	{
		screen_scale -= scale_increment;
	}
	
	if (button == 3) // Scrollwheel up
	{
		screen_scale += scale_increment;
	}
	
	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{

	// Rotate the viewing angle using x, y, last_x, last_y
	vdegree_change_x = (x - mouse_last_x) * vdegree_change_scale;
	vdegree_change_y = (y - mouse_last_y) * vdegree_change_scale;

	// Update last x and y for the next call to this function
	mouse_last_x = x;
	mouse_last_y = y;

	glutPostRedisplay();
}

void displayMe(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Rotate view
	glRotatef(vdegree_change_y, 1.0, 0.0, 0.0);
	glRotatef(vdegree_change_x, 0.0, 1.0, 0.0);
	glScalef(screen_scale,screen_scale,screen_scale);
	screen_scale = 1; // Reset to 1 while waiting for another button input

	// Draw a forest of trees
	for (int i = 0; i < numTrees; i++)
	{
		drawList(TreeList,
			treeInfo[i][0], treeInfo[i][1], treeInfo[i][2],
			treeInfo[i][3], treeInfo[i][4], treeInfo[i][5],
			treeInfo[i][6]);
		
	}
	// Draw field of grass
	for (int i = 0; i < numGrass; i++)
	{
		drawList(GrassList,
			grassInfo[i][0], grassInfo[i][1], grassInfo[i][2],
			grassInfo[i][3], grassInfo[i][4], grassInfo[i][5],
			grassInfo[i][6]);
		
	}

	// Draw field
	drawList(FieldList, 0, 0, 0, 0, 1.0, 0, 1);

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jonah Brooks");
	glClearDepth(100.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(60, 1, .5, 100.0);
	gluLookAt(0.0, 1.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	srand(time(NULL));


	// Initialize treeInfo
	for (int i = 0; i < numTrees; i++)
	{
		treeInfo[i][0] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2 - fieldRadius;//x
		treeInfo[i][2] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2 - fieldRadius;//z
		treeInfo[i][1] = 0.0;//y
		treeInfo[i][3] = 0;//ux
		treeInfo[i][4] = 1;//uy
		treeInfo[i][5] = 0;//uz
		treeInfo[i][6] = 1;//scale

	}

	// Initialize grassInfo
	for (int i = 0; i < numGrass; i++)
	{
		grassInfo[i][0] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2 - fieldRadius;//x
		grassInfo[i][2] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2 - fieldRadius;//z
		grassInfo[i][1] = 0.0;//y
		grassInfo[i][3] = 0;//ux
		grassInfo[i][4] = 1;//uy
		grassInfo[i][5] = 0;//uz
		grassInfo[i][6] = 1;//scale

	}


	initLists();

	// Run main loop and functions
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutDisplayFunc(displayMe);
	glutMainLoop();
	return 0;
}