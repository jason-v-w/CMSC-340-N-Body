#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "bodies.h"


// define constants
#define NUM_THREADS 40


// define globals
double           time_step = 0.1; //all units are given in base SI
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
void* findForces(void);
void* updatePosAndVels(void);


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
  
//   while (1) {
//     
//     // set all forces to 0
//     clearForces();
//     
//     // initialize all threads
//     pthread_t threads[NUM_THREADS];
//     for (int t=0; t<NUM_THREADS; ++t) {
//       pthread_create(&(threads[t]), NULL, findForces, NULL);
//     }
//     
//     // block on thread completion
//     for (int t=0; t<NUM_THREADS; ++t) {
//       pthread_join(threads[t], NULL);
//     }
//     
//   } //END: while (1)
  
  clearForces();
  
  for (int i=0; i<numBodies; ++i) {
     //pair p = getNextBodySet();
     //printf("<%d, %d>\n", p.a, p.b);
    //int b = getNextBody();
    printf("<%f, %f, %f>\n", forces[i].x, forces[i].y, forces[i].z);
    fflush(stdout);
  }
//   body b0 = {0,  0,0,0, -1,0,200,1};
//   body b1 = {142,0,0,0,140,0, 10,1};
//   
//   body currentBodies[] = {b0, b1};
//   body computingBodies[] = {b0, b1};
//   
//   
//   double time = 0;
//   while (1) {
//     body body0 = currentBodies[0];
//     body body1 = currentBodies[1];
//     body newBody0 = computingBodies[0];
//     body newBody1 = computingBodies[1];
//     
//     vector3D force = getForce(body0, body1);
//     
//     vector3D accel0 = getAcceleration(body0, force);
//     vector3D accel1 = getAcceleration(body0, negateVector3D(force));
//     
//     newBody0.vel = vector3DSum(body0.vel, vector3DScale(accel0, time_step));
//     newBody1.vel = vector3DSum(body1.vel, vector3DScale(accel1, time_step));
//     
//     newBody0.pos = vector3DSum(body0.pos, vector3DScale(body0.vel, time_step));
//     newBody1.pos = vector3DSum(body1.pos, vector3DScale(body1.vel, time_step));
//     
//     // copy contents of computingBodies into currentBodies
//     memcpy((void*)currentBodies, (void*)computingBodies, sizeof(body)*num_bodies);
//     
//     // increment time
//     time += time_step;
//   }
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


void *findForces(void) {
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


void *updatePosAndVels(void) {
  pair p;
  
  while (1) {
    p = getNextBodySet();
    if (p.a == -1 && p.b == -1) {
      // kill this thread
      pthread_exit(NULL);
    } else {
      vector3D accelOfA = getAcceleration(bodies[p.a], forces[p.a]);
      vector3D accelOfB = getAcceleration(bodies[p.b], forces[p.b]);
    }
  }
}




