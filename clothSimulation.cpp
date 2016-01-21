
#include "stdafx.h"


//#include <windows.h>
#include <iostream>
#include <GL/glut.h>
//#include <stdout.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "PerlinNoise.h"


//GLOBALS

//lighting:

GLfloat LightAmbient[]   =	{ 1.0,0.0,0.0,1.0 };
GLfloat LightDiffuse[]   =	{ 0.5,0.5,0.5,1.0 };
GLfloat LightPosition[]  =	{ 0,3.,-2.,0 };
GLfloat LightPosition2[] =	{ 0,3.,2.,0 };


Vector wind;
Vector gravity;

bool g_bPaused = false;


PerlinNoise3D * perlin;

Vector vecDebug = F3dVector (0.0, 0.0, 0.0);
float fDotDebug = 0.;
float m_time = 0.;
//float m_Damping, m_k, m_stepSize, m_Dist, m_dt;

void initNoise(){
	srand((unsigned)time(NULL));
	
	perlin->createNoise();
	/*
	perlinY->createNoise();
	perlinZ->createNoise();
	*/
}

struct Particle
{
	Vector m_pos; //position of the particle in space  
	Vector m_normal; //normal needed to calculate the force created by the wind
	Vector m_Velocity; //the change in position
	Vector m_Accelaration; //change in accelaration
};

class Flag
{
protected:

	bool m_Initialized;
	
	int m_Width, m_Height;

	float m_spring_const; //Stiffness
	float m_Damping;	  //energy loss due to friction	
	float mass;

	float m_DirectDistance; //Direct distance between two particles
	float m_AslantDistance; //Diagonal Distance between two particles

	Particle* m_Particles;
	int m_NumParticles;
	GLuint * m_Indices;
	int m_NumIndices;  

private:
	Vector GetForce(int xParticle, int yParticle, Vector OtherVertexPos, float StandardDistance);
	void UpdateNormals();


public:
	Flag();
	
	void Initialize(int Width, int Height, float FlagSize, float spring_const, float damping);

	void UpdateVertices( float dtime, Vector gravity, Vector wind);
	void Render();
	void setMass(int i, int j, float m);
};

#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f);

Flag::Flag()
{
	m_Initialized = false;
}

void Flag::Initialize(int Width, int Height, float FlagSize, float spring_const, float damping)
{
	//*********************
	//Init vertex array:
	this->m_NumParticles = Width * Height;
	numGlobalParticlesX = Width;
	numGlobalParticlesY = Height;

	this->m_Width = Width;
	this->m_Height = Height;
	this->m_spring_const = spring_const;
	this->m_Damping = damping;
	this->m_Initialized = true;

	//Flag.Initialize(40,26, 2.0f, 100.0f, 0.977f);
	//Create an array of particles that represents the flag particle matrix
	this->m_Particles = new Particle[this->m_NumParticles]; 

	    
	//Flag size is the amount of space a flag takes
	//Direct Distane hold the distance between each particle = the length of a spring
	m_DirectDistance = FlagSize /(Width - 1);

	//Holds the distance between two particles that are diagonally across from each other 
	m_AslantDistance = sqrt(2.0f)* m_DirectDistance;


	for (int yc = 0; yc < Height; yc++)
		for (int xc = 0; xc < Width; xc++)
		{
			int ArrayPos = yc * Width + xc; //Accessing the particle(i, j) of the flag
			

			//init the position of each particle 
			m_Particles[ArrayPos].m_pos = F3dVector(xc * m_DirectDistance,
											     yc * m_DirectDistance,
												 0.0f);

			//init the normal of each particle 
			m_Particles[ArrayPos].m_normal = F3dVector(0.0f,
											    	 0.0f,
													-1.0f);

			//each particle holds a texture coordinate that corresponds to the coordinate of the flag 
            //m_Particles[ArrayPos].m_TexCoords.x = xc / float ( Width - 1);
			//m_Particles[ArrayPos].m_TexCoords.y = yc / float ( Height - 1);

			//init velocity which is NULL for now
			m_Particles[ArrayPos].m_Velocity = NULL_VECTOR;
		}
	


	m_NumIndices = 3 * 2 * (Width - 1) * (Height - 1);
	m_Indices = new GLuint[m_NumIndices];

	for (int yc = 0; yc < Height - 1; yc++)
		for (int xc = 0; xc < Width - 1; xc++)
		{
			//Create two triangles:

			int ArrayPos = 6 * ( yc * (Width - 1) + xc);
			m_Indices[ArrayPos+0] = yc * Width + xc;
			m_Indices[ArrayPos+1] = yc * Width + xc + 1;
			m_Indices[ArrayPos+2] = (yc + 1) * (Width) + xc + 1;

			m_Indices[ArrayPos+3] = yc * Width + xc;
			m_Indices[ArrayPos+4] = (yc + 1) * Width + xc + 1;
			m_Indices[ArrayPos+5] = (yc + 1) * Width + xc;

		}
}

// NormalLength = the length of the spring if the cloth is unstretched

Vector Flag::GetForce(int xOtherVertex, int yOtherVertex, Vector ParticlePos, float NormalLength)
{	
	
	//if ((xOtherVertex >= 0 ) && (xOtherVertex < m_Width) && (yOtherVertex >= 0 ) && (yOtherVertex < m_Height))
	if ((xOtherVertex >= 0 ) && (xOtherVertex < m_Width) && (yOtherVertex >= 0 ) && (yOtherVertex < m_Height))
	{
		//if (m_Particles[0].m_pos == ParticlePos)

		Vector SpringVector = m_Particles[xOtherVertex + m_Width * yOtherVertex].m_pos - ParticlePos;
		float len = GetF3dVectorLength(&SpringVector);

		if (len == 0.0f){  
			return NULL_VECTOR;  
		}
		
				float x = len - NormalLength;
		SpringVector = SpringVector * float(1 / len);
		Vector ForceVector = SpringVector * (x * m_Globalk);
		
		return ForceVector;

	} else return NULL_VECTOR;

}

void Flag::UpdateNormals()
{
	for (int xc = 0; xc < m_Width - 1; xc ++)
		for (int yc = 0; yc < m_Height - 1; yc++)
		{
			int ArrayPos = yc * m_Width + xc;

			//Calculating the normal:
			//Take the direction vectors 
			//1.) from the left to the right neighbor 
			//2.) from the upper to the lower neighbor.
			//The vector orthogonal to these 

			Vector u,v,p1,p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)

			if (xc > 0) p1 = m_Particles[ArrayPos-1].m_pos;
			else
						p1 = m_Particles[ArrayPos].m_pos;	
			if (xc < m_Width-1) 
						p2 = m_Particles[ArrayPos+1].m_pos;
			else
						p2 = m_Particles[ArrayPos].m_pos;	
			u = p2-p1; //vector from the left neighbor to the right neighbor
			if (yc > 0) p2 = m_Particles[ArrayPos - m_Width].m_pos;
			else
						p1 = m_Particles[ArrayPos].m_pos;	
			if (yc < m_Height - 1)
						p1 = m_Particles[ArrayPos + m_Width].m_pos;
			else
						p1 = m_Particles[ArrayPos].m_pos;

			v = p1-p2; //vector from the upper neighbor to the lower neighbor
			//calculate the normal:
			m_Particles[ArrayPos].m_normal = Normalize3dVector(CrossProduct(&u,&v));

		}
}

void Flag::UpdateVertices(float dtime, Vector gravity, Vector wind)
{
double scaler = 1.0;
double scaler1 = 1.0;
	
    //Vector * a = new Vector [m_Width * m_Height];  //acceleration array

	for (int xc = 0; xc < m_Width; xc ++){
		for (int yc = 0; yc < m_Height; yc++){
		//if (xc > 0 && xc < (m_Width - 1))
			if (yc >= 0 && yc < (m_Height - 1))
			
			{
		
			//if (xc != 0 || yc != 0){

			int ArrayPos = yc * m_Width + xc;
			Vector VertexPos = m_Particles[ArrayPos].m_pos;

			m_Particles[ArrayPos].m_Accelaration = gravity/scaler1;

			
			//Calculate accelaration for a particle add the sum of forces of its 8 nieghbors
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-1,yc, VertexPos, m_DirectDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+1,yc, VertexPos, m_DirectDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc,  yc-1, VertexPos, m_DirectDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc,  yc+1, VertexPos, m_DirectDistance)/scaler;

			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-1,yc-1,VertexPos, m_AslantDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-1,yc+1,VertexPos, m_AslantDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+1,yc-1,VertexPos, m_AslantDistance)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+1,yc+1,VertexPos, m_AslantDistance)/scaler;



			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-2,yc, VertexPos, m_DirectDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+2,yc, VertexPos, m_DirectDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc,  yc-2, VertexPos, m_DirectDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc,  yc+2, VertexPos, m_DirectDistance*2.)/scaler;

			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-2,yc-2,VertexPos, m_AslantDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc-2,yc+2,VertexPos, m_AslantDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+2,yc-2,VertexPos, m_AslantDistance*2.)/scaler;
			m_Particles[ArrayPos].m_Accelaration = m_Particles[ArrayPos].m_Accelaration + GetForce(xc+2,yc+2,VertexPos, m_AslantDistance*2.)/scaler;



			//wind:
			if (ArrayPos != m_GlobalparticleY * m_Width + m_GlobalparticleX){
			wind.x = sin(m_Particles[ArrayPos].m_pos.x + m_time + (perlin->Turbulence(m_Particles[ArrayPos].m_pos.x + m_time, m_Particles[ArrayPos].m_pos.y + m_time, m_Particles[ArrayPos].m_pos.z + m_time, 'x'))) * m_GlobalScale;
			wind.y = sin(m_Particles[ArrayPos].m_pos.y + m_time + (perlin->Turbulence(m_Particles[ArrayPos].m_pos.x + m_time, m_Particles[ArrayPos].m_pos.y + m_time, m_Particles[ArrayPos].m_pos.z + m_time, 'y'))) * m_GlobalScale;
			wind.z = sin(m_Particles[ArrayPos].m_pos.z + m_time + (perlin->Turbulence(m_Particles[ArrayPos].m_pos.x + m_time, m_Particles[ArrayPos].m_pos.y + m_time, m_Particles[ArrayPos].m_pos.z + m_time, 'z'))) * m_GlobalScale;
			
			//printf("wind is : %g, %g, %g \n",wind.x, wind.y, wind.z );
		
			}
			else{
			wind.x = m_GlobalWindX;
			wind.y = m_GlobalWindY;
			wind.z = m_GlobalWindZ;
				
			}
			
			float DotProduct = Normalize3dVector(wind) * Normalize3dVector(m_Particles[ArrayPos].m_normal);
			fDotDebug = DotProduct;
			
			//printf("dot product is :%g\n", DotProduct);
			
			wind.x = wind.x * DotProduct;
			wind.y = wind.y * DotProduct;
			wind.z = wind.z * DotProduct;			


			m_Particles[ArrayPos].m_Accelaration =  m_Particles[ArrayPos].m_Accelaration + wind;

		
		} else 
		{
			m_Particles[yc * m_Width + xc].m_Accelaration = NULL_VECTOR;
		}
		
		}
	}

	for (int xc = 0; xc < m_Width; xc ++)
		for (int yc = 0; yc < m_Height; yc++)
		{
			int ArrayPos = yc * m_Width + xc;

			
			m_Particles[ArrayPos].m_Velocity = m_Particles[ArrayPos].m_Velocity +  m_Particles[ArrayPos].m_Accelaration * m_GlobalstepSize;
			
			m_Particles[ArrayPos].m_Velocity = m_Particles[ArrayPos].m_Velocity * m_GlobalDamping;

			m_Particles[ArrayPos].m_pos = m_Particles[ArrayPos].m_pos + m_Particles[ArrayPos].m_Velocity * m_GlobalstepSize;	

		}

	
	UpdateNormals();

}


void Flag::setMass(int i, int j, float m)
{
	int SIZE = 20;
	if (i == SIZE-1 && j!= SIZE-1 && j!=0) // A- 5 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (i == SIZE-1 && j == SIZE-1) // B- 3 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (i == SIZE-1 && j == 0) // C- 3 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (i == 0 && j == SIZE-1) // D- 3 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (i == 0 && j != SIZE-1 && j !=0) // E- 5 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (i == 0 && j == 0) // F 3 triangles
	{
		mass = 3.0*m*(1.0/3.0);
	}
	else if (j == 0 && i != SIZE-1 && i !=0) // G- 5 triangles
	{
		mass = 5.0*m*(1.0/3.0);
	}
	else if (j == SIZE-1 && i != SIZE-1 && i !=0) // H - 5 triangles
	{
		mass = 5.0*m*(1.0/3.0);
	}
	else // O - 8triangles
	{
		mass = 8.0*m*(1.0/3.0);
	}
}

void Flag::Render()
{
	
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						m_Particles);
	
	glNormalPointer(	GL_FLOAT,
						sizeof(Particle),
						&m_Particles[0].m_normal.x);
	
		
	glDrawElements(	GL_TRIANGLES, //mode
						m_NumIndices,  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						m_Indices);
}


Flag Flag;

void UpdateScene( float deltaTime)
{
	Flag.UpdateVertices(deltaTime, gravity, wind); //updates the vertices due to time, gravity and wind
}


void DrawScene(void)
{
	
	glPushMatrix();
		Flag.Render();
	glPopMatrix();
}


void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	
	glTranslatef( -1.0f, 0.0f, -15.0f );
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);
	
	DrawScene();
   
	glFlush();			
	glutSwapBuffers();
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(17.0,(GLdouble)x/(GLdouble)y,0.3,40.0);
	glViewport(0,0,x,y);  //Use the whole window for rendering
	glMatrixMode(GL_MODELVIEW);
	
}

void KeyDown(unsigned char key, int x, int y){
	//int debug;
	switch(key){
		case 'p':
			//particleParameters();
			break;
		case 'f':
			//forceParameters();
			break;
		
			exit(0);
			break;
	}
	glutPostRedisplay();
}


void Idle(void)
{
	//Do the physical calculation for one step:
	if (!g_bPaused){
		m_time += m_GlobalstepSize;
		UpdateScene(m_GlobalstepSize);
	}
	//render the scene:
	Display();
}


void initParameters(float damp, float k, float dt, float dist)
{
       m_GlobalDamping = damp;
	   m_Globalk = k;
	   m_GlobalstepSize = dt;
	   m_GlobalDist = dist; 
	   m_GlobalScale = 1.;
	   m_GlobalparticleY = -1;
	   m_GlobalparticleX = -1;
}




int main (int argc, char **argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(600,600);
	//Create a window with rendering context and everything else we need
	glutCreateWindow("Curtain");
	
	initNoise();
		
	initParameters(0.977f, 100.0f, 0.05, 2.0f);
	//initialize flag, wind and gravity:
	Flag.Initialize(30,26, 2.0f, 100.0f, 0.177f);
	

	wind = F3dVector (0.,0.,0.);
	gravity = F3dVector(0.0,-1.915,0.0005);

		
	//Enable the vertex array functionality:
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glClearColor(0.0,0.0,0.0,0.0);  //blue background = sky
		
	//Switch on solid rendering:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE ,1);
	glEnable(GL_DEPTH_TEST);

	//Initialize lighting:
	glEnable(GL_LIGHT1);								
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);

	glFrontFace(GL_CCW);   //Tell OGL which orientation shall be the front face
	glShadeModel(GL_SMOOTH);

	
	//Callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	//glutMouseFunc(mouseButton);
	glutIdleFunc(Idle);
	
	glutMainLoop();
	return 0;
}
