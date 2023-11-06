#include "Goblin.h"

const float DIST_DETECCION  = 500.0;
const float DIST_ATAQUE     = 900.0;

//Constantes para calcular la posicion del goblin
const int COLISION_X        = 14;
const int COLISION_Y        = 32;
const int COLISION_INX      = 6;

//Constructor
Goblin::Goblin(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
    dir = rand() % 2;
    
    desplazamiento = 0;
    contador = 0;
    caer = false;
    ladoColisiones={-1, -1, -1};

    enRango = false;

    animeGoblin = new Animacion(sp);
    animeGoblin->addAnimation("GoblinDerecha",75.f,0,1,3,1,24,24);
    animeGoblin->addAnimation("GoblinIzquierda",75.f,4,1,7,1,24,24);
}
void Goblin::update(Tiempo dt){
    Enemigo::update(dt);
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
      posicionAnterior = posicion;
        //Cancelar posibilidad de caer
        if(caer){
          //Caer
          desplazamiento += getVelocidad()*dt.segundos();
          desplazamiento = min(desplazamiento, 24);
          posicion[1]+=desplazamiento;
        }else{
          if(dir){
            //Desplaza derecha
            desplazamiento = getVelocidad()*dt.segundos();
            animeGoblin->play("GoblinDerecha",dt.milisegundos());
          }else{
            //Desplaza izquierda
            desplazamiento = getVelocidad()*dt.segundos()*(-1);
            animeGoblin->play("GoblinIzquierda",dt.milisegundos());
          }
          
          posicion[0]+=desplazamiento;
        }

        //Comprueba las colisiones tras el movimiento y actualiza el sprite
        comprobarColisiones();
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
                //Actualiza el estado del goblin a jugador en rango
                enRango = true;

                int des = 0;
                if(dir){
                  //Desplaza derecha
                  des -= getVelocidad()*dt.segundos();
                }else{
                  //Desplaza izquierda
                  des += getVelocidad()*dt.segundos();
                }

                float distanciaFutura = sqrt(pow(posJugador[0]-posicion[0]-des, 2) + pow(posJugador[1]-posicion[1], 2));

                if(distanciaFutura > distancia)
                  dir = !dir;

                //Reinicia el contador del ataque
                contador -= tiempoRecuperacion;
                
                //Si esta cerca crea un proyectil
                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                float angulo = atan2(dirDisp[1],dirDisp[0]);
                angulo = angulo*180/M_PI;
                Sprite* spDisparo = new Sprite(partida->getTexturas(2), 16*2 + 2, 16*4 + 4, 10, 10);
                spDisparo->setRotation(angulo);
                Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, dirDisp, 250, 1000, {});
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

Goblin::~Goblin(){
  delete animeGoblin;
  ladoColisiones.clear();
}

//Se comprueban las colisiones del Goblin
void Goblin::comprobarColisiones(){
    EstadoPartida *estado               = EstadoPartida::Instancia();
    map<vector<int>, int>* colisionMap  = estado->getSalaActual()->getMapaColisiones();
    vector<int> tamMapa                 = estado->getSalaActual()->getTamanyoMapa();
    int tamTiles                        = estado->getSalaActual()->getConstTamTiles();

    vector<int> posComprobar = {-1,-1};

    //Id del tile a comprobar
    int idCol         = estado->getSalaActual()->getIdColision(1);
    int idPlat        = estado->getSalaActual()->getIdColision(2);
    int idPDerecha    = estado->getSalaActual()->getIdColision(4);
    int idPIzquierda  = estado->getSalaActual()->getIdColision(6);

    bool tocandoSuelo   = false;
    bool sueloIzquierda = false;
    bool sueloDerecha   = false;

if(posicion[0]>=0 && posicion[0]/32<tamMapa[0] && posicion[1]>=0 && posicion[1]/32<tamMapa[1]) {
    ladoColisiones = {0.0, 0.0, 0.0, 0.0};
    //Parte central derecha
    posComprobar[0] = (posicion[0]+COLISION_X)/tamTiles;
    posComprobar[1] = (posicion[1]+0)/tamTiles;
    auto tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[0] = 0;
      }else if(tile->second == idCol || tile->second == idPDerecha){
        ladoColisiones[0] = ((int)(posicion[0]+COLISION_X)%tamTiles)*(-1) +1;
      }
    }else{
      ladoColisiones[0] = 0;
    }


    //Parte central Abajo
    posComprobar[0] = (posicion[0]+0)/tamTiles;
    posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idCol || tile->second == idPlat){
        ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
        tocandoSuelo = true;
      }
      //Parte abajo izquierda
      posComprobar[0] = (posicion[0]-COLISION_X+COLISION_INX)/tamTiles;
      posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
      tile = colisionMap->find(posComprobar);
      if(tile->second == idCol || tile->second == idPlat){
        ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
        sueloIzquierda = true;
        tocandoSuelo = true;
      }
      //Parte abajo derecha
      posComprobar[0] = (posicion[0]+COLISION_X-COLISION_INX)/tamTiles;
      posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
      tile = colisionMap->find(posComprobar);
      if(tile->second == idCol || tile->second == idPlat){
        ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
        sueloDerecha = true;
        tocandoSuelo = true;
      }
    }else{
      ladoColisiones[1] = 0;
    }

    //Parte central izquierda
    posComprobar[0] = (posicion[0]-COLISION_X)/tamTiles;
    posComprobar[1] = (posicion[1]+0)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[2] = 0;
      }else if(tile->second == idCol || tile->second == idPIzquierda){
        ladoColisiones[2] = ((int)(posicion[0]-COLISION_X)%tamTiles) - 1;
      }
    }else{
      ladoColisiones[2] = 0;
    }

    //Colision izquierda
    if(ladoColisiones[2] != 0){
      posicion[0] += (tamTiles-ladoColisiones[2]);
      dir=true;
    }

    //Colision derecha
    if(ladoColisiones[0] != 0){
      posicion[0] += ladoColisiones[0];
      dir=false;
    }

    //Colision abajo
    if(tocandoSuelo){
      posicion[1]+=ladoColisiones[1];

      if(caer) {
        caer = false;
        desplazamiento = 0;
      }
      else {
        if(dir) {
          //Se esta moviendo hacia la derecha
          if(!sueloDerecha) {
            dir = false;
          }
        }
        else {
          //Se esta moviendo hacia la izquierda
          if(!sueloIzquierda) {
            dir = true;
          }
        }
      }
    }else if(!caer){
      caer = true;
      desplazamiento = 0;
    }
  }
}