#include "ProyectilRebota.h"

const int tamTiles = 32;

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad, el numero de rebotes y un vector con las runas elementales que tiene el arma.
* Pos 0->Quemadura: 0 no tiene la runa, 1 quemado leve, 2 quemado grave
* Pos 1->Congelacion: 0 no tiene la runa, 1 congelacion leve, 2 congelacion grave
* Pos 2->Electrico: 0 no tiene la runa, 1 electrocutado leve, 2 electrocutado grave 
*/
ProyectilRebota::ProyectilRebota(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int rebotesM, vector<int> runas) : Proyectil(dJ, dE, s, pos, dir, vel, 100000) {
    runasElementales    = runas;
    rebotesMaximos      = max(0, rebotesM);
    rebotesActuales     = rebotesMaximos;
}

//Destructor
ProyectilRebota::~ProyectilRebota() {
    runasElementales.clear();
}

//Actualiza el proyectil
void ProyectilRebota::update(Tiempo elapsedTime) {
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
    else if(!detenido && danyoEnemigo > 0) {        //Comprueba si colisiona con algun enemigo
        //Recupera los enemigos del nivel y comprueba si el proyectil impacta en alguno
        vector<Enemigo*>* enemigos = estado->getSalaActual()->getEnemigos();
        for(unsigned int i=0; i<enemigos->size() && !colision; i++) {
            //Comprueba si el sprite del enemigo intersecta con el del proyectil
            if((*enemigos)[i]->getSprite()->intersect(sprite)) {
                //Danya al enemigo y elimina el proyectil
                colision = true;
                (*enemigos)[i]->perderVida(danyoEnemigo);

                if(runasElementales.size() == 3){
                    //Se  comprueba si tiene alguna de las runas elementales
                    if(runasElementales[0] == 1){
                        (*enemigos)[i]->quemar(false, -1);
                    }else if(runasElementales[0] == 2){
                        (*enemigos)[i]->quemar(true, -1);
                    }

                    if(runasElementales[1] == 1){
                        (*enemigos)[i]->congelar(false, -1);
                    }else if(runasElementales[1] == 2){
                        (*enemigos)[i]->congelar(true, -1);
                    }

                    if(runasElementales[2] == 1){
                        (*enemigos)[i]->electrocutar(false, -1);
                    }else if(runasElementales[2] == 2){
                        (*enemigos)[i]->electrocutar(true, -1);
                    }
                }

                //Aumenta el robo de vida del jugador
                jugador->aumentarRoboVida(danyoEnemigo);
            }
        }
    }
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        if(!colision) {
            //Comprueba si colisiona con el mapa
            if(comprobarColisionMapa()) {
                //Disminuye los rebotes restantes antes de ser destruido
                rebotesActuales--;
                if(rebotesActuales < 0) {
                    //Si ha rebotado el maximo de veces, se destruye
                    destruir = true;
                }
            }
        }
        else {
            //Genera particulas al haber colisionado con un ser
            for(int i=0; i<5; i++) {
                new ParticulaDireccional(posicion, {255, 0, 0, 255}, 120.0, 500, direccion, 0.2, 0.5);
            }
        }

        //Comprueba que no ha llegado al alcance maximo
        if(!colision){
            alcance-= velocidad*elapsedTime.segundos();
            if(alcance <= 0){
                destruir = true;
            }
        }
    }

    //Si ha colisionado la destruye
    if(colision) {
        destruir = true;
    }
}

bool ProyectilRebota::comprobarColisionMapa() {
    bool colision = false;
    vector<bool> ladoCol = {false, false};
    EstadoPartida* estado = EstadoPartida::Instancia();

    //Recupera el mapa de colisiones del nivel
    map<vector<int>, int>* mapa = estado->getSalaActual()->getMapaColisiones();
    vector<int> pos = {(int)posicion[0], (int)posicion[1]};
    int idPlataformas = estado->getSalaActual()->getIdColision(2);
    vector<int> posCalc = {pos[0]/tamTiles, pos[1]/tamTiles};

    //Comprueba si su posicion coincide con alguna colision del mapa
    if(mapa->find(posCalc) != mapa->end() && mapa->find(posCalc)->second != idPlataformas) {
        colision = true;
        ladoCol = {true, true};
    }
    //Comprueba que no colisiona ninguna lado del sprite
    vector<int> pixels = sprite->getGlobalPixelSize();
    vector<int> posDesp = {pos[0]/tamTiles, (pos[1]-pixels[1]/2)/tamTiles};
    if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
        //Colisiona por la parte superior
        colision = true;
        ladoCol[1] = true;
        pos[1] += tamTiles - (pos[1]-pixels[1]/2)%tamTiles;
    }
    
    posDesp = {(pos[0]+pixels[0]/2)/tamTiles, pos[1]/tamTiles};
    if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
        //Colisiona por el lado derecho
        colision = true;
        ladoCol[0] = true;
        pos[0] -= (pos[0]+pixels[0]/2)%tamTiles;
    }

    posDesp = {pos[0]/tamTiles, (pos[1]+pixels[1]/2)/tamTiles};
    if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
        //Colisiona por la parte inferior
        colision = true;
        ladoCol[1] = true;
        pos[1] -= (pos[1]+pixels[1]/2)%tamTiles;
    }

    posDesp = {(pos[0]-pixels[0]/2)/tamTiles, pos[1]/tamTiles};
    if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
        //Colisiona por el lado izquierdo
        colision = true;
        ladoCol[0] = true;
        pos[0] += tamTiles - (pos[0]-pixels[0]/2);
    }

    //Si se ha detectado alguna colision, se comprueban los rebotes para cambiar la direccion
    if(colision) {
        if(ladoCol[0])
            direccion[0] *= -1.0;
        if(ladoCol[1])
            direccion[1] *= -1.0;
    }

    return colision;
}