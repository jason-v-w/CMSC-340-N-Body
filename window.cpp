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


/* Main method */
int main() {
  // set up window
  sf::RenderWindow window(sf::VideoMode(win_width, win_height), "N-Body", sf::Style::Close|sf::Style::Titlebar);

  while (1) {
    
    
    
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
	window.close();
      }
    }
    
    // Do display operations
    window.clear();
    window.display();
  }
    
  return 0;
}
