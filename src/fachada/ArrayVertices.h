#pragma once

#include <SFML/Graphics.hpp>
#include "VentanaRender.h"

using namespace std;

class ArrayVertices {
    public:
        ArrayVertices(int, int);

        ~ArrayVertices();

        void setVertexColor(int, int, int, int, int);
        void setArrayColor(int, int, int, int);
        void setVertexPosition(int, int, int);
        void setVertexTexCoord(int, float, float);
        vector<int> getVertexPosition(int);
        void draw();
        sf::VertexArray getArrayVertices();
        vector<int> getBounds();

    private:
        sf::VertexArray array;              //Array de vertices de SFML
};