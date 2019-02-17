#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// singletone
class AudioManager {
public:
	static bool closed, changed, shoot;
	static std::string playList[9];
	static std::shared_ptr<sf::SoundBuffer> magnumBuff[4];
	static std::shared_ptr<sf::SoundBuffer> frontBuffer;
	static std::shared_ptr<sf::SoundBuffer> backBuffer;
	static std::shared_ptr<sf::Sound> currentSound;
	static std::shared_ptr<sf::Sound> shootSound;
	static int init();
	static int load(unsigned no);
	static void stop();
	static void play();
	static void update();
};


#endif