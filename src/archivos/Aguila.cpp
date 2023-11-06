#include "Aguila.h"

const float DISTANCIA_MAX       = 500.0;
const int DESPLAZAMIENTO_X      = 110;
const int INCRM_DESPL_X         = 28;
const float ACELERACION         = 10.0;
const float ACELERACION_ENFADO  = 20.0;
const float MULTIP_VEL_ENFADO   = 1.6;
const int TIEMPO_ENFADADO       = 1200;

//Constructor
Aguila::Aguila(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    danyado         = 0;
    contador        = 0;
    direccion       = rand() % 2;
    velocidadActual = 0;
    xUltimoCambio   = -1;
    

    animacion = new Animacion(sprite);
    animacion->addAnimation("AguilaLenta", 100.0, 0, 1, 4, 1, 16, 16);
    animacion->addAnimation("AguilaRapida", 60.0, 0, 1, 4, 1, 16, 16);
}

//Destructor
Aguila::~Aguila() {
    delete animacion;
}

//Actualiza el aguila
void Aguila::update(Tiempo t) {
    Enemigo::update(t);

    //Comprueba que no esta detenido
    if(!detenido) {
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Al sufrir danyo, se mueve y ataca mas deprisa
        if(danyado > 0) {
            animacion->play("AguilaRapida", t.milisegundos());
            danyado -= t.milisegundos() + tiempoAntes.milisegundos();
            //Si ha pasado un cuarto del tiempo de recuperacion, dispara
            if(contador >= tiempoRecuperacion/4) {
                contador -= tiempoRecuperacion/4;

                Sprite* spDisparo = new Sprite(EstadoPartida::Instancia()->getTexturas(2), 3, 262, 10, 7);
                spDisparo->setScale(2.0, 2.0);
                spDisparo->setRotation(90.0);
                Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, {0, 1}, 180, 800, {});
                EstadoPartida::Instancia()->anyadirProyectil(p);
            }

            if(direccion) {
                //Se mueve hacia la derecha, con mayor velocidad maxima
                velocidadActual += ACELERACION_ENFADO*(t.segundos()+tiempoAntes.segundos());
                velocidadActual = min(velocidadActual, (float)getVelocidad()*MULTIP_VEL_ENFADO);
            }
            else {
                //Se mueve hacia la izquierda, con mayor velocidad maxima
                velocidadActual -= ACELERACION_ENFADO*2*(t.segundos()+tiempoAntes.segundos());
                velocidadActual = max(velocidadActual, (float)getVelocidad()*MULTIP_VEL_ENFADO*(-1));
            }
        }
        else {
            animacion->play("AguilaLenta", t.milisegundos());
            //Si ha pasado el tiempo de recuperacion, dispara
            if(contador >= tiempoRecuperacion) {
                contador -= tiempoRecuperacion;

                Sprite* spDisparo = new Sprite(EstadoPartida::Instancia()->getTexturas(2), 3, 262, 10, 7);
                spDisparo->setScale(2.0, 2.0);
                spDisparo->setRotation(90.0);
                Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, {0, 1}, 180, 800, {});
                EstadoPartida::Instancia()->anyadirProyectil(p);
            }

            if(direccion) {
                //Se mueve hacia la derecha
                velocidadActual += ACELERACION*(t.segundos()+tiempoAntes.segundos());
                velocidadActual = min(velocidadActual, (float)getVelocidad());
            }
            else {
                //Se mueve hacia la izquierda
                velocidadActual -= ACELERACION*(t.segundos()+tiempoAntes.segundos());
                velocidadActual = max(velocidadActual, (float)getVelocidad()*(-1));
            }
        }
        comprobarColisiones();
        posicionAnterior = posicion;

        //Actualiza la posicion y el sprite
        posicion[0] += velocidadActual;

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
}

//Sobrecarga de perderVida ed Enemigo
void Aguila::perderVida(int d) {
    Enemigo::perderVida(d);

    //Al recibir danyo, el aguila se enfada
    if(danyado <= 0) {
        contador    = 0;
        danyado     = TIEMPO_ENFADADO;
    }
    else {
        danyado += TIEMPO_ENFADADO/2;
    }
}

//Comprueba si el aguila va a chocar con una pared para cambiar de direccion
void Aguila::comprobarColisiones() {
    bool colision = false;

    //Recupera las colisiones
    Sala* salaActual                = EstadoPartida::Instancia()->getSalaActual();
    map<vector<int>, int>* mapaCol  = salaActual->getMapaColisiones();
    int tamTiles                    = salaActual->getConstTamTiles();
    int idPlat                      = salaActual->getIdColision(2);
    vector<int> tamMapa             = salaActual->getTamanyoMapa();
    vector<int> posComprobar        = {(int)posicion[0]/tamTiles, (int)posicion[1]/tamTiles};

    //Comprueba que no se ha salido del mapa por algun error
    if(posComprobar[0] >= tamMapa[0] || posComprobar[0] < 0 || posComprobar[1] >= tamMapa[1] || posComprobar[1] < 0) {
        muerto = true;
    }
    else {
        //Funcionamiento correcto
        if(direccion) {
            //Se esta desplazando hacia la derecha
            posComprobar[0] = ((int)posicion[0] + DESPLAZAMIENTO_X + INCRM_DESPL_X)/tamTiles;
        }
        else {
            //Se esta desplazando hacia la izquierda
            posComprobar[0] = ((int)posicion[0] - DESPLAZAMIENTO_X - INCRM_DESPL_X)/tamTiles;
        }

        //Comprueba si hay una colision en la posicion a comprobar
        auto i = mapaCol->find(posComprobar);
        if(i != mapaCol->end() && i->second != idPlat) {
            //Si detecta colision y no es de plataforma, cambia la direccion de movimiento
            colision = true;
        }
        else {
            //Si no detecta colision, comprueba la misma posicion ligeramente desplazada
            if(direccion) {
                posComprobar[0] = ((int)posicion[0] + DESPLAZAMIENTO_X)/tamTiles;
            }
            else {
                posComprobar[0] = ((int)posicion[0] - DESPLAZAMIENTO_X)/tamTiles;
            }
            auto i = mapaCol->find(posComprobar);
            if(i != mapaCol->end() && i->second != idPlat) {
                colision = true;
            }
            else {
                //Comprueba una posicion aun mas cercana
                if(direccion) {
                    posComprobar[0] = ((int)posicion[0] + DESPLAZAMIENTO_X*2/3)/tamTiles;
                }
                else {
                    posComprobar[0] = ((int)posicion[0] - DESPLAZAMIENTO_X*2/3)/tamTiles;
                }

                auto i = mapaCol->find(posComprobar);
                if(i != mapaCol->end() && i->second != idPlat) {
                    colision = true;
                }
            }
        }

        //Hace una comprobacion extra mas cercana
        if(!colision) {
            if(direccion) {
                posComprobar[0] = ((int)posicion[0] + DESPLAZAMIENTO_X/2)/tamTiles;
            }
            else {
                posComprobar[0] = ((int)posicion[0] - DESPLAZAMIENTO_X/2)/tamTiles;
            }

            auto i = mapaCol->find(posComprobar);
            if(i != mapaCol->end() && i->second != idPlat) {
                colision = true;
            }
        }

        //Si ha detectado colision, cambia la direccion de movimiento
        if(colision) {
            if(danyado > 0) {
                if(direccion && velocidadActual > 0) {
                    modificarDireccion();
                }
                else if(!direccion && velocidadActual < 0) {
                    modificarDireccion();
                }

                velocidadActual *= 0.5;
            }
            else {
                modificarDireccion();
                xUltimoCambio = posicion[0];
            }
        }
        else if(xUltimoCambio != -1){
            //Si no detecta, comprueba si se ha desplazado la distancia maxima
            if(abs(posicion[0]-xUltimoCambio) >= DISTANCIA_MAX) {
                modificarDireccion();
                xUltimoCambio = posicion[0];
            }
        }

        //Comprueba si se encuentra en una colision, en ese caso, vuelve a la posicion anterior
        posComprobar[0] = ((int)posicion[0])/tamTiles;
        i = mapaCol->find(posComprobar);
        if(i != mapaCol->end() && i->second != idPlat) {
            //Si detecta colision
            if(velocidadActual > 0 && direccion) {
                modificarDireccion();
                posicion = posicionAnterior;
            }
            else if(velocidadActual < 0 && !direccion) {
                modificarDireccion();
                posicion = posicionAnterior;
            }
        }
    }
}

//Modifica la direccion en la que se desplaza
void Aguila::modificarDireccion() {
    if(direccion) {
        direccion = false;
    }
    else {
        direccion = true;
    }
}

//Resetea el aguila
void Aguila::reset() {
    contador    = 0;
    danyado     = 0;
}