#include "Trampa.h"

const int CADENCIA_BASE     = 600;
const int VELOCIDAD_FLECHA  = 300;

const string ANIM_DISPARAR  = "TrampaDisparar";
const float VEL_ANIM        = 20.0;

//Constructor. Recibe un int que indica la direccion de disparo (0 = arriba, 1 = derecha...).
Trampa::Trampa(Textura* t, Sprite* s, int dir):ElementoEntorno(t, s, false){
    dir = min(max(dir, 0), 3);

    //Crea el vector con la direccion de los proyectiles
    switch(dir) {
        case 0: direccion = {0.0, -1.0};    break;
        case 1: direccion = {1.0, 0.0};     break;
        case 2: direccion = {0.0, 1.0};     break;
        case 3: direccion = {-1.0, 0.0};    break;
    }

    contador    = 0;

    animacion   = new Animacion(sprite);
    animacion->addAnimation(ANIM_DISPARAR, VEL_ANIM, 0, 0, 2, 1, 16, 16);
}

//Destructor
Trampa::~Trampa() {
    direccion.clear();
}

void Trampa::update(Tiempo t) {
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Reproduce la animacion
        animacion->playOnce(ANIM_DISPARAR, t.milisegundos());
        if(animacion->getFinished()) {
            //Vuelve a colocar el primer sprite de la animacion cuando termina
            sprite->setRect(0, 0, 16, 16);
        }

        //Reinicia el reloj y, si fuera necesario, el tiempo de Detenible
        relojDetenible.restart();
        if(tiempoAntes.milisegundos() > 0) {
            tiempoAntes.setTiempoCero();
        }

        //Si ha pasado suficiente tiempo desde el ultimo disparo, genera otro proyectil
        if(contador >= CADENCIA_BASE) {
            contador -= CADENCIA_BASE;

            //Crea el sprite a partir de la textura de disparos
            EstadoPartida* estado   = EstadoPartida::Instancia();
            int tamTiles            = estado->getSalaActual()->getConstTamTiles();
            Textura* texFlecha      = estado->getTexturas(2);
            Sprite* sFlecha         = new Sprite(texFlecha, 80, 48, 16, 16);

            //Calcula la orientacion del proyectil segun la direccion de disparo
            float angulo = atan2(direccion[1], direccion[0]);
            sFlecha->setRotation(-45 + angulo*180.0/M_PI);

            //Crea el proyectil y lo anyade al estado
            ProyectilRecto* flecha = new ProyectilRecto(1, 0, sFlecha, {posicion[0] + tamTiles/2.0f, posicion[1] + tamTiles/2.0f}, direccion, VELOCIDAD_FLECHA, 10000, {});
            estado->anyadirProyectil(flecha);
        }
        else if(contador > CADENCIA_BASE-VEL_ANIM*5) {
            //Resetea la animacion para poder reproducirla de nuevo
            animacion->reset();
        }
    }
}