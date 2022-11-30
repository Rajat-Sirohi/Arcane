#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <vector>

class SoundManager
{
public:
    bool muteOn = false;
    SoundManager();
    ~SoundManager();
    void loadSound(std::string soundName);
    void stopSound(std::string soundName);
    void playSound(std::string soundName, bool loop);
    void playMusic(std::string filepath);
    void toggleMute();
    
private:
    std::map<std::string, sf::SoundBuffer> SoundBuffers;
    std::map<std::string, std::vector<sf::Sound>> Sounds;
    sf::Music Music;
};

#endif
