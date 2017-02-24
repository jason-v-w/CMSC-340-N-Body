#include <pthread.h>
#include <stdlib.h>
#include "bodies.h"
#include "nbody.h"

extern body *bodies;
extern double timeStep;

intPair getNextBodySet(pthread_mutex_t mutex) {
    intPair p;
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


int getNextBody(pthread_mutex_t mutex) {
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


void *updateForces(void *vMutex) {
  intPair p;
  
  while (1) {
    p = getNextBodySet(*((pthread_mutex_t*)vMutex));
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


void *updatePosAndVels(void *vMutex) {
  int x;
  
  while (1) {
    x = getNextBody(*((pthread_mutex_t*)vMutex));
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