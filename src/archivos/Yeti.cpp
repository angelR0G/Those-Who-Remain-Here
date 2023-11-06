#include "Yeti.h"

const float DIST_ATAQUE     = 600.0;
const int TIEMPO_COMPROB    = 1000;
const int CANT_DISPAROS     = 8;
const int TIEMPO_ENTRE_DISP = 200;

//Constantes para calcular la posicion del yeti
const int COLISION_X        = 14;
const int COLISION_Y        = 30;
const int COLISION_INX      = 6;

const string ANIM_ANDAR     = "YetiAndando";
const string ANIM_ATACAR    = "YetiAtacando";

//Constructor
Yeti::Yeti(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
    dir = rand() % 2;
    
    desplazamiento      = 0;
    contador            = 0;
    caer                = false;
    ladoColisiones      = {-1, -1, -1};

    disparando          = false;
    disparosRestantes   = 0;

    animacion = new Animacion(sp);
    animacion->addAnimation(ANIM_ANDAR, 300.f,0,1,3,1,24,28);
    animacion->addAnimation(ANIM_ATACAR, 120.f,0,0,3,0,24,28);
}
void Yeti::update(Tiempo dt){
    Enemigo::update(dt);
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        posicionAnterior = posicion;
        //Comprueba si esta cayendo
        if(caer){
          //Desplaza el sprite hacia abajo y le impide atacar
          desplazamiento  += getVelocidad()*dt.segundos();
          desplazamiento  = min(desplazamiento, 24.0f);
          posicion[1]     += desplazamiento;
          disparando      = false;
          contador        = TIEMPO_COMPROB;
        }else if(!disparando){
          //Si no esta disparando se mueve
          if(dir){
            //Desplaza derecha
            desplazamiento = getVelocidad()*dt.segundos();
            animacion->play(ANIM_ANDAR,dt.milisegundos());

            //Se asegura que esta mirando hacia la derecha
            vector<float> escalaActual = sprite->getScale();
            if(escalaActual[0] < 0) {
              sprite->setScale(abs(escalaActual[0]), escalaActual[1]);
            }
          }else{
            //Desplaza izquierda
            desplazamiento = getVelocidad()*dt.segundos()*(-1);
            animacion->play(ANIM_ANDAR,dt.milisegundos());

            //Se asegura que esta mirando hacia la izquierda
            vector<float> escalaActual = sprite->getScale();
            if(escalaActual[0] > 0) {
              sprite->setScale(escalaActual[0]*(-1.0f), escalaActual[1]);
            }
          }
          
          posicion[0]+=desplazamiento;
        }

        //Comprueba las colisiones tras el movimiento y actualiza el sprite
        comprobarColisiones();
        if(contador >= 0) {
          contador -= dt.milisegundos() + tiempoAntes.milisegundos();
        }
        
        if(contador <= 0 && !disparando) {
            EstadoPartida* partida = EstadoPartida::Instancia();
            Jugador* j = partida->getJugador();
            vector<float> posJugador = j->getPosNueva();

            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if(distancia < DIST_ATAQUE) {
                //Orienta el enemigo hacia el jugador
                if(posJugador[0] >= posicion[0])
                  dir = true;
                else
                  dir = false;

                //Actualiza el sprite
                vector<float> escalaActual = sprite->getScale();
                if(dir && escalaActual[0] < 0) {
                  sprite->setScale(abs(escalaActual[0]), escalaActual[1]);
                }
                else if(!dir && escalaActual[0] > 0) {
                  sprite->setScale(escalaActual[0]*(-1.0f), escalaActual[1]);
                }

                //Actualiza su estado para que dispare
                disparando        = true;
                disparosRestantes = CANT_DISPAROS;
                contador          = 1000;
            }
            else {
              contador = TIEMPO_COMPROB;
            }
        }

        if(disparando) {         
          //Reproduce la animacion de disparo
          animacion->play(ANIM_ATACAR, dt.milisegundos());

          //Controla la cadencia
          if(contador <= 0) {
            //Determina una direccion aleatoria
            float angulo =  static_cast<float>(rand() % 121 - 60);
            if(!dir) {
              //Si esta apuntando hacia la izquierda corrige el angulo
              angulo = 180.0 - angulo;
            }
            vector<float> dirDisp = {static_cast<float>(cos(angulo*M_PI/180)), static_cast<float>(sin(angulo*M_PI/180))};

            //Crea el sprite
            EstadoPartida* partida = EstadoPartida::Instancia();
            Sprite* spDisparo = new Sprite(partida->getTexturas(2), 244, 261, 8, 8);
            spDisparo->setScale(2.0, 2.0);

            Proyectil* p = new ProyectilRebota(danyo, 0, spDisparo, posicion, dirDisp, 200, 4, {});
            partida->anyadirProyectil(p);

            //Reduce la cantidad de disparos restantes
            disparosRestantes--;
            contador = TIEMPO_ENTRE_DISP;
          }

          if(disparosRestantes <= 0) {
            //Reinicia el contador del ataque al disparar el ultimo proyectil
            contador    = tiempoRecuperacion;
            disparando  = false;
          }
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
}

Yeti::~Yeti(){
  delete animacion;
  ladoColisiones.clear();
}

//Resetea el enemigo
void Yeti::reset() {
  contador    = TIEMPO_COMPROB;
  disparando  = false;
}

//Se comprueban las colisiones del Goblin
void Yeti::comprobarColisiones(){
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

if(posicion[0]>=0 && posicion[0]/tamTiles<tamMapa[0] && posicion[1]>=0 && posicion[1]/tamTiles<tamMapa[1]) {
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