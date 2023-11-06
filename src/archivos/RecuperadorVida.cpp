#include "RecuperadorVida.h"

//Constructor
RecuperadorVida::RecuperadorVida(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true) {
}

RecuperadorVida::~RecuperadorVida(){}

//
void RecuperadorVida::update(Tiempo elapsedTime){
    EstadoPartida *estado = EstadoPartida::Instancia();
    Jugador *j = estado->getJugador();

    j->recuperaVida(4);
    new ParticulaSimpleCuadrada(j->getPosNueva(), {255, 249, 161, 255}, 120, 300);
    new ParticulaSimpleCuadrada(j->getPosNueva(), {255, 249, 161, 255}, 120, 300);
    new ParticulaSimpleCuadrada(j->getPosNueva(), {255, 249, 161, 255}, 120, 300);
}

void RecuperadorVida::dibujar(){
    if(sprite!=nullptr){
        sprite->draw();
    }
}

void RecuperadorVida::interactuar(){

}