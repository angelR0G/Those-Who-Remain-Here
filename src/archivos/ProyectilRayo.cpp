#include "ProyectilRayo.h"

const int TIEMPO_CARGA          = 2500;
const int TIEMPO_DISPARO        = 2000;
const int TIEMPO_TOTAL        =5000;



using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad, el alcanze y un vector con las runas elementales que tiene el arma.
* Pos 0->Quemadura: 0 no tiene la runa, 1 quemado leve, 2 quemado grave
* Pos 1->Congelacion: 0 no tiene la runa, 1 congelacion leve, 2 congelacion grave
* Pos 2->Electrico: 0 no tiene la runa, 1 electrocutado leve, 2 electrocutado grave 
*/
ProyectilRayo::ProyectilRayo(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int a, vector<int> runas) : Proyectil(dJ, dE, s, pos, dir, vel, a) {
    estadoRayo = Cargando;
    animacion = new Animacion(sprite);
    animacion->addAnimation("dispara", 60.0, 0, 1, 2, 1, 32, 32);
    animacion->addAnimation("cargando", 60.0, 0, 0, 2, 0, 32, 33);
    posicion = pos;
    contador = 0;

    s->setScale(3,3);
}

//Destructor
ProyectilRayo::~ProyectilRayo() {
    if(sprite != nullptr){
        delete sprite;
        sprite = nullptr;
    }
}

//Actualiza el proyectil
void ProyectilRayo::update(Tiempo tiempo) {
    EstadoPartida* estado = EstadoPartida::Instancia();
    Jugador* jugador = estado->getJugador();
    contador += tiempo.milisegundos() + tiempoAntes.milisegundos();
    if(contador >= TIEMPO_TOTAL) {
                    //Comprueba la distancia a la que se encuentra el jugador
                    destruir =true;

         }
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Guarda el estado actual como el anterior
        /*posicionAnterior = posicion;

        //Modifica la posicion actual
        posicion[0] += direccion[0]*velocidad*elapsedTime.segundos();
        posicion[1] += direccion[1]*velocidad*elapsedTime.segundos();*/

        //Resetea el reloj de Detenible y tiempoAntes
        relojDetenible.restart();
        tiempoAntes.setTiempoCero();

        switch(estadoRayo) {
            case Cargando:
                animacion->play("cargando", tiempo.milisegundos());
                if(contador >= TIEMPO_CARGA) {
                    contador = 0;
                    estadoRayo = Dispara;
                    cacahuete++;
                    //Genera particulas para indicar claramente que ya ha aparecido
                    for(int i = 10; i > 0; i--) {
                        new ParticulaSimpleCuadrada(posicion, {39, 66, 58, 255}, 30, 1000);
                    }                    
                }
                
                break;
            case Dispara:
                if(cacahuete!=0){
                    sprite->setScale(3,3);
                    animacion->play("dispara", tiempo.milisegundos());
                  //Comprueba si colisiona con el jugador
                if(danyoJugador > 0 && jugador->getSprite()->intersect(sprite)) {
                    //Danya al jugador y elimina el proyectil
                    jugador->danyarVida(danyoJugador);
                }
                if(contador >= TIEMPO_DISPARO) {
                    //Comprueba la distancia a la que se encuentra el jugador
                    destruir =true;

                }
                }
                
                break;           
        }
    }
}
