#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#define NUM_BODIES 2

double time_step = 0.001; //all units are given in base SI
int num_bodies = 2;

using namespace std;

typedef struct pix_coord {
  int x,y;
} pix_coord;

typedef struct point {
  double x,y,z;
} point;


extern "C" {
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
    //static const double G = 1; //6.674e−11
    vector3D p, f;
    double pseudoMagnitude;
    
    p.x = bodyFrom.pos.x - bodyOn.pos.x;
    p.y = bodyFrom.pos.y - bodyOn.pos.y;
    p.z = bodyFrom.pos.z - bodyOn.pos.z;
    
    pseudoMagnitude = 1 * G * bodyOn.mass * bodyFrom.mass / pow(getMagnitude(p), 3);
    printf("\tpseudoMag: %16.14f\n", pseudoMagnitude);
    
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


  /* Given a vector, the opposite vector will be returned
  * 
  * @author Jason Vander Woude
  */
  vector3D negateVector3D(vector3D vec) {
    vector3D v;
    v.x = -vec.x;
    v.y = -vec.y;
    v.z = -vec.z;
    return v;
  }


  /* Given two vectors, the sum will be returned
  * 
  * @author Jason Vander Woude
  */
  vector3D vector3DSum(vector3D a, vector3D b) {
    vector3D sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    sum.z = a.z + b.z;
    return sum;
  }


  /* Given a vector and scalar, the scaled vector will be returned
  * 
  * @author Jason Vander Woude
  */
  vector3D vector3DScale(vector3D vec, double scale) {
    vector3D v;
    v.x = vec.x * scale;
    v.y = vec.y * scale;
    v.z = vec.z * scale;
    return v;
  }
}


int win_width = 900;
int win_height = 900;
bool mouse_left_pressed = false; // Boolean to monitor when mouse is pressed
pix_coord mouse_left_press = {0, 0};


/* Main method */
int main() {

	// Create window
	sf::RenderWindow window(sf::VideoMode(win_width, win_height), 
	                        "N-Body", 
	                        sf::Style::Close|sf::Style::Titlebar);
	
	sf::View view(sf::FloatRect((int)(-win_width/2), (int)(-win_height/2), win_width, win_height));
	
	window.setView(view);
	
	
	sf::CircleShape circ1(5); circ1.setFillColor(sf::Color(100, 250, 50));
	sf::CircleShape circ2(5); circ2.setFillColor(sf::Color(255, 0, 50));
	

	body b0 = {0,  0,0   ,0, 100, 0,   200000000000000000,1};
	body b1 = {160,0,0   ,0,-90,0,        100000000000000,1};
	
	//body b0 = {0,     0,0   ,0, 0, 0,       5972200000000000000000000,1}; //earth
	//body b1 = {384400000,0,0   ,0,10,0,     73477000000000000000000,1}; //moon
	
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
	  
	  circ1.setPosition(sf::Vector2f(currentBodies[0].pos.x/10, currentBodies[0].pos.y/10));
	  circ2.setPosition(sf::Vector2f(currentBodies[1].pos.x/10, currentBodies[1].pos.y/10));
	  //window.clear();
	  window.draw(circ1);
	  window.draw(circ2);
	  window.display();
	  
	  
	  usleep(100);
	  
	  sf::Event event;
	  while (window.pollEvent(event)) {
	    switch (event.type) {
	      // Window closed
	      case sf::Event::Closed:
		      exit(0);
		      window.close();
		      break;
	      default:
		break;
	    }
	  }
	  
	  // increment time
	  time += time_step;
	}
	
	
	
	
// 	while (window.isOpen()) {
// 		sf::Event event;
// 		while (window.pollEvent(event)) {
// 			switch (event.type) {
// 				
// 				// Window closed
// 				case sf::Event::Closed:
// 					window.close();
// 					break;
// 
// 				// Mouse Button Pressed
// 				case sf::Event::MouseButtonPressed:
// 					switch (event.mouseButton.button) {
// 						
// 						// Left Button Pressed
// 						case sf::Mouse::Left:
// 							mouse_left_pressed=true;
// 							cout << "start: " << mouse_left_press.x << endl;
// 							mouse_left_press.x = event.mouseButton.x;
// 							mouse_left_press.y = event.mouseButton.y;
// 							break;
// 							
// 						// Right Button Pressed
// 						case sf::Mouse::Right:
// 							// TODO
// 							break;
// 
// 						// Default event.mouseButton.button
// 						default:
// 							// Won't be processed
// 							break;
// 					} //END: event.mouseButton.button
// 					break;
// 
// 				// Mouse Button Released
// 				case sf::Event::MouseButtonReleased:
// 					switch (event.mouseButton.button) {
// 						
// 						// Left Button Pressed
// 						case sf::Mouse::Left:
// 							mouse_left_pressed=false;
// 							cout << "start: " << mouse_left_press.x << endl;
// 							cout << "end:   " << event.mouseButton.x << endl;
// 							cout << event.mouseButton.x - mouse_left_press.x << endl;
// 							circ1.setPosition(
// 								p1.x+event.mouseButton.x - mouse_left_press.x,
// 								p1.y);
// 							break;
// 							
// 						// Right Button Pressed
// 						case sf::Mouse::Right:
// 							// TODO
// 							break;
// 
// 						// Default event.mouseButton.button
// 						default:
// 							// Won't be processed
// 							break;
// 					} //END: event.mouseButton.button
// 					break;
// 
// 				// Mouse Moved
// 				case sf::Event::MouseMoved:
// 					
// 					break;
// 
// 				// Default event.type
// 				default:
// 					// Won't process other events
// 					break;
// 					
// 			} //END: switch (event.type)
// 		} //END: while (window.pollEvent(event))
// 
// 		
// 		
// 		// Do display operations
// 		window.clear();
// 		window.draw(circ1);
// 		window.draw(circ2);
// 		window.draw(circ3);
// 		window.draw(circ4);
// 		window.display();
// 		
// 	} //END: while (window.isOpen())
 
	return 0;
}
