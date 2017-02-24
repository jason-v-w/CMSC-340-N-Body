#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "bodies.h"


// define constants
#define NUM_THREADS 40


// define globals
double           timeStep = 1000; //all units are given in base SI
int              numBodies = 3;
pthread_mutex_t  mutex;
body            *bodies;
vector3D        *forces;


// define structs
typedef struct pair {
  int a, b;
} pair;


// forward declarations
pair getNextBodySet();
int getNextBody();
void clearForces();
void* updateForces();
void* updatePosAndVels();


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
      pthread_create(&(threads[t]), NULL, updateForces, NULL);
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }
    
    // initialize all threads
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updatePosAndVels, NULL);
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


pair getNextBodySet() {
    pair p;
    int static finished = 0;
  
    //guard with mutex
    //TODO: check if mutex can be locked later
    pthread_mutex_lock(&mutex);
    
    if (finished==0) {
      static int i = 0;
      static int j = 0;
      
      if (j==i) {
	++i;
	j=0;
      }
      
      //store return value
      p.a = i;
      p.b = j;
      
      ++j;
      
      if (i==numBodies) {
	finished = 1;
      }
    }
    
    if (finished) {
      p.a = -1;
      p.b = -1;
    }
      
    //release mutex
    //TODO: check if mutex can be released earlier
    pthread_mutex_unlock(&mutex);
    
    return p;
}


int getNextBody() {
  pthread_mutex_lock(&mutex);
    static int b = -1;
    static int finished = 0;
    ++b;
    if (b==numBodies)
      finished = 1;
    if (finished==1)
      b = -1;
  pthread_mutex_unlock(&mutex);
  
  return b;
}


void clearForces() {
  for (int i=0; i<numBodies; ++i) {
    forces[i].x = 0;
    forces[i].y = 0;
    forces[i].z = 0;
  }
}


void *updateForces() {
  pair p;
  
  while (1) {
    p = getNextBodySet();
    if (p.a == -1 && p.b == -1) {
      // kill this thread
      pthread_exit(NULL);
    } else {
      // compute interaction forces between body A and body B
      vector3D forceOnA = getForce(bodies[p.a], bodies[p.b]);
      vector3D forceOnB = negateVector3D(forceOnA);
      
      // add the respective forces to the running total forces
      forces[p.a] = vector3DSum(forces[p.a], forceOnA);
      forces[p.b] = vector3DSum(forces[p.b], forceOnB);
    }
  }
}


void *updatePosAndVels() {
  int x;
  
  while (1) {
    x = getNextBody();
    if (x == -1) {
      // kill this thread
      pthread_exit(NULL);
    } else {
      vector3D accel = getAcceleration(bodies[x], forces[x]);
      // x = x_0 + v*t - 1/2*a*t^2
      bodies[x].pos = vector3DSum(
                                  vector3DSum(
                                              bodies[x].pos, 
                                              vector3DScale(bodies[x].vel, timeStep)), 
                                  vector3DScale(accel, 0.5*timeStep*timeStep));
      // v = v_0 + a*t
      bodies[x].vel = vector3DSum(bodies[x].vel, vector3DScale(accel, timeStep));
    }
  }
}




