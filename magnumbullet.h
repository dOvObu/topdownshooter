#ifndef MAGNUMBULLET_H
#define MAGNUMBULLET_H
#include "aabb.h"
#include "vec.h"
#include "tot.h"
#include "math_constants.h"
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>

struct MagnumBullet {

	static std::list<std::shared_ptr<MagnumBullet> > bullets;

	bool removeIt = false;
	sf::Vector2f position;//, direction;
	float angle;
	MagnumBullet (float x, float y, float angle);
	~MagnumBullet ();
	void draw (sf::RenderWindow& window);
	AABB* attackBlock;
};

MagnumBullet* shootByMagnumBullet (float x, float y, float angle);

void updateMagnumBullets (sf::RenderWindow& window);

#endif // MAGNUMBULLET_H
