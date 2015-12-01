// tree-modelling.cpp : Defines the entry point for the console application.

//

#include "stdafx.h"
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct{
	double x,y,z;
}point;

point p1[5][4] = {0.5,0,0, 0.5,7,0, -2,8,0, -4,9,0,
				  1,0,0, 1,7,0, -1,8,0, -3,9,0,
				1.5,0,0, 1.5,7,0, 2,8,0, 4,9,0};


/*************************************************
*********tree varables **************/

double u,v,w, u1,v1,w1, u2,v2,w2;
double S,T,R1,R2, S1,T1,S2,T2;
double h1,h2;
point Po ={0.0,0.0,0.00002}, Pa = {0.0,4.0,0.0},Pa1={0.0, 6.0,0.0}, Pa2={0.0, 5.0,0.0}, Pa3={0.0, 5.5,0.0},Pb,Pb1;//= {3.0, 5.0, 0.0};
point tip1,tip2,Py,Py1;
point Pu,temp3={0},temp4={0};
point temp1_array;
point parent[100], child1[100], child2[100];



void bezierCurve();
void aona_kunii();

void aona_kunii2(point temp1, point temp2, int n, double R1, double R2, double h1, double h2);
void aona_kunii3(point temp3, point temp4, int n, double R1, double R2, double h1, double h2);
void aona_kunii4(point temp1, point temp2, int n);

//********** camera variables *************//
//*****************************************//

double zoom = 20.0f;
double rotx = 10;
double roty = -10.001f;
double tx = 0;
double ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};
int i=0,j;
//k=0;
// ******** Functions *****************//
//************************************//

void display();
void reshape(int w, int h);
void Motion(int x,int y);
void Mouse(int b,int s,int x,int y);
void InitiaPoints();
void callTree();
void callTree1();
void callTree2();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("Tree Generation");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(NULL);
	//callTree();
	
	glutMainLoop();

  return 0;
}

void InitiaPoints()
{
	glBegin(GL_LINE_STRIP);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(Po.x, Po.y, Po.z);
		glVertex3f(Pa.x, Pa.y, Pa.z);
		glVertex3f(Pa1.x, Pa1.y, Pa1.z);
	glEnd();

}


void drawTree()
{
glPushMatrix();
glRotatef(-20, 0.0,1.0,0.0);
glTranslatef(-3,0,0);
	InitiaPoints();
	glPushMatrix();
	glRotatef(40, 0.0,1.0,0.0);
	callTree();
	glPopMatrix();
	glPushMatrix();
	glRotatef(40, 0.0,1.0,0.0);
	callTree1();
	glPopMatrix();
	
glPopMatrix();
}

void callTree()
{
aona_kunii2(Pa,Po, 2, 0.5,0.5,0,-45);
//aona_kunii2(Pa1,Po, 2, 0.2,0.2,0,-45);
}

void callTree1()
{
aona_kunii2(Pa2,Po, 2, 0.3,0.3,0,45);
aona_kunii2(Pa1,Po, 3, 0.2,0.2,0,0);
//aona_kunii2(Pa3,Po, 1, 0.3,0.3,0,-45);
}

void callTree2()
{
  aona_kunii3(Pa3,Po, 2, 0.2,0.2,-45,0);
}


void aona_kunii2(point temp1, point temp2, int n, double R1, double R2, double h1, double h2)
{	
	u1 = temp1.x - temp2.x;
	v1 = temp1.y - temp2.y;
	w1 = temp1.z - temp2.z;
	S1 = (double)1/sqrt(u1*u1+v1*v1);
	double SZ = (double)1/sqrt(u1*u1+w1*w1);
	T1 = sqrt(u1*u1 + v1*v1 + w1*w1);

	Py.x = temp1.x + R1*((u1*cos(h1)) - S1 * T1 * v1*sin(h1));
	Py.y = temp1.y + R1*((v1*cos(h1)) + S1 * T1 * u1*sin(h1));
	Py.z = temp1.z + R1*(w1*cos(h1) - SZ * T1 * w1*sin(h1));
	//Py.z = temp1.z + R1*w1*cos(h1);
	//child1[k] = Py;
	//printf("py.z is : %g\n", Py.z);
	Pb.x = temp1.x + R2*((u1*cos(h2)) - S1 * T1 * v1*sin(h2));
	Pb.y = temp1.y + R2*((v1*cos(h2))+ S1 * T1 * u1*sin(h2));
	Pb.z = temp1.z + R2*(w1*cos(h2) - SZ * T1 * w1*sin(h2));
	//child2[k] = Pb;

	//glRotatef(90,1.0,1.0,0.0);

	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(temp1.x, temp1.y, temp1.z);
	glVertex3f(Pb.x, Pb.y, Pb.z);
	
	//glColor3f(1.0,1.0,0.0);
	glVertex3f(temp1.x, temp1.y, temp1.z);
	glVertex3f(Py.x, Py.y, Py.z);
	
	glEnd();

if (n < 1)
		return;
	else
	{
	
	n--;
	
	}

temp2 = temp1;
temp1 = Pb;
aona_kunii2(temp1,temp2, n, 0.4,0.4,0,-45);//RIGHT,  blue

temp2 = temp1;
temp1 =Py;
aona_kunii3(temp1,temp2, n, 0.2,0.3,0,0);// green
//
//temp2 = temp1;
//temp1 =Pb;
//aona_kunii3(temp1,temp2, n, 0.3,0.2,45,0);// green

}




void aona_kunii3(point temp3, point temp4, int n, double R1, double R2, double h1, double h2)
{
		
	u2 = temp3.x - temp4.x;
	v2 = temp3.y - temp4.y;
	w2 = temp3.z - temp4.z;
	S2 = (double)1/sqrt(u2*u2+v2*v2);
	double SZ = (double)1/sqrt(u2*u2+w2*w2);
	T2 = sqrt(u2*u2 + v2*v2 + w2*w2);

	Py1.x = temp3.x + R1*((u2*cos(h1)) - S2 * T2 * v2*sin(h1));
	Py1.y = temp3.y + R1*((v2*cos(h1)) + S2 * T2 * u2*sin(h1));
	Py1.z = temp3.z + R1*((w2*cos(h1))  - S2 * T2 * w1*sin(h1));

	Pb1.x = temp3.x + R2*((u2*cos(h2)) - S2 * T2 * v2*sin(h2));
	Pb1.y = temp3.y + R2*((v2*cos(h2)) + S2 * T2 * u2*sin(h2));
	Pb1.z = temp3.z + R2*(w2*cos(h2) - S2 * T2 * w1*sin(h2));

	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(temp3.x, temp3.y, temp3.z);
	glVertex3f(Py1.x, Py1.y, Py1.z);
	//glColor3f(0.0,1.0,1.0);
	glVertex3f(temp3.x, temp3.y, temp3.z);
	glVertex3f(Pb1.x, Pb1.y, Pb1.z);
	
	glEnd();

if (n < 1)
		return;
	else
	{
		//printf("n is :%d, %g, %g\n", n, temp1.x, temp2.x);
	n--;
	}


temp4 = temp3;
temp3 = Py1;
aona_kunii3(temp3,temp4, n, 0.4,0.4,0,-45);//RIGHT,  blue

temp4 = temp3;
temp3 = Pb1;
aona_kunii3(temp3,temp4, n, 0.2,0.3,0,0);// green
//	
}



void reshape(int w, int h)
{

// prevent divide by 0 error when minimised
	
if(w==0) 
		
h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,(double)w/h,0.1,100);
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

	glBegin(GL_LINES);
	
//for(int i=-10;i<=10;++i) 
//{	
//	//double  r = 2*sin((double)i);
//	glColor3f(1.0,1.0,0.0);
//	glVertex3f(i,0,-10);
//	glVertex3f(i,0,10);
//	glVertex3f(10,0,i);
//	glVertex3f(-10,0,i);
//
//	}
//	
//glEnd();

glPushMatrix();
//glRotatef(-20, 0.0,1.0,0.0);
//glTranslatef(3,0,0);
InitiaPoints();
	callTree();
	glRotatef(-40, 0.0,1.0,0.0);
	callTree1();
	callTree2();
glPopMatrix();

//glPushMatrix();
//drawTree();
//glPopMatrix();

glFlush();	
//glutPostRedisplay();
//glutSwapBuffers();
}


void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[2] )
	{
		zoom -= (double) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (double) 0.5f * diffy;
			roty += (double) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				tx += (double) 0.05f * diffx;
				ty -= (double) 0.05f * diffy;
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


