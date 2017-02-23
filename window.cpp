#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>

double time_step = 10; //all units are given in base SI
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
  double getMagnitude(vector3D vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
  }
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
  vector3D getAcceleration(body body, vector3D force) {
    //a = F/m
    vector3D a;
    
    a.x = force.x / body.mass;
    a.y = force.y / body.mass;
    a.z = force.z / body.mass;
    
    return a;
  }
  vector3D negateVector3D(vector3D vec) {
    vector3D v;
    v.x = -vec.x;
    v.y = -vec.y;
    v.z = -vec.z;
    return v;
  }
  vector3D vector3DSum(vector3D a, vector3D b) {
    vector3D sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    sum.z = a.z + b.z;
    return sum;
  }
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
	  
	  
	  // Create 4 circles
	  sf::CircleShape circ1(5);
	  sf::CircleShape circ2(20);
	  
	  circ1.setFillColor(sf::Color(100, 250, 50));
	  circ2.setFillColor(sf::Color(100, 250, 50));
	  
	  circ1.setPosition(newBody0.pos.x, newBody0.pos.y);
	  circ1.setPosition(newBody1.pos.x, newBody1.pos.y);
	  
	  window.clear();
	  window.draw(circ1);
	  window.draw(circ2);
	  window.display();
	  
	  
	  // copy contents of computingBodies into currentBodies
	  memcpy((void*)currentBodies, (void*)computingBodies, sizeof(body)*num_bodies);
	  
	  // increment time
	  time += time_step;
	  usleep(10000);
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
