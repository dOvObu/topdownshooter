#include "magnumbullet.h"


std::list<std::shared_ptr<MagnumBullet> > MagnumBullet::bullets;

MagnumBullet::MagnumBullet (float x, float y, float angle_)
	:position (x, y),
	 //direction (getVec (0.1f, angle)),
	 angle (angle_),
	 attackBlock (getBox (x,y, 5.f,5.f))
{}

MagnumBullet::~MagnumBullet () {
	removeBox (attackBlock);
}

void MagnumBullet::draw (sf::RenderWindow& window) {
	position += getVec(-15.f * tot::getDTime (), angle);//direction;
	attackBlock->pos[0] = position.x, attackBlock->pos[1] = position.y;

	sf::Vertex v[] = {
		getVec (2.f, angle + M_PI) + position,
		getVec (2.f, angle + 3.f * M_PI / 2.f) + position,
		getVec (12.f, angle) + position,
		getVec (2.f, angle + M_PI / 2.f) + position
	};
	v[2].color = sf::Color(255, 255, 0, 255);
	window.draw(v, 4, sf::Quads);
}

MagnumBullet* shootByMagnumBullet (float x, float y, float angle) {
	MagnumBullet* ret;
	MagnumBullet::bullets.push_back (
				std::shared_ptr<MagnumBullet> (ret = new MagnumBullet (x, y, angle)));
	return ret;
}

void updateMagnumBullets (sf::RenderWindow& window) {
	auto it = MagnumBullet::bullets.begin ();
	while (it != MagnumBullet::bullets.end ()) {
		if ((*it)->removeIt) {
			it = MagnumBullet::bullets.erase (it);
		} else {
			(*it)->draw (window);
			it++;
		}
	}
}
