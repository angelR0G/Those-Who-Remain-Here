#include "ProyectilRayoOjo.h"
/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad, el tiempo de preparacion y el de ataque, por ultimo un vector con la escala a aplicar al rayo*/

const int TIEMPO_HACER_DANYO = 1000;

ProyectilRayoOjo::ProyectilRayoOjo(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int tP, int tA, vector<float> a) : Proyectil(dJ, dE, s, pos, dir, vel, -1) {
    estados = PO_PreparaAtaque;
    contador = 0;
    tiempoPreparar = tP;
    tiempoAtaque = tA;
    cooldownDanyo = 0;
    alcanceEscala = a;
    
}

ProyectilRayoOjo::~ProyectilRayoOjo(){

}

void ProyectilRayoOjo::update(Tiempo t){
    contador += t.milisegundos() + tiempoAntes.milisegundos();
    cooldownDanyo += t.milisegundos() + tiempoAntes.milisegundos();
    if(!detenido){
        //Resetea el reloj de Detenible y tiempoAntes
        relojDetenible.restart();
        tiempoAntes.setTiempoCero();
    }
    if(contador >= tiempoPreparar + tiempoAtaque){
        destruir = true;
    }
    EstadoPartida *ePartida = EstadoPartida::Instancia();
    Jugador* jugador = ePartida->getJugador();
    if(!destruir){
        switch (estados)
        {
            case PO_PreparaAtaque:
                sprite->setScale(alcanceEscala[0], alcanceEscala[1]);
                if(contador >=tiempoPreparar){
                    estados = PO_Ataque;
                }
                break;
                
            case PO_Ataque:
                //Se cambia el sprite del proyectil y se comprueba si golpea al jugador
                sprite->setRect(10, 10, 10, 6);
                if(cooldownDanyo >= TIEMPO_HACER_DANYO){
                    cooldownDanyo = 0;
                    if(danyoJugador > 0 && jugador->getSprite()->intersect(sprite)) {
                        //Danya al jugador
                        jugador->danyarVida(danyoJugador);
                    }
                }
                break;
            default:
                break;
        }
    }
}