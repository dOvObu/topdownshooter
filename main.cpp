#include "math_constants.h"
#include "tot.h"
#include "audiomanager.h"
#include "mouse.h"
#include "player.h"
#include "aabb.h"
#include "vec.h"
#include "evilbox.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <random>



int main () {

	bool start = false;
	AABB::bodies.reserve (40);
	EvilBox::boxes.reserve (4);

	walls[0] = getBox (400,0, 400,30);//up
	walls[1] = getBox (400,600, 400,30);//down
	walls[2] = getBox (0,300, 30,300);//left
	walls[3] = getBox (800,300, 30,300);//right

	AudioManager::init();
	std::thread audio (AudioManager::update);
	audio.detach ();

	getEvilBox (200, 300, m_pi*0.25f, m_pi);
	getEvilBox (600, 300, m_pi, 0.f * m_pi);
	getEvilBox (400, 100, 0.5f * m_pi, 1.5f * m_pi);
	getEvilBox (400, 500, 1.5f * m_pi, 0.5f * m_pi);
	
	sf::RenderWindow window (sf::VideoMode(800,600), "Hi");

	sf::RectangleShape r;
	r.setSize ({20, 20});

	Player::position = {400, 300};

	while (window.isOpen ()) {
		sf::Event event;
		tot::updateDTime ();
		while (window.pollEvent (event)) {
			if (event.type == sf::Event::Closed) {
				window.close ();
				AudioManager::closed = true;
			}
		}
		Mouse::update (window);
		r.setPosition (Mouse::pos);

		enableDamageForPlayer ();
		enableDamageForEvilBox ();
		Player::control ();

		if (start) for (auto& it : EvilBox::boxes) it->control ();
		else if (lenSqrt(Player::velocity) != 0.f || sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			start = true;
		}

		window.clear ();
		window.draw (r);
		updateMagnumBullets (window);
		Player::draw (window);
		for (auto& it : EvilBox::boxes) it->draw (window);
		for (int i = 0; i < 4; ++i) {
			walls[i]->cheatDraw (window, Player::body);
		}
		window.display();
	}

	std::cout << MagnumBullet::bullets.size () << std::endl;

	return 0;
}
