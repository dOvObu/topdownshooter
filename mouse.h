#ifndef MOUSE_H
#define MOUSE_H
#include <SFML/Graphics.hpp>

// singletone
class Mouse {
public:
	static sf::Vector2f pos;
	static void update (sf::RenderWindow& window);
};


#endif
