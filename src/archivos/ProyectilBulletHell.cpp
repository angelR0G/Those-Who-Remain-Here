#include "ProyectilBulletHell.h"

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve y la velocidad
*/
ProyectilBulletHell::ProyectilBulletHell(int dJ, Sprite* s, vector<float> pos, vector<float> dir, int vel) : Proyectil(dJ, 0, s, pos, dir, vel, -1) {}

//Destructor
ProyectilBulletHell::~ProyectilBulletHell() {}

//Actualiza el proyectil
void ProyectilBulletHell::update(Tiempo elapsedTime) {
    bool colision = false;
    EstadoPartida* estado = EstadoPartida::Instancia();
    Jugador* jugador = estado->getJugador();

    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Guarda el estado actual como el anterior
        posicionAnterior = posicion;

        //Modifica la posicion actual
        posicion[0] += direccion[0]*velocidad*elapsedTime.segundos();
        posicion[1] += direccion[1]*velocidad*elapsedTime.segundos();

        //Resetea el reloj de Detenible y tiempoAntes
        relojDetenible.restart();
        tiempoAntes.setTiempoCero();
    }

    //Comprueba si colisiona con el jugador
    if(danyoJugador > 0 && jugador->getSprite()->intersect(sprite)) {
        //Danya al jugador y elimina el proyectil
        jugador->danyarVida(danyoJugador);
        colision = true;
    }
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        if(!colision) {
            EstadoPartida* estado = EstadoPartida::Instancia();
            vector<int> tamMapa = estado->getSalaActual()->getTamanyoMapa();
            tamMapa[0] *= estado->getSalaActual()->getConstTamTiles();
            tamMapa[1] *= estado->getSalaActual()->getConstTamTiles();
            if(posicion[0] < 0 || posicion[1] < 0 || posicion[0] > tamMapa[0] || posicion[1] > tamMapa[1]){
                colision = true;
            }
        }
    }

    //Si ha colisionado la destruye
    if(colision) {
        destruir = true;
    }
}