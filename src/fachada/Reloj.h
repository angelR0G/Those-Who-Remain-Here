#pragma once

#include <SFML/Graphics.hpp>
#include "Tiempo.h"

using namespace std;

class Reloj{
    public: 
        Reloj();

        ~Reloj();

        Tiempo getElapsedTime();
        Tiempo restart();

    private:
        sf::Clock reloj;
};