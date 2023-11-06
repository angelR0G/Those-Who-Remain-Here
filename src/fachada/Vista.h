#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

class Vista {
    public:
        Vista(int, int, int, int);

        ~Vista();

        void setAreaVista(int, int, int, int);
        void setCentro(float x, float y);
        sf::View getVista();
        vector<float> getCentro();
        void setZoom(float);
    private:
        sf::View vista;
        int ancho;
        int alto;
};