#pragma once

#include <SFML/Graphics.hpp>
#include "Fuente.h"
#include "VentanaRender.h"

using namespace std;

class Texto{
    public:
        Texto(string);
        ~Texto();

        void setString(string);
        void setFuente(Fuente*);
        void setTamanyo(int);
        void setColorTexto(vector<int>);
        void setColorContorno(vector<int>);
        void setGrosorContorno(float);
        void setPosicion(float, float);
        void setEscala(float, float);
        void draw();
        sf::Text* getTexto();
        float getAncho();
        float getAlto();
        bool getContains(float, float);
    private:
        class Fuente* fuente;
        sf::Text* texto;
};
