#include "player.h"


sf::Vector2f Player::position;
sf::Vector2f Player::velocity;
sf::Vector2f Player::imp (0.f, 0.f);
AABB* Player::body = getBox (0,0, 30,30);
float Player::angle;
float Player::mass = 80.f;
float Player::HP = 100.f;

inline void drawHPbar (float HP, sf::RenderWindow& window, sf::Vertex v[]) {
	v[0].position= {5.f, 552.f}, v[3].position= {790.f * HP/100.f + 5.f, 552.f};
	v[1].position= {5.f,547.f}, v[2].position= {790.f * HP/100.f + 5.f, 547.f};
	v[0].color=v[1].color=v[2].color=v[3].color;
	window.draw(v, 4, sf::Quads);
}

void Player::draw (sf::RenderWindow& window) {

	auto dif = position - Mouse::pos;
	Player::angle = atan2f(dif.y, dif.x);// угол поворота персонажа, в радианах

	sf::Vertex v[] = {
		getVec (30.f, angle + M_PI) + position,
		getVec (10.f, angle + 3.f * M_PI / 2.f) + position,
		getVec (10.f, angle) + position,
		getVec (10.f, angle + M_PI / 2.f) + position
	};
	v[0].color = sf::Color(255,0,0,255);
	v[1].color = sf::Color(155,0,0,255);
	v[3].color = sf::Color(200,0,0,255);
	window.draw(v, 4, sf::Quads);
	drawHPbar (HP, window, v);
}



void setWillOfPlayer (float a, sf::Vector2f& velocity) {
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) {velocity += {0.f,-a };}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) {velocity += {0.f, a };}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::A)) {velocity += {-a ,0.f};}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::D)) {velocity += { a ,0.f};}
}



void Player::control () {
	static bool tr = false;
	if (HP > 0.f){
		const float acceleration = 8.f / mass * tot::getDTime (); // ускорение игрока
		velocity *= 0.99f; // коэффициент трения-качения
		imp *= 0.95f;
		setWillOfPlayer (acceleration, velocity);

		body->pos[0] = position.x, body->pos[1] = position.y;

		velocity += imp;
		setCollisionEffects (velocity, body);
		position += velocity;

		if (sf::Mouse::isButtonPressed (sf::Mouse::Left)) {
			if (!tr) {
				AudioManager::shoot = tr = true;
				shootByMagnumBullet (position.x, position.y, angle);
			}
		} else { tr = false; }
	}
}



void Player::printlog () {
	std::cout << "Player pos["<< (int)position.x << ',' << (int)position.y << "]"
						<<" vel["<< (int)velocity.x << ',' << (int)velocity.y << "]\n";
}
