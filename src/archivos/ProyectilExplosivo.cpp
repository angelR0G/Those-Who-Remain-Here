#include "ProyectilExplosivo.h"

const int tamTiles = 32;

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, el sprite de la explosion, la posicion inicial,
*la direccion en la que se mueve, la velocidad y un vector con las runas elementales que tiene el arma.
* Pos 0->Quemadura: 0 no tiene la runa, 1 quemado leve, 2 quemado grave
* Pos 1->Congelacion: 0 no tiene la runa, 1 congelacion leve, 2 congelacion grave
* Pos 2->Electrico: 0 no tiene la runa, 1 electrocutado leve, 2 electrocutado grave 
*/
ProyectilExplosivo::ProyectilExplosivo(int dJ, int dE, Sprite* s, Sprite* sE, Sprite* sV, vector<float> pos, vector<float> dir, int vel, float r, int g, int d, vector<int> runas, int a) : Proyectil(dJ, dE, s, pos, dir, vel, a) {
    spriteExplosion  = sE;
    radio            = r;
    gas              = g;
    dobleExplosion   = d;
    spriteVeneno     = sV;
    runasElementales = runas;
}

//Destructor
ProyectilExplosivo::~ProyectilExplosivo() {
    runasElementales.clear();
}

//Actualiza el proyectil
void ProyectilExplosivo::update(Tiempo elapsedTime) {
    bool colision = false;
    EstadoPartida* estado = EstadoPartida::Instancia();
    Jugador* jugador = estado->getJugador();
    vector<int> tamMapa  = estado->getSalaActual()->getTamanyoMapa();
    Explosion* explosion = nullptr;
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Guarda el estado actual como el anterior
        posicionAnterior = posicion;

        //Modifica la posicion actual
        posicion[0] += direccion[0]*velocidad*elapsedTime.segundos();
        posicion[1] += direccion[1]*velocidad*elapsedTime.segundos();

        //Resetea el reloj de Detenible
        relojDetenible.restart();
    }
    if(!detenido && danyoEnemigo > 0) {        //Comprueba si colisiona con algun enemigo
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

                colision = true;

                //Aumenta el robo de vida del jugador
                jugador->aumentarRoboVida(danyoEnemigo);
            }
        }
    }
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        if(!colision) {
            //Comprueba si colisiona con el mapa
            colision = comprobarColisionMapa();
        }

        //Comprueba que si se sale del mapa se borre la bala
        if(!colision){
            if(alcance == -1){
                if(posicion[0]<0 || posicion[0]/32>tamMapa[0] || posicion[1]<0 || posicion[1]/32>tamMapa[1]) {
                    destruir = true;
                }
            }else{
                alcance-= velocidad*elapsedTime.segundos();
                if(alcance <= 0){
                    destruir = true;
                }
            }
        }
    }

    //Si ha colisionado la destruye
    if(colision) {
        destruir = true;
        explosion = new Explosion(danyoJugador, danyoEnemigo, spriteExplosion, spriteVeneno, posicion, radio, gas, dobleExplosion);
        estado->anyadirExplosion(explosion);
    }
}

bool ProyectilExplosivo::comprobarColisionMapa() {
    bool colision = false;
    EstadoPartida* estado = EstadoPartida::Instancia();

    //Recupera el mapa de colisiones del nivel
    map<vector<int>, int>* mapa = estado->getSalaActual()->getMapaColisiones();
    vector<int> pos = {(int)posicion[0], (int)posicion[1]};
    int idPlataformas = estado->getSalaActual()->getIdColision(2);
    vector<int> posCalc = {pos[0]/tamTiles, pos[1]/tamTiles};

    //Comprueba si su posicion coincide con alguna colision del mapa
    if(mapa->find(posCalc) != mapa->end() && mapa->find(posCalc)->second != idPlataformas) {
        colision = true;
    }
    else {
        //Comprueba que no colisiona ninguna esquina del sprite
        vector<int> pixels = sprite->getGlobalPixelSize();
        vector<int> posDesp = {(pos[0]-pixels[0]/2)/tamTiles, (pos[1]-pixels[1]/2)/tamTiles};
        if(posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1]) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }
        
        posDesp = {(pos[0]+pixels[0]/2)/tamTiles, (pos[1]-pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }

        posDesp = {(pos[0]-pixels[0]/2)/tamTiles, (pos[1]+pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }

        posDesp = {(pos[0]+pixels[0]/2)/tamTiles, (pos[1]+pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }
    }

    return colision;
}