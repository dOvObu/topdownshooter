#include "evilbox.h"
#include "player.h"
#include "aabb.h"

std::vector<std::shared_ptr<EvilBox>> EvilBox::boxes;


EvilBox* getEvilBox (float x, float y, float phi, float angle) {
	EvilBox::boxes.push_back(std::shared_ptr<EvilBox>(new EvilBox (x, y, phi, angle)));
}



EvilBox::EvilBox (float x, float y, float phi_, float a = 0.f) : phi(phi_), angle(a) {
	body = getBox (x, y, 40, 40);
	attackBox = getBox (x, y, 10, 10);
	position = {x, y};
}

EvilBox::~EvilBox () {
	removeBox (body);
	removeBox (attackBox);
}


void EvilBox::draw (sf::RenderWindow& window) {

	auto dif = position - Player::position;
	float angle2 = atan2f(dif.y, dif.x); // под таким углом цель
	if (fabs(angle2 - angle) > 2.1f){
		if (angle < 0.f && angle2 > 0.f) { angle += 2.f*M_PI; }
		if (angle > 0.f && angle2 < 0.f) { angle -= 2.f*M_PI; }
	}
	omega = (angle2 - angle) * 0.005f;
	angle += omega; // угол поворота коробки, в радианах
	sf::Vertex v[] = {
		getVec (40.f, angle + 0) + position,
		getVec (40.f, angle + 1.f/2.f * M_PI) + position,
		getVec (45.f, angle + M_PI) + position,
		getVec (40.f, angle + 3.f/2.f * M_PI) + position
	};
	v[0].color = sf::Color(0,155,0,255);
	v[1].color = sf::Color(0,100,0,255);
	v[2].color = sf::Color(255,0,0,255);
	v[3].color = sf::Color(0,100,0,255);
	window.draw(v, 4, sf::Quads);
}

void EvilBox::control () {

	static float dt = 0.f;
	dt = tot::getDTime ();
	//setWillOfAI (acceleration, velocity);
	if (phi < M_PI) phi += dt * 0.02f;
	else phi += dt * 0.08f;
	if (phi >= M_PI*2.f) phi=0.f;
	float A = - 3.5f * dt;
	velocity = getVec(A * std::sin(phi) + 0.8f * A, angle);
	//printf ("A ~ %f\n", 10.f * std::sin( * 0.1f));
	//velocity *= 0.99f; // коэффициент трения-качения

	setCollisionEffects (velocity, body);
	position += velocity;
	body->pos[0] = position.x, body->pos[1] = position.y;
	attackBox->pos[0] = position.x, attackBox->pos[1] = position.y;

	/*if (sf::Mouse::isButtonPressed (sf::Mouse::Left)) {
		if (!tr) { AudioManager::shoot = tr = true; }
	} else { tr = false; }*/
}


void enableDamageForPlayer () {
	for (auto& it : EvilBox::boxes) {
		if (it->attackBox->collided (*Player::body)) {
			sf::Vector2f dif = Player::position - it->position;
			float angle = std::atan2 (dif.y, dif.x);
			Player::imp += getVec (0.01f, angle);
			if (Player::HP > 0.f) Player::HP -= 0.2f;
		}
	}
}

void enableDamageForEvilBox () {
	for (auto& it : MagnumBullet::bullets) {
		// ломается, при сталкновении со стеной
		for (int i = 0; i < 4; ++i) {
			if (walls[i]->collided (*it->attackBlock)) {
				it->removeIt = true;
				break;
			}
		}
		// и при столкновении со блоком
		if (!it->removeIt) {
			for (auto jt : EvilBox::boxes) {
				if (jt->HP != 0) {
					if (it->attackBlock->collided (*(jt->body))) {
						it->removeIt = true;
						--(jt->HP);
						break;
					}
				}
			}
		}
	}
	// убрать противников с нулевым HP
	auto it = EvilBox::boxes.begin ();
	while (it != EvilBox::boxes.end ()) {
		if ((*it)->HP <= 0) {
			it = EvilBox::boxes.erase (it);
		} else {
			it++;
		}
	}
}
