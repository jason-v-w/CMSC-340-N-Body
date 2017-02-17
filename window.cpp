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

	sf::RenderWindow window(sf::VideoMode(width, height), 
	                        "N-Body", 
	                        sf::Style::Close|sf::Style::Titlebar);

	// Create sprite to hold image data
	sf::Sprite image;

	sf::RectangleShape selector(sf::Vector2f(select_width, select_height));
	selector.setOutlineThickness(1);
	selector.setOutlineColor(sf::Color(128, 0, 128));
	selector.setFillColor(sf::Color(0, 0, 0, 0));

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
							// TODO
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
							// TODO
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

				// Default event.type
				default:
					// Won't process other events
					break;
					
			} //END: switch (event.type)
		} //END: while (window.pollEvent(event))

		// Do display operations
		window.clear();
		window.display();
		
	} //END: while (window.isOpen())
 
	return 0;
}
