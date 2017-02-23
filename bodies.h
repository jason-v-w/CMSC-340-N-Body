#ifndef BODIES_H
#define BODIES_H

// 3 dimensional position vector
typedef struct vector3D {
  double x, y, z;
} vector3D;


// spacial body
typedef struct body {
  vector3D pos;
  vector3D vel;
  double mass, density;
} body;


/* Given a vector3D, the vector magnitude is returned
 * 
 * @author Jason Vander Woude
 */
double getMagnitude(vector3D vec);


/* Given two bodies, the force on body 1 from body 2 will be returned in a force struct
 * 
 * @author Jason Vander Woude
 */
vector3D getForce(body bodyOn, body bodyFrom);


/* Given a body and the total force vector acting on it the acceleration vector will be returned
 * 
 * @author Jason Vander Woude
 */
vector3D getAcceleration(body body, vector3D force);


/* Given a vector, the opposite vector will be returned
 * 
 * @author Jason Vander Woude
 */
vector3D negateVector3D(vector3D vec);


/* Given two vectors, the sum will be returned
 * 
 * @author Jason Vander Woude
 */
vector3D vector3DSum(vector3D a, vector3D b);


/* Given a vector and scalar, the scaled vector will be returned
 * 
 * @author Jason Vander Woude
 */
vector3D vector3DScale(vector3D vec, double scale);

#endif //BODIES_H