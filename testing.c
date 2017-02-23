#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "bodies.h"
#define NUM_BODIES 2

double time_step = 1; //all units are given in base SI

int main () {
  body b0 = {0,  0,0   ,0, -1,0,   20000000000000000,1};
  body b1 = {142,0,0   ,0,14,0,      1000000000000000,1};
  
  printf("mass: %f\n", b1.mass);
  
  body currentBodies[] = {b0, b1};
  body computingBodies[NUM_BODIES];
  memcpy((void*)computingBodies, (void*)currentBodies, sizeof(body)*NUM_BODIES);    
  printf("1: (%f, %f), <%f, %f>\n", currentBodies[1].pos.x, currentBodies[1].pos.y, currentBodies[1].vel.x, currentBodies[1].vel.y);
    
  
  
  double time = 0;
  while (1) {
    //printf("0: (%f, %f), <%f, %f>\n", currentBodies[0].pos.x, currentBodies[0].pos.y, currentBodies[0].vel.x, currentBodies[0].vel.y);
    //printf("1: (%f, %f), <%f, %f>\n", currentBodies[1].pos.x, currentBodies[1].pos.y, currentBodies[1].vel.x, currentBodies[1].vel.y);
    
    vector3D force = getForce(currentBodies[0], currentBodies[1]);
    printf("\tforce: [%f, %f, %f]\n", force.x, force.y, force.z);
    
    vector3D accel0 = getAcceleration(currentBodies[0], force);
    vector3D accel1 = getAcceleration(currentBodies[1], negateVector3D(force));
    printf("\taccel: [%f, %f, %f]\n", accel1.x, accel1.y, accel1.z);
    
    computingBodies[0].vel = vector3DSum(currentBodies[0].vel, vector3DScale(accel0, time_step));
    computingBodies[1].vel = vector3DSum(currentBodies[1].vel, vector3DScale(accel1, time_step));
    
    computingBodies[0].pos = vector3DSum(currentBodies[0].pos, vector3DScale(currentBodies[0].vel, time_step));
    computingBodies[1].pos = vector3DSum(currentBodies[1].pos, vector3DScale(currentBodies[1].vel, time_step));
    
    // copy contents of computingBodies into currentBodies
    memcpy((void*)currentBodies, (void*)computingBodies, sizeof(body)*NUM_BODIES);
    printf("1: (%f, %f), <%f, %f>\n", currentBodies[1].pos.x, currentBodies[1].pos.y, currentBodies[1].vel.x, currentBodies[1].vel.y);
    usleep(1000000*time_step);
    
    // increment time
    time += time_step;
  }
}