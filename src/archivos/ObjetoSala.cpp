#include "ObjetoSala.h"

const float ACELERACION     = 16.0;
const float VEL_INICIAL     = -160.0;
const float VEL_MAXIMA      = 180.0;

//Constructor. Recibe la posicion del objeto y dos punteros a sprite y textura.
ObjetoSala::ObjetoSala(vector<float> p, Sprite* s, Textura* t) {
    posicion            = p;
    posicionAnterior    = p;
    sprite              = s;
    textura             = t;
    colocado            = false;
    recogido            = false;
    velocidadActual     = VEL_INICIAL;

    //Se asegura de colocar el origen del sprite en el centro
    vector<int> aux = s->getRect();
    sprite->setOrigin(aux[2]/2.0, aux[3]/2.0);
    sprite->setPosition(posicion[0], posicion[1]);
    marco = Marco::Instancia()->getMarco();
}

//Destructor
ObjetoSala::~ObjetoSala() {
    delete sprite;
    delete textura;
    posicion.clear();
    posicionAnterior.clear();
}

//Actualiza la posicion del objeto. Recibe el Tiempo, el mapa de colisiones y el tamanyo de los tiles en pantalla
void ObjetoSala::update(Tiempo t, map<vector<int>, int>* mapa, int tamTiles) {
    //Comprueba si el objeto ya esta en el suelo y, por lo tanto, no se mueve
    if(!colocado && !recogido) {
        //Actualiza la posicion anterior
        posicionAnterior = posicion;

        //Actualiza la velocidad
        if(velocidadActual < VEL_MAXIMA) {
            velocidadActual += ACELERACION;
            velocidadActual = min(velocidadActual, VEL_MAXIMA);
        }

        //Desplaza el objeto
        posicion[1] += velocidadActual*t.segundos();
        
        //Comprueba si colisiona
        int coordInf = sprite->getGlobalPixelSize()[1]/2;
        auto iterator = mapa->find({(int)posicion[0]/tamTiles, ((int)posicion[1]+coordInf)/tamTiles});
        if(iterator != mapa->end()) {
            //Si colisiona lo devuelve a la posicion que le corresponde
            posicion[1] -= ((int)posicion[1]+coordInf)%tamTiles + 2;

            //Indica que ya ha tocado el suelo
            colocado = true;
        }
    }
    else if(colocado && posicion[1] != posicionAnterior[1]) {
        posicionAnterior[1] = posicion[1];
    }
}

//Dibuja el objeto interpolado
void ObjetoSala::dibujar(float percentTick) {
    //Comprueba que el jugador no ha recogido el objeto
    if(!recogido) {
        if(posicion[1] != posicionAnterior[1]) {
            //Si se ha desplazado, actualiza la posicion del sprite
            float y = posicionAnterior[1]*(1-percentTick) + posicion[1]*percentTick;
            sprite->setPosition(posicion[0], y);
        }
        sprite->draw();
    }
}

//Devuelve si el objeto ha sido recogido
bool ObjetoSala::getRecogido() {
    return recogido;
}

//Devuelve la posicion del objeto
vector<float> ObjetoSala::getPosicion() {
    return posicion;
}