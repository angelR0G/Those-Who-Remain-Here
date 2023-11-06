#include "MuroDestructible.h"

const string ANIM_DESTRUIR      = "DestruirMuroDestructible";

//Constructor
MuroDestructible::MuroDestructible(Textura* t, Sprite* s):ElementoEntorno(t,s,false) {
    destruido           = false;
    animacionTerminada  = false;
    tileOcupado         = {};

    animacion   = new Animacion(sprite);
    animacion->addAnimation(ANIM_DESTRUIR, 30.0, 0, 0, 2, 2, 32, 32);
}

//Destructor
MuroDestructible::~MuroDestructible() {
    delete animacion;
}

//Actualiza el muro, reproduciendo la animacion al romperse
void MuroDestructible::update(Tiempo t) {
    //Cuando ha terminado la animacion de destruirse, ya no se actualiza
    if(!animacionTerminada) {
        //Comprueba que el tiempo no esta detenido
        if(!detenido && destruido) {
            animacion->playOnce(ANIM_DESTRUIR, t.milisegundos());
            if(animacion->getFinished()) {
                //Cuando acaba la animacion, elimina la colision
                vector<vector<int>> aux = {tileOcupado};
                EstadoPartida::Instancia()->getSalaActual()->eliminarColision(&aux);
                animacionTerminada      = true;
            }
        }

        //La primera vez que se ejecuta, guarda el tile en el que se encuentra y crea una colision en el
        if(!destruido && tileOcupado.size() != 2) {
            //Recupera la sala y los datos necesarios
            Sala* sala      = EstadoPartida::Instancia()->getSalaActual();
            int tamTiles    = sala->getConstTamTiles();
            int idCol       = sala->getIdColision(1);
            tileOcupado     = {(int)posicion[0]/tamTiles, (int)posicion[1]/tamTiles};

            //Crea un mapa para anyadir la colision a la sala
            map<vector<int>, int> mapaCol = {{tileOcupado, idCol}};

            sala->anyadirColision(&mapaCol);
        }
    }
}

//Rompe el muro para que no colisione
void MuroDestructible::romper() {
    destruido = true;
}

//Sobrecarga el metodo dibujar para no hacerlo cuando esta roto
void MuroDestructible::dibujar() {
    //Comprueba que no esta roto ni esta reproduciendo su animacion
    if(!animacionTerminada) {
        sprite->draw();
    }
}

//Devuelve si el muro esta roto
bool MuroDestructible::getDestruido() {
    return destruido;
}

//Devuelve el sprite del muro
Sprite* MuroDestructible::getSprite() {
    return sprite;
}