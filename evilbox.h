#ifndef EVILBOX_H
#define EVILBOX_H

#include "aabb.h"
#include "player.h"
#include <memory>
#include <SFML/Graphics.hpp>



struct EvilBox {
	EvilBox (float x, float y, float phi, float angle);
	~EvilBox ();
	void control ();
	void draw (sf::RenderWindow& window);
	sf::Vector2f position;
	sf::Vector2f velocity;
	int HP = 5;
	float angle;
	float omega;
	float mass;
	float phi;
	AABB* body;
	AABB* attackBox;
	static std::vector<std::shared_ptr<EvilBox> > boxes;
};

EvilBox* getEvilBox (float x, float y, float phi, float angle);

void enableDamageForPlayer ();

void enableDamageForEvilBox ();

#endif // EVILBOX_H
