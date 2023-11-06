#include "EspiritusOscuros.h"

const string ANIM_IDLE          = "EspiritusOscurosIdle";
const float DISTANCIA_EFECTO    = 80.0;
const float DISTANCIA_OPACIDAD  = 100.0;
const int TIEMPO_UPDATE         = 100;
const int TIEMPO_RALENTIZACION  = 1000;

//Constructor
EspiritusOscuros::EspiritusOscuros(Textura* t, Sprite* s):ElementoEntorno(t,s,false) {
    apagado     = false;
    contador    = 0;

    animacion   = new Animacion(sprite);
    animacion->addAnimation(ANIM_IDLE, 40.0, 0, 0, 4, 5, 100, 80);
}

//Destructor
EspiritusOscuros::~EspiritusOscuros() {
    delete animacion;
}

//Actualiza los espiritus
void EspiritusOscuros::update(Tiempo t) {
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
            }
            else if(distancia <= DISTANCIA_EFECTO) {
                //Si no tiene linterna, le aplica ralentizacion
                jugador->setRalentizacion(TIEMPO_RALENTIZACION);
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
}