#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "nbody.h"
#include "bodies.h"


// define constants
#define NUM_THREADS 40


// define globals
double           timeStep = 1000; //all units are given in base SI
int              numBodies = 3;
pthread_mutex_t  mutex;
body            *bodies;
vector3D        *forces;


/* Main method
 * 
 * @author Ethan Brummel, Garth Van Donselaar, Jason Vander Woude
 */
int main () {
  //initialize mutex
  pthread_mutex_init(&mutex, NULL);
  
  //get memory space
  //TODO: handle error condition
  bodies = (body*)malloc(sizeof(body) * numBodies);
  forces = (vector3D*)malloc(sizeof(vector3D) * numBodies);
  
  bodies[0].pos.x = 0;
  bodies[0].pos.y = 0;
  bodies[0].pos.z = 0;
  bodies[0].vel.x = 0;
  bodies[0].vel.y = 0;
  bodies[0].vel.z = 0;
  bodies[0].mass = 200000000;
  bodies[0].density = 1;
  
  bodies[1].pos.x = 142;
  bodies[1].pos.y = 0;
  bodies[1].pos.z = 0;
  bodies[1].vel.x = 0;
  bodies[1].vel.y = -140;
  bodies[1].vel.z = 0;
  bodies[1].mass = 10000000;
  bodies[1].density = 1;
  
  bodies[2].pos.x = -142;
  bodies[2].pos.y = 0;
  bodies[2].pos.z = 0;
  bodies[2].vel.x = 0;
  bodies[2].vel.y = 140;
  bodies[2].vel.z = 0;
  bodies[2].mass = 10000000;
  bodies[2].density = 1;
  
  printf("1:\n");
  for (int i=0; i<numBodies; ++i) {
    printf("p:<%f, %f, %f>\t\tv:<%f, %f, %f>\n", bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z, 
	                                     bodies[i].vel.x, bodies[i].vel.y, bodies[i].vel.z);
    fflush(stdout);
  }
  
//   while (1) {
     
    // set all forces to 0
    clearForces();

    // initialize all threads
    pthread_t threads[NUM_THREADS];
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updateForces, (void*)(&mutex));
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }
    
    // initialize all threads
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updatePosAndVels, (void*)(&mutex));
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }
    
     
//   } //END: while (1)
  
  
  printf("2:\n");
  for (int i=0; i<numBodies; ++i) {
    printf("p:<%f, %f, %f>\t\tv:<%f, %f, %f>\n", bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z, 
	                                         bodies[i].vel.x, bodies[i].vel.y, bodies[i].vel.z);
    fflush(stdout);
  }
}




