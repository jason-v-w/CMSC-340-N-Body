#include <math.h>
#include "bodies.h"



/* Given a vector3D, the vector magnitude is returned
 * 
 * @author Jason Vander Woude
 */
double getMagnitude(vector3D vec) {
  return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}


/* Given two bodies, the force on body 1 from body 2 will be returned in a force struct
 * 
 * @author Jason Vander Woude
 */
vector3D getForce(body bodyOn, body bodyFrom) {
  static const double G = 0.00000000006674; //6.674e−11
  vector3D p, f;
  double pseudoMagnitude;
  
  p.x = bodyFrom.pos.x - bodyOn.pos.x;
  p.y = bodyFrom.pos.y - bodyOn.pos.y;
  p.z = bodyFrom.pos.z - bodyOn.pos.z;
  
  pseudoMagnitude = -1 * G * bodyOn.mass * bodyFrom.mass / pow(getMagnitude(p), 3);
  
  f.x = pseudoMagnitude * p.x;
  f.y = pseudoMagnitude * p.y;
  f.z = pseudoMagnitude * p.z;
  
  return f;
}


/* Given a body and the total force vector acting on it the acceleration vector will be returned
 * 
 * @author Jason Vander Woude
 */
vector3D getAcceleration(body body, vector3D force) {
  //a = F/m
  vector3D a;
  
  a.x = force.x / body.mass;
  a.y = force.y / body.mass;
  a.z = force.z / body.mass;
  
  return a;
}