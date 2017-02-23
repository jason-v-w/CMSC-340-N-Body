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

typedef struct pix_coord {
  int x,y;
} pix_coord;

typedef struct point {
  double x,y,z;
} point;



int win_width = 900;
int win_height = 500;
bool mouse_left_pressed = false; // Boolean to monitor when mouse is pressed
pix_coord mouse_left_press = {0, 0};


/* Main method */
int main() {

	// Create 4 points
	point p1 = {0,0,0};
	point p2 = {100,0,0};
	point p3 = {0,100,0};
	point p4 = {0,0,100};

	// Create 4 circles
	sf::CircleShape circ1(20);
	sf::CircleShape circ2(20);
	sf::CircleShape circ3(20);
	sf::CircleShape circ4(20);
	
	circ1.setFillColor(sf::Color(100, 250, 50));
	circ2.setFillColor(sf::Color(100, 250, 50));
	circ3.setFillColor(sf::Color(100, 250, 50));
	circ4.setFillColor(sf::Color(100, 250, 50));

	// Position the circles
	circ1.setPosition(p1.x, p1.y);
	circ2.setPosition(p2.x, p2.y);
	circ3.setPosition(p3.x, p3.y);
	circ4.setPosition(p4.x, p4.y);

	// Create window
	sf::RenderWindow window(sf::VideoMode(win_width, win_height), 
	                        "N-Body", 
	                        sf::Style::Close|sf::Style::Titlebar);
	
	sf::View view(sf::FloatRect(0, 0, win_width, win_height));
	
	window.setView(view);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				
				// Window closed
				case sf::Event::Closed:
					window.close();
					break;

				// Mouse Button Pressed
				case sf::Event::MouseButtonPressed:
					switch (event.mouseButton.button) {
						
						// Left Button Pressed
						case sf::Mouse::Left:
							mouse_left_pressed=true;
							cout << "start: " << mouse_left_press.x << endl;
							mouse_left_press.x = event.mouseButton.x;
							mouse_left_press.y = event.mouseButton.y;
							break;
							
						// Right Button Pressed
						case sf::Mouse::Right:
							// TODO
							break;

						// Default event.mouseButton.button
						default:
							// Won't be processed
							break;
					} //END: event.mouseButton.button
					break;

				// Mouse Button Released
				case sf::Event::MouseButtonReleased:
					switch (event.mouseButton.button) {
						
						// Left Button Pressed
						case sf::Mouse::Left:
							mouse_left_pressed=false;
							cout << "start: " << mouse_left_press.x << endl;
							cout << "end:   " << event.mouseButton.x << endl;
							cout << event.mouseButton.x - mouse_left_press.x << endl;
							circ1.setPosition(
								p1.x+event.mouseButton.x - mouse_left_press.x,
								p1.y);
							break;
							
						// Right Button Pressed
						case sf::Mouse::Right:
							// TODO
							break;

						// Default event.mouseButton.button
						default:
							// Won't be processed
							break;
					} //END: event.mouseButton.button
					break;

				// Mouse Moved
				case sf::Event::MouseMoved:
					
					break;

				// Default event.type
				default:
					// Won't process other events
					break;
					
			} //END: switch (event.type)
		} //END: while (window.pollEvent(event))

		// Do display operations
		window.clear();
		window.draw(circ1);
		window.draw(circ2);
		window.draw(circ3);
		window.draw(circ4);
		window.display();
		
	} //END: while (window.isOpen())
 
	return 0;
}
