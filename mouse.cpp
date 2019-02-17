#include "mouse.h"

sf::Vector2f Mouse::pos;

void Mouse::update (sf::RenderWindow& window) {
	//const sf::View& view = window.getView ();
	pos = sf::Vector2f (sf::Mouse::getPosition (window));// + view.getCenter ();
}