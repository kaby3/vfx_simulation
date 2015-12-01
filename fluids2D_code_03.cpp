// fluids01.cpp : Defines the entry point for the console application.

//#include "stdafx.h"
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIZE 20
#define TANK_WIDTH 20

#define TANK_HEIGHT 20
#define NO_OF_CELLS 20
#define TOT_CELLS (NO_OF_CELLS*NO_OF_CELLS)

#define CELL_WIDTH 2

#define PARTICLES_WIDTH 120
#define PARTICLES_HEIGHT 40

#define NUM_OF_PARTICLES   PARTICLES_WIDTH*PARTICLES_HEIGHT



double buffer_vel[TANK_WIDTH * TANK_HEIGHT][2];
double force_u = 0.1, force_v = -0.1;
//double t;

#include "2D_Main.h"

double newParticlePos[NUM_OF_PARTICLES];


void Fluid::buildTank()
{
		glBegin(GL_LINE_LOOP);

			glVertex3f(-20.,-20.0,0.0);
			glVertex3f(20.,-20.0,0.0);
			glVertex3f(20.,20.0,0.0);
			glVertex3f(-20.,20.0,0.0);
		glEnd();

}

//printf("pp_number is : %d\n", **pp_cNumber);
void Fluid::buildCubes()
{
	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				glPushMatrix();
				glColor3f(1.0,0.0,1.0);
				glTranslatef((i*2 - 20),(j*2 - 20),0);
				buildOneCell();
				glPopMatrix();
			}

// glPointSize(5);
// glBegin(GL_POINTS);
// glColor3f(0.0,0.0,0.0);
// glVertex3f((7*2 - 20),(9*2 - 20),0);39957.6,
// glColor3f(1.0,0.0,0.0);
// glVertex3f(0,0,0);
// glEnd();
}


void Fluid::buildOneCell()
{
	glBegin(GL_LINE_LOOP);
			glVertex3f(-1.0,-1.0,0.0);
			glVertex3f(1.0,-1.0,0.0);
			glVertex3f(1.0,1.0,0.0);
			glVertex3f(-1.0,1.0,0.0);
	glEnd();
}


void Fluid::updateGrid()
{
	//colorCells();
	initialise_to_zero();
	find_time_step();
	locateFluidCell();
	update_Boundry_Velocity();
	update_Boundry_Pressure();
	updateCellVelocity_fullCells();
	updatePressure_AllCells();
	updatePressure_fullCells();
	updatePressure_surfaceCells();
	
	//test();
	updateCellVelocity_surfaceCells(); // surface cells
	//pressureProjection(); // conjugate gradient method
	//Fosters_PressureTerm();
	//update_Total_Cell_Velocity(); // VELOCITY WITH PRESSURE
	update_Boundry_Velocity();
	update_Boundry_Pressure();
	t.test();
	updateParticlePos();
	tot_particles_in_tank();
}



void Fluid::InitialiseParticle()
{

double a = 2.0;
double b = -1.4;
double s,t;

//printf("NUM OF PARTICLE IS : %d\n", NUM_OF_PARTICLES);
this->f_NumParticles = NUM_OF_PARTICLES+1;
this->f_Particles = new Particle[this->f_NumParticles];

this->f_numCubes = NO_OF_CELLS * NO_OF_CELLS;
this->f_cubes = new Cube[this->f_numCubes];

//for (i = 0; i < NUM_OF_PARTICLES; i++)
for(i = 0; i< PARTICLES_WIDTH; i++)
	{
		s = -18. + ((double)i/(double)(PARTICLES_WIDTH-1)) * 34.0; // s starts at -19.0,  width is 36
	for(j = 0; j< PARTICLES_HEIGHT; j++)
		{

			t = -18. + (double)j/(double)(PARTICLES_HEIGHT-1) * 10.0;// t starts at -19.0,  width is 9

			int arrayPos = i + PARTICLES_WIDTH * j;

			//printf("s is : %g, %g\n", s, t);
			//printf("randx, randy, randz : %g, %g\n", randx, randy);
			f_Particles[arrayPos].f_pos = F3dVector( s, t, 0.0);


			double randx1 = a*((double)rand() / ((double)(RAND_MAX)+ b ));
			double randy1 = a*((double)rand() / ((double)(RAND_MAX)+ b ));
			double randz1 =0.0;

			f_Particles[arrayPos].f_vel = F3dVector( randx1, randy1, 0);
			//if(arrayPos==2){
			//printf("initialise pos values :%g, %g\n",  f_Particles[i].f_pos.x, f_Particles[i].f_pos.y);
			//printf("particle %d velocity : %g, %g, %g \n",arrayPos, f_Particles[arrayPos].f_vel.x, f_Particles[arrayPos].f_vel.y, randz1);
			//}
		}
	}


f_NumIndices = NUM_OF_PARTICLES;
f_Indices = new GLuint[f_NumIndices];

for (i = 0; i < NUM_OF_PARTICLES; i++)
{
		f_Indices[i] = i;
}


maximum_Particle_Velocity();

}


void Fluid::initialiseCubePos()
{
	
}

void Fluid::InitialseCubes() // initialise velocity and pressure
{

bool A,B,C,D;
double a = 0;
double b = -0.4;

	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;

				f_cubes[arrayPos].c_pos = F3dVector((i*2 - 20),(j*2 - 20),0); // CELL POSITION
				//f_cubes[arrayPos].particleInCell = 0;
			}

	for(i=1; i<TANK_WIDTH-1; i++)
		for(j=1;j<TANK_HEIGHT-1; j++) // VELOCITY AND PRESSURE OF THE CELLS
			{
				int arrayPos = j + TANK_WIDTH * i;


				double randx = rand() % 1;
				double randy = rand() % 1 - 0.5;
				double randz = rand() % 12 - 6;

				randx = randx +((double)rand() / ((double)(RAND_MAX)+ b ));
				randy = randy +((double)rand() / ((double)(RAND_MAX)+ b ));
				randz = randz + ((double)rand() / ((double)(RAND_MAX)+ b));

				if(f_cubes[arrayPos].particleInCell)
						{

							f_cubes[arrayPos].u[0] = randx; f_cubes[arrayPos].u[1] = randy; // VELOCITY


							//if(arrayPos==41 || arrayPos==42 || arrayPos==61 || arrayPos==62)
								//{
									//printf(" i j arrayPos is : %d, %d, %d,  %g, %g\n", i,j, arrayPos, f_cubes[arrayPos].u[0],f_cubes[arrayPos].u[1]);

								//}
						

							A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
							B = f_cubes[arrayPos-1].particleInCell;
							C = f_cubes[arrayPos+1].particleInCell;
							D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

							if(A && B && C && D) // PRESSURE
								{
									f_cubes[arrayPos].pressure =  randz;
								}
							else
								{
									f_cubes[arrayPos].pressure =  0.0;
								}
						}
				else
						{
							f_cubes[arrayPos].u[0] = 0.0; f_cubes[arrayPos].u[1] = 0.0;
							//printf("particle in cell : %d\n", arrayPos);
						}

				f_cubes[arrayPos].u[0] = randx; f_cubes[arrayPos].u[1] = randy;
				f_cubes[arrayPos].pressure =  randz;
			}// FOR LOOP

update_Boundry_Velocity();
temp_cell_velocity();
updatePressure_surfaceCells();
update_Boundry_Pressure();

for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
		{
			int arrayPos = j + TANK_WIDTH * i;
			if(arrayPos==41 ||arrayPos==42 || arrayPos==61 || arrayPos==62)
			{
				//printf(" 1--- cell velocities : %d,  %g, %g\n",  arrayPos, f_cubes[arrayPos].u[0],f_cubes[arrayPos].u[1]);
			}
		}


}


void Fluid::temp_cell_velocity()
{
	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				buffer_vel[arrayPos][0] = f_cubes[arrayPos].u[0];
				buffer_vel[arrayPos][1] = f_cubes[arrayPos].u[1];
			}
}

void Fluid::initialise_to_zero()
{
for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;

				f_cubes[arrayPos].particleInCell = 0;
			}

}


void Fluid::test()
{
// 	for (i = 0; i < NUM_OF_PARTICLES; i++)
// 		{
// 			//glTranslatef(f_Particles[i].f_pos.x, f_Particles[i].f_pos.y, f_Particles[i].f_pos.z);
// 			glBegin(GL_POINTS);
// 			glVertex3f(f_Particles[i].f_pos.x, f_Particles[i].f_pos.y, f_Particles[i].f_pos.z);
// 			glEnd();
// 			// boundary conditions
// 			//printf(" particle pos is : %g, %g, %g\n", f_Particles[i].f_pos.x, f_Particles[i].f_pos.y, f_Particles[i].f_pos.z);
// 		}



	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				//if (j==1)
				{
						//if(f_cubes[arrayPos].particleInCell)
						//printf("fluid cell %d: %d\n",arrayPos, f_cubes[arrayPos].particleInCell);
					//printf(" vel of i, j cells [%d][%d] : %g, %g\n",i, j,f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1]);
					//printf("buffer vel is %d: %g, %g\n", arrayPos, buffer_vel[arrayPos][0], buffer_vel[arrayPos][1]);
				}
			}
}


void Fluid::find_time_step()
{
double scalar = 0.999;


	maximum_Particle_Velocity();
	time_dif = scalar * (CELL_WIDTH/max);
	//printf(" max is : %g   time difference is : %g\n", max, time_dif);

	time_dif2 = scalar *((CELL_WIDTH*CELL_WIDTH)/4.0);
	//printf(" max is : %g   time difference1 is : %g time difference2 is : %g\n", max, time_dif, time_dif2);
}


void Fluid::locateFluidCell()
{


int p_counter =0,cel_no = 275;
double x1,y1,z1, x2,y2,z2, *fluid_info;
NO_OF_FLUIDCELLS = 0;
int n;


//printf(" Cube pos is %d: %g, %g, %g\n", cel_no, f_cubes[cel_no].c_pos.x, f_cubes[cel_no].c_pos.y, f_cubes[cel_no].c_pos.z);

for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;

		f_cubes[arrayPos].particleInCell = 0;

		p_counter =0;
		f_cubes[arrayPos].particle_no = NULL;

		for(k = 0; k< NUM_OF_PARTICLES; k++)
			{
				//printf("num of particles :%d\n", k);
				x1 = f_cubes[arrayPos].c_pos.x - 1.0; y1 = f_cubes[arrayPos].c_pos.y - 1.0; z1 = f_cubes[arrayPos].c_pos.z - 1.0;

				x2 = f_cubes[arrayPos].c_pos.x + 1.0; y2 = f_cubes[arrayPos].c_pos.y + 1.0; z2 = f_cubes[arrayPos].c_pos.z + 1.0;


				// locating the particle within the cell
				if ((f_Particles[k].f_pos.x >= x1 && f_Particles[k].f_pos.x <= x2) && (f_Particles[k].f_pos.y >= y1 && f_Particles[k].f_pos.y <= y2))
					{


						//printf("True for particle:  %d  Cell : %d , %d\n", k, i, j);
						//f_cubes[i].particle_no[p_counter] = k;30
						//printf("Particle pos is : %g, %g\n",f_Particles[k].f_pos.x, f_Particles[k].f_pos.y );
						//printf("Cell pos is : %g, %g\n", f_cubes[arrayPos].c_pos.x, f_cubes[arrayPos].c_pos.y);
						f_cubes[arrayPos].particleInCell = 1;
						p_counter ++;

						//printf("p counter is : %d\n", p_counter);

						f_cubes[arrayPos].particle_no = (int*) realloc (this->f_cubes[arrayPos].particle_no, p_counter * sizeof(int));
    					if (this->f_cubes[arrayPos].particle_no==NULL)
      					 { puts ("Error (re)allocating memory"); exit (1); }
     					this->f_cubes[arrayPos].particle_no[p_counter-1] = k;
					}
// 				else if ((f_Particles[k].f_pos.x < x1 && f_Particles[k].f_pos.x > x2) && (f_Particles[k].f_pos.y < y1 && f_Particles[k].f_pos.y > y2))
// 					{
// 						f_cubes[arrayPos].particleInCell = 0;
// 					}
// 				else
// 					{
// 						f_cubes[arrayPos].particleInCell = 0;
// 						//printf(" testing arrayPos is : %d\n", arrayPos);
// 					}

			} //............. NO OF PARTICLES



			if (p_counter >= 1)//printf("\n");
			{
				NO_OF_FLUIDCELLS++;
				//printf("no of fluid cells : %d\n",NO_OF_FLUIDCELLS );
				cell_number = (int*) realloc (this->cell_number, NO_OF_FLUIDCELLS * sizeof(int));
				if (this->cell_number==NULL)https://mymail.tees.ac.uk/w?.BQ.EE.ZxT_.Btw.EB_Ew.CEMV-S.L
      					 { puts ("Error12 (re)allocating memory"); exit (1); }

				this->cell_number[NO_OF_FLUIDCELLS-1] = arrayPos;

				no_of_particles = (int*) realloc (this->no_of_particles, NO_OF_FLUIDCELLS * sizeof(int));
				if (this->no_of_particles==NULL)
      					 { puts ("Error (re)allocating memory"); exit (1); }
				this->no_of_particles[NO_OF_FLUIDCELLS-1] = p_counter;

			}

			//free (f_cubes[arrayPos].numbers);

} //............... TOT CELLS


			//printf("No of cells : %d\n", NO_OF_FLUIDCELLS);
				for (j=0; j< NO_OF_FLUIDCELLS; j++)
					{
					//printf("cell no: %d  no of particle : %d  particle no : ", cell_number[j], no_of_particles[j]);
						 	for (k =0; k<no_of_particles[j]; k++)
 							{
								//printf("%d ",f_cubes[cell_number[j]].particle_no[k]);
								f_Particles[f_cubes[cell_number[j]].particle_no[k]].particleCell_no = cell_number[j];
 									if ((f_cubes[cell_number[j]].particle_no[k]) == 9)
 										{
 											//printf("particle no  : %d ",f_cubes[cell_number[j]].particle_no[k]);
 											//printf("cell number for the particle : %d\n",cell_number[j]);
 										}
 							}
							//printf("\n");
					}
				//setParticleVelocity();
}// locateFluidCell()


void Fluid::update_Boundry_Velocity() // soild-fluid
{
	for(i=0; i<2; i++) // A
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;

				f_cubes[arrayPos].u[0] = 0.0;
				f_cubes[arrayPos].u[1] = 0.0;//-f_cubes[arrayPos+TANK_WIDTH].u[1];

			}

for(i=0; i<TANK_WIDTH; i++) // B
		for(j=19;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				//printf(" arrayPos is : %d\n", arrayPos);
				f_cubes[arrayPos].u[0] = 0.0;//-f_cubes[arrayPos-1].u[0];
				f_cubes[arrayPos].u[1] = 0.0;//-f_cubes[arrayPos-TANK_WIDTH].u[1];
			}

for(i=0; i<TANK_WIDTH; i++) // C
		for(j=0;j<2; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;

				f_cubes[arrayPos].u[0] = 0.0;//-f_cubes[arrayPos+1].u[0];
				f_cubes[arrayPos].u[1] = 0.0;//-f_cubes[arrayPos+TANK_WIDTH].u[1];
			}

for(i=19; i<TANK_WIDTH; i++) // D
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				//printf(" arrayPos is : %d\n", arrayPos);
				f_cubes[arrayPos].u[0] = 0.0;//-f_cubes[arrayPos- TANK_WIDTH].u[0];
				f_cubes[arrayPos].u[1] = 0.0;//f_cubes[arrayPos-TANK_WIDTH].u[1];
				//printf("D boundry velocity is :%d,  %g, %g\n", arrayPos,f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1] );
			}
}




void Fluid::update_Boundry_Pressure() // BOUNDARY PRESSURE solid - fluid
{
	for(i=0; i<1; i++) // A
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;

				f_cubes[arrayPos].pressure = f_cubes[arrayPos+TANK_WIDTH].pressure;
			}

for(i=0; i<TANK_WIDTH; i++) // B
		for(j=19;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				//printf(" arrayPos is : %d\n", arrayPos);
				f_cubes[arrayPos].pressure = f_cubes[arrayPos-1].pressure;
			}

for(i=0; i<TANK_WIDTH; i++) // C
		for(j=0;j<1; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				f_cubes[arrayPos].pressure = f_cubes[arrayPos+1].pressure;
			}

for(i=19; i<TANK_WIDTH; i++) // D
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH * i;
				//printf(" arrayPos is : %d\n", arrayPos);
				f_cubes[arrayPos].pressure = f_cubes[arrayPos-TANK_WIDTH].pressure;
			}
}



void Fluid::updateCellVelocity_fullCells() // updating full cells only - cells surrounded by 4 fluidcells
{
bool A,B,C,D; // fluid cell or empty cell;


	for(i=1; i<TANK_WIDTH-1; i++)
			for(j=1;j<TANK_HEIGHT-1; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
						{
							A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
							B = f_cubes[arrayPos-1].particleInCell;
							C = f_cubes[arrayPos+1].particleInCell;
							D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

							if(A && B && C && D)
								{

									f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
									f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);

									if(arrayPos==41 ||arrayPos==42 || arrayPos==61 || arrayPos==62)
										{
											//printf("%d is a full cell---------------------------------\n", arrayPos);
											//printf("2 ----full cell velocities %d: %g, %g\n", arrayPos, f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1]);
										}

									//f_cubes[arrayPos+TANK_WIDTH].u[0] = finite_differences(arrayPos+TANK_WIDTH, 0);
									//f_cubes[arrayPos+1].u[1] = finite_differences(arrayPos+1, 1);


									//buffer_vel[arrayPos][0] = f_cubes[arrayPos].u[0];
									//buffer_vel[arrayPos][1] = f_cubes[arrayPos].u[1];
									//buffer_vel[arrayPos+TANK_WIDTH][0]= f_cubes[arrayPos+TANK_WIDTH].u[0];
									//buffer_vel[arrayPos+1][1]=f_cubes[arrayPos+1].u[1];
								}
						}
			}// FOR

temp_cell_velocity(); // COPY ALL CELL VELOCITY

}

void Fluid::updatePressure_AllCells() // update pressure and velocities
{
bool A,B,C,D; // fluid cell or empty cell;
double B1,B0,D1,D2,D3;

double p_diff, temp_u, temp_v, temp_u1, temp_v1;

B0 =1.7;
B1 = (B0/(2.0*time_dif))*(2.0/CELL_WIDTH*CELL_WIDTH);

	for(i=1; i<TANK_WIDTH-1; i++)
			for(j=1;j<TANK_HEIGHT-1; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					//if(f_cubes[arrayPos].particleInCell)
				D1 = (1.0/CELL_WIDTH) * (f_cubes[arrayPos].u[0] - f_cubes[arrayPos+TANK_WIDTH].u[0]);
				D2 = (1.0/CELL_WIDTH) * (f_cubes[arrayPos +1].u[1] - f_cubes[arrayPos].u[1]);

				D3 = -(D1+D2);

				p_diff = B1*D3;

				//temp_u = f_cubes[arrayPos].u[0];
				//temp_v = f_cubes[arrayPos].u[1];
				//temp_u1 = f_cubes[arrayPos+TANK_WIDTH].u[0];
				//temp_v1 = f_cubes[arrayPos+1].u[1];

				//printf("B1 : %g,  D1: %g, pressure is : %g\n",B1, D1,f_cubes[arrayPos].pressure );

				f_cubes[arrayPos].u[0] +=  (time_dif/CELL_WIDTH)*(p_diff);
										//printf(" presure is : %g vel is : %g\n", f_cubes[arrayPos].pressure, f_cubes[arrayPos].u[0]);
				f_cubes[arrayPos].u[1] -=  (time_dif/CELL_WIDTH)*(p_diff);
				//f_cubes[arrayPos+TANK_WIDTH].u[0] -=  (time_dif/CELL_WIDTH)*(p_diff);
				//f_cubes[arrayPos+1].u[1] +=  (time_dif/CELL_WIDTH)*(p_diff);

				//f_cubes[arrayPos].pressure += p_diff;

				//}// if f_cubes[arrayPos].particleInCell
			}// for loop

temp_cell_velocity();

}

void Fluid::updatePressure_fullCells() // check for divergence
{

//time_dif = 0.8;
//printf("time diff : %g \n", time_dif);

bool A,B,C,D; // fluid cell or empty cell;
double B1,B0,D1,D2,D3; // D1 - divergence

double p_diff,temp_u,temp_v,temp_u1,temp_v1,e = 0.0001;

B0 =1.7;
B1 = (B0/(2.0*time_dif))*(2.0/CELL_WIDTH*CELL_WIDTH);

	for(i=1; i<TANK_WIDTH-1; i++)
			for(j=1;j<TANK_HEIGHT-1; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
						//arrayPos = 171;
					if(f_cubes[arrayPos].particleInCell)
						{

							A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
							B = f_cubes[arrayPos-1].particleInCell;
							C = f_cubes[arrayPos+1].particleInCell;
							D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

							if(A && B && C && D)
								{

								//do{
									for(k=0; k< 10; k++)
									{

										D1 = (1.0/CELL_WIDTH) * (f_cubes[arrayPos].u[0] - f_cubes[arrayPos+TANK_WIDTH].u[0]);
										D2 = (1.0/CELL_WIDTH) * (f_cubes[arrayPos +1].u[1] - f_cubes[arrayPos].u[1]);

										D3 = -(D1+D2);

										//printf(" k : %d  arrayPos : %d D3 : %g\n", k, arrayPos,D3);

										//printf("arrayPos is : %d   B1 : %g,  D3: %g, pressure is : %g\n",arrayPos, B1, D3,f_cubes[arrayPos].pressure );

										if(D3 < e)break;


										p_diff = B1*D3;



										//temp_u = f_cubes[arrayPos].u[0];
										//temp_v = f_cubes[arrayPos].u[1];
										//temp_u1 = f_cubes[arrayPos+TANK_WIDTH].u[0];
										//temp_v1 = f_cubes[arrayPos+1].u[1];

										f_cubes[arrayPos].u[0] +=  (time_dif/CELL_WIDTH)*(p_diff);
										//printf(" presure is : %g vel is : %g\n", f_cubes[arrayPos].pressure, f_cubes[arrayPos].u[0]);
										f_cubes[arrayPos].u[1] -=  (time_dif/CELL_WIDTH)*(p_diff);
										f_cubes[arrayPos+TANK_WIDTH].u[0] -=  (time_dif/CELL_WIDTH)*(p_diff);
										f_cubes[arrayPos+1].u[1] +=  (time_dif/CELL_WIDTH)*(p_diff);

										//printf("Vel is : %g, %g\n", f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1]);



									} // for k
									//}while (D1 < e);

									f_cubes[arrayPos].pressure += p_diff;

 								}
// 							else
// 								{
// 									f_cubes[arrayPos].pressure = 0.0;
// 								}

						}// if f_cubes[arrayPos].particleInCell
			}// for loop

temp_cell_velocity();
}

void Fluid::updatePressure_surfaceCells()// Pressure for fluid - air
{
bool A,B,C,D;
	for(i=1; i<TANK_WIDTH-1; i++)
			for(j=1;j<TANK_HEIGHT-1; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
					{
						A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
						B = f_cubes[arrayPos-1].particleInCell;
						C = f_cubes[arrayPos+1].particleInCell;
						D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

						if(!A || !B || !C ||!D)
							{
								f_cubes[arrayPos].pressure = 0.0;
							}
						//else if(!C)
						//	{
								//f_cubes[arrayPos+1].pressure = f_cubes[arrayPos].pressure;
							//}

					}
			}


}


void Fluid::updateCellVelocity_surfaceCells() // surface cells
{

//double force_u = 0.1, force_v = 0.2;
bool A,B,C,D; // fluid cell or empty cell;
int a,b,c,d;  // array Position.
int fun = 0;

int find = 190;

//maximum_Particle_Velocity();
//time_dif = CELL_WIDTH/max;

//printf("updateCellVelocity  time is : %g\n", time_dif);

		for(i=1; i<TANK_WIDTH-1; i++)
			for(j=1;j<TANK_HEIGHT-1; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
					{
							fun++;

								A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
								B = f_cubes[arrayPos-1].particleInCell;
								C = f_cubes[arrayPos+1].particleInCell;
								D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

								a = arrayPos-TANK_WIDTH; b = arrayPos-1; c =arrayPos+1; d = arrayPos+TANK_WIDTH;

								// if cell has 1 surface face.

								if(!A && B && C && D) // if A false
									{

										//if(arrayPos == find)
												//printf(" 1    !A && B && C && D found the arrayPos %d\n", arrayPos);

										f_cubes[arrayPos].u[0] = buffer_vel[d][0] + buffer_vel[c][1] - buffer_vel[arrayPos][1];
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);

										//f_cubes[arrayPos+TANK_WIDTH].u[0] = finite_differences(arrayPos+TANK_WIDTH, 0);
										//f_cubes[arrayPos+1].u[1] =finite_differences(arrayPos+1, 1);

									}
								else if(A && !B && C && D) // if B false
									{
										//if(arrayPos == find)
												//printf("2  A && !B && C && D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] =  buffer_vel[d][0] - buffer_vel[arrayPos][0] + buffer_vel[c][1] + force_v*0.0;


									}
								else if(A && B && !C && D) // if C false
									{
										//if(arrayPos == find)
												//printf("3  A && B && !C && D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos + 1].u[1] = buffer_vel[arrayPos][0] - buffer_vel[d][0] + buffer_vel[arrayPos][1] + force_v*0.0;

										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);

										//velocity of an empty face is not reliable

									}
								else if (A && B && C && !D)
									{
										//if(arrayPos == find)
											//	printf("4  A && B && C && !D found the arrayPos %d\n", arrayPos);
										f_cubes[d].u[0] = buffer_vel[d][0] + buffer_vel[arrayPos][1] -buffer_vel[c][1];

										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
										//velocity of an empty face is not reliable

									}

								// if cell has 2 surface face
								else if (!A && B && C && !D) // 1
									{
										// do nothing
											//printf("5  !A && B && C && !D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] = force_u;
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
									}
								else if (A && !B && !C && D) // 2
									{
										// do nothing
										//printf("6  A && !B && !C && D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] = force_v;
									}
								else if (A && !B && C && !D) // 3
									{
										//if(arrayPos == find)
										//printf("7  A && !B && C && !D found the arrayPos %d\n", arrayPos);

										// EMPTY FACE
										f_cubes[d].u[0] = buffer_vel[arrayPos][0];

										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] =  buffer_vel[arrayPos+1][1] + force_v;

									}
								else if(!A && !B && C && D) // 4
									{
										//if(arrayPos == find)
												//printf("8   !A && !B && C && D found the arrayPos %d\n", arrayPos);

										f_cubes[arrayPos].u[0] = buffer_vel[d][0];
										f_cubes[arrayPos].u[1] = buffer_vel[arrayPos+1][1] + force_v;
									}
								else if(!A && B && !C && D) // 5
									{
										//if(arrayPos == find)
												//printf("9   !A && B && !C && D found the arrayPos %d\n", arrayPos);

										f_cubes[arrayPos].u[0] =  buffer_vel[d][0];
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
									}
								else if(A && B && !C && !D)// 6
									{
										//if(arrayPos == find)
												//printf("10 ---A && B && !C && !D found the arrayPos %d\n", arrayPos);

										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
									}

								//if cell has 3 surface faces
								else if (A && !B && !C && !D) //----1
									{
										//if(arrayPos == find)
											//	printf("11  A && !B && !C && !D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
										f_cubes[arrayPos].u[1] =force_v;
									}
								else if (!A && B && !C && !D)// ---- 2
									{
										//if(arrayPos == find)
											//	printf("12   !A && B && !C && !D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] =buffer_vel[arrayPos][0];
										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
									}
								else if (!A && !B && !C && D)
									{
										//if(arrayPos == find)
												//printf("13   !A && !B && !C && D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] =  buffer_vel[arrayPos+TANK_WIDTH][0];
										f_cubes[arrayPos].u[1] =  force_v;
									}
								else if (!A && !B && C && !D)
									{
										//if(arrayPos == find)
												//printf("14  !A && !B && C && !D found the arrayPos %d\n", arrayPos);
										f_cubes[arrayPos].u[0] = buffer_vel[arrayPos][0];
										f_cubes[arrayPos].u[1] =  buffer_vel[arrayPos+1][1] + force_v;
									}

								// if a cell has 4 surface faces
								else if(!A && !B && !C && !D)
									{
										// body forces only
											f_cubes[arrayPos].u[0] =  force_u;
											f_cubes[arrayPos].u[1] =  force_v;
									}

// 								else if(A && B && C && D)
// 									{
// 										if(arrayPos == find)
// 												printf("A && B && C && D found the arrayPos %d\n", arrayPos);
// 										f_cubes[arrayPos].u[0] = finite_differences(arrayPos, 0);
// 										f_cubes[arrayPos].u[1] = finite_differences(arrayPos, 1);
// 									}

							else  {

// 										if(arrayPos==41 ||arrayPos==42 || arrayPos==61 || arrayPos==62)
// 										{
// 											//printf("62 is a full cell---------------------------------\n");
// 											printf("3--- surface cell velocities %d: %g, %g\n", arrayPos, f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1]);
// 											//printf("$$$$$$$$$$$$$$$Checking for error %d : %d, %d, %d, %d \n",arrayPos, A, B,C,D);
// 										}

									}
					}

							if(arrayPos==41 ||arrayPos==42 || arrayPos==61 || arrayPos==62)
										{
											//printf("62 is a full cell---------------------------------\n");
											//printf("3--- surface cell velocities %d: %g, %g\n", arrayPos, f_cubes[arrayPos].u[0], f_cubes[arrayPos].u[1]);
											//printf("$$$$$$$$$$$$$$$Checking for error %d : %d, %d, %d, %d \n",arrayPos, A, B,C,D);
										}

			} // FOR tank height
			//printf("total cells are : %d\n", fun);
			//printf("------------------------\n");

temp_cell_velocity();


}

double Fluid::finite_differences(int aPos, int n)
{

double au, bu,cu, lap1, lapu, lapv;
double av, bv, cv;

double tot_vel,p;

//printf("finite dif is \n");
if(n == 0)
	{
		au =(time_dif / (4.*CELL_WIDTH));

		bu = ((buffer_vel[aPos- TANK_WIDTH][0] + buffer_vel[aPos][0]) * (buffer_vel[aPos- TANK_WIDTH][0] + buffer_vel[aPos][0])) - ((buffer_vel[aPos+ TANK_WIDTH][0] + buffer_vel[aPos][0]) * (buffer_vel[aPos+ TANK_WIDTH][0] + buffer_vel[aPos][0]));

		cu = (buffer_vel[aPos- 1][0] + buffer_vel[aPos][0]) * (buffer_vel[aPos- TANK_WIDTH][1] + buffer_vel[aPos][1]) - ((buffer_vel[aPos+ 1][0] + buffer_vel[aPos][0]) * (buffer_vel[aPos- TANK_WIDTH + 1][1] + buffer_vel[aPos + 1][1]));

		lap1 =( viscosity * time_dif)/ CELL_WIDTH*CELL_WIDTH;

		lapu = buffer_vel[aPos- TANK_WIDTH][0] + buffer_vel[aPos- 1][0] - 4*buffer_vel[aPos][0] + buffer_vel[aPos+ TANK_WIDTH][0] + buffer_vel[aPos+ 1][0];

		//if(aPos == 289)
		//printf("au :%g  bu: %g  cu: %g lap1: %g lapu: %g  \n", au, bu, cu, lap1, lapu);
		p= (time_dif/CELL_WIDTH) * (f_cubes[aPos].pressure - f_cubes[aPos-TANK_WIDTH].pressure);
		tot_vel = buffer_vel[aPos][0] +  (au* (bu + cu)) + (lap1*lapu) - p;// + time_dif*force_u;
	}
else
	{
			av =  (time_dif / (4.*CELL_WIDTH));

			bv = ((buffer_vel[aPos - TANK_WIDTH][1] + buffer_vel[aPos][1])* (buffer_vel[aPos-1][0] + buffer_vel[aPos][0]))
			- ((buffer_vel[aPos + TANK_WIDTH][1] + buffer_vel[aPos][1]) * (buffer_vel[aPos+ TANK_WIDTH - 1][0] + buffer_vel[aPos+ TANK_WIDTH][0]));

			cv = (buffer_vel[aPos-1][1] + buffer_vel[aPos][1])*(buffer_vel[aPos-1][1] + buffer_vel[aPos][1]) - (buffer_vel[aPos+1][1] + buffer_vel[aPos][1])*(buffer_vel[aPos+1][1] + buffer_vel[aPos][1]);

			lapv = buffer_vel[aPos- TANK_WIDTH][1] + buffer_vel[aPos- 1][1] - 4*buffer_vel[aPos][1] + buffer_vel[aPos+ TANK_WIDTH][1] + buffer_vel[aPos+ 1][1];

			p= (time_dif/CELL_WIDTH) * (f_cubes[aPos].pressure - f_cubes[aPos-1].pressure);

			tot_vel = buffer_vel[aPos][1] + (av *(bv + cv)) + (lap1 * lapv) -p + time_dif*force_v;
	}

//printf("advection is %g\n", advection);
return tot_vel;
}


void Fluid::maximum_Particle_Velocity()
{

max = modulus(f_Particles[0].f_vel);

	for(i=1; i< NUM_OF_PARTICLES; i++)
		{
			//printf("Particle velocity : %g, %g, mod is:  %g\n", f_Particles[i].f_vel.x, f_Particles[i].f_vel.y, modulus(f_Particles[i].f_vel));

			if (modulus(f_Particles[i].f_vel) > max)
				{
					max = modulus(f_Particles[i].f_vel);
					maxVel = f_Particles[i].f_vel;
				}
		}
		//max = max +1.0;
		//printf("max particle velocity : %g \n", max);
		//printf("Particle max velocity : %g, %g, %g\n",maxVel.x, maxVel.y, maxVel.z);
}


void Fluid::updateParticlePos()
{

int n, cell_no;

//maximum_Particle_Velocity();


double x1,y1,z1, x2,y2,z2;
//time_dif = CELL_WIDTH/max; // time

//printf("updateParticlePos   time diff is : %g\n", time_dif);

Vector V1, V2, p_pos;

// pp_cNumber = &cNumber; // testing pointers to pointers

	for(i=0; i< NUM_OF_PARTICLES; i++)
		{
			cNumber = &f_Particles[i].particleCell_no;


			V1 = getVelocity(f_Particles[i].f_pos.x, f_Particles[i].f_pos.y, i, 1);

			if(i==9)
				{
					//printf("**** pos values :%g, %g\n",  f_Particles[i].f_pos.x, f_Particles[i].f_pos.y);
					//printf("particle %d interpolated velocity V1 is :%g,%g\n",i, V1.x,V1.y);
					//printf("**** pos values :%g, %g\n",  f_Particles[i].f_pos.x + 0.5*time_dif*V1.x, f_Particles[i].f_pos.y + 0.5*time_dif*V1.y);
				}

			V2 = getVelocity( f_Particles[i].f_pos.x + 1.0*time_dif*V1.x,    f_Particles[i].f_pos.y + 1.0*time_dif*V1.y, i, 0);
			//if(i==9)
			//printf("particle %d interpolated velocity V2 is :%g,%g\n",i, V2.x,V2.y);

			f_Particles[i].f_vel.x = V2.x;
			f_Particles[i].f_vel.y = V2.y;
			f_Particles[i].f_vel.z = 0.0;

			f_Particles[i].f_pos = f_Particles[i].f_pos + (V1 + V2) * (time_dif/2.0); // carlson way
			if(i==9)
			{
					//printf("particle pos is : %g, %g particle velocity : %g, %g\n",f_Particles[i].f_pos.x,  f_Particles[i].f_pos.y, f_Particles[i].f_vel.x, f_Particles[i].f_vel.y);
			}
		}
		//printf("-----\n");
}


Vector Fluid::getVelocity(double x, double y, int p, bool testing)
{
		Vector V;
		//printf("values are : %g, %g\n",x/CELL_WIDTH, y/CELL_WIDTH -1.0 );
		V.x = getInterpolatedValue(x/CELL_WIDTH, y/CELL_WIDTH -0.5, 0, p, testing );
		V.y = getInterpolatedValue(x/CELL_WIDTH - 0.5, y/CELL_WIDTH, 1, p, testing );
		//printf("y of v is : %g\n",y/CELL_WIDTH );
		//printf("first V is : %g, %g\n", V.x, V.y);
		V.z = 0.0;
		return V;
}


double Fluid::getInterpolatedValue(double x, double y, int n, int p, bool testing)
{

double fun2;
	int i = (int)floor(x);
	int j = (int)floor(y);

	//if(n==0)
	//printf("cube vel is : %g\n", f_cubes[*cNumber + TANK_WIDTH + 1].u[n]);
	//printf(" i and j is :  %g\n", (i+1-x)*(j+1-y) * f_cubes[*cNumber].u[n],j);

//if(p==9 && n==0)
if (*cNumber==41)
	{
 		//printf("4 --vel P %d: arrayPos : %d, %g\n", n, *cNumber, f_cubes[*cNumber].u[n] );
 		//printf("4 --vel D %d: arrayPos : %d, %g\n", n, *cNumber+TANK_WIDTH, f_cubes[*cNumber+TANK_WIDTH].u[n] );
 		//printf("4 --vel C %d: arrayPos : %d, %g\n", n, *cNumber+1, f_cubes[*cNumber+1].u[n] );
 		//printf("4 --vel E %d: arrayPos : %d, %g\n", n, *cNumber + TANK_WIDTH + 1, f_cubes[*cNumber + TANK_WIDTH + 1].u[n]);


		//printf("weight functions : %g,  %g,  %g,  %g \n",(i+1-x)*(j+1-y),(x-i)*(j+1-y), (i+1 -x)*(y-j), (x-i)*(y-j) );
	}

fun2 = (i+1-x)*(j+1-y) * f_cubes[*cNumber].u[n] +  (x-i)*(j+1-y)*f_cubes[*cNumber + TANK_WIDTH].u[n] +(i+1-x) * (y-j) * f_cubes[*cNumber+1].u[n] + (x-i)*(y-j) * f_cubes[*cNumber + TANK_WIDTH + 1].u[n];


//fun2 = (i+1-x)*(j+1-y) * buffer_vel[*cNumber][n] + (x-i)*(j+1-y)*buffer_vel[*cNumber + TANK_WIDTH][n] +	(i+1 -x)*(y-j)*buffer_vel[*cNumber+1][n] + (x-i)*(y-j) * buffer_vel[*cNumber + TANK_WIDTH + 1][n];
if(p==39)
{
	//printf("----------------------fun2 is : %g    %g, %g\n", fun2, buffer_vel[*cNumber + TANK_WIDTH + 1][0], buffer_vel[*cNumber + TANK_WIDTH + 1][1] );
}

return fun2;
}


void Fluid::Fosters_PressureTerm()
{
double B,B0,D; // B - relaxation coefficient, D - Divergence within the cell
//D = new double(NO_OF_FLUIDCELLS);


B0 =1.7;
B = (B0/(2.0*time_dif))*(2.0/CELL_WIDTH*CELL_WIDTH);

	for(i=0; i<TANK_WIDTH; i++)
			for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
						{
							D = -( (  (1.0/CELL_WIDTH) * (f_cubes[arrayPos + TANK_WIDTH].u[0] - f_cubes[arrayPos].u[0])) + ((1.0/CELL_WIDTH)*(f_cubes[arrayPos + TANK_WIDTH].u[1] - f_cubes[arrayPos].u[1])) );

// 							//if(D1 > 0.0 || D1 < 0.0)
							//printf("B and D1 is :%g     %g",B,  D1);

							f_cubes[arrayPos].pressure += B*D;
							//if(f_cubes[arrayPos].pressure != 0.0)
							//printf("  pressure is : %g\n", f_cubes[arrayPos].pressure);
						}
					else
						{
							f_cubes[arrayPos].pressure = 0.0;
						}
			}
}

void Fluid::update_Total_Cell_Velocity()
{

bool A,B,C,D;
	for(i=0; i<TANK_WIDTH; i++)
			for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
						{

							A = f_cubes[arrayPos-TANK_WIDTH].particleInCell;
							B = f_cubes[arrayPos-1].particleInCell;
							C = f_cubes[arrayPos+1].particleInCell;
							D = f_cubes[arrayPos+TANK_WIDTH].particleInCell;

							if(A && B && C && D)
								{

									//f_cubes[arrayPos].u[0] += +(time_dif/CELL_WIDTH)*(f_cubes[arrayPos].pressure);// - f_cubes[arrayPos-TANK_WIDTH].pressure );
									//f_cubes[arrayPos].u[1] += +(time_dif/CELL_WIDTH)*(f_cubes[arrayPos].pressure);// - f_cubes[arrayPos-TANK_WIDTH].pressure );
									//f_cubes[arrayPos+TANK_WIDTH].u[0] += -(time_dif/CELL_WIDTH)*(f_cubes[arrayPos].pressure);// - f_cubes[arrayPos-TANK_WIDTH].pressure );
									//f_cubes[arrayPos+1].u[1] += -(time_dif/CELL_WIDTH)*(f_cubes[arrayPos].pressure);// - f_cubes[arrayPos-TANK_WIDTH].pressure );
								}
						}
			}
}


double dotProduct(Vector a, Vector b)
{
	return a.x * b.x + a.y * b.y;
}


void Fluid::updateParticleVelocity()
{
int n;
// weight functions
double A0 = 1.0, A1 = 1.0, A2= 1.0, A3=1.0;
double A4=1.0, A5=1.0, A6=1.0,A7=1.0, A8=1.0;

	for(i=0; i< NUM_OF_PARTICLES; i++)
		{
			//printf("particle's cell no : %d \n", f_Particles[i].particleCell_no);

			n = f_Particles[i].particleCell_no;

		if (((f_Particles[i].f_pos.x >= f_cubes[n].c_pos.x) &&  (f_Particles[i].f_pos.x < f_cubes[n].c_pos.x + 1)) && ((f_Particles[i].f_pos.y >= f_cubes[n].c_pos.y) &&  (f_Particles[i].f_pos.y < f_cubes[n].c_pos.y + 1)))
					{
						f_Particles[i].f_vel.x = A1*f_cubes[n+TANK_WIDTH - 1].u[0] + A2*f_cubes[n+1].u[0] + A0*f_cubes[n].u[0] + A8*f_cubes[n+TANK_WIDTH].u[0];
						f_Particles[i].f_vel.y = A1*f_cubes[n+TANK_WIDTH - 1].u[1] + A2*f_cubes[n+1].u[1] + A0*f_cubes[n].u[1] + A8*f_cubes[n+TANK_WIDTH].u[1];
						//f_Particles[i].f_vel.z = 0.0;
					}


			if ((f_Particles[i].f_pos.x <= f_cubes[n].c_pos.x  &&  f_Particles[i].f_pos.x < f_cubes[n].c_pos.x -1) && (f_Particles[i].f_pos.y >= f_cubes[n].c_pos.y  &&  f_Particles[i].f_pos.y < f_cubes[n].c_pos.y + 1))
			{
				f_Particles[i].f_vel.x = A1*f_cubes[n+TANK_WIDTH - 1].u[0] + A2*f_cubes[n+1].u[0] + A0*f_cubes[n].u[0] + A8*f_cubes[n+TANK_WIDTH].u[0];
				f_Particles[i].f_vel.y = A1*f_cubes[n+TANK_WIDTH - 1].u[1] + A2*f_cubes[n+1].u[1] + A0*f_cubes[n].u[1] + A8*f_cubes[n+TANK_WIDTH].u[1];
			}

			if((f_Particles[i].f_pos.x <= f_cubes[n].c_pos.x  && f_Particles[i].f_pos.x < f_cubes[n].c_pos.x -1)&& (f_Particles[i].f_pos.y <= f_cubes[n].c_pos.y &&  f_Particles[i].f_pos.y > f_cubes[n].c_pos.y - 1))
			{
				f_Particles[i].f_vel.x = A0*f_cubes[n+TANK_WIDTH].u[0] + A4*f_cubes[n].u[0] + A5*f_cubes[n-1].u[0] + A6*f_cubes[n+TANK_WIDTH-1].u[0];
				f_Particles[i].f_vel.y = A0*f_cubes[n+TANK_WIDTH].u[1] + A4*f_cubes[n].u[1] + A5*f_cubes[n-1].u[1] + A6*f_cubes[n+TANK_WIDTH-1].u[1];
			}


			if((f_Particles[i].f_pos.x >= f_cubes[n].c_pos.x  && f_Particles[i].f_pos.x < f_cubes[n].c_pos.x +1)&& (f_Particles[i].f_pos.y <= f_cubes[n].c_pos.y &&  f_Particles[i].f_pos.y > f_cubes[n].c_pos.y - 1))
			{
				f_Particles[i].f_vel.x = A0*f_cubes[n+TANK_WIDTH].u[0] + A4*f_cubes[n].u[0] + A5*f_cubes[n-1].u[0] + A6*f_cubes[n+TANK_WIDTH-1].u[0];
				f_Particles[i].f_vel.y = A0*f_cubes[n+TANK_WIDTH].u[1] + A4*f_cubes[n].u[1] + A5*f_cubes[n-1].u[1] + A6*f_cubes[n+TANK_WIDTH-1].u[1];
			}

		}
}


double modulus(Vector a )
{
		return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

bool Fluid::check_Particle_inCell(Vector p_pos)
{
double x1,y1,z1, x2,y2,z2;
	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
			int arrayPos = j + TANK_WIDTH *i;

// 			for(k = 0; k< NUM_OF_PARTICLES; k++)
// 				{
				//printf("num of particles :%d\n", k);
				x1 = f_cubes[arrayPos].c_pos.x - 1.0; y1 = f_cubes[arrayPos].c_pos.y - 1.0; z1 = f_cubes[arrayPos].c_pos.z - 1.0;

				x2 = f_cubes[arrayPos].c_pos.x + 1.0; y2 = f_cubes[arrayPos].c_pos.y + 1.0; z2 = f_cubes[arrayPos].c_pos.z + 1.0;


				// locating the particle within the cell
				if ((p_pos.x >= x1 && p_pos.x <= x2) && (p_pos.y >= y1 && p_pos.y <= y2))
					{
						return arrayPos;
					}
				//}

			}
}

void Fluid::colorCells()
{
//printf("No of cells : %d\n", NO_OF_FLUIDCELLS);
	for(i=0; i<TANK_WIDTH; i++)
		for(j=0;j<TANK_HEIGHT; j++)
			{
				int arrayPos = j + TANK_WIDTH *i;
					if(f_cubes[arrayPos].particleInCell)
					{
							//printf("particle cell   : %d\n", arrayPos );
						if((f_cubes[arrayPos +TANK_WIDTH].particleInCell) && (f_cubes[arrayPos-TANK_WIDTH].particleInCell) && (f_cubes[arrayPos + 1].particleInCell) && (f_cubes[arrayPos-1].particleInCell))  // full cells
							{
								glPushMatrix();
								glColor3f(1.0,0.0,0.0); // red
								glTranslatef((i*2 - 20),(j*2 - 20),0);
								buildOneCell();
								glPopMatrix();
								//printf("full cell   : %d\n", arrayPos );
								// make cell blue
								//printf("true : %d\n", f_cubes[arrayPos].particleInCell);
								//printf("array pos :%d, i %d j %d\n", arrayPos, i, j);
							}
						else // surface cells
							{
								glPushMatrix();
								glColor3f(0.0,1.0,0.0); // green
								glTranslatef((i*2 - 20),(j*2 - 20),0);
								buildOneCell();
								glPopMatrix();
								//printf("surface cell : %d\n", arrayPos );
							}
					}
					else  // empty cells
					{
						glPushMatrix();
						glColor3f(1.0,1.0,0.0); //yellow
						glTranslatef((i*2 - 20),(j*2 - 20),0);
						buildOneCell();
						glPopMatrix();
								//printf(" Empty cell : %d\n", arrayPos);
					}
			} // tank height
}

void Fluid::Render()
{
glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						f_Particles);

glDrawElements(	GL_POINTS, //mode
						f_NumIndices,  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						f_Indices);
}


void Fluid::tot_particles_in_tank()
{
int tot_particles = 0;
	for(k=0; k< NUM_OF_PARTICLES; k++)
		{
			if ((f_Particles[k].f_pos.x >= -21.0 && f_Particles[k].f_pos.x <= 21.0) && (f_Particles[k].f_pos.y >= -21.0 && f_Particles[k].f_pos.y <= 21.0))
				{
					tot_particles++;
					if(k==56)
						{
							//printf("velocity is : %g, %g  ",  f_Particles[k].f_vel.x, f_Particles[k].f_vel.y);
							//printf("pos is  %d:      %g, %g\n",k, f_Particles[k].f_pos.x, f_Particles[k].f_pos.y );

						}

				}
			else
				{
					if(k==9)
					{
					//printf("+++++particle number is : %d  ", k);
					//printf("velocity is %d: %g, %g  ", k, f_Particles[k].f_vel.x, f_Particles[k].f_vel.y);
					//printf("pos is  :      %g, %g\n",f_Particles[k].f_pos.x, f_Particles[k].f_pos.y );
					}

				}
		}

//printf("no of particles inside the tank : %d\n",tot_particles);
//printf("---------------------------\n");
}

void idle()
{
	t.updateGrid();

	glutPostRedisplay();
}

void timerFunc(int value)
{
	t.updateGrid();

	glutPostRedisplay();
	glutTimerFunc(163, timerFunc,1);
}

void KeyDown(unsigned char key, int x, int y){
	int debug;
	switch(key){
		case 'm':
			t.updateGrid();
			break;
		case 'f':
			glutIdleFunc(NULL);
			break;
		case 'p':
			t.tot_particles_in_tank();
		case 'i':
			t.updateParticlePos();

		case 27:
			exit(0);
			break;
	}
	glutPostRedisplay();
}


