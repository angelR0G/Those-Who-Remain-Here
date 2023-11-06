#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Fuente{
    public:
        Fuente(string);
        ~Fuente();
        sf::Font* getFuente();
    
    private:
        sf::Font* fuente;
};
