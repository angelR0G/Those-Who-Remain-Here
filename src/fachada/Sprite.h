#pragma once

#include <SFML/Graphics.hpp>
#include "Textura.h"
#include "VentanaRender.h"

using namespace std;

class Sprite {
    public:
        Sprite(Textura*, int, int, int, int);
        Sprite(Sprite&);

        ~Sprite();

        void draw();
        void setScale(float, float);
        void setRotation(float);
        void setOrigin(float, float);
        void setPosition(float, float);
        vector<float> getScale();
        float getRotation();
        vector<float> getOrigin();
        vector<float> getPosition();
        void setTextura(Textura*);
        void setRect(int, int, int, int);
        vector<int> getRect();
        void mover(int, int);
        bool intersect(Sprite*);
        vector<int> getGlobalPixelSize();
        sf::Sprite* getSprite();
        bool contains(float, float);
        void setColor(vector<int>);
        vector<int> getColor();

    private:
        //class Textura* textura;             //Puntero a la textura del Sprite
        sf::Sprite* sprite;                  //Sprite de SFML
};