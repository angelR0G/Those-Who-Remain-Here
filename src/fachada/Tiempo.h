#pragma once

#include <SFML/Graphics.hpp>
#include "Tiempo.h"

using namespace std;

class Tiempo{
    public:
        Tiempo();

        ~Tiempo();

        float segundos();
        int milisegundos();
        void setTiempoCero();
        void setTiempo(sf::Time);

    private:
        sf::Time tiempo;                //Tiempo de SFML
};