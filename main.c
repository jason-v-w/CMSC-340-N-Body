#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h> // for usleep
#include <float.h> // for DBL_MAX
#include "gfx.h"
#include "bodies.h"
#include "filereader.c"


// define constants
#define NUM_THREADS 4


// define globals
char             file[] = "collision.in";
double           timeStep;// = 100; //all units are given in base SI
//int              numBodies = 3;
int		 numBodies;
pthread_mutex_t  mutex;
pthread_mutex_t  testing_mutex;
body            *bodies;
vector3D        *forces;
int getNextBodySeti, getNextBodySetj, getNextBodySetfinished;
int getNextBodyb, getNextBodyfinished;
double displayScale;// = 0.6; // arbitraryish


//set window size
int win_x_size = 1200;
int win_y_size = 800;

// define structs
typedef struct pair {
  int a, b;
} pair;


// forward declarations
pair getNextBodySet(int reset);
int getNextBody(int reset);
void clearForces();
void* updateForces();
void* updatePosAndVels();
void display_system();
void print_system_info(int iteration);
void *checkForCollisions();


/* Main method
 * 
 * @author Ethan Brummel, Garth Van Donselaar, Jason Vander Woude
 */
int main () {
  static int display_delay = 10000;
  
  //initialize mutex
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&testing_mutex, NULL);
  
  //get memory space
  //TODO: handle error condition
  numBodies = getNumBodies(file);
  printf("%d\n",numBodies); fflush(stdout);
  bodies = (body*)malloc(sizeof(body) * numBodies);
  forces = (vector3D*)malloc(sizeof(vector3D) * numBodies);
  readFile(file, numBodies, bodies);

  timeStep = getTimeStep(file);
  displayScale = getDisplayScale(file);

  printf("%lf\n",timeStep);
  printf("%lf\n",displayScale);
  printf("%lf\n",numBodies);
  fflush(stdout);
  print_system_info(0);
  //exit(0);
  
  // Open a new window for drawing.
  gfx_open(win_x_size, win_y_size, "N-Body");

  // initialize all threads
  pthread_t threads[NUM_THREADS];
  
  int iter = 0;
  while (1) {
    // print system state info to terminal
    print_system_info(iter++);
     
    // display system on screen
    display_system();
    
    // set all forces to 0
    clearForces();

    getNextBodySet(1);
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updateForces, NULL);
    }
    
    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }

    getNextBody(1);
    // initialize all threads
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updatePosAndVels, NULL);
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }

    checkForCollisions();

    // sleep while main work is being done in other threads
    // this is not the ideal implementation but is easy to code
    usleep(display_delay);
    
     
  } //END: while (1)
}


pair getNextBodySet(int reset) {
    //static int finished, i, j;
    
      pair p;
      
      if (reset) {
	getNextBodySetfinished = 0;
	getNextBodySeti = 0;
	getNextBodySetj = 0;
	p.a = -1;
	p.b = -1;
	return p;
      }
      
      if (getNextBodySetfinished==0) {
	if (getNextBodySetj==getNextBodySeti) {
	  ++getNextBodySeti;
	  getNextBodySetj=0;
	}
	
	//store return value
	p.a = getNextBodySeti;
	p.b = getNextBodySetj;
	
	++getNextBodySetj;
	
	if (getNextBodySeti==numBodies) {
	  getNextBodySetfinished = 1;
	}
      }
      
      if (getNextBodySetfinished) {
	p.a = -1;
	p.b = -1;
      }

    return p;
}


int getNextBody(int reset) {
    //static int b, finished = 0;
  
    if (reset) {
      getNextBodyfinished = 0;
      getNextBodyb = -1;
      //pthread_mutex_unlock(&mutex);
      return getNextBodyb;
    }
    
    ++getNextBodyb;
    if (getNextBodyb==numBodies)
      getNextBodyfinished = 1;
    if (getNextBodyfinished==1)
      getNextBodyb = -1;

  return getNextBodyb;
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
    pthread_mutex_lock(&mutex);
    p = getNextBodySet(0);
    pthread_mutex_unlock(&mutex);
    
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
    pthread_mutex_lock(&mutex);
    x = getNextBody(0);
    pthread_mutex_unlock(&mutex);
    
    if (x == -1) {
      // kill this thread
      pthread_exit(NULL);
    } else {
      vector3D accel = getAcceleration(bodies[x], forces[x]);
      printf("%lf\n", vector3DMag(accel));fflush(stdout);
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


/*

*/
void *checkForCollisions() {
  static int collisionCount = 0;
  getNextBodySet(1);
  //printf("CALLED\n");fflush(stdout);
  pair p;
  double distance, aRad, bRad;
  
  while (1) {
    //pthread_mutex_lock(&mutex);
    p = getNextBodySet(0);
    //pthread_mutex_unlock(&mutex);
    
    if (p.a == -1 && p.b == -1) {
      return NULL;
    } else {
      distance = vector3DMag(vector3DSum(negateVector3D(bodies[p.a].pos), bodies[p.b].pos)) * displayScale;
      //printf("%lf\n",distance);fflush(stdout);
      aRad = bodies[p.a].disp_radius;
      bRad = bodies[p.b].disp_radius;
      if (distance < aRad + bRad && !(bodies[p.a].mass==0 || bodies[p.b].mass==0 )) {//collision
	printf("COLLISION! %d\n", collisionCount++); fflush(stdout);
        int bigger = aRad > bRad ? p.a : p.b;
        int smaller = aRad > bRad ? p.b : p.a;
	bodies[bigger].mass = bodies[bigger].mass + bodies[smaller].mass;
        //Not correctly calculated
        //double volume = 4.0/3*M_PI*(pow(bodies[bigger].disp_radius,3) + pow(bodies[smaller].disp_radius,3));
        //double newRad = cbrt(volume*3.0/4/M_PI);

        double area = M_PI*(pow(bodies[bigger].disp_radius,2) + pow(bodies[smaller].disp_radius,2));
        double newRad = sqrt(area/M_PI);
        printf("Radius changed from %lf to %lf", bodies[bigger].disp_radius, newRad);
        //bodies[bigger].disp_radius = bodies[bigger].disp_radius + bodies[smaller].disp_radius;
          bodies[bigger].disp_radius = newRad;
          
	  bodies[smaller].pos.x = DBL_MAX;
	  bodies[smaller].pos.y = DBL_MAX;
	  bodies[smaller].pos.z = DBL_MAX;
	  bodies[smaller].vel.x = 0;
	  bodies[smaller].vel.y = 0;
	  bodies[smaller].vel.z = 0;
	  bodies[smaller].mass = 0;
	  bodies[smaller].radius = 0;
	  bodies[smaller].disp_radius = 0;
	}
      }  
    }
}


void display_system() {
  //static double displayScale = 1e-9; // arbitraryish
  gfx_clear();
  gfx_color(255,200,100);
  for (int i=0; i<numBodies; ++i) {
    switch(i) {
      case 0:
	gfx_color(255,0,0);
	break;
      case 1:
	gfx_color(0,255,0);
	break;
      default:
	gfx_color(0,0,255);
    }
    gfx_color(bodies[i].color.r, bodies[i].color.g, bodies[i].color.b);
    gfx_circle(bodies[i].pos.x*displayScale + win_x_size/2, 
	       bodies[i].pos.y*displayScale + win_y_size/2, 
	       bodies[i].disp_radius);
  }
  gfx_flush();
}


void print_system_info(int iteration) {
  printf("%d:\n",iteration);
  for (int i=0; i<numBodies; ++i) {
    printf("p:<%f, %f, %f>\t\tv:<%f, %f, %f>\t\tf:<%f, %f, %f>\n", 
	    bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z,
	    bodies[i].vel.x, bodies[i].vel.y, bodies[i].vel.z, 
	    forces[i].x, forces[i].y, forces[i].z);
    fflush(stdout);
  } 
}

