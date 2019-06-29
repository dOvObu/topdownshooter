#include "aabb.h"

std::vector<std::shared_ptr<AABB> > AABB::bodies;
AABB* walls[4];

AABB* getBox (float x, float y, float hw, float hh) {
	AABB* box;
	AABB::bodies.push_back (std::shared_ptr<AABB>(box = new AABB (x, y, hw, hh)));
	return box;
}

void removeBox (AABB* aabb) {
	for (auto it = AABB::bodies.begin (); it != AABB::bodies.end(); it++) {
		if (it->get () == aabb) {
			AABB::bodies.erase (it);
			return;
		}
	}
}

void setCollisionEffects (sf::Vector2f& velocity, AABB* body) {
	for (int i = 0; i < 4; ++i) { // столкновение со стенами
		if (body->collided (*(walls[i]))) {
			collisionType type = walls[i]->getTypeOfCollision (*(body), true);
			switch (type) {
			case collisionType::fromUp:
				if (velocity.y > 0.f) velocity.y = 0.f;
				break;
			case collisionType::fromDown:
				if (velocity.y < 0.f) velocity.y = 0.f;
				break;
			case collisionType::fromLeft:
				if (velocity.x > 0.f) velocity.x = 0.f;
				break;
			case collisionType::fromRight:
				if (velocity.x < 0.f) velocity.x = 0.f;
				break;
			default:
				break;
			}
		}
	}
}
