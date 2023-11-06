#pragma once

#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;

class Buffer {
    public:
        Buffer(string);

        ~Buffer();

        sf::SoundBuffer* getBuffer();

    private:
        sf::SoundBuffer* buffer;               
};