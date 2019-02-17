#include "audiomanager.h"


std::string AudioManager::playList[9] = {
		"LandoftheDamnedP6eace.wav",
		"Fight5.wav",
		"PersepolisMedium.wav",
		"TeotihuacanThemeAction.wav",
		"TeotihuacanThemePeace.wav",
		"TheElephantAtriumAttack.wav",
		"TheGreatPyramid.wav",
		"TowerOfBabylonMedium.wav",
		"theDamnedAttack.wav"
	};
template<typename T>
using sptr = std::shared_ptr<T>;

typedef sf::SoundBuffer sndBuff_t;


bool AudioManager::closed = false;
bool AudioManager::changed = false;
bool AudioManager::shoot = false;


std::shared_ptr<sf::SoundBuffer> AudioManager::magnumBuff[4] = {
	sptr<sndBuff_t>(new sndBuff_t()),
	sptr<sndBuff_t>(new sndBuff_t()),
	sptr<sndBuff_t>(new sndBuff_t()),
	sptr<sndBuff_t>(new sndBuff_t())
};

sptr<sndBuff_t> AudioManager::frontBuffer (new sndBuff_t());
sptr<sndBuff_t> AudioManager::backBuffer (new sndBuff_t());
sptr<sf::Sound> AudioManager::currentSound (new sf::Sound());
sptr<sf::Sound> AudioManager::shootSound (new sf::Sound());




void AudioManager::update() {
	int i = 1, magnumNo = 0;
	load (i);
	changed = false;
	while (i < 9 - 1 && !closed) {
		if (shoot) {
			shootSound->setBuffer(*(magnumBuff[magnumNo]));
			shootSound->play();
			++magnumNo;
			if (magnumNo == 4) magnumNo = 0;
			shoot = false;
		}
		if (currentSound->getStatus() == sf::Sound::Status::Stopped || changed) {
			play ();
			load (i + 1); // preload next music
			changed = false;
			++i;
		}
	}
}


void AudioManager::stop() {
}


int AudioManager::init() {
	int ret = 0;
	for (int i = 0; i < 4; ++i) {
		if (!magnumBuff[i]->loadFromFile ("sound/magnum"+std::to_string(i)+".wav")) {
			ret = -1;
		}
	}

	std::cout << "test " << ret << std::endl;
	return ret;
}




void AudioManager::play() {
	currentSound->setBuffer (*backBuffer);
	currentSound->play ();
	std::swap (frontBuffer, backBuffer);
}



int AudioManager::load (unsigned no) {
	if (backBuffer->loadFromFile("sound/" + playList[no])) {
		changed = true;
		return 0;
	}
	return -1;
}