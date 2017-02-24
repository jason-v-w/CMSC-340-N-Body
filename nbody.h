#ifndef NBODY_H
#define NBODY_H

#include <pthread.h>
#include "bodies.h"

extern int numBodies;
extern vector3D *forces;

// define structs
typedef struct pair {
  int a, b;
} pair;


// define functions
pair getNextBodySet(pthread_mutex_t mutex);
int getNextBody(pthread_mutex_t mutex);
void clearForces();
void *updateForces();
void *updatePosAndVels();

#endif //NBODY_H