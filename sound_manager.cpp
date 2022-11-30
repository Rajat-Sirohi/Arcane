#include "sound_manager.h"

SoundManager::SoundManager()
{
    
}

SoundManager::~SoundManager()
{
    
}

void SoundManager::loadSound(std::string soundName)
{
    SoundBuffers[soundName].loadFromFile("audio/" + soundName + ".ogg");
}

void SoundManager::stopSound(std::string soundName)
{
    for (auto it = Sounds[soundName].begin(); it != Sounds[soundName].end(); ) {
        it->stop();
        Sounds[soundName].erase(it);
    }
}

void SoundManager::playSound(std::string soundName, bool loop)
{
    // Clear away stopped sounds
    for (auto pair : Sounds) {
        std::string name = pair.first;
        for (auto it = Sounds[name].begin(); it != Sounds[name].end(); ) {
            sf::Sound::Status status = it->getStatus();
            if (status == sf::Sound::Stopped) {
                Sounds[name].erase(it);
            } else { ++it; }
        }
    }
    
    // Play sound if not muted
    if (!muteOn) {
        sf::Sound newSound;
        newSound.setBuffer(SoundBuffers[soundName]);
        newSound.setLoop(loop);
	
        Sounds[soundName].push_back(newSound);
        Sounds[soundName].back().play();
    }
}

void SoundManager::playMusic(std::string filepath)
{
    Music.openFromFile(filepath);
    Music.play();
}

void SoundManager::toggleMute()
{
    if (muteOn) { Music.play(); }
    else        { Music.pause(); }
    muteOn = !muteOn;
}