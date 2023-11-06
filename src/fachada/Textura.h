#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

class Textura {
    public:
        Textura(string);

        ~Textura();

        sf::Texture* getTextura();
        vector<int> getSize();

    private:
        sf::Texture* textura;               //Textura de SFML
};