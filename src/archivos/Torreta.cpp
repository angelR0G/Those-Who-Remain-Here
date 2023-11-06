#include "Torreta.h"

const float DIST_DETECCION  = 500.0;
const float DIST_ATAQUE     = 900.0;

//Constructor
Torreta::Torreta(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias, vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
    int inicial = rand() % 20;
    if(inicial%2 == 0) {
        levitando = true;
    }
    else {
        levitando = false;
    }
    desplazamiento  = (float)inicial;
    posInicial      = {0, 0};

    enRango = false;
    contadorDisparo = 0;
}

void Torreta::update(Tiempo dt){
    Enemigo::update(dt);
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Actualiza la posicion
        posicionAnterior = posicion;
        if(levitando) {
            //Desplaza la cabeza hacia arriba
            desplazamiento  += getVelocidad()*dt.segundos();
            posicion[1]     = posInicial[1] - desplazamiento;

            if(desplazamiento > 20.0) {
                levitando = false;
            }
        }
        else {
            //Desplaza la cabeza hacia abajo
            desplazamiento -= getVelocidad()*dt.segundos();
            posicion[1]     = posInicial[1] - desplazamiento;

            if(desplazamiento < 0.0) {
                levitando = true;
            }
        }

        //Modifica la direccion en la que mira
        EstadoPartida* partida = EstadoPartida::Instancia();
        Jugador* j = partida->getJugador();
        vector<float> posJugador = j->getPosNueva();

        vector<float> escalaActual = sprite->getScale();
        if(posJugador[0] >= posicion[0]) {
            sprite->setScale(abs(escalaActual[0]), escalaActual[1]);
        }
        else if(escalaActual[0] > 0){
            sprite->setScale(escalaActual[0]*(-1.0f), escalaActual[1]);
        }

        //Genera particulas en la torreta
        if(rand() % 2) {
            int desp = rand() % 17 - 9;
            new ParticulaDireccional({posicion[0]+desp, posicion[1]+10}, {255, 255, 255, 100}, 8.0, 1600, {0, 1}, 0.1, 0.2);
        }

        contadorDisparo += dt.milisegundos() + tiempoAntes.milisegundos();
        if(contadorDisparo > tiempoRecuperacion) {
            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if((!enRango && distancia <= DIST_DETECCION) || (enRango && distancia <= DIST_ATAQUE)) {
                //Actualiza el estado de la torreta a jugador en rango
                enRango = true;
                
                //Si esta cerca crea un proyectil
                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                Sprite* spDisparo = new Sprite(partida->getTexturas(2), 81, 82, 14, 14);
                Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, dirDisp, 250, 1000, {});
                partida->anyadirProyectil(p);
            }
            else{
                //El jugador se ha alejado
                enRango = false;
            }

            contadorDisparo -= tiempoRecuperacion;
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
    
    
}

Torreta::~Torreta(){

}

//Actualiza la posicion de la torreta
void Torreta::setPosicion(vector<float> p) {
    Enemigo::setPosicion(p);
    if(p.size() == 2) {
        posInicial  = p;
    }
}