
#include "stdafx.h"
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_BOIDS 152
#define RADIUS 2.0
#define CIRC_RADIUS 5.0
//#define OUT_RADIUS 25.0




typedef struct 
{
	double x,y,z;
}pos;

double OUT_RADIUS=30.0;

int count = 0;
pos K ={0,0,CIRC_RADIUS}, vec_c, newInterpos;
pos BOID_POS[NUM_BOIDS], NEW_BOID_POS[NUM_BOIDS];
pos dir[NUM_BOIDS];
pos BOID_VELOCITY[NUM_BOIDS];
//pos LOCAL_BOID_POS[NUM_BOIDS] = {0};
pos *LOCAL_BOID_POS = NULL;
pos *LOCAL_BOID_VEL = NULL;
pos BOID_DIRECTION[3]; 
pos P={OUT_RADIUS,0.0,0.0}, C ={0.0,0.0,0.0};
pos *localBoidsPos = NULL, *localBoidsVel = NULL, newPos={0}, newVel={0};
pos newDis={0}, newDir = {0}, newObAvoidance = {0};
double direcX, direcZ, mod, u=0;

#include "Initialise.h"



void keyboard(unsigned char key, int x, int y)
{	
	// continue animation
	if (key == 'a')
		updateBoids();
	if (key == 'z')
	{		
		glutIdleFunc(NULL);
		for(j=0; j<NUM_BOIDS; j++)
		dir[j].y = dir[j].y;
	}
	if (key == 'x')
		glutIdleFunc(timer);
	if (key == 'c')
		glutTimerFunc(7, timerFunc, 1);
	if (key == 'r'|| 'R')
		InitialPos();
	if (key == 'f'|| 'F')
		OUT_RADIUS += 1.;
	if (key == 'g'|| 'G')
		OUT_RADIUS -= 1.;

	glutPostRedisplay();
	return;

}


void drawCube()
{	
	//glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);
	//glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glutSolidCube(1.0);
	
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(4.0,0.0,0.0);
	glEnd();
	glPopMatrix();
}

void obstacle()
{	
	glTranslatef(0.0,3.5,0.0);
	glColor3f(1.0,1.0,0.0);
	glutSolidCube(7.0);
}

void InitialPos()
{
	for(int i=0; i<NUM_BOIDS; i++)
	{
		double x = 30. + rand() % 70 -5.;
		double y = rand() % 20 -5;
		double z = rand() % 80 -37;

		angle = 60. * M_PI/180.0;//(RANDOM_RANGE(60.0, 70.0)) * M_PI/180.0;
		velocity = RANDOM_RANGE(-0.8, 1.0) + MEAN_VELOCITY ;
		BOID_VELOCITY[i].x= velocity * cos(angle);
		BOID_VELOCITY[i].y= velocity * sin(angle)*0.0;
		BOID_VELOCITY[i].z= velocity * cos(angle);

		//printf("boids velocity is : %g, %g, %g \n", BOID_VELOCITY[i].x, BOID_VELOCITY[i].y, BOID_VELOCITY[i].z);

		BOID_POS[i].x = x + BOID_VELOCITY[i].x ;
		BOID_POS[i].y = 0.0;
		BOID_POS[i].z = z + BOID_VELOCITY[i].z;
		dir[i] = BOID_POS[i];

		//printf("boid pos :%g, %g, %g\n",BOID_POS[i].x, BOID_POS[i].y, BOID_POS[i].z);
	}

		/*BOID_DIRECTION[0].x = 0; BOID_DIRECTION[0].y = 0; BOID_DIRECTION[0].z = 0;
		BOID_DIRECTION[1].x = -15; BOID_DIRECTION[1].y = 0; BOID_DIRECTION[1].z = -15;
		BOID_DIRECTION[2].x = -45; BOID_DIRECTION[2].y = 0; BOID_DIRECTION[2].z = 0;*/
}


void updateBoids()
{
	//pos newInterpos;
	double deg;
	double scaler = 1.0;
	
	for (j=0; j<NUM_BOIDS; j++)
	{

		newDis = BOID_checkDistance(BOID_POS[j], NUM_BOIDS);
		newPos = BOID_avgPosition(BOID_POS[j], NUM_BOIDS);
		newVel = BOID_avgVelocity(BOID_VELOCITY[j], NUM_BOIDS);
		
		//printf(" boid pos is %d: %g, %g, %g\n", j, BOID_POS[j].x, BOID_POS[j].y, BOID_POS[j].z);
		newObAvoidance = avoidObject(BOID_POS[j]);	

		if ((0. < BOID_POS[j].x && BOID_POS[j].x <=CIRC_RADIUS)&&  (BOID_POS[j].z <= CIRC_RADIUS+1 && BOID_POS[j].z >=-(CIRC_RADIUS+0.25)))
		{
			newInterpos = interpolation(BOID_POS[j]);

			BOID_POS[j].x = newInterpos.x/scaler;
			BOID_POS[j].y = newInterpos.y/scaler;
			BOID_POS[j].z = newInterpos.z/scaler;

			/*if(j==20)
				printf(" 1 - boid pos is %d: %g, %g, %g\n",j, newInterpos.x, newInterpos.y, newInterpos.z);*/
		}
		
		else if ((CIRC_RADIUS < BOID_POS[j].x && BOID_POS[j].x <= OUT_RADIUS) && (BOID_POS[j].z <= 1+CIRC_RADIUS && BOID_POS[j].z >=-(CIRC_RADIUS+1)))
			{		
												
					direcX = ((newObAvoidance.x  - BOID_POS[j].x) /100.0);
					direcZ = ((newObAvoidance.z - BOID_POS[j].z) /100.0);

					mod = sqrt(direcX*direcX + direcZ*direcZ);
				
					if (direcZ < 0)
						deg = 180;
					else
						deg = -180;
					
					dir[j].x =0.0;
					dir[j].y = acos(direcX/mod)* deg/M_PI;
					dir[j].z = 0.0;
					//printf("in \n");
					BOID_POS[j].x += ((newObAvoidance.x  - BOID_POS[j].x) /100.0);
					BOID_POS[j].y +=  newObAvoidance.y;
					BOID_POS[j].z += ((newObAvoidance.z - BOID_POS[j].z) /100.0);

				/*if(j==20)
					printf(" 2 - boid pos is %d: %g, %g, %g\n",j, BOID_POS[j].x, BOID_POS[j].y, BOID_POS[j].z);*/
			}
			else 
			{	
				direcX = (newPos.x + newDis.x +  newVel.x  - 0.15)*t;
				direcZ = (newPos.z + newDis.z +  newVel.z )*t;
				//printf(" out \n");
				mod = sqrt(direcX*direcX + direcZ*direcZ);
				
				if (direcZ < 0)
					deg = 180;
				else
					deg = -180;
				
				dir[j].x =0.0;
				dir[j].y = acos(direcX/mod)* deg/M_PI;
				dir[j].z = 0.0;

				/*newVel.x = 0;
				newVel.z = 0;*/

				BOID_POS[j].x += (newPos.x + newDis.x +  newVel.x - 0.15)*t;
				BOID_POS[j].y += newObAvoidance.y;
				BOID_POS[j].z += (newPos.z + newDis.z +  newVel.z)*t;

			//	printf(" boid pos is : %g, %g, %g\n", BOID_POS[j].x, BOID_POS[j].y, BOID_POS[j].z);
			}
		}// for loop
						
		for(i=0; i<NUM_BOIDS;i++)
		{
			NEW_BOID_POS[i].x = BOID_POS[i].x;
			NEW_BOID_POS[i].y = BOID_POS[i].y;
			NEW_BOID_POS[i].z = BOID_POS[i].z;
		}
}

double findteta(pos B)
{
	double teta, dist;
	
	if (B.z < 0)
		K.z = CIRC_RADIUS*sin((double)-90);
	else 
		K.z = CIRC_RADIUS*sin((double)90);
	
	vec_c.x = K.x - B.x;
	vec_c.y = K.y - B.y;
	vec_c.z = K.z - B.z;
	
	dist = sqrt(vec_c.x*vec_c.x + vec_c.y*vec_c.y + vec_c.z*vec_c.z);

	teta = 2* asin(dist/((double)2*CIRC_RADIUS))* ((double)180/M_PI);
	//printf("teta is : %g\n", teta);

	return teta/2;
}

pos interpolation(pos B)
{
	double scaler = 10;
	double teta;
	pos center,P;
	teta = findteta(B);
//	teta = teta * (M_PI/(double)180);

	center.x = CIRC_RADIUS*cos(teta*(M_PI/(double)180.) + ((M_PI/2.)-2.*teta*(M_PI/(double)180.)));
	center.y = 0.;
	center.z = CIRC_RADIUS*sin(teta*(M_PI/(double)180.) + ((M_PI/2.)-2.*teta*(M_PI/(double)180.)));

	// quadratic interpolation
	//printf(" center pos is : %g, %g, %g\n", center.x, center.y, center.z);

	if (B.z < 0.0)
		u = double(-0.01);
	else
		u = double(0.02);

	P.x = (1.-u)*(1.-2.*u)*B.x + 4.0*u*(1.-u)*center.x + u*(2.*u - 1.)*C.x;
	P.y = (1.-u)*(1.-2.*u)*B.y + 4.0*u*(1.-u)*center.y + u*(2.*u - 1.)*C.y;
	P.z = (1.-u)*(1.-2.*u)*B.z + 4.0*u*(1.-u)*center.z + u*(2.*u - 1.)*C.z;
	
//printf(" u  is : %g\n", u);
	//printf("------ P is  is : %g, %g, %g\n", P.x, P.y, P.z);
	
return P;
}

pos BOID_checkDistance(pos b, int n)
{
	double X,Y,Z, distance,div = 200;
	pos C1={0};
	for(int i =0; i< n; i++)
		if (BOID_POS[i].x != b.x || BOID_POS[i].y != b.y || BOID_POS[i].z != b.z)
		{			
			X = b.x - BOID_POS[i].x;
			Y = b.y - BOID_POS[i].y;
			Z = b.z - BOID_POS[i].z;

			distance = sqrt(X*X + Y*Y + Z*Z);
			
			if (distance <=3.0)
			{
				C1.x = (C1.x - (BOID_POS[i].x - b.x))/div;
				C1.y = (C1.y - (BOID_POS[i].y - b.y))/div;
				C1.z = (C1.z - (BOID_POS[i].z - b.z))/div;

			}
			
		}
		//printf("check distance :%g, %g, %g\n",C.x, C.y, C.z);
	return C1;
}

pos BOID_avgPosition(pos b, int n)
{
	pos new_position ={0};
	if (BOID_POS[j].x <= OUT_RADIUS && (BOID_POS[j].z <= OUT_RADIUS && BOID_POS[j].z >=-OUT_RADIUS))
	{
			new_position.x = 0;
			new_position.y = 0;
			new_position.z = 0;
	}

	else 
	{
	for (int i=0; i< n; i++)
		if( (b.x != BOID_POS[i].x) || (b.y != BOID_POS[i].y) || (b.z != BOID_POS[i].z))
		{
			new_position.x += BOID_POS[i].x;
			new_position.y += BOID_POS[i].y;
			new_position.z += BOID_POS[i].z;
		}
	
	new_position.x = new_position.x/(double)(NUM_BOIDS-1);
	new_position.y = new_position.y/(double)(NUM_BOIDS-1);
	new_position.z = new_position.z/(double)(NUM_BOIDS-1);

	new_position.x = (new_position.x - b.x)/(double)200;
	new_position.y = (new_position.y - b.y)/(double)200;
	new_position.z = (new_position.z - b.z)/(double)200;
	
	//printf("new pos : %.2f, %.2f, %.2f\n", new_position.x, new_position.y, new_position.z );
	}
	return new_position;
}

pos BOID_avgVelocity(pos b , int n)
{
	pos new_velocity ={0};
	for (int i=0; i< n; i++)
		if (BOID_VELOCITY[i].x != b.x || BOID_VELOCITY[i].y != b.y || BOID_VELOCITY[i].z != b.z)
		{
			new_velocity.x += BOID_VELOCITY[i].x;
			new_velocity.y += BOID_VELOCITY[i].y;
			new_velocity.z += BOID_VELOCITY[i].z;

		}
	
	new_velocity.x = new_velocity.x/(double)(NUM_BOIDS-1);
	new_velocity.y = new_velocity.y/(double)(NUM_BOIDS-1);
	new_velocity.z = new_velocity.z/(double)(NUM_BOIDS-1);
	
	new_velocity.x = (new_velocity.x - b.x)/18.0;
	new_velocity.y = (new_velocity.y - b.y)/18.0;
	new_velocity.z = (new_velocity.z - b.z)/18.0;
	//printf("new_velocity is  :%g,%g,%g\n", new_velocity.x, new_velocity.y, new_velocity.z);
	return new_velocity;
}

pos BOID_avgDirection(pos b)
{
	pos new_direction ={0};
	double dir = 130;
		
	new_direction.x = BOID_DIRECTION[d].x;
	new_direction.y = BOID_DIRECTION[d].y;
	new_direction.z = BOID_DIRECTION[d].z;
		
	new_direction.x = (new_direction.x - b.x)/dir;
	new_direction.y = (new_direction.y - b.y)/dir;
	new_direction.z = (new_direction.z - b.z)/dir;
	//printf("new_direction is  :%g,%g,%g\n", new_direction.x, new_direction.y, new_direction.z);
	return new_direction;

}

pos centerPos()
{
	
	pos new_position ={0};
	for (int i=0; i< NUM_BOIDS; i++)
		//if( (b.x != BOID_POS[i].x) || (b.y != BOID_POS[i].y) || (b.z != BOID_POS[i].z))
		{
			new_position.x += BOID_POS[i].x;
			new_position.y += BOID_POS[i].y;
			new_position.z += BOID_POS[i].z;
		}
	
	new_position.x = new_position.x/(double)(NUM_BOIDS);
	new_position.y = new_position.y/(double)(NUM_BOIDS);
	new_position.z = new_position.z/(double)(NUM_BOIDS);

	return new_position;

}

pos avoidObject(pos boid)
{
	
	double a,b,c;
	double X,Y,Z;
	double k,s,t;
	pos U, W, B={0}, UV, UVV;

	P = boid;

			X = C.x-P.x;
			Y = C.y - P.y;
			Z = C.z-P.z;

			a = sqrt(X*X + Y*Y + Z*Z);
			b = X*(direcX/mod) + 0 + Z*(direcZ/mod);
			c = sqrt(a*a - b*b );
			
			if(c < OUT_RADIUS)
			{
			   // printf(" intersecting :%g, %g\n" , direcX,  direcZ);
				k = sqrt(a*a - CIRC_RADIUS*CIRC_RADIUS);
				t = (k*k - CIRC_RADIUS*CIRC_RADIUS - a*a)/(-2.0*a);
				s = sqrt( CIRC_RADIUS*CIRC_RADIUS - t*t);

				U.x = X/a; U.y =Y/a; U.z = Z/a;
				
				// cross product UxV
				UV.x = 0;
				UV.y = (U.z*direcX -U.x*direcZ);
				UV.z = 0;
				
				// cross product UxV xU
				UVV.x = UV.y*direcZ -0.0;
				UVV.y = 0.0;
				UVV.z = 0.0 - UV.y*direcX;
								
				W.x = UVV.x/sqrt(UVV.x*UVV.x + UVV.y*UVV.y + UVV.z*UVV.z);
				W.y = UVV.y/sqrt(UVV.x*UVV.x + UVV.y*UVV.y + UVV.z*UVV.z);
				W.z = UVV.z/sqrt(UVV.x*UVV.x + UVV.y*UVV.y + UVV.z*UVV.z);

				//printf(" ans is :%g\n", W.z);
				B.x = (P.x + (a - t)*U.x + s*W.x);
				B.y =  (P.y + (a - t)*U.y + s*W.y);
				B.z =  (P.z + (a - t)*U.z + s*W.z);
								
				//printf("b is : %.2f, %.2f, %.2f\n", B.x, B.y, B.z);
			}
				
	return B;
}



void displayInitialPos()
{
	for (int i=0; i< NUM_BOIDS; i++)
	{
		/*if(i==20)
		glColor3f(0.0,1.0,1.0);
		else*/
		glColor3f(1.0,0.168,0.089);

		glPushMatrix();
		glTranslatef(NEW_BOID_POS[i].x,NEW_BOID_POS[i].y,NEW_BOID_POS[i].z);
		glRotatef(dir[i].y, 0.0,1.0,0.0);
			glPushMatrix();
			drawCube();
			glPopMatrix();
		glPopMatrix();
	}
	
	/*for(j=0; j<3; j++)
	{
	glPushMatrix();
	glColor3f(0.0,0.5,0.5);
	glTranslatef(BOID_DIRECTION[j].x, BOID_DIRECTION[j].y, BOID_DIRECTION[j].z);
	glutSolidSphere(1.0, 10, 10);
	glPopMatrix();
	}*/
}


void drawCircle()
{
   GLint i;
   GLfloat cosine, sine;
  // double const r = 4.0;
	
  
   glBegin(GL_LINE_STRIP);
      for(i=0;i<100;i++){
         cosine = CIRC_RADIUS * cos(i*4*M_PI/100.0);
         sine = CIRC_RADIUS * sin(i*4*M_PI/100.0);
		 glColor3f(1.0,0.2,0.2);
         glVertex3f(cosine,0,sine);}

   glEnd();

    glBegin(GL_LINE_STRIP);
      for(i=0;i<100;i++){
         cosine = OUT_RADIUS * cos(i*4*M_PI/100.0);
         sine = OUT_RADIUS * sin(i*4*M_PI/100.0);
		 glColor3f(1.0,0.2,0.2);
         glVertex3f(cosine,0,sine);}

   glEnd();
   glPointSize(5);
   glBegin(GL_POINTS);
     glVertex3f(4.16,0,2.76);
	  glVertex3f(4.16,0,-2.76);
   glEnd();
}

void timer()
{	
	updateBoids();
	glutPostRedisplay();
}

void timerFunc(int value)
{
	updateBoids();
	glutPostRedisplay();
	glutTimerFunc(13, timerFunc,1);
}


//printf("pos of local boids is :%g, %g, %g\n"
//, localBoids[j].x,localBoids[j].y, localBoids[j].z);*/


