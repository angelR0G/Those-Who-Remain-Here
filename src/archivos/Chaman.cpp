#include "Chaman.h"

const int TIEMPO_GENERACION     = 2000;

// VALORES PARA CREAR EL KAMIKAZE
//Salud
const int SALUD_KAMIKAZE            = 170;

//Danyo
const int DANYO_1                   = 1;

//Velocidad
const int VELOCIDAD_KAMIKAZE        = 200;

//Tiempo de recuperacion de los ataques (ms)
const int TIEMPO_REC_KAMIKAZE       = 200;

//Esencias soltadas al morir
const int ESENCIAS_KAMIKAZE         = 0;

//Sprites (ruta)
const string SPRITE_KAMIKAZE        = "resources/sprites/pnjs/Kamikaze.png";

//ID de los materiales que suelta al ser derrotado
const vector<int> MAT_KAMIKAZE      =   {};

//Constructor
Chaman::Chaman(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp) {
    contador = 0;
    estado = ChamanEsperando;
    sprite->setPosition(posicion[0], posicion[1]);

    animacion = new Animacion(sprite);
    animacion->addAnimation("ChamanEsperando", 30.0, 0, 0, 3, 0, 14, 18);
    animacion->addAnimation("ChamanAtacando", 30.0, 0, 1, 3, 1, 14, 18);
}

//Destructor
Chaman::~Chaman() {
    delete animacion;
}

//Actualiza el enemigo
void Chaman::update(Tiempo t) {
    Enemigo::update(t);

    //Comprueba que el enemigo no esta detenido
    if(!detenido) {
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        //Actua segun el estado
        switch(estado) {
            case ChamanEsperando:
                //Reproduce la animacion
                animacion->play("ChamanEsperando", t.milisegundos());

                if(contador >= tiempoRecuperacion) {
                    //Si ha pasado suficiente tiempo, comienza a generar un kamikaze
                    contador = 0;
                    estado = ChamanGenerando;
                }
                break;
            case ChamanGenerando:
                //Reproduce la animacion
                animacion->play("ChamanAtacando", t.milisegundos());

                //Genera particulas
                new ParticulaSimpleCuadrada({posicion[0], posicion[1]-30}, {39, 66, 58, 255}, 10, 2000);

                if(contador >= TIEMPO_GENERACION) {
                    //Si ha pasado suficiente tiempo, genera el kamikaze
                    contador = 0;
                    estado = ChamanEsperando;
                    
                    //Genera el kamikaze del mismo modo que la fabrica de enemigos
                    Textura* t6 = new Textura(SPRITE_KAMIKAZE);
                    Sprite* s6 = new Sprite(t6, 0, 0, 14, 14);
                    s6->setOrigin(14/2, 14/2);
                    s6->setScale(2, 2);
                    Enemigo* kamikaze = new Kamikaze(SALUD_KAMIKAZE, DANYO_1, VELOCIDAD_KAMIKAZE, TIEMPO_REC_KAMIKAZE, ESENCIAS_KAMIKAZE, MAT_KAMIKAZE, t6, s6);

                    kamikaze->setPosicion(EstadoPartida::Instancia()->getJugador()->getPosNueva());

                    //Anyade el enemigo a la sala
                    EstadoPartida::Instancia()->getSalaActual()->anyadirEnemigo(kamikaze);
                }
                break;
        }
    }
}

//Resetea el enemigo
void Chaman::reset() {
    Enemigo::reset();
    estado      = ChamanEsperando;
    contador    = 0;
}