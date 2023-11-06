#include "ProyectilPerforante.h"

const int tamTiles = 32;

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad, el alcanze, un vector con las runas elementales que tiene el arma y la perforacion.
* Para el vector de runas:
* Pos 0->Quemadura: 0 no tiene la runa, 1 quemado leve, 2 quemado grave
* Pos 1->Congelacion: 0 no tiene la runa, 1 congelacion leve, 2 congelacion grave
* Pos 2->Electrico: 0 no tiene la runa, 1 electrocutado leve, 2 electrocutado grave
* La perforacion es un int que indica cuantos enemigos perfora o -1 si es infinito (no perfora el mapa)
*/
ProyectilPerforante::ProyectilPerforante(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int a, vector<int> runas, int perf) : Proyectil(dJ, dE, s, pos, dir, vel, a) {
    runasElementales    = runas;
    perforando          = false;
    perforacionMaxima   = perf;
    perforacionActual   = 0;
}

//Destructor
ProyectilPerforante::~ProyectilPerforante() {
    runasElementales.clear();
}

//Actualiza el proyectil
void ProyectilPerforante::update(Tiempo elapsedTime) {
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
        //Comprueba que no esta perforando al jugador y que aun puede atravesar
        if(!perforando && (perforacionMaxima == -1 || perforacionActual <= perforacionMaxima)) {
            //Danya al jugador y elimina el proyectil
            jugador->danyarVida(danyoJugador);

            //Actualiza la perforacion
            perforando = true;
            perforacionActual++;
        }
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
                
                //Comprueba que no esta perforando al enemigo y que aun puede atravesar
                if(!perforando && (perforacionMaxima == -1 || perforacionActual <= perforacionMaxima)) {
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

                    //Actualiza la perforacion
                    perforando = true;
                    perforacionActual++;
                }
            }
        }
    }

    if(colision) {
        //Genera particulas al haber colisionado con un ser
        for(int i=0; i<5; i++) {
            new ParticulaDireccional(posicion, {255, 0, 0, 255}, 120.0, 500, direccion, 0.2, 0.5);
        }
    }
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        if(comprobarColisionMapa()) {
            //Comprueba si colisiona con el mapa
            destruir = true;
        }

        //Comprueba que no ha llegado al alcance maximo
        if(!colision){
            alcance-= velocidad*elapsedTime.segundos();
            if(alcance <= 0){
                destruir = true;
            }
        }
    }

    //Si ha colisionado comprueba si se debe destruir
    if(colision) {
        if(perforacionMaxima > -1 && perforacionActual>perforacionMaxima) {
            destruir = true;
        }
    }
    else {
        //Si no actualiza su estado
        perforando = false;
    }
}

bool ProyectilPerforante::comprobarColisionMapa() {
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