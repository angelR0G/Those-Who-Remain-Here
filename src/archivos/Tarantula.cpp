#include "Tarantula.h"
const float VEL_MAX_V       = 50;
const float VEL_MAX_Y       = 15;



const float DIST_DETECCION  = 500.0;
const float DIST_ATAQUE     = 900.0;

//Constantes para calcular la posicion del tarantula
const int COLISION_X        = 14;
const int COLISION_Y        = 32;
const int COLISION_INX      = 6;

//Constructor
Tarantula::Tarantula(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
    dir = rand() % 2;
    cont=0;
    desplazamiento = 0;
    contador = 0;
    caer = false;
    ladoColisiones={-1, -1, -1};

    enRango = false;
    relojSalto =new Reloj();
    animeTarantula = new Animacion(sp);
   /* animeTarantula->addAnimation("TarantulaDerecha",75.f,0,2,5,2,32,32);
    animeTarantula->addAnimation("TarantulaIzquierda",75.f,0,1,5,1,32,32);*/
    animeTarantula->addAnimation("TarantulaIzquierda",75.f,0,0,3,0,16,12);
    animeTarantula->addAnimation("TarantulaDerecha",75.f,0,1,3,1,16,12);

    animeTarantula->addAnimation("TarantulaSIzquierda",75.f,3,0,3,0,16,12);
    animeTarantula->addAnimation("TarantulaSDerecha",75.f,3,1,3,1,16,12);
}
void Tarantula::update(Tiempo dt){
    Enemigo::update(dt);
    EstadoPartida* partida = EstadoPartida::Instancia();
            Jugador* j = partida->getJugador();
    if(j->getSprite()->intersect(sprite)) {
                  //Danya al jugador y elimina el proyectil
                 j->danyarVida(1);
      
             }   
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
      posicionAnterior = posicion;
        //Cancelar posibilidad de caer
        if(caer){

           if(dir){
            //Desplaza derecha
          

            desplazamiento = getVelocidad()*dt.segundos();
            animeTarantula->playOnce("TarantulaSDerecha",dt.milisegundos());
          }else{
            //Desplaza izquierda
            desplazamiento = getVelocidad()*dt.segundos()*(-1);
            animeTarantula->playOnce("TarantulaSIzquierda",dt.milisegundos());
          }
          
          posicion[0]+=desplazamiento;
          
          caida=VEL_MAX_V*dt.segundos();
     
         despla+=caida;
         if(despla>VEL_MAX_Y){
           despla=VEL_MAX_Y;
         }
         posicion[1]+=despla;
        }else{
          if(cont>0.5){
            cont=0;
            saltar(dt.segundos());
            relojSalto->restart();
          }             
          cont+=dt.segundos();

          

          if(dir){
            //Desplaza derecha
            desplazamiento = getVelocidad()*dt.segundos();
            animeTarantula->play("TarantulaDerecha",dt.milisegundos());
          }else{
            //Desplaza izquierda
            desplazamiento = getVelocidad()*dt.segundos()*(-1);
            animeTarantula->play("TarantulaIzquierda",dt.milisegundos());
          }
          
          posicion[0]+=desplazamiento;
          posicion[1]+=despla;

        }


        //Comprueba las colisiones tras el movimiento y actualiza el sprite
        comprobarColisiones();
        if(contador <= tiempoRecuperacion) {
          contador += dt.milisegundos() + tiempoAntes.milisegundos();
        }
        
        if(contador > tiempoRecuperacion) {
            
            vector<float> posJugador = j->getPosNueva();

            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if((!enRango && distancia <= DIST_DETECCION) || (enRango && distancia <= DIST_ATAQUE)) {
                enRango = true;

                int des = 0;
                if(dir){
                  //Desplaza derecha
                  des -= getVelocidad()*dt.segundos();
                }else{
                  //Desplaza izquierda
                  des += getVelocidad()*dt.segundos();
                }

                //Reinicia el contador del ataque
                contador -= tiempoRecuperacion;
                
                //Si esta cerca crea un proyectil
                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                float angulo = atan2(dirDisp[1],dirDisp[0]);
                angulo = angulo*180/M_PI;
                Sprite* spDisparo = new Sprite(partida->getTexturas(2), 16*29 + 0, 16*1 + 1, 16, 16);
                //spDisparo->setRotation(angulo);
                spDisparo->setScale(2,2);

                Proyectil* p = new ProyectilBoomerang(danyo, 0, spDisparo, posicion, dirDisp, 250, {},10);
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

Tarantula::~Tarantula(){
  delete animeTarantula;
  delete relojSalto;
}

//Se comprueban las colisiones del 
void Tarantula::comprobarColisiones(){
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
    int idPArriba  = estado->getSalaActual()->getIdColision(3);
    int idPAbajo  = estado->getSalaActual()->getIdColision(5);


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
    //Parte central Arriba
    posComprobar[0] = (posicion[0]+0)/tamTiles;
    posComprobar[1] = (posicion[1]-COLISION_Y)/tamTiles;
     tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[3] = 0;
      }else if(tile->second == idCol || tile->second == idPArriba){
        ladoColisiones[3] = ((int)(posicion[1]-COLISION_Y)%tamTiles)+1;
      }else if(tile->second == idCol || tile->second == idPArriba){
        ladoColisiones[3] = ((int)(posicion[1]-COLISION_Y)%tamTiles)*(-1)+1;

      }
    }else{
      ladoColisiones[3] = 0;
    }


    //Parte central Abajo
    posComprobar[0] = (posicion[0]+0)/tamTiles;
    posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idCol || tile->second == idPlat|| tile->second == idPAbajo){
        ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
        tocandoSuelo = true;
      }
      //Parte abajo izquierda
      posComprobar[0] = (posicion[0]-COLISION_X+COLISION_INX)/tamTiles;
      posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
      tile = colisionMap->find(posComprobar);
      if(tile->second == idCol || tile->second == idPlat|| tile->second == idPAbajo){
        ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
        sueloIzquierda = true;
        tocandoSuelo = true;
      }
      //Parte abajo derecha
      posComprobar[0] = (posicion[0]+COLISION_X-COLISION_INX)/tamTiles;
      posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
      tile = colisionMap->find(posComprobar);
      if(tile->second == idCol || tile->second == idPlat|| tile->second == idPAbajo){
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
  if(ladoColisiones[3] != 0){
      posicion[1] += (tamTiles-ladoColisiones[3])/2;
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
void Tarantula::saltar(float dt){

    despla= -400*dt;
  
 
}