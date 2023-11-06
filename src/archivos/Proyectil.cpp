#include "Proyectil.h"

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad y el alcance.
*/
Proyectil::Proyectil(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int a) {
    danyoJugador        = dJ;
    danyoEnemigo        = dE;
    sprite              = s;
    posicion            = pos;
    posicionAnterior    = pos;
    direccion           = dir;
    velocidad           = vel;
    alcance             = a;

    destruir = false;

    //Asegura que el origen del sprite se encuentra en el centro
    vector<int> tamSprite   = s->getGlobalPixelSize();
    vector<float> escala    = s->getScale();
    sprite->setOrigin(tamSprite[0]/(escala[0]*2.0), tamSprite[1]/(escala[1]*2.0));
}

//Destructor
Proyectil::~Proyectil() {
    posicionAnterior.clear();
    posicion.clear();
    direccion.clear();

    if(sprite != nullptr){
        delete sprite;
        sprite = nullptr;
    }
}

//Dibuja el proyectil
void Proyectil::draw(float percentTick, int update_tick_time) {
    VentanaRender* ventana = VentanaRender::Instancia();

    //Si el proyectil se ha detenido mantiene su estado
    if(detenido || percentTick < tiempoAntes.milisegundos()/update_tick_time) {
        percentTick = tiempoAntes.milisegundos()/update_tick_time;
    }
    
    //Calcula la posicion del sprite interpolando la posicion anterior y la nueva
    float x = posicionAnterior[0]*(1-percentTick) + posicion[0]*percentTick;
    float y = posicionAnterior[1]*(1-percentTick) + posicion[1]*percentTick;

    //Si va a desaparecer por alcance, reduce la opacidad
    if(alcance < 40 && alcance >= 0) {
        sprite->setColor({255, 255, 255, static_cast<int>((alcance/40.0)*255)});
    }

    //Actualiza la posicion del sprite y lo dibuja
    sprite->setPosition(x, y);
    ventana->dibujar(sprite);
}

bool Proyectil::getDestruir(){
    return destruir;
}

//Cambiar velocidad proyectil
void Proyectil::setVelocidad(int p_vel){
    velocidad = p_vel;
}