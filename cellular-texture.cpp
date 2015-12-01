// cellular-texture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define size 512
#define NUM_POINTS 50


void display();
void randomPoints();
float calMinDistance(int x, int y);
void drawPoints();
void randomizePoints();
void findMin();
void findMax();
void keyboard(unsigned char key, int x, int y);

int i=0, j=0;

//-------------//
int option=0;
//-------------//

int randx[NUM_POINTS], randy[NUM_POINTS];
float dist[NUM_POINTS]={0};
float distbuffer[size][size];
float max_distbuffer = 0.0;
float min_distbuffer = 0.0;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("Cellular Textures");
	
	glutDisplayFunc(display);
	//glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	glOrtho(0.0, size, 0.0, size, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	randomizePoints();
	
	findMin();
	findMax();
	
	glutMainLoop();

  return 0;
}


void display()
{
	//float min;
	glClearColor(0.0, 0.0, 0.0, 0.0);/*Background colour*/

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	//randomPoints();
	drawPoints();
			
	//glFlush();	
	glutSwapBuffers();

}

void randomizePoints()
{
	printf("option :%d\n", option);
for(int k=0; k < NUM_POINTS; k++)
	{
		randx[k] = rand()%size; 
		randy[k] = rand()%size; 
	}


	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
		{
			distbuffer[i][j]= calMinDistance(i,j);
			
		}
}



void randomPoints()
{	
	glPointSize(1.0);
	for(int k=0; k< NUM_POINTS; k++)
	{
		glBegin(GL_POINTS);
		glVertex2f(randx[k], randy[k]);
		glEnd();
	}
	
}

void drawPoints()
{
	float col,r,g,b, random, noise;

	const static int q = 15;
	const static float c1 = (1 << q) - 1;
	const static float c2 = ((int)(c1 / 3)) + 1;
	const static float c3 = 1.f / c1;

	glPointSize(4.0);
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
		{
			//col = ((distbuffer[i][j])/max_distbuffer); // pic 1 on http://www.blackpawn.com/texts/cellular/default.html
			//col = 1-((distbuffer[i][j])/max_distbuffer);// pic2  make 1 less to get different effects
			

			random = ((float)rand() / (float)(RAND_MAX + 1));
					
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			g = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			b = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));

			col =((distbuffer[i][j]-min_distbuffer)/(max_distbuffer-min_distbuffer)); // pic 3
			//col =75.0 * ((distbuffer[i][j]-min_distbuffer)/(max_distbuffer-min_distbuffer)); // pic 4

			glBegin(GL_POINTS);
			//glColor3f(col,0,0); // pic 1 use option 1
			//glColor3f(1-col,0,0); // pic 2 use option 1
			glColor3f(3*col*r,3*col*g,3*col*b);// pic 3 on the website option 0
			//glColor3f(cos(col),0.,0.);  // option 1,2, 3
			glVertex2f(i,j);
			glEnd();

		}
		
}

float calMinDistance(int x, int y)
{		
		float min1, min2, min3;
		float a,b;
		
		for(int k =0; k < NUM_POINTS; k++)
			{
				a = (float)(randx[k] - x)*(randx[k] - x);
				b = (float)(randy[k] - y)*(randy[k] - y);
				dist[k] = sqrt(a+b);
				//printf("min dis is :%d,   %g, %g, %g\n",k,   a, b, dist[k]);
				//printf("randx, randy is :%d, %d, %d\n",k, randx[k], randy[k]);
			}
		
		//for(int k =0; k < NUM_POINTS; k++)
		//{
		//	float dx = (x - randx[k]);
		//	float dy = (y - randy[k]);

		//	if (dx > size/2)
		//		dx = size-dx;
		//	if (dy > size/2)
		//		 dy = size-dy;
	
		//	return sqrt( dx*dx + dy*dy );
		//	//printf("min dis is :%d,   %g, %g, %g\n",k,   a, b, dist[k]);
		//	//printf("randx, randy is :%d, %d, %d\n",k, randx[k], randy[k]);
		//}

		min1 = dist[0];
		for(int k =1; k < NUM_POINTS; k++)
			if (dist[k] < min1)
				min1 = dist[k];

		min2 =dist[0];
		for(int k =1; k < NUM_POINTS; k++)
			if(dist[k] > min1 && dist[k]< min2)
				min2 = dist[k];

		min3 =dist[0];
		for(int k =1; k < NUM_POINTS; k++)
			if(dist[k] > min2 && dist[k]< min3)
				min3 = dist[k];
				
		//printf("min2 is :%g, %g\n", min1, min2);
		if(option ==1){
			//printf("option 1\n");
			return min1;
		}
		else if(option == 0){
			//printf("option 0\n");
				return min2-min1;  // pic 3
		}
		else if(option == 2){
			//printf("option 2\n");
				return min2*min1;
		}
}


void findMin()
{
	
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
		{
			if (distbuffer[i][j] < min_distbuffer)
				min_distbuffer = distbuffer[i][j];
		}

		printf("min-disstbuffer :%g\n",min_distbuffer);

}

void findMax()
{
	
	for(i=0; i<size; i++)
		for(j=0; j<size; j++)
		{
			if (distbuffer[i][j] > max_distbuffer)
				max_distbuffer = distbuffer[i][j];
		}

		printf("max-disstbuffer :%g\n",max_distbuffer);

}


void keyboard(unsigned char key, int x, int y)
{	
	if (key == 'z')
		option = 0;
	if (key == 'x'){
		
		option = 1;
	}
		

	glutPostRedisplay();
	return;

}
