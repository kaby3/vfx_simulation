// 3D_IRF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define size 5000

typedef struct 
{ 
	float x,y, z;
}Point;

Point  mPoint[4][size];

//********** camera variables *************//
//*****************************************//

float zoom = 20.0f;
float rotx = 40;
float roty = -30.001f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};
int i = 0,j = 0;
int option = 1;

// ******** Functions *****************//
//************************************//

void display();
void reshape(int w, int h);
void Motion(int x,int y);
void Mouse(int b,int s,int x,int y);
void chaosGame();
void keyboard(unsigned char key, int x, int y);
float midPoint(float a, float b);
void drawPoints();
void randomTesting();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("3D IFS");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(NULL);
	randomTesting();
	chaosGame();
	glutMainLoop();

  return 0;
}

void reshape(int w, int h)
{

// prevent divide by 0 error when minimised
	
if(w==0) 
		
h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void display()
{

 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);	


	if (option == 1)
	{
		
		drawPoints();
	}
	/*if (option == 2)
		iteratedFuncSystems();*/

// draw grid
//	glBegin(GL_LINES);
//	
//for(int i=-10;i<=10;++i) 
//{	
//	//double  r = 2*sin((double)i);
//	glColor3f(1.0,1.0,0.0);
//	glVertex3f(i,0,-10);
//	glVertex3f(i,0,10);
//	glVertex3f(10,0,i);
//	glVertex3f(-10,0,i);
//
//}
//	
//glEnd();


glFlush();	
//glutPostRedisplay();
//glutSwapBuffers();
}


void chaosGame()
{	
	

	float x[4][3], y[4][3], z[4][3];
	int r;

	x[0][0] = -5.0; y[0][0] = 0.0; z[0][0]= 5.0;
	x[0][1] = 0; y[0][1] = 5; z[0][1] = 0.0;
	x[0][2] = 5.0; y[0][2] = 0.0; z[0][2] = 5.0;

	x[1][0] = 5.0; y[1][0] = 0.0; z[1][0]= 5.0;
	x[1][1] = 0; y[1][1] = 5; z[1][1] = 0.0;
	x[1][2] = 5.0; y[1][2] = 0.0; z[1][2] = -5.0;

	x[2][0] = 5.0; y[2][0] = 0.0; z[2][0]= -5.0;
	x[2][1] = 0; y[2][1] = 5; z[2][1] = 0.0;
	x[2][2] = -5.0; y[2][2] = 0.0; z[2][2] = -5.0;

	x[3][0] = -5.0; y[3][0] = 0.0; z[3][0]= -5.0;
	x[3][1] = 0; y[3][1] = 5; z[3][1] = 0.0;
	x[3][2] = -5.0; y[3][2] = 0.0; z[3][2] = 5.0;
	

	mPoint[0][0].x = 0; mPoint[0][0].y = 2.5, mPoint[0][0].z = 0.0;

	for(i =0; i<4; i++)
		for(j=0; j<3; j++)

	{
		glPointSize(4.5);
		glColor3f(1.0,0.0,1.0);
		glBegin(GL_POINTS);
		glVertex3f(x[i][j], y[i][j], z[i][j]);
		glEnd();
	}



	
	for (i = 0; i < 4; i++)
		for(j=0; j<size; j++)

	{	
		r = rand() % 3 +1;
		

		if(r == 1)
		{
			mPoint[i][j+1].x = midPoint(x[i][0],mPoint[i][j].x);
			mPoint[i][j+1].y = midPoint(y[i][0],mPoint[i][j].y);
			mPoint[i][j+1].z = midPoint(z[i][0],mPoint[i][j].z);
		}

		if(r == 2)
		{
			mPoint[i][j+1].x = midPoint(x[i][1],mPoint[i][j].x);
			mPoint[i][j+1].y = midPoint(y[i][1],mPoint[i][j].y);
			mPoint[i][j+1].z = midPoint(z[i][1],mPoint[i][j].z);
		}

		if(r == 3)
		{
			mPoint[i][j+1].x = midPoint(x[i][2],mPoint[i][j].x);
			mPoint[i][j+1].y = midPoint(y[i][2],mPoint[i][j].y);
			mPoint[i][j+1].z = midPoint(z[i][2],mPoint[i][j].z);
		}
		
		//printf("rand  :%d  midpoint x , y %g, %g\n", r, mPoint.x, mPoint.y);
		
	}

}

void drawPoints()
{
	for (i = 0; i < 4; i++)
		for(j=0; j<size; j++)
		{
			float t = j * (1.0/size);
			//printf("t is : %g\n", t);
			glPointSize(3.5);
			glColor3f(t,t,0.0);
			glBegin(GL_POINTS);
			glVertex3f(mPoint[i][j].x, mPoint[i][j].y, mPoint[i][j].z);
			glEnd();
		}

}

void randomTesting()
{
	for(int i = 0; i < 10; i++)
	{
	//double r1 = 100 + ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
	//double r2 = ((double)rand() / ((double)(RAND_MAX)+(double)(0.5)));

	int r1 = rand() % 500 + 200;

	//printf("r1 & r2 : %d\n", r1);
	}

}

float midPoint(float a, float b)
{
	return (a+b)/2;
}

void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[2] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}


void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{	
	
	if (key == '1')
		option = 1;
	if (key == '2')
		option = 2;
	if (key == '3')
		option = 3;
	if (key == '4')
		option = 4;


	glutPostRedisplay();
	return;
}




