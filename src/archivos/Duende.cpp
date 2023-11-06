#include "Duende.h"

const float DIST_DETECCION  = 400.0;
const float DIST_ATAQUE     = 900.0;
const int ACEL_MAX_GRAVEDAD = 20;
const int TIEMPO_PERSEC     = 2500;       //Tiempo que persigue al jugador antes de atacar
const int BALAS_RAFAGA      = 4;          //Cantidad de balas por ataque
const float DIST_ATAQUE_DIR = 60.0;       //Distancia a la que el duende ataca directamente

//Constantes para calcular la posicion del duende
const int COLISION_X        = 14;
const int COLISION_Y        = 32;
const int COLISION_INX      = 6;

//Constructor
Duende::Duende(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
  //Inicializa su estado de manera aleatoria
  direccionMov    = rand() % 2;
  desplazamiento  = 0;
  balasRestantes  = BALAS_RAFAGA;
  cayendo         = false;
  ladoColisiones  = {-1, -1, -1};
  sueloIzquierda  = true;
  sueloDerecha    = true;

  estado = EnemigoDuende::DuendeCaminando;
  animacionDuende = new Animacion(sp);
  animacionDuende->addAnimation("DuendeDerecha",75.f,0,1,3,1,24,24);
  animacionDuende->addAnimation("DuendeIzquierda",75.f,4,1,7,1,24,24);
  animacionDuende->addAnimation("DuendeQuietoIzquierda",75.f,4,0,7,0,24,24);
  animacionDuende->addAnimation("DuendeQuietoDerecha",75.f,0,0,3,0,24,24);
}
void Duende::update(Tiempo dt){
  Enemigo::update(dt);
  //Comprueba que no se ha detenido el tiempo
  if(!detenido) {
      posicionAnterior = posicion;
      if(cayendo){
        //Cae, reinicia su estado al hacerlo
        desplazamiento += getVelocidad()*dt.segundos();
        desplazamiento = min(desplazamiento, ACEL_MAX_GRAVEDAD);
        posicion[1]+=desplazamiento;
        estado = DuendeCaminando;
        if(direccionMov) {
          animacionDuende->play("DuendeQuietoDerecha",dt.milisegundos());
        }
        else {
          animacionDuende->play("DuendeQuietoIzquierda",dt.milisegundos());
        }
        
      }
      else {
        Jugador* j = nullptr;
        vector<float> posJugador;
        float distancia = 0.0;
        switch (estado){
          case DuendeCaminando:
            if(direccionMov){
              //Desplaza derecha
              posicion[0] += getVelocidad()*dt.segundos()/2.0;
              animacionDuende->play("DuendeDerecha",dt.milisegundos());
            }else{
              //Desplaza izquierda
              posicion[0] -= getVelocidad()*dt.segundos()/2.0;
              animacionDuende->play("DuendeIzquierda",dt.milisegundos());
            }
            
            //Comprueba si el jugador se ha acercado
            j = EstadoPartida::Instancia()->getJugador();
            posJugador = j->getPosNueva();
            distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if(distancia <= DIST_DETECCION) {
              estado = DuendeEnRango;
              contador = 0;
            }
            break;
          case DuendeEnRango:
            j = EstadoPartida::Instancia()->getJugador();
            posJugador = j->getPosNueva();
            
            if(posJugador[0] >= posicion[0] && sueloDerecha){
              //Desplaza derecha
              posicion[0] += getVelocidad()*dt.segundos();
              animacionDuende->play("DuendeDerecha",dt.milisegundos());
            }else if(posJugador[0] <= posicion[0] && sueloIzquierda){
              //Desplaza izquierda
              posicion[0] -= getVelocidad()*dt.segundos();
              animacionDuende->play("DuendeIzquierda",dt.milisegundos());
            }
            else {
              posicion[0] = posicion[0];
            }
            
            //Comprueba si el jugador se ha alejado
            distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if(distancia > DIST_ATAQUE) {
              estado = DuendeCaminando;
            }
            else {
              if(distancia <= DIST_ATAQUE_DIR) {
                //Si se ha acercado demasiado, el duende ataca directamente
                estado          = DuendeAtacando;
                contador        = 0;
                balasRestantes  = BALAS_RAFAGA;
              }
              else {
                //Suma el tiempo que ha pasado al contador
                contador += dt.milisegundos() + tiempoAntes.milisegundos();

                if(contador >= TIEMPO_PERSEC) {
                  //Al permanecer un tiempo en rango, comienza a atacar
                  estado    = DuendePreparandoAtaque;
                  contador  = 0;
                }
              }
            }
            break;
          case DuendePreparandoAtaque: 
            //Suma el tiempo que ha pasado al contador
            contador += dt.milisegundos() + tiempoAntes.milisegundos();

            //Recoge la posicion del jugador
            j = EstadoPartida::Instancia()->getJugador();
            posJugador = j->getPosNueva();

            if(posJugador[0] >= posicion[0]) {
              animacionDuende->play("DuendeQuietoDerecha",dt.milisegundos());
            }
            else {
              animacionDuende->play("DuendeQuietoIzquierda",dt.milisegundos());
            }

            if(contador >= tiempoRecuperacion/2) {
              //Al pasar el tiempo de preparacion, ataca
              estado          = DuendeAtacando;
              contador        = 0;
              balasRestantes  = BALAS_RAFAGA;
            }
            break;
          case DuendeAtacando:
            //Calcula el tiempo entre balas
            int incremento = tiempoRecuperacion/(BALAS_RAFAGA + 1);

            //Suma el tiempo que ha pasado al contador
            contador += dt.milisegundos() + tiempoAntes.milisegundos();

            //Recoge la posicion del jugador
            j = EstadoPartida::Instancia()->getJugador();
            posJugador = j->getPosNueva();
            if(posJugador[0] >= posicion[0]) {
              animacionDuende->play("DuendeQuietoDerecha",dt.milisegundos());
            }
            else {
              animacionDuende->play("DuendeQuietoIzquierda",dt.milisegundos());
            }
            
            for(int i=BALAS_RAFAGA; i>0; i--) {
              if(i == balasRestantes && contador >= incremento*(5-i)) {
                //Crea un proyectil
                posJugador = j->getPosNueva();
                distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};

                Sprite* spDisparo = new Sprite(EstadoPartida::Instancia()->getTexturas(2), 3, 390, 9, 6);
                spDisparo->setScale(2.0, 2.0);
                Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, dirDisp, 200, 1200, {});
                EstadoPartida::Instancia()->anyadirProyectil(p);

                //Resta una bala a las balas restantes de la rafaga
                balasRestantes--;
              }
            }

            if(contador > tiempoRecuperacion) {
              contador  = 0;
              estado    = DuendeCaminando;
            }
            break;
        }
      }

      //Comprueba las colisiones tras el movimiento
      comprobarColisiones();

      //Reinicia el tiempo antes de detener si es necesario y el reloj
      relojDetenible.restart();
      if(tiempoAntes.milisegundos()>0)
        reiniciarTiempoAntes();
  }    
}

Duende::~Duende(){

}

//Se comprueban las colisiones del duende
void Duende::comprobarColisiones(){
    EstadoPartida *estadoPartida        = EstadoPartida::Instancia();
    map<vector<int>, int>* colisionMap  = estadoPartida->getSalaActual()->getMapaColisiones();
    vector<int> tamMapa                 = estadoPartida->getSalaActual()->getTamanyoMapa();
    int tamTiles                        = estadoPartida->getSalaActual()->getConstTamTiles();

    vector<int> posComprobar = {-1,-1};

    //Id del tile a comprobar
    int idCol         = estadoPartida->getSalaActual()->getIdColision(1);
    int idPlat        = estadoPartida->getSalaActual()->getIdColision(2);
    int idPDerecha    = estadoPartida->getSalaActual()->getIdColision(4);
    int idPIzquierda  = estadoPartida->getSalaActual()->getIdColision(6);

    bool tocandoSuelo   = false;
    sueloIzquierda = false;
    sueloDerecha   = false;

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
      direccionMov=true;
    }

    //Colision derecha
    if(ladoColisiones[0] != 0){
      posicion[0] += ladoColisiones[0];
      direccionMov=false;
    }

    //Colision abajo
    if(tocandoSuelo){
      posicion[1]+=ladoColisiones[1];

      if(cayendo) {
        cayendo = false;
        desplazamiento = 0;
      }
      else {
        if(direccionMov) {
          //Se esta moviendo hacia la derecha
          if(!sueloDerecha) {
            direccionMov = false;
          }
        }
        else {
          //Se esta moviendo hacia la izquierda
          if(!sueloIzquierda) {
            direccionMov = true;
          }
        }
      }
    }else if(!cayendo){
      //Al caer, su estado vuelve a caminar
      cayendo   = true;
      estado    = DuendeCaminando;
      contador  = 0;
      desplazamiento = 0;
    }
  }
}