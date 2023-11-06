#include "Enemigo.h"

const int TIEMPO_SANGRADO   = 200;

const int QUEMADO_LEVE          = 2;
const int QUEMADO_GRAVE         = 4;
const int CONGELADO_LEVE        = 20;
const int CONGELADO_GRAVE       = 40;
const int ELECTROCUTADO_LEVE    = 10;
const int ELECTROCUTADO_GRAVE   = 25;
const int TIEMPO_DEFECTO_ELEM   = 10000;
const int PROB_SOLTAR_MATERIAL  = 40;

//Constructor
Enemigo::Enemigo(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> mat,Textura* t, Sprite* sp){

  //Texturas y Sprite del enemigo
  tex = t;
  sprite = sp;

  //Inicializar variables
  this->danyo               = danyo;
  this->vida                = vida;
  this->velocidad           = velocidad;
  this->tiempoRecuperacion  = tiempoRecuperacion;
  esenciasDerrotado         = esencias;
  idMatRecompensa           = mat;

  muerto = false;
  sangrando = 0;

  electrocutado   = {0, 0};
  quemado         = {0, 0};
  congelado       = {0, 0};
  tiempoArdiendo  = 0;
  contadorPart    = 0;
  if(sprite != nullptr) {
    colorSprite   = sprite->getColor();
  }
  else {
    colorSprite   = {255, 255, 255, 255};
  }

  posicion = {-1,-1};
  posicionAnterior = {-1,-1};

  inventario = nullptr;
  sonidos = new ControladorSonido();
}

Enemigo::~Enemigo(){

  delete tex;
  delete sprite;
  inventario = nullptr;
}

//Resta vida al enemigo al recibir un impacto
void Enemigo::perderVida(int danyo){
    vida-=danyo;

    //Si esta electrocutado recibe mas danyo
    if(electrocutado[1] > 0) {
      vida -= danyo*(electrocutado[0]/100.0);
    }

    if(vida <= 0) {
      muerte();
      muerto = true;
    }

    sangrando = TIEMPO_SANGRADO;
}

void Enemigo::dibujar(float percent, int update_milisegundos){
  //Calcula las coordenadas interpoladas
  float coordX = posicion[0];
  float coordY = posicion[1];

  if(!detenido) {
    coordX = posicionAnterior[0]*(1-percent) + posicion[0]*percent;
    coordY = posicionAnterior[1]*(1-percent) + posicion[1]*percent;
  }
  else {
    percent = (float)tiempoAntes.milisegundos()/update_milisegundos;
    coordX = posicionAnterior[0]*(1-percent) + posicion[0]*percent;
    coordY = posicionAnterior[1]*(1-percent) + posicion[1]*percent;
  }
  sprite->setPosition(coordX, coordY);
  sprite->draw();
}

//Actualiza el enemigo
void Enemigo::update(Tiempo t) {
  if(!detenido) {
    bool afectadoPorEstado = false;

    if(sangrando > 0) {
      //Si ha recibido danyo, se dibuja mas rojizo
      sprite->setColor({255, 50, 50, colorSprite[3]});
      sangrando -= t.milisegundos();

      if(sangrando <= 0) {
        sprite->setColor(colorSprite);
      }
    }

    if(quemado[1] > 0) {
      //Si esta quemado recibe danyo cada segundo
      tiempoArdiendo += t.milisegundos();
      if(tiempoArdiendo >= 1000) {
        tiempoArdiendo -= 1000;
        perderVida(quemado[0]);
      }
    }

    //Actualiza todos los estados elementales
    if(quemado[1] > 0) {
      afectadoPorEstado = true;
      quemado[1] -= t.milisegundos();
      if(quemado[1] <= 0) {
        quemado[1] = 0;
        tiempoArdiendo = 0;
      }
    }
    if(congelado[1] > 0) {
      afectadoPorEstado = true;
      congelado[1] -= t.milisegundos();
      if(congelado[1] < 0) {
        congelado[1] = 0;
      }
    }
    if(electrocutado[1] > 0) {
      afectadoPorEstado = true;
      electrocutado[1] -= t.milisegundos();
      if(electrocutado[1] < 0) {
        electrocutado[1] = 0;
      }
    }

    if(afectadoPorEstado) {
      //Genera particulas de los estados correspondientes
      contadorPart += t.milisegundos();

      if(contadorPart >= 400) {
        contadorPart -= 400;

        if(quemado[1] > 0) {          //Particulas quemado
          new ParticulaSimpleCuadrada(sprite->getPosition(), {255, 120, 45, 255}, 40, 800);
          new ParticulaSimpleCuadrada(sprite->getPosition(), {255, 56, 24, 255}, 40, 800);
        }
        if(congelado[1] > 0) {        //Particulas congelado
          new ParticulaSimpleCuadrada(sprite->getPosition(), {137, 207, 240, 255}, 40, 800);
          new ParticulaSimpleCuadrada(sprite->getPosition(), {255, 255, 255, 255}, 40, 800);
        }
        if(electrocutado[1] > 0) {    //Particulas electrocutado
          new ParticulaSimpleCuadrada(sprite->getPosition(), {255, 228, 16, 255}, 40, 800);
          new ParticulaSimpleCuadrada(sprite->getPosition(), {255, 238, 108, 255}, 40, 800);
        }
      }
    }
  }
}

void Enemigo::muerte(){
  Guardado* guardado = Guardado::Instancia();

  //Guarda las esencias que contiene al ser derrotado
  guardado->setEsencias(guardado->getEsencias() + esenciasDerrotado);

  for(int i=0; i<esenciasDerrotado; i+=50) {
    new ParticulaEsencia(posicion, {255, 255, 255, 255}, 500);
    new ParticulaEsencia(posicion, {255, 255, 255, 255}, 400);
    new ParticulaEsencia(posicion, {255, 255, 255, 255}, 300);
  }
}

Sprite* Enemigo::getSprite(){
  return sprite;
}

void Enemigo::setPosicion(vector<float> pos){
    posicion = pos;
    posicionAnterior = pos;
}

//Devuelve si el enemigo esta muerto
bool Enemigo::getMuerto() {
  return muerto;
}

//Resetea los relojes para el ataque y el movimiento
void Enemigo::reset() {
  reiniciarTiempoAntes();
  relojDetenible.restart();
}

//Recupera la velocidad del enemigo, teniendo en cuenta si esta congelado
int Enemigo::getVelocidad() {
  int vel = velocidad;
  if(congelado[1] > 0) {
    vel *= ((100-congelado[0])/100.0);
  }
  return vel;
}

/*Quema al enemigo indicando si son quemaduras graves (mas danyo) y el tiempo que permanecera quemado.
*Si recibe un numero menor que 0 se asignara un tiempo por defecto.
*/
void Enemigo::quemar(bool grave, int tiempo) {
  //Asigna un valor dependiendo si es grave o no
  if(grave) {
    quemado[0] = QUEMADO_GRAVE;
  }
  else {
    quemado[0] = QUEMADO_LEVE;
  }

  //Si el tiempo es superior a 0 lo asigna, sino asigna uno por defecto
  if(tiempo > 0) {
    quemado[1] = tiempo;
  }
  else {
    quemado[1] = TIEMPO_DEFECTO_ELEM;
  }
}

/*Congela al enemigo indicando si es grave (mas ralentizacion) y el tiempo que permanecera congelado.
*Si recibe un numero menor que 0 se asignara un tiempo por defecto.
*/
void Enemigo::congelar(bool grave, int tiempo) {
  //Asigna un valor dependiendo si es grave o no
  if(grave) {
    congelado[0] = CONGELADO_GRAVE;
  }
  else {
    congelado[0] = CONGELADO_LEVE;
  }

  //Si el tiempo es superior a 0 lo asigna, sino asigna uno por defecto
  if(tiempo > 0) {
    congelado[1] = tiempo;
  }
  else {
    congelado[1] = TIEMPO_DEFECTO_ELEM;
  }
}

/*Electrocuta al enemigo indicando si es grave (mas danyo recibido) y el tiempo que permanecera electrocutado.
*Si recibe un numero menor que 0 se asignara un tiempo por defecto.
*/
void Enemigo::electrocutar(bool grave, int tiempo) {
  //Asigna un valor dependiendo si es grave o no
  if(grave) {
    electrocutado[0] = ELECTROCUTADO_GRAVE;
  }
  else {
    electrocutado[0] = ELECTROCUTADO_LEVE;
  }

  //Si el tiempo es superior a 0 lo asigna, sino asigna uno por defecto
  if(tiempo > 0) {
    electrocutado[1] = tiempo;
  }
  else {
    electrocutado[1] = TIEMPO_DEFECTO_ELEM;
  }
}

//Actualiza la referencia al inventario del jugador
void Enemigo::setInventario(Inventario* i) {
  inventario = i;
}

/*Devuelve aleatoriamente la id de un material. Existe la posibilidad de que no devuelve
*ningun material, en cuyo caso devolvera -1.
*/
int Enemigo::getMaterialRecompensa() {
  int id = -1;
  int random = rand() % 100;
  int prob = PROB_SOLTAR_MATERIAL;

  //Si el jugador tiene el objeto que aumenta la probabilidad, se suma
  if(inventario != nullptr && inventario->poseeObjeto(5)) {
    prob += 20;
  }

  if(random < prob && idMatRecompensa.size() > 0) {
    //Se elige aleatoriamente una id de las disponibles
    random = rand() % idMatRecompensa.size();
    id = idMatRecompensa[random];
  }

  return id;
}

//Recupera la posicion del enemigo
vector<float> Enemigo::getPosicion() {
  return posicion;
}

//Comprueba si el inventario que referencia el enemigo posee un objeto con la id indicada
bool Enemigo::inventarioPoseeObjeto(int id) {
  bool posee = false;

  if(inventario != nullptr) {
    posee = inventario->poseeObjeto(id);
  }

  return posee;
}