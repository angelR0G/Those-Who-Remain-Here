#include "MuroOscuro.h"

const string ANIM_IDLE          = "MuroOscuroIdle";
const float DISTANCIA_OPACIDAD  = 100.0;
const int TIEMPO_UPDATE         = 100;

//Constructor
MuroOscuro::MuroOscuro(Textura* t, Sprite* s):ElementoEntorno(t, s, false) {
    apagado     = false;
    contador    = 0;
    colisiones.clear();

    animacion   = new Animacion(sprite);
    animacion->addAnimation(ANIM_IDLE, 20.0, 0, 0, 7, 3, 50, 100);
}

//Destructor 
MuroOscuro::~MuroOscuro() {
    delete animacion;

    for(unsigned int i=0; i<colisiones.size(); i++) {
        colisiones[i].clear();
    }
    colisiones.clear();
}

//Actualiza el muro, impidiendo pasar al jugador si no tiene linterna
void MuroOscuro::update(Tiempo t) {
    //Comprueba que no esta detenido
    if(!detenido) {
        animacion->play(ANIM_IDLE, t.milisegundos());

        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Reinicia el reloj y el tiempoAntes si fuera necesario
        relojDetenible.restart();
        if(tiempoAntes.milisegundos() > 0) {
            tiempoAntes.setTiempoCero();
        }

        Jugador* jugador            = EstadoPartida::Instancia()->getJugador();
        vector<float> posJugador    = jugador->getPosNueva();
        float distancia             = sqrt(pow(posJugador[0] - posicion[0], 2) + pow(posJugador[1] - posicion[1], 2));
        //Si el jugador no tiene linterna y ha pasado el tiempo suficiente, comprueba su posicion
        if(!apagado && contador >= TIEMPO_UPDATE) {
            //Comprueba si tiene la linterna
            if(jugador->getLinterna()) {
                apagado = true;
                EstadoPartida::Instancia()->getSalaActual()->eliminarColision(&colisiones);
            }

            contador -= TIEMPO_UPDATE;
        }

        //A partir de cierta distancia, reduce su opacidad si el jugador tiene una linterna
        if(apagado && distancia < DISTANCIA_OPACIDAD) {
            sprite->setColor({255, 255, 255, static_cast<int>((distancia/DISTANCIA_OPACIDAD)*255)});
        }
        else if(sprite->getColor()[3] < 255){
            //Si se esta lejos, recupera su opacidad original
            sprite->setColor({255, 255, 255, 255});
        }
    }

    //La primera vez que se ejecute, creara las colisiones
    if(!apagado && colisiones.size() == 0) {
        Sala* sala  = EstadoPartida::Instancia()->getSalaActual();
        int tamTile = sala->getConstTamTiles();

        //Calcula el tile de la base
        vector<int> pos = {(int)posicion[0]/tamTile, (int)posicion[1]/tamTile};

        //Anyade al vector de colisiones 4 tiles a partir de la base
        for(int i=0; i<4; i++) {
            colisiones.push_back({pos[0], pos[1]-i});
        }

        //Crea un mapa para las colisiones
        int idCol   = sala->getIdColision(1);
        map<vector<int>, int> mapaCol;

        for(unsigned int i=0; i<colisiones.size(); i++) {
            mapaCol[colisiones[i]] = idCol;
        }

        //Actualiza las colisiones de la sala
        sala->anyadirColision(&mapaCol);
    }
}