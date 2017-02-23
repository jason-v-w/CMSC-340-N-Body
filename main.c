#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "bodies.h"

// double time_step = 0.1; //all units are given in base SI
int numBodies = 3;
pthread_mutex_t mutex;
body *currentBodies;
body *computingBodies;

typedef struct pair {
  int a, b;
} pair;


pair getNextBodySet(int numBodies);


int main () {
  //initialize mutex
  pthread_mutex_init(&mutex, NULL);
  
  //get memory space
  //TODO: handle error condition
  currentBodies = (body*)malloc(sizeof(body) * numBodies);
  
  
  //get memory space
  //TODO: handle error condition
  computingBodies = (body*)malloc(sizeof(body) * numBodies);
  
  
  
  
  
  
  
  for (int i=0; i<8; ++i) {
     pair p = getNextBodySet(numBodies);
     printf("<%d, %d>\n", p.a, p.b);
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


pair getNextBodySet(int numBodies) {
    pair p;
    int static finished = 0;
  
    //guard with mutex
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
      
      //release mutex
      pthread_mutex_unlock(&mutex);
      
      if (i==numBodies) {
	finished = 1;
      }
    }
    
    if (finished) {
      p.a = -1;
      p.b = -1;
    }
    
    return p;
}


void* threadJob(void) {
  pair p;
  
  while (1) {
    p = getNextBodySet(numBodies);
    if (p.a == -1 && p.b == -1) {
     //kill this thread 
    }
  }
  
}