// 3 dimensional position vector
typedef struct position {
  double x, y, z;
} position;

// 3 dimensional velocity vector
typedef struct velocity {
  double x, y, z;
} velocity;

// 3 dimensional acceleration vector
typedef struct acceleration {
  double x, y, z;
} acceleration;

// 3 dimensional force vector
typedef struct force {
  double x, y, z;
} force;

typedef struct body {
  double density, mass;
  position pos;
  velocity vel;
} body;


/*


/* Given two bodies, the force on body 1 from body 2 will be returned in a force struct
 */
force getForce(body bodyOn, body bodyFrom) {
  const double G = 0.00000000006674; //6.674e−11
  position p;
  double pseudoMagnitude;
  force f;
  
  p.px = bodyFrom.pos.px - bodyOn.pos.px;
  p.py = bodyFrom.pos.py - bodyOn.pos.py;
  p.pz = bodyFrom.pos.pz - bodyOn.pos.pz;
  
  pseudoMagnitude = -1 * G * bodyOn.mass * bodyFrom.mass
  
  f.fx =
}


acceleration getAcceleration(body body, double mass, force force) {
  //TODO
}