// CS450 Project 2
// Jonah Brooks
// 10/16/2015

#include "glut.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "heli.550"

# define M_PI           3.14159265358979323846  /* pi */

GLuint HeliList;
GLuint ConeList;
GLuint TreeList;
GLuint GrassList;
GLuint FieldList;
GLuint BladeList;

const GLfloat fieldRadius = 30.0;

const int numTrees = 1000;
const GLfloat treeHeight = 1.00;
const GLfloat treeRadius = .25;
GLfloat treeInfo[numTrees][7];

const int numGrass = 10000;
const GLfloat grassHeight = treeHeight*0.1;
const GLfloat grassWidth = treeRadius*0.01;
const GLfloat grassLength = treeRadius*0.05;
GLfloat grassInfo[numGrass][7];

// helicopter parameters:

const GLfloat BLADE_RADIUS = 1.0;
const GLfloat BLADE_WIDTH = 0.4;
const GLfloat BLADE_SPEED = 10.0;
GLfloat heliy = 2 * treeHeight;
GLfloat helix = 0;
GLfloat heliz = 0;
GLfloat helis = 0.25;
GLfloat helir = 0; // Don't adjust this. It doesn't work with the first person perspective.
GLfloat blader = 0;
int viewInside = false;


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
void toggleView(int id);

void displayMe(void);

void drawList(GLuint lst, GLfloat x, GLfloat y, GLfloat z,
	GLfloat ux, GLfloat uy, GLfloat uz,
	GLfloat rx, GLfloat ry, GLfloat rz,
	GLfloat scale)
{
	glPushMatrix();
	// Move to cone to (x,y,z)
	glTranslatef(x, y, z);
	// Rotate around the new x axis as specified by rdeg
	glRotatef(rx, 1, 0, 0);
	// Rotate around the new y axis as specified by rdeg
	glRotatef(ry, 0, 1, 0);
	// Rotate around the new z axis as specified by rdeg
	glRotatef(rz, 0, 0, 1);
	// Rotate the list so it points in the (ux,uy,uz) direction
	// relative to the global rotation
	glRotatef((180.0 / M_PI)*acos(uy / sqrt(ux*ux + uy*uy + uz*uz)), uz, 0, -ux);
	// Resize the cone in all dimensions
	glScalef(scale, scale, scale);
	glCallList(lst);

	glPopMatrix();

}

void initLists()
{
	// Make the BladeList for drawing the helicopter blades
	// This is mostly cut-paste from the class website
	// draw the helicopter blade with radius BLADE_RADIUS and
	// width BLADE_WIDTH centered at (0.,0.,0.) in the XY plane

	BladeList = glGenLists(1);
	glNewList(BladeList, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glVertex2f(BLADE_RADIUS, BLADE_WIDTH / 2.);
			glVertex2f(0., 0.);
			glVertex2f(BLADE_RADIUS, -BLADE_WIDTH / 2.);

			glVertex2f(-BLADE_RADIUS, -BLADE_WIDTH / 2.);
			glVertex2f(0., 0.);
			glVertex2f(-BLADE_RADIUS, BLADE_WIDTH / 2.);
		glEnd();
	glEndList();

	// Make HeliList for drawing the helicopter
	// This is mostly cut-paste from the class website
	HeliList = glGenLists(1);
	glNewList(HeliList, GL_COMPILE);
		int i;
		struct point *p0, *p1, *p2;
		struct tri *tp;
		float p01[3], p02[3], n[3];

		glPushMatrix();
		glTranslatef(0., -1., 0.);
		glRotatef(97., 0., 1., 0.);
		glRotatef(-15., 0., 0., 1.);
		glBegin(GL_TRIANGLES);
			for (i = 0, tp = Helitris; i < Helintris; i++, tp++)
			{
				p0 = &Helipoints[tp->p0];
				p1 = &Helipoints[tp->p1];
				p2 = &Helipoints[tp->p2];

				/* fake "lighting" from above: */

				p01[0] = p1->x - p0->x;
				p01[1] = p1->y - p0->y;
				p01[2] = p1->z - p0->z;
				p02[0] = p2->x - p0->x;
				p02[1] = p2->y - p0->y;
				p02[2] = p2->z - p0->z;
				//Cross(p01, p02, n);
				n[0] = p01[1] * p02[2] - p01[2] * p02[1];
				n[1] = p01[2] * p02[1] - p01[1] * p02[2];
				n[2] = p01[0] * p02[1] - p01[0] * p02[1];

				//Unit(n, n);
				n[0] = n[0] / sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
				n[1] = n[1] / sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
				n[2] = n[2] / sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

				n[1] = fabs(n[1]);
				n[1] += .25;
				if (n[1] > 1.)
					n[1] = 1.;
				glColor3f(0., n[1], 0.);
				glVertex3f(p0->x, p0->y, p0->z);
				glVertex3f(p1->x, p1->y, p1->z);
				glVertex3f(p2->x, p2->y, p2->z);
			}
		glEnd();
		glPopMatrix();
	glEndList();


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
				glVertex3f(treeRadius*cos(M_PI*ang / 180.0), 0,
					treeRadius*sin(M_PI*ang / 180.0));
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
				glVertex3f(treeRadius / 2.5 * cos(M_PI*ang / 180.0), .2*treeHeight,
					treeRadius / 2.5 * sin(M_PI*ang / 180.0));
				// Vertex on bottom edge
				glVertex3f(treeRadius / 2.5 * cos(M_PI*ang / 180.0), -.1*treeHeight,
					treeRadius / 2.5 * sin(M_PI*ang / 180.0));
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
				glVertex3f(treeRadius / 2.5 * cos(M_PI*ang / 180.0), -.1*treeHeight,
					treeRadius / 2.5 * sin(M_PI*ang / 180.0));
			}
		glEnd();

		// Draw branches
		drawList(ConeList, 0.0, 0.2*treeHeight, 0.0, 0, 1, 0, 0, 0, 0, 1);
		drawList(ConeList, 0.0, 0.4*treeHeight, 0.0, 0, 1, 0, 0, 0, 0, .9);
		drawList(ConeList, 0.0, 0.6*treeHeight, 0.0, 0, 1, 0, 0, 0, 0, .8);
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
	if (!viewInside)
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
	}
	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	if (!viewInside) 
	{
		// Rotate the viewing angle using x, y, last_x, last_y
		vdegree_change_x = (x - mouse_last_x) * vdegree_change_scale;
		vdegree_change_y = (y - mouse_last_y) * vdegree_change_scale;

		// Update last x and y for the next call to this function
		mouse_last_x = x;
		mouse_last_y = y;
	}
	glutPostRedisplay();
}

void toggleView(int id)
{
	GLfloat lax = -0.4*helis + helix; // x location of inside view
	GLfloat lay = 1.8*helis + heliy; // y location of inside view
	GLfloat laz = -4.9*helis + heliz; // z location of inside view

	glLoadIdentity();
	gluPerspective(75, 1, .01, 100.0);
	viewInside = !viewInside;
	if(viewInside)
	{
		// Rotate around y axis by helir
		//lax = laz*sin((M_PI / 180.0)*helir) + lax*cos((M_PI / 180.0)*helir);
		//lay = lay;
		//laz = laz*cos((M_PI / 180.0)*helir) - lax*sin((M_PI / 180.0)*helir);
		gluLookAt(lax, lay, laz, 0, heliy, -10, 0, 1, 0);
	}
	else
	{
		gluLookAt(0, 1.0, 7.0,
			0.0, 0.0, -10.0,
			0.0, 1.0, 0.0);
	}

}

void displayMe(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate view
	glRotatef(vdegree_change_y, 1.0, 0.0, 0.0);
	glRotatef(vdegree_change_x, 0.0, 1.0, 0.0);
	glScalef(screen_scale, screen_scale, screen_scale);
	screen_scale = 1; // Reset to 1 while waiting for another button input

					  // Draw a forest of trees
	for (int i = 0; i < numTrees; i++)
	{
		drawList(TreeList,
			treeInfo[i][0], treeInfo[i][1], treeInfo[i][2],
			treeInfo[i][3], treeInfo[i][4], treeInfo[i][5],
			0, 0, 0,
			treeInfo[i][6]);

	}
	// Draw field of grass
	for (int i = 0; i < numGrass; i++)
	{
		drawList(GrassList,
			grassInfo[i][0], grassInfo[i][1], grassInfo[i][2],
			grassInfo[i][3], grassInfo[i][4], grassInfo[i][5],
			0, 0, 0,
			grassInfo[i][6]);

	}

	// Draw field
	drawList(FieldList, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1);

	glPushMatrix();
	glRotatef(helir, 0, 1, 0);
	// Draw helicopter
	drawList(HeliList, helix, heliy, heliz, 0, 1.0, 0, 0, 0, 0, helis);

	// Draw big blade
	drawList(BladeList, 0.0 + helix, 2.9*helis + heliy, -2 * helis, 0.0 +
		heliz, 0, 1.0, 0, blader, 0, 5 * helis);
	// Draw small blade
	drawList(BladeList, 0.5*helis + helix, 2.5*helis + heliy, 9.0 * helis
		+ heliz, 0, 1.0, 0, 0, 90, blader * 3, 1.5 * helis);
	glPopMatrix();

	glFlush();
}

void Animate()
{
	blader += BLADE_SPEED;
	if (blader > 360)
	{
		blader -= 360;
	}

	glutPostRedisplay();
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
	gluPerspective(75, 1, .01, 100.0);
	gluLookAt(0.0, 1.0, 7.0,
	 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0);

	srand(time(NULL));


	// Initialize treeInfo
	for (int i = 0; i < numTrees; i++)
	{
		treeInfo[i][0] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2
			- fieldRadius;//x
		treeInfo[i][2] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius * 2
			- fieldRadius;//z
		treeInfo[i][1] = 0.0;//y
		treeInfo[i][3] = 0;//ux
		treeInfo[i][4] = 1;//uy
		treeInfo[i][5] = 0;//uz
		treeInfo[i][6] = 1;//scale

	}

	// Initialize grassInfo
	for (int i = 0; i < numGrass; i++)
	{
		grassInfo[i][0] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius *
			2 - fieldRadius;//x
		grassInfo[i][2] = (GLfloat)rand() / (GLfloat)RAND_MAX * fieldRadius *
			2 - fieldRadius;//z
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
	int toggleMenu = glutCreateMenu(toggleView);
	glutAddMenuEntry("Toggle View", toggleMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutIdleFunc(Animate);
	glutDisplayFunc(displayMe);
	glutMainLoop();
	return 0;
}