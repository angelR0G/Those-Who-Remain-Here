#include "ElementoEntorno.h"

const float DIST_INTER_DEFECTO      = 40.0;

//Constructor. Recibe la textura, el sprite y un bool que indica si se puede interactuar con el
ElementoEntorno::ElementoEntorno(Textura* tex, Sprite* s, bool inter) {
    posicion        = {0.0, 0.0};
    textura         = tex;
    sprite          = s;
    interactuable   = inter;
    distInteraccion = DIST_INTER_DEFECTO;
}

//Destructor
ElementoEntorno::~ElementoEntorno() {
    delete textura;
    delete sprite;
}

//Update por defecto. No hace nada.
void ElementoEntorno::update(Tiempo t) {}

//Dibuja el sprite
void ElementoEntorno::dibujar() {
    sprite->draw();
}

//Metodo que se ejecuta al interactuar con el elemento. Por defecto vacio
void ElementoEntorno::interactuar() {}

//Devuelve la posicion
vector<float> ElementoEntorno::getPosicion() {
    return posicion;
}

//Devuelve si se puede interactuar con el objeto
bool ElementoEntorno::getInteractuable() {
    return interactuable;
}

//Actualiza la posicion del elemento y el sprite
void ElementoEntorno::setPosicion(vector<float> pos) {
    if(pos.size() == 2) {
        posicion = pos;
        sprite->setPosition(pos[0], pos[1]);
    }
}

//Recupera la distancia a la que se puede interactuar con el elemento
float ElementoEntorno::getDistInteraccion() {
    return distInteraccion;
}