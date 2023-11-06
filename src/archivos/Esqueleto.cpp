#include "Esqueleto.h"

const float DIST_DETECCION  = 1000.0;
const float DIST_ATAQUE     = 900.0;

//Constantes para calcular la posicion del esqueleto

//Constructor
Esqueleto::Esqueleto(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    contador = 0;
    direccionMira = 1;
    enRango = false;

    animeEsqueleto = new Animacion(sp);
    animeEsqueleto->addAnimation("EsqueletoDerecha",75.f,0,1,3,1,24,24);
    animeEsqueleto->addAnimation("EsqueletoIzquierda",75.f,4,1,7,1,24,24);
}
void Esqueleto::update(Tiempo dt){
    Enemigo::update(dt);
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        posicionAnterior = posicion;
        if(direccionMira == 1){
            animeEsqueleto->play("EsqueletoDerecha", dt.milisegundos());
        }else if(direccionMira == -1){
            animeEsqueleto->play("EsqueletoIzquierda", dt.milisegundos());
        }
        
        //Comprueba las colisiones tras el movimiento y actualiza el sprite
        //comprobarColisiones();
        if(contador <= tiempoRecuperacion) {
          contador += dt.milisegundos() + tiempoAntes.milisegundos();
        }
        
        if(contador > tiempoRecuperacion) {
            EstadoPartida* partida = EstadoPartida::Instancia();
            Jugador* j = partida->getJugador();
            vector<float> posJugador = j->getPosNueva();

            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if((!enRango && distancia <= DIST_DETECCION) || (enRango && distancia <= DIST_ATAQUE)) {
                //Actualiza el estado del esqueleto a jugador en rango
                enRango = true;

                //Reinicia el contador del ataque
                contador -= tiempoRecuperacion;
                
                //Si esta cerca crea un proyectil

                vector<float> dirDispCalc = {0,0};
                int vel = 30;
                if(posJugador[0]-posicion[0]>0){//Dispara derecha
                    direccionMira = 1;
                }else{
                    direccionMira = -1;
                }

                dirDispCalc[0]=min((distancia*direccionMira)/40, 26.0f);
                dirDispCalc[1]=-20;
                Sprite* spDisparo = new Sprite(partida->getTexturas(2), 16*5, 16*9, 16, 16);
                spDisparo->setScale(2, 2);
                Proyectil* p = new ProyectilParabolico(danyo, 0, spDisparo, posicion, dirDispCalc, vel, 1000, {});
                
                partida->anyadirProyectil(p);
            }
            else{
                //El jugador se ha alejado
                enRango = false;
            }
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
}

Esqueleto::~Esqueleto(){
    delete animeEsqueleto;
}