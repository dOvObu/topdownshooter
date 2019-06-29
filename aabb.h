#ifndef AABB_H
#define AABB_H
#include "vec.h"
#include "math_constants.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

enum class collisionType {fromUp, fromRight, fromDown, fromLeft};


struct AABB {
	float size[2], pos[2];
	float halpha;
	void setPos (float x, float y) {
		pos[0] = x, pos[1] = y;
	}
	bool collided (const AABB& aabb) {
		return (aabb.pos[0] - aabb.size[0] < pos[0] + size[0])
				&& (pos[0] - size[0] < aabb.pos[0] + aabb.size[0])
				&& (aabb.pos[1] - aabb.size[1] < pos[1] + size[1])
				&& (pos[1] - size[1] < aabb.pos[1] + aabb.size[1]);
	}
	collisionType getTypeOfCollision (const AABB& aabb, bool pr=false) {
		float phi = std::atan2 (aabb.pos[1]-pos[1],aabb.pos[0]-pos[0]);
		//phi += 0.25f * M_PI;
		if (halpha >= fabs(phi)) {
			return collisionType::fromRight;
		}
		if (m_pi - halpha <= fabs (phi)) {
			return collisionType::fromLeft;
		}
		if (phi < 0.f) {
			return collisionType::fromUp;
		}
		return collisionType::fromDown;
	}

	void cheatDraw (sf::RenderWindow& window, AABB* aabb) {
		sf::Vector2f p = {pos[0], pos[1]};
		sf::Vertex v[5] = {
			getVec(100, f0 (halpha)) + p, getVec(100, f1 (halpha)) + p,
			getVec(100, f2 (halpha)) + p, getVec(100, f3 (halpha)) + p,
			sf::Vector2f(0, 0)
		};
		v[4] = v[0];
		window.draw(v, 5, sf::LineStrip);
		v[0].position = p;
		v[1].position = getVec (80, std::atan2(aabb->pos[1]-p.y, aabb->pos[0]-p.x))+p;
		window.draw(v, 2, sf::LineStrip);
	}

	AABB (float x, float y, float hw, float hh) {

		pos[0] = x, pos[1] = y;
		size[0] = hw, size[1] = hh;
		halpha = std::atan2 (2.f*hh, 2.f*hw);

	}
	~AABB () {}
	static std::vector<std::shared_ptr<AABB> > bodies;
private:
	float f0 (float halpha) {return -halpha;};
	float f1 (float halpha) {return halpha;};
	float f2 (float halpha) {return m_pi - halpha;};
	float f3 (float halpha) {return m_pi + halpha;};
};

extern AABB* walls[4];

AABB* getBox (float x, float y, float hw, float hh);
void removeBox (AABB* aabb);


void setCollisionEffects (sf::Vector2f& velocity, AABB* body);

#endif // AABB_H
