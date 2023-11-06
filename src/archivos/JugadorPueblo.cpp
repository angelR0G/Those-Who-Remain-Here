#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>

#include "JugadorPueblo.h"

//Constantes valores velocidad
const float VEL_MAX_H       = 240.0;
const float INCRE_VEL       = 80.0;
const float VEL_MAX_V       = 300.0;
const float GRAVEDAD        = 170.0;
const float POTENCIA_SALTO  = 700.0;
const float TIEMPO_SALTO    = 0.42;

//Escala para el perosnaje
const float escala          = 2.5;

//Tamanyo de los inputs
const int TAM_INPUT         = 19;

//Constantes para calcular la posion del jugador en las coordenadas
const int COLISION_X        = 15;
const int COLISION_Y        = 20;
const int COLISION_INX      = 6;
const int COLISION_INY      = 9;

const int SALTO_COYOTE_MAX  = 2;

const int ESPERA_INTER      = 300;

const float DISTANCIA_INTERACCION   = 40.0;       //Distancia a la que se pueden encontrar los objetos interactuables

//Constructor
JugadorPueblo::JugadorPueblo(){
    //Se crea la textura del jugador y se inicializa el sprite
    tex     = new Textura("resources/sprites/personaje/mHero.png");
    sprite  = new Sprite(tex, 0, 0, 14, 18);

    //Se coloca el origen del sprite
    sprite->setOrigin(8.0, 9.0 + 3.0);

    //Se hace el escalado
    sprite->setScale(escala, escala);

    //Se inicializan las variables del jugador
    desaceleracion    = 0.5f;
    salto             = false;
    saltoCoyote       = SALTO_COYOTE_MAX;
    saltando          = false;
    tocaTecho         = false;
    puedeBajar        = false;
    cambiandoSala     = false;
    esperaInteraccion = ESPERA_INTER;
    estadoAnimacion   = JugadorPuebloEstados::QuietoPueblo;
    interactua        = false;
    //Inicializar vectores del jugador
    velocidad.push_back(0);
    velocidad.push_back(0);
    ladoColisiones={-1, -1, -1, -1};
    coordsNuevas = {0, 0};
    coordsAntiguas = {0, 0};

    coordsPuerta = {0, 0};
    //Se asigna la posicion inicial del esprite y las coordenadas del estado
    sprite->setPosition(coordsAntiguas[0], coordsAntiguas[1]);
    //Inicializo los relojes
    relojSalto = new Reloj();
    

    this->animacion = new Animacion(this->sprite);

    this->animacion->addAnimation("Quieto",100.f,0,0,3,0,14,18);
    this->animacion->addAnimation("Derecha",75.f,0,1,3,1,14,18);
    this->animacion->addAnimation("Izquierda",75.f,5,1,8,1,14,18);
    this->animacion->addAnimation("Salto",30.f,0,2,3,2,14,18);

    //Se crean los sonidos para el jugador
    sonidos = new ControladorSonido();
    sonidos->addSonido("salto",new Buffer("resources/sonidos/salto.wav"),40);
    sonidos->addSonido("movimiento",new Buffer("resources/sonidos/Pasos.ogg"),40);
}

JugadorPueblo::~JugadorPueblo(){
  //Se borran los vectores
  velocidad.clear();
  ladoColisiones.clear();
  coordsAntiguas.clear();
  coordsNuevas.clear();

  //Se borran los new
  delete tex;
  delete sprite;
  delete relojSalto;
}

void JugadorPueblo::update(int elapsedTime, bool input[TAM_INPUT]){
  if(!cambiandoSala){
    float dt = elapsedTime/1000.0;
    EstadoPueblo *estado = EstadoPueblo::Instancia();
    coordsAntiguas = coordsNuevas;
    estadoAnimacion = JugadorPuebloEstados::QuietoPueblo;
    //Verifico si se pulsa alguna tecla de movimiento
    if(input[4] && !input[6]){ //A - Izquierda
      //Se calcula el max y min de la velocidad
      estadoAnimacion = JugadorPuebloEstados::IzquierdaPueblo;
      if(velocidad[0]>0){
        velocidad[0]=0.f;
      }else{
        velocidad[0] = max(velocidad[0]-INCRE_VEL, (-1)*VEL_MAX_H);
      }
      sonidos->play("movimiento");
    }

    if(input[6] && !input[4]){ //D - Derecha
      //Se calcula el max y min de la velocidad
      estadoAnimacion = JugadorPuebloEstados::DerechaPueblo;
      if(velocidad[0]<0){
        velocidad[0]=0.f;
      }else{
        velocidad[0] = min(velocidad[0]+INCRE_VEL, VEL_MAX_H);
      }
      sonidos->play("movimiento");
    }
    
    if(!input[4] && !input[6]){
      velocidad[0] *= desaceleracion;
    }

    if(input[5]){ //S - Bajar
      puedeBajar = true;
    }else{
      puedeBajar = false;
    }
    
    if(input[7] || input[8]){ //W o espacio - Salto
      if(salto == true){
        sonidos->play("salto");
        estadoAnimacion = JugadorPuebloEstados::SaltoPueblo;
        resetearSalto();
        saltando = true;
      }
      saltar();

    }else{
      saltando = false;
    }

    if(tocaTecho == true){
      saltando = false;
    }

    //Se aplica la gravedad
    if(!saltando && saltoCoyote<SALTO_COYOTE_MAX){
      velocidad[1] += GRAVEDAD*(relojSalto->getElapsedTime().segundos()+1)*2;
      velocidad[1] = min(velocidad[1], VEL_MAX_V);
    }

    coordsNuevas[0] = coordsAntiguas[0] + velocidad[0]*dt;
    coordsNuevas[1] = coordsAntiguas[1] + velocidad[1]*dt;

    //Comprueba si hay un PNJ cerca para interactuar
    if(input[13] && esperaInteraccion <= 0) {
      float d1 = 0.0;
      PNJ* pnj = estado->getPNJCercano(coordsNuevas, &d1);

      //Si la distancia es demasiado grande, no podra interactuar
      if(d1 > DISTANCIA_INTERACCION)
        d1 = -1.0;

      //Comprueba que distancia dentro del alcance
      if(d1 >= 0) {
        pnj->interactuar();     
        //Actualiza el valor de espera de interaccion para que no se ejecute muchas veces seguidas
        esperaInteraccion = ESPERA_INTER;
      }
    }
    if(esperaInteraccion > 0) {
      esperaInteraccion -= elapsedTime;
    }


    comprobarColisiones();

    if(cambiandoSala) {
      VentanaRender::Instancia()->setTiempoFade(elapsedTime);
    }
    actualizarAnimacion(elapsedTime); 
  }
}

void JugadorPueblo::actualizarAnimacion(float dt){

    if(this->estadoAnimacion == JugadorPuebloEstados::QuietoPueblo){

      this->animacion->play("Quieto",dt);

    }else if(this->estadoAnimacion == JugadorPuebloEstados::DerechaPueblo){
       this->animacion->play("Derecha",dt);

    }else if(this->estadoAnimacion == JugadorPuebloEstados::IzquierdaPueblo){
       this->animacion->play("Izquierda",dt);

    }else if(this->estadoAnimacion == JugadorPuebloEstados::SaltoPueblo){
       this->animacion->play("Salto",dt);

    }
}
//Se comprueban las colisines en las cuatro direcciones del personaje con el mapa, las plataformas y puertas
void JugadorPueblo::comprobarColisiones(){
  EstadoPueblo *estado = EstadoPueblo::Instancia();
  map<vector<int>, int>* colisionMap = estado->getPueblo()->getMapaColisiones();
  vector<int> tamMapa                 = estado->getPueblo()->getTamanyoMapa();
  int tamTiles                        = estado->getPueblo()->getConstTamTiles();
  vector<int> posComprobar = {-1,-1};
  bool tocandoSuelo = false;
  int puerta = -1;
  //Id del tile a comprobar
  int idCol         = estado->getPueblo()->getIdColision(1);
  int idPlat        = estado->getPueblo()->getIdColision(2);
  int idPArriba     = estado->getPueblo()->getIdColision(3);
  int idPDerecha    = estado->getPueblo()->getIdColision(4);
  int idPAbajo      = estado->getPueblo()->getIdColision(5);
  int idPIzquierda  = estado->getPueblo()->getIdColision(6);
  //Comprueba que no se ha salido del mapa
  if(coordsNuevas[0]>=0 && coordsNuevas[0]/32<tamMapa[0] && coordsNuevas[1]>=0 && coordsNuevas[1]/32<tamMapa[1]) {
    //Parte central derecha
    posComprobar[0] = (coordsNuevas[0]+COLISION_X)/tamTiles;
    posComprobar[1] = (coordsNuevas[1]+0)/tamTiles;
    auto tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[0] = 0;
      }else if(tile->second == idCol){
        ladoColisiones[0] = ((int)(coordsNuevas[0]+COLISION_X)%tamTiles)*(-1) +1;
      }else{
        //Parte derecha abajo
        posComprobar[0] = (coordsNuevas[0]+COLISION_X)/tamTiles;
        posComprobar[1] = (coordsNuevas[1]+COLISION_Y-COLISION_INY)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile->second == idCol){
          ladoColisiones[0] = ((int)(coordsNuevas[0]+COLISION_X)%tamTiles)*(-1) +1;
        }else{
          //Parte derecha arriba
          posComprobar[0] = (coordsNuevas[0]+COLISION_X)/tamTiles;
          posComprobar[1] = (coordsNuevas[1]-COLISION_Y+COLISION_INY)/tamTiles;
          tile = colisionMap->find(posComprobar);
          if(tile->second == idCol){
            ladoColisiones[0] = ((int)(coordsNuevas[0]+COLISION_X)%tamTiles)*(-1) +1;
          }else{
            //Comprobar puerta de la derecha
            posComprobar[0] = (coordsNuevas[0]+COLISION_X)/tamTiles;
            posComprobar[1] = (coordsNuevas[1]+0)/tamTiles;
            tile = colisionMap->find(posComprobar);
            if(tile->second == idPDerecha){
              puerta = 1;
            }else{
              ladoColisiones[0] = ((int)(coordsNuevas[0]+COLISION_X)%tamTiles)*(-1) +1;
            }
          }
        }
      }
    }else{
      ladoColisiones[0] = 0;
    }

    //Parte central Abajo
    posComprobar[0] = (coordsNuevas[0]+0)/tamTiles;
    posComprobar[1] = (coordsNuevas[1]+COLISION_Y)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat && coordsAntiguas[1]<coordsNuevas[1] && puedeBajar){
        ladoColisiones[1] = 0;
      }else if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<coordsNuevas[1] && !puedeBajar)){
        ladoColisiones[1] = ((int)(coordsNuevas[1]+COLISION_Y)%tamTiles)*(-1) +1;
      }else{
        //Parte abajo izquierda
        posComprobar[0] = (coordsNuevas[0]-COLISION_X+COLISION_INX)/tamTiles;
        posComprobar[1] = (coordsNuevas[1]+COLISION_Y)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<coordsNuevas[1] && !puedeBajar)){
          ladoColisiones[1] = ((int)(coordsNuevas[1]+COLISION_Y)%tamTiles)*(-1) +1;
        }else{
          //Parte abajo derecha
          posComprobar[0] = (coordsNuevas[0]+COLISION_X-COLISION_INX)/tamTiles;
          posComprobar[1] = (coordsNuevas[1]+COLISION_Y)/tamTiles;
          tile = colisionMap->find(posComprobar);
          if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<coordsNuevas[1] && !puedeBajar)){
            ladoColisiones[1] = ((int)(coordsNuevas[1]+COLISION_Y)%tamTiles)*(-1) +1;
          }else{
            //Comprobar puerta de abajo
            posComprobar[0] = (coordsNuevas[0]+0)/tamTiles;
            posComprobar[1] = (coordsNuevas[1]+COLISION_Y)/tamTiles;
            tile = colisionMap->find(posComprobar);
            if(tile->second == idPAbajo){
              puerta = 2;
            }else{
              ladoColisiones[1] = ((int)(coordsNuevas[1]+COLISION_Y)%tamTiles)*(-1) +1;
            }
          }
        }
      }
    }else{
      //Con las colisiones se comprueba si el personaje esta tocando el suelo
      posComprobar[0] = (coordsNuevas[0]+0)/tamTiles;
      posComprobar[1] = (coordsNuevas[1]+COLISION_Y+COLISION_INY)/tamTiles;
      tile = colisionMap->find(posComprobar);
      if(tile != colisionMap->end()){
        if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<=coordsNuevas[1] && !puedeBajar)){
          tocandoSuelo = true;
        }else{
          //Parte abajo izquierda
          posComprobar[0] = (coordsNuevas[0]-COLISION_X+COLISION_INX)/tamTiles;
          posComprobar[1] = (coordsNuevas[1]+COLISION_Y+COLISION_INY)/tamTiles;
          tile = colisionMap->find(posComprobar);
          if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<=coordsNuevas[1] && !puedeBajar)){
            tocandoSuelo = true;
          }else{
            //Parte abajo derecha
            posComprobar[0] = (coordsNuevas[0]+COLISION_X-COLISION_INX)/tamTiles;
            posComprobar[1] = (coordsNuevas[1]+COLISION_Y+COLISION_INY)/tamTiles;
            tile = colisionMap->find(posComprobar);
            if(tile->second == idCol || (tile->second == idPlat && coordsAntiguas[1]<=coordsNuevas[1] && !puedeBajar)){
              tocandoSuelo = true;
            }
          }
        }
      }
      ladoColisiones[1] = 0;
    }

    //Parte central izquierda
    posComprobar[0] = (coordsNuevas[0]-COLISION_X)/tamTiles;
    posComprobar[1] = (coordsNuevas[1]+0)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[2] = 0;
      }else if(tile->second == idCol){
        ladoColisiones[2] = ((int)(coordsNuevas[0]-COLISION_X)%tamTiles) - 1;
      }else{
        //Parte izquierda abajo
        posComprobar[0] = (coordsNuevas[0]-COLISION_X)/tamTiles;
        posComprobar[1] = (coordsNuevas[1]+COLISION_Y-COLISION_INY)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile->second == idCol){//Colision derecha
          ladoColisiones[2] = ((int)(coordsNuevas[0]-COLISION_X)%tamTiles) - 1;
        }else{
          //Parte izquierda arriba
          posComprobar[0] = (coordsNuevas[0]-COLISION_X)/tamTiles;
          posComprobar[1] = (coordsNuevas[1]-COLISION_Y+COLISION_INY)/tamTiles;
          tile = colisionMap->find(posComprobar);
          if(tile->second == idCol){
            ladoColisiones[2] = ((int)(coordsNuevas[0]-COLISION_X)%tamTiles) - 1;
          }else{
            //Comprobar puerta de la izquierda
            posComprobar[0] = (coordsNuevas[0]-COLISION_X)/tamTiles;
            posComprobar[1] = (coordsNuevas[1]+0)/tamTiles;
            tile = colisionMap->find(posComprobar);
            if(tile->second == idPIzquierda){
              puerta = 3;
            }else{
              ladoColisiones[2] = ((int)(coordsNuevas[0]-COLISION_X)%tamTiles) - 1;
            }
          }
        }
      }
    }else{
      ladoColisiones[2] = 0;
    }

    //Parte central arriba
    posComprobar[0] = (coordsNuevas[0]-0)/tamTiles;
    posComprobar[1] = (coordsNuevas[1]-COLISION_Y)/tamTiles;
    tile = colisionMap->find(posComprobar);
    if(tile != colisionMap->end()){
      if(tile->second == idPlat){
        ladoColisiones[3] = 0;
      }else if(tile->second == idCol){
        ladoColisiones[3] = ((int)(coordsNuevas[1]-COLISION_Y)%tamTiles) - 1;
      }else{
        //Parte arriba a la izquierda
        posComprobar[0] = (coordsNuevas[0]-COLISION_X+COLISION_INX)/tamTiles;
        posComprobar[1] = (coordsNuevas[1]-COLISION_Y)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile->second == idCol){
          ladoColisiones[3] = ((int)(coordsNuevas[1]-COLISION_Y)%tamTiles) - 1;
        }else{
          //Parte arriba a la derecha
          posComprobar[0] = (coordsNuevas[0]+COLISION_X-COLISION_INX)/tamTiles;
          posComprobar[1] = (coordsNuevas[1]-COLISION_Y)/tamTiles;
          tile = colisionMap->find(posComprobar);
          if(tile->second == idCol){
            ladoColisiones[3] = ((int)(coordsNuevas[1]-COLISION_Y)%tamTiles) - 1;
          }else{
            //Comprobar puerta de arriba
            posComprobar[0] = (coordsNuevas[0]+0)/tamTiles;
            posComprobar[1] = (coordsNuevas[1]-COLISION_Y)/tamTiles;
            tile = colisionMap->find(posComprobar);
            if(tile->second == idPArriba){
              puerta = 0;
            }else{
              ladoColisiones[3] = ((int)(coordsNuevas[1]-COLISION_Y)%tamTiles) - 1;
            }
          }
        }
      }
    }else{
      ladoColisiones[3] = 0;
    }

    //Por si el pesonaje se sale del mapa aparece en el punto de aparicion de la puerta por la que ha entrado en la sala
    if(ladoColisiones[0] == -1 && ladoColisiones[1] == -1 && ladoColisiones[2] == -1 && ladoColisiones[3] == -1){
      coordsNuevas = coordsPuerta;
      coordsAntiguas = coordsPuerta;
    }
    //Colision izquierda
    if(ladoColisiones[2] != 0){
      velocidad[0] = 0;
      coordsNuevas[0] += (tamTiles-ladoColisiones[2]);
    }

    //Colision derecha
    if(ladoColisiones[0] != 0){
      velocidad[0] = 0;
      coordsNuevas[0] += ladoColisiones[0];
    }

    //Colision arriba
    if(ladoColisiones[3] != 0){
      tocaTecho = true;
      
      velocidad[1] = 0;
      coordsNuevas[1] += (tamTiles-ladoColisiones[3]);
    }else {
      tocaTecho = false;
    }
    
    //Colision abajo
    if(ladoColisiones[1] != 0){
      if(velocidad[1] >= 0){
        salto       = true;
        saltoCoyote = SALTO_COYOTE_MAX;
      }

      velocidad[1] = 0;
      coordsNuevas[1]+=ladoColisiones[1];

    }else if(!tocandoSuelo){
      if(saltoCoyote>0){
        saltoCoyote--;
      }else{
        salto = false;
      }
      //salto = false;
    }

    //Con las colisiones recogidas se escoge que puerta ha tocado y se cambia la sala
    //Se asignan las coordenadas del jugador a las del punto de aparicion de la puerta correspondiente
    if(puerta != -1){
      VentanaRender *ventana = VentanaRender::Instancia();
      ventana->fadeIn(25); 
      estado->setEntrandoMazmorra(true);
      cambiandoSala = true;
    }
    else {
      cambiandoSala = false;
    }
  }else {
    coordsNuevas    = coordsPuerta;
    coordsAntiguas  = coordsPuerta;
    velocidad       = {0.0, 0.0};
  }
  
}

//Dibuja al jugador
void JugadorPueblo::dibujar(float percentTick){
    if(!interactua){
        float coordX = coordsAntiguas[0]*(1-percentTick) + coordsNuevas[0]*percentTick;
        float coordY = coordsAntiguas[1]*(1-percentTick) + coordsNuevas[1]*percentTick;

        sprite->setPosition(coordX, coordY);
    }
    sprite->draw();
}

//Resetear el reloj que controla el salto del personaje
void JugadorPueblo::resetearSalto(){
    relojSalto->restart();
}

//Metodo de salto
void JugadorPueblo::saltar(){
  if(saltando==true && relojSalto->getElapsedTime().segundos()<TIEMPO_SALTO){

    velocidad[1] = (-1)*POTENCIA_SALTO*(1-(relojSalto->getElapsedTime().segundos()/TIEMPO_SALTO));
    saltoCoyote = 0;
  }
  else {
    saltando = false;
  }
}

//Devuelve si el jugador puede bajar por las plataformas
int JugadorPueblo::getPuedeBajar(){
  return puedeBajar;
}

//Devuelve la posicion nueva del jugador
vector<float> JugadorPueblo::getPosNueva(){
  return coordsNuevas;
}

//Devuelve la referencia al sprite del jugador
Sprite* JugadorPueblo::getSprite(){
  return sprite;
}

void JugadorPueblo::setPosicion(vector<float> pos, bool antiguas){
  coordsNuevas = pos;
  if(antiguas){
    coordsAntiguas = pos;
  }
  //Actualiza el sprite
  sprite->setPosition(pos[0], pos[1]);
}

void JugadorPueblo::setSpawn(vector<float> spawn){
  coordsPuerta = spawn;
}

void JugadorPueblo::setInteracutando(bool b){
    interactua = b;
}