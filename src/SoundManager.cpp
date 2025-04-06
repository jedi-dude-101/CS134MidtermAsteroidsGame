#pragma once
#include "ofSoundPlayer.h"
#include <map>

class SoundManager {
public:
	void load() {
		sounds["Shoot"].load("sounds/LaserGunSFX.mp3");
		sounds["Explosion"].load("sounds/ExplosionSFX.mp3");
		sounds["Thrust"].load("sounds/RocketThrustSFX.mp3");
	}
	void play(std::string name) {
		if (sounds.count(name)) {
			sounds[name].play();
		}
	}
private:
	std::map<std::string, ofSoundPlayer> sounds;
};