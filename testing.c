#include <stdio.h>
#include <string.h>
#include "bodies.h"

double time_step = 0.1; //all units are given in base SI
int num_bodies = 2;

int main () {
  body b0 = {0,  0,0,0, -1,0,200,1};
  body b1 = {142,0,0,0,140,0, 10,1};
  
  body currentBodies[] = {b0, b1};
  body computingBodies[] = {b0, b1};
  
  
  double time = 0;
  while (1) {
    body body0 = currentBodies[0];
    body body1 = currentBodies[1];
    body newBody0 = computingBodies[0];
    body newBody1 = computingBodies[1];
    
    vector3D force = getForce(body0, body1);
    
    vector3D accel0 = getAcceleration(body0, force);
    vector3D accel1 = getAcceleration(body0, negateVector3D(force));
    
    newBody0.vel = vector3DSum(body0.vel, vector3DScale(accel0, time_step));
    newBody1.vel = vector3DSum(body1.vel, vector3DScale(accel1, time_step));
    
    newBody0.pos = vector3DSum(body0.pos, vector3DScale(body0.vel, time_step));
    newBody1.pos = vector3DSum(body1.pos, vector3DScale(body1.vel, time_step));
    
    // copy contents of computingBodies into currentBodies
    memcpy((void*)currentBodies, (void*)computingBodies, sizeof(body)*num_bodies);
    
    // increment time
    time += time_step;
  }
}