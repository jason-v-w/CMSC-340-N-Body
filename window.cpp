#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>

using namespace std;


// define a struct used to hold a 3D point
typedef struct point {
   double x,y,z;
} point;


int win_width = 900;
int win_height = 1200;
bool mouse_pressed = false; // Boolean to monitor when mouse is pressed


extern "C" {
  #include "bodies.h"
  #include "nbody.h"
  // define constants
  #define NUM_THREADS 40


  // define globals
  double           timeStep = 1000; //all units are given in base SI
  int              numBodies = 3;
  pthread_mutex_t  mutex;
  body            *bodies;
  vector3D        *forces;
  
}



/* Main method */
int main() {
  
  //initialize mutex
  pthread_mutex_init(&mutex, NULL);
  
  //get memory space
  //TODO: handle error condition
  bodies = (body*)malloc(sizeof(body) * numBodies);
  forces = (vector3D*)malloc(sizeof(vector3D) * numBodies);
  
  // initialize bodies
  bodies[0].pos.x = 0;
  bodies[0].pos.y = 0;
  bodies[0].pos.z = 0;
  bodies[0].vel.x = 0;
  bodies[0].vel.y = 0;
  bodies[0].vel.z = 0;
  bodies[0].mass = 200000000;
  bodies[0].density = 1;
  
  bodies[1].pos.x = 142;
  bodies[1].pos.y = 0;
  bodies[1].pos.z = 0;
  bodies[1].vel.x = 0;
  bodies[1].vel.y = -140;
  bodies[1].vel.z = 0;
  bodies[1].mass = 10000000;
  bodies[1].density = 1;
  
  bodies[2].pos.x = -142;
  bodies[2].pos.y = 0;
  bodies[2].pos.z = 0;
  bodies[2].vel.x = 0;
  bodies[2].vel.y = 140;
  bodies[2].vel.z = 0;
  bodies[2].mass = 10000000;
  bodies[2].density = 1;
  
  
  
  // set up window
  sf::RenderWindow window(sf::VideoMode(win_width, win_height), "N-Body", sf::Style::Close|sf::Style::Titlebar);

  cout << timeStep << endl;
  
  while (1) {
    
    clearForces();

    // initialize all threads
    pthread_t threads[NUM_THREADS];
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updateForces, (void*)(&mutex));
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }
    
    // initialize all threads
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_create(&(threads[t]), NULL, updatePosAndVels, (void*)(&mutex));
    }

    // block on thread completion
    for (int t=0; t<NUM_THREADS; ++t) {
      pthread_join(threads[t], NULL);
    }
    
    
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
	window.close();
	exit(0);
      }
    }
    
    // Do display operations
    window.clear();
    window.display();
  }
    
  return 0;
}
