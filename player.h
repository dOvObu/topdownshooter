#ifndef PLAYER_H
#define PLAYER_H
#include "mouse.h"
#include "audiomanager.h"
#include "tot.h"
#include "vec.h"
#include "aabb.h"
#include "magnumbullet.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>


// singleton
class Player {
public:
	static void draw (sf::RenderWindow& window);
	static void control ();
	static void printlog();
	static sf::Vector2f position;
	static sf::Vector2f velocity;
	static sf::Vector2f imp;
	static float angle;
	static float mass;
	static float HP;
	static AABB* body;
};


#endif
