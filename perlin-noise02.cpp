// perlin-noise02.cpp : Defines the entry point for the console application.
// //pesudocode for 2D taken from: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

#include "stdafx.h"
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define SIZE 300
#define size 300
#define m_size 100


int i,j;
int seed = 10000;
float perlin[size][size]={0},a;
float noise[size][size];


void display(void);
float Noise(int x, int y);
void initialise();
float  SmoothNoise(int x, int y);
float Interpolate(float a, float b, int x);
float InterpolatedNoise(float x, float y);
float PerlinNoise_2D(int x, int y);
void drawperlinNoise();
float PerlinNoise_2D_add(int x, int y, float pers, float freq, float octaves,float amp);


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("Perlin Noise");
	glutDisplayFunc(display);

	initialise();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 300, 0.0,300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutMainLoop();

  return 0;
}



void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawperlinNoise();
	glFlush();

	return;
}


void drawperlinNoise()
{
//float b;
for (int x =0; x < size; x++)
	for(int y = 0; y < size; y++)
	{	
		//float PerlinNoise_2D_add(int x, int y, float pers, float freq, float oct,float amp)
		//perlin[x][y] = PerlinNoise_2D(x,y);
		perlin[x][y] = 0.0;
		perlin[x][y] += PerlinNoise_2D_add(x,y,0.015,0.019,2,0.9);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.15,0.193,4,0.2);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.085,0.33,5,0.2);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.15,0.733,4,0.2);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.15,1.34,5,0.2);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.15,0.034,6,0.1);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.095,0.14,5,0.2);
		//perlin[x][y] += PerlinNoise_2D_add(x,y,0.015,0.014,2,0.2);
					
		glPointSize(2.0);
		glBegin(GL_POINTS);

		/*	if(perlin[x][y] == 0.0)
			{	
				///printf("zero is %d, %d: %f\n", x,y, perlin[x][y]);
				glColor3f(0.0,0.0,0.7);
			}
			else if(perlin[x][y] > 0.0 && perlin[x][y] <= 0.1 )
				glColor3f(0.7,0.0,0.0);
			else if (perlin[x][y] > 0.1)*/
				
				
				glColor3f(perlin[x][y]*0.9,0.0,perlin[x][y]*0.9);
				
				
			glVertex2i(x,y);
		glEnd();
		
		//if (a < 0.0)
		/*if(perlin[x][y] == 0.0)
			printf("a is %d, %d: %f\n", x,y, perlin[x][y]);*/
		//else 
	}
}


void initialise()
{
		for (int x =30; x < 50; x++)
			for(int y = 20; y < 50; y++)
			{
				double fun = PerlinNoise_2D(x,y);
				//printf(" fun is : %g\n", fun);
			}
}

float Noise(int x, int y)
{	
	int r1 = 15731;
	int r2 = 789221;
	int r3 = 1376312589;

	int n = x + y * 57;
	n = (n<<13) ^ n;

    return ( 1.0 - ( (n * (n * n * r1 + r2) + r3) & 0x7fffffff) / 1073741824.0);
}


float SmoothNoise(int x, int y)
{
    float corners = ( Noise(x-1, y-1)+ Noise(x+1, y-1)+ Noise(x-1, y+1)+ Noise(x+1, y+1) ) / 16;
    float sides   = ( Noise(x-1, y)  + Noise(x+1, y)  + Noise(x, y-1)  + Noise(x, y+1) ) /  8;
    float center  =  Noise(x, y) / 4;

	//printf("smooth Noise is : %g\n", corners + sides + center);
    return corners + sides + center;
}


float InterpolatedNoise(float x, float y)
{
int integer_X, integer_Y;
float fractional_X, fractional_Y;
float v1,v2,v3,v4, i1, i2;

      integer_X    = int(x);
      fractional_X = x - integer_X;

	  //printf("fractional-X is :%f\n", fractional_X);

      integer_Y    = int(y);
      fractional_Y = (int)y - integer_Y;

      v1 =  SmoothNoise(integer_X,     integer_Y);
      v2 =  SmoothNoise(integer_X + 1, integer_Y);
      v3 =  SmoothNoise(integer_X,     integer_Y + 1);
      v4 =  SmoothNoise(integer_X + 1, integer_Y + 1);

	  i1 = Interpolate(v1 , v2 , fractional_X);
      i2 = Interpolate(v3 , v4 , fractional_X);
		
	   //printf("v1 is : %g\n", i1);
		//if (int(x) == 2 && (int)y == 2)
		//printf("---interpolated noise is :%g\n", Interpolate(i1 , i2 , fractional_Y));

      return Interpolate(i1 , i2 , fractional_X);
}



float Interpolate(float a, float b, int x)
{
	float ft,f;
	ft = x * 3.1415927;
	f = (1 - cos(ft)) * .5;

	return  a*(1-f) + b*f;

}


  



float PerlinNoise_2D_add(int x, int y, float pers, float freq, float oct,float amp)
{
	
int n;
float total,p;

  total = 0.0;
  p = pers;
  n = oct - 1;

  //freq = 0.015;//pow(2.0, i);
  //amplitude = 1.0;//pow(p, i);


  for(i=0; i<n; i++)
  {
     // freq = pow(0.2, i);
     // amp = pow(p, i);
     total = total + InterpolatedNoise((float)x * freq, (float)y * freq) * amp;
	 freq =(freq);
	 amp =   amp * pers;
  }

 double cloudCoverage = 0.90;   // USER ADJUSTABLE
 double cloudDensity = .61;     // USER ADJUSTABLE

 total = (total + cloudCoverage) * cloudDensity;
 
   if (total < 0.0) 
	   total = 0.;
   else if (total > 1.0)
			total = 1.;

   return  total;
}

float PerlinNoise_2D(int x, int y)
{

int n, Number_Of_Octaves = 16; 
float total,p,persistence = 0.65;
float frequency, amplitude;

      total = 0.0;
      p = persistence;
      n = Number_Of_Octaves - 1;
	  frequency = 0.015;//pow(2.0, i);
     // amplitude = 1.0;//pow(p, i);

  for(i=0; i<n; i++)
  {
      //frequency = pow(0.2, i);
     amplitude = pow(p, i);
      total = total + InterpolatedNoise((float)x * frequency, (float)y * frequency) * amplitude;
	  frequency = frequency * 1.55;
      //amplitude = amplitude * persistence;
  }

// double cloudCoverage = 0.30;   // USER ADJUSTABLE
  // double cloudDensity = 3.1;    // USER ADJUSTABLE

  //total = (total + cloudCoverage) * cloudDensity;
 
   if (total < 0.0) 
	   total = 0.;
   else if (total > 1.0)
			total = 1.;


      return ( total);
}

