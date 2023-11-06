#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>

#include "Jugador.h"
#include "ParticulaSimpleCuadrada.h"

//Constantes valores velocidad
const float VEL_MAX_H       = 240.0;
const float INCRE_VEL       = 80.0;
const float VEL_MAX_V       = 300.0;
const float GRAVEDAD        = 170.0;
const float POTENCIA_SALTO  = 730.0;
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

const int SALTO_COYOTE_MAX  = 3;

//Otras constantes
const float ROBO_VIDA_MAX   = 135.0;
const int COOLDOWN_TIEMPO   = 1000;
const int CARGA_MIN_TIEMPO  = 500;                //Carga minima necesaria para poder activar el poder
const int VEL_CARGA_TIEMPO  = 100;                //Velocidad de carga del poder
const int ESPERA_INTER      = 300;                //Tiempo de reposo entre las pulsaciones de la tecla E o TAB
const int TIEMPO_DANYO      = 200;                //Tiempo que el personaje permanece rojo al recibir danyo
const int DANYO_MAZA        = 80;
const float DISTANCIA_RECOGER       = 40.0;       //Distancia a la que se pueden recoger los objetos del mapa
const float DISTANCIA_ATEMPORAL     = 150.0;
const int INVULNERABILIDAD_BASE     = 1000;       //Tiempo de invulnerabilidad al recibir danyo
const float FRACCION_ROBO_VIDA      = 15.0;       //Divide el danyo que inflinge para el robo de vida
const int PROB_RECUPERAR_MAS_VIDA   = 20;
const string ANIM_MUERTE            = "JugadorMuriendo";
const string RUTA_TEX_MAZA          = "resources/sprites/personaje/Maza.png";
const int TIEMPO_REC_MAZA           = 1000;
const int TIEMPO_USO_MAZA           = 600;

//Constructor
Jugador::Jugador(){
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
    tiempoDetenido    = false;
    muerto            = false;
    linterna          = false;
    maza              = false;
    mazaActivada      = false;
    zoom              = false;
    zoommantenido     = false;
    recuperacionMaza  = 0;
    invulnerable      = 0;
    contadorPart      = 0;
    ralentizacion     = 0;
    esperaInteraccion = ESPERA_INTER;
    estadoAnimacion   = JugadorEstados::Quieto;

    //Inicializar vectores del jugador
    velocidad         = {0.0, 0.0};
    ladoColisiones    = {-1, -1, -1, -1};
    coordsNuevas      = {0, 0};
    coordsAntiguas    = {0, 0};
    coordsPortal      = {0,0};
    coordsPuerta      = {0.0, 0.0};
    posDetenerTiempo  = {-1.0, -1.0};
    posMaza           = {0, 0};
    dirMaza           = {0, 0};

    //Se asigna la posicion inicial del esprite y las coordenadas del estado
    sprite->setPosition(coordsAntiguas[0], coordsAntiguas[1]);

    //Inicializo los relojes
    relojSalto = new Reloj();
    relojHabilidad = new Reloj();

    vidaMax         = 2;
    vida            = vidaMax*2;
    escudo          = 0;
    roboVida        = 0.0;
    cargaTiempo     = 5000;
    cargaTiempoMax  = 5000;
    cargaTiempoVel  = VEL_CARGA_TIEMPO;
    inmuneGolpes    = 0;
    
    //Se recupera la referencia a arma
    FabricaArmas *fabricaArma = FabricaArmas::Instancia();
    arma = fabricaArma->fabricarArma(0);

    disparado = false;
    this->animacion = new Animacion(this->sprite);

    this->animacion->addAnimation("Quieto",100.f,0,0,3,0,14,18);
    this->animacion->addAnimation("Derecha",75.f,0,1,3,1,14,18);
    this->animacion->addAnimation("Izquierda",75.f,5,1,8,1,14,18);
    this->animacion->addAnimation("Salto",30.f,0,2,3,2,14,18);
    this->animacion->addAnimation(ANIM_MUERTE,50.f,0,4,3,5,14,18);

    //Creamos inventario
    inventario = new Inventario(&arma);

    
    timer    = 100;
    moveVec.push_back(0);
    moveVec.push_back(0);
    disparo=false;
    moverGanchobool=false;
    gancho = new Gancho();

    sonidos = new ControladorSonido();
    sonidos->addSonido("salto",new Buffer("resources/sonidos/salto.wav"),40);
    sonidos->addSonido("movimiento",new Buffer("resources/sonidos/Pasos.ogg"),40);

    //Crea el sprite de la maza
    texMaza     = new Textura(RUTA_TEX_MAZA);
    spriteMaza  = new Sprite(texMaza, 0, 0, 42, 71);
    spriteMaza->setOrigin(21.0, 70.0);
}

Jugador::~Jugador(){
  //Se borran los vectores
  velocidad.clear();
  ladoColisiones.clear();
  coordsAntiguas.clear();
  coordsNuevas.clear();
  posMaza.clear();
  dirMaza.clear();

  //Se borran los new
  delete tex;
  delete sprite;
  delete relojSalto;
  delete relojHabilidad;
  delete arma;
  delete animacion;
  delete inventario;
  delete spriteMaza;
  delete texMaza;
  delete sonidos;
}

void Jugador::update(int elapsedTime, bool input[TAM_INPUT]){
  float dt = elapsedTime/1000.0;
  EstadoPartida *estado = EstadoPartida::Instancia();
  coordsAntiguas = coordsNuevas;
  estadoAnimacion = JugadorEstados::Quieto;

  if(muerto) {
    //Update cuando el jugador esta muerto
    updateMuerto(elapsedTime);
  }
  else {
    //Verifico si se pulsa alguna tecla de movimiento
    if(input[4] && !input[6]&&!moverGanchobool&&!(getDisparo()&&!getGancho()->getVuelta())){ //A - Izquierda
      //Se calcula el max y min de la velocidad
      estadoAnimacion = JugadorEstados::Izquierda;
      if(velocidad[0]>0){
        velocidad[0]=0.f;
      }else {
        velocidad[0] = max(velocidad[0]-INCRE_VEL, (-1)*getVelMax(0));
      }
      sonidos->play("movimiento");
    }

    if(input[6] && !input[4]&&!moverGanchobool&&!(getDisparo()&&!getGancho()->getVuelta())){ //D - Derecha
      //Se calcula el max y min de la velocidad
      estadoAnimacion = JugadorEstados::Derecha;
      if(velocidad[0]<0){
        velocidad[0]=0.f;
      }else{
        velocidad[0] = min(velocidad[0]+INCRE_VEL, getVelMax(0));
      }
      if(salto){
        sonidos->play("movimiento");
      }
      
    }
    
    if(!input[4] && !input[6]){
      velocidad[0] *= desaceleracion;
    }

    if(input[5] && !moverGanchobool){ //S - Bajar
      puedeBajar = true;
    }else if(!(disparo&&gancho->getColision())){
      puedeBajar = false;
    }
    
    if((input[7] || input[8]) && !moverGanchobool){ //W o espacio - Salto
      if(salto == true){
        estadoAnimacion = JugadorEstados::Salto;
        resetearSalto();
        saltando = true;
        sonidos->play("salto");

      }
      saltar();

    }else{
      saltando = false;
    }

    if(tocaTecho == true){
      saltando = false;
    }

    //Se aplica la gravedad
    if(!saltando && saltoCoyote<SALTO_COYOTE_MAX && !moverGanchobool && !(getDisparo() && !getGancho()->getVuelta())){
      velocidad[1] += GRAVEDAD*(relojSalto->getElapsedTime().segundos()+1)*2;
      velocidad[1] = min(velocidad[1], VEL_MAX_V);
    }

    if((getDisparo()&&!getGancho()->getVuelta())&&!moverGanchobool){
      velocidad[1] = 0;
      velocidad[0] = 0;

    }

    coordsNuevas[0] = coordsAntiguas[0] + velocidad[0]*dt;
    coordsNuevas[1] = coordsAntiguas[1] + velocidad[1]*dt;
    comprobarColisiones();

    if(cambiandoSala) {
      VentanaRender::Instancia()->setTiempoFade(elapsedTime);
    }

    //Recarga del arma cuando se pulsa la R
    if(input[14]){//R
      cooldown = 0;
      //arma->coolDownRecarga();
      arma->vaciarCargador();
    }
    //Actualiza el cooldown del arma
    arma->updateCoolDown(elapsedTime);
    if(!arma->puedeDisparar()){
      arma->coolDownRecarga(elapsedTime);
    }

    //Si el arma es una escopeta y tiene equipada la runa de disparar dos cartuchos
    if(arma->getRunasPequenyas()[4] || arma->getRunasGrandes()[4]){
      if(dynamic_cast<Escopeta*>(arma)){
        if(disparado && cooldown>=1.0f){
          //Pone el cooldown de disparo a la cadencia para poder disparar
          arma->setCooldown(true);
          arma->disparar(elapsedTime);
          //Resetea los cooldowns
          cooldown = 0;
          arma->setCooldown(false);
          disparado = false;
        }else if(disparado){
          //Aumenta el cooldown de la segunda bala
          cooldown +=1;
        }
      }
    }
    
    //Cuando se pulsa el boton dispara
    if(input[16] && !moverGanchobool){//Boton izq raton
      //Si se cumple que se puede volver a atacar se actualiza que ha disparado
      if(arma->getCooldown() >= arma->getCadencia()){
        disparado = true;
      }
      
      arma->disparar(elapsedTime);

      if(tiempoDetenido) {
        //Al disparar reanuda el tiempo
        reanudarTiempo();
      }
    }

    //Pulsa el boton de la maza
    if(input[12] && maza && recuperacionMaza == 0) {
      //Si ha conseguido la maza y ha pasado el tiempo de recuperacion la usa
      mazaActivada      = true;
      recuperacionMaza  = TIEMPO_USO_MAZA;
      rotacionMaza =0;
      
      if(tiempoDetenido) {
        //Al usar la maza reanuda el tiempo
        reanudarTiempo();
      }

      //Calcula la direccion segun la posicion del raton
      VentanaRender* ventana = VentanaRender::Instancia();
      vector<float> posRaton = ventana->mapPixelToCoords(ventana->getPosicionRaton());
      
      if(posRaton[0]-coordsNuevas[0]<0){
          dirMazamouse=-1;
          dirMaza     = {28,-48};
      }else{
        dirMazamouse=1;
        dirMaza     = {0,-48};
      }
    }
    //Si ha usado la maza, actualiza el tiempo de recuperacion para poder usarla de nuevo
    if(recuperacionMaza > 0) {
      recuperacionMaza = max(0, recuperacionMaza-elapsedTime);
    }
    actualizarMaza();

    timer+=dt;

  if(input[17] && !disparo && gancho->getCooldown()<timer){//Boton izq raton
    Guardado* save = Guardado::Instancia();
    if(save->getGancho()){
      timer   = 0;
      gancho->crear();
      
      // gancho= new Gancho();
      disparo =true;
    }
    

    }

    if(disparo){
      moverGanchobool=gancho->getColision();
      }else if(moverGanchobool){
        moverGanchobool=false;
      }

    //Input traer objetos de otra linea temporal
    if(input[18]) {
      recuperarObjetoAtemporal();
    }

    //Z, Hacer zoom
    if(input[19]&&!zoommantenido){
      Vista *v= EstadoPartida::Instancia()->getVista();
      if(zoom){
        v->setZoom(1.0f);
        zoom = false;
      }else{
        v->setZoom(0.5f);
        zoom = true;
      }
      zoommantenido = true;
    }else if(!input[19]){
      zoommantenido = false;
    }
    
    //Input para abrir el menu
    if(input[9] && esperaInteraccion <= 0){
      Particula::eliminarParticulas();
      estado->cambiarEstado(3);
      esperaInteraccion = ESPERA_INTER;
      if(zoom){
        Vista *v= EstadoPartida::Instancia()->getVista();
        v->setZoom(1.0f);
        zoom = false;
      }
    }

    //Input para abrir el inventario
    if(input[10] && esperaInteraccion <= 0){
      estado->cambiarEstado(1);
      esperaInteraccion = ESPERA_INTER;
      if(zoom){
        Vista *v= EstadoPartida::Instancia()->getVista();
        v->setZoom(1.0f);
        zoom = false;
      }
    }

    //Comprueba si hay un objeto que recoger cerca
    if(input[13] && esperaInteraccion <= 0) {
      float d1 = 0.0, d2 = 0.0;
      ObjetoSala* obj = estado->getSalaActual()->getObjetoSalaCercano(coordsNuevas, &d1);
      ElementoEntorno* elem = estado->getSalaActual()->getElementoInteractuableCercano(coordsNuevas, &d2);

      //Si la distancia es demasiado grande, no podra recoger el objeto del mapa
      if(d1 > DISTANCIA_RECOGER)
        d1 = -1.0;

      //Comprueba que distancia dentro del alcance es menor, si alguna lo esta
      if(d1 >= 0 || d2 >= 0) {
        if(d1 < 0 && d2 >= 0) {         //El objeto no esta al alcance pero el elemento si
          elem->interactuar();
          estado->getSalaActual()->entrarPortal();
        }
        else if(d1 >= 0 && d2 < 0) {    //El elemento no esta al alcance pero el objeto si
          obj->recoger(inventario);
        }
        else if(d1 < d2) {              //El objeto esta mas cerca que el elemento
          obj->recoger(inventario);
        }
        else {                          //El elemento esta mas cerca que el objeto
          elem->interactuar();
          estado->getSalaActual()->entrarPortal();
        }

        //Actualiza el valor de espera de interaccion para que no se ejecute muchas veces seguidas
        esperaInteraccion = ESPERA_INTER;
      }
      else {
        recuperarObjetoAtemporal();
      }
    }
    if(esperaInteraccion > 0) {
      esperaInteraccion -= elapsedTime;
    }

    //Actualiza los valores relativos al poder del tiempo
    if(tiempoDetenido) {
      //Resta el tiempo que lleva detenido el tiempo
      cargaTiempo -= elapsedTime;
      if(cargaTiempo <= 0) {
        //Si se agota el poder reanuda el tiempo
        reanudarTiempo();
      }

      //Genera particulas
      contadorPart += elapsedTime;
      if(contadorPart > 100) {
        contadorPart -= 100;
        new ParticulaSimpleCuadrada(coordsNuevas, {255, 255, 255, 255}, 20, 1000);
      }
    }
    else if(cargaTiempo < cargaTiempoMax) {
      //Carga la habilidad si no se esta usando
      cargaTiempo += cargaTiempoVel*dt;
      cargaTiempo = min(cargaTiempo, cargaTiempoMax);
    }

    //Boton para detener el tiempo
    if(input[11] && !moverGanchobool) {
      //Primero comprueba que ha pasado un tiempo minimo desde la ultima pulsacion
      if(relojHabilidad->getElapsedTime().milisegundos() >= ESPERA_INTER) {
        if(tiempoDetenido) {
          //Si el tiempo esta detenido, se reanuda
          reanudarTiempo();
        }
        else if(cargaTiempo > CARGA_MIN_TIEMPO && relojHabilidad->getElapsedTime().milisegundos() >= COOLDOWN_TIEMPO){
          //Si el tiempo no esta detenido y tiene suficiente poder, lo detiene
          detenerTiempo();
        }
      }
    }

    //Actualiza el sprite si es invulnerable
    if(invulnerable > 0) {
      int tiempoDanyo = TIEMPO_DANYO;

      if(inventario->poseeObjeto(10)) {
        //Si posee el objeto que aumenta la invulnerabilidad, ajusta los calculos
        tiempoDanyo -= INVULNERABILIDAD_BASE*2;
      }

      //Modifica el color del sprite dependiendo del estado de invulnerabilidad
      if(invulnerable > INVULNERABILIDAD_BASE-tiempoDanyo) {
        sprite->setColor({255, 50, 50, 255});
      }
      else {
        sprite->setColor({255, 255, 255, 125});
      }

      //Resta el tiempo que ha pasado
      invulnerable -= elapsedTime;

      //Si se acaba la invulnerabilidad, dibuja de nuevo el sprite correctamente
      if(invulnerable <= 0) {
        invulnerable = 0;
        sprite->setColor({255, 255, 255, 255});
      }
    }
    
    //Si el tiempo no esta detenido y tiene poca vida, genera particulas
    if(!tiempoDetenido && vida <= 2) {
      contadorPart += elapsedTime;
      //Cuanta menos vida tiene, mas particulas genera
      if(vida == 2) {
        if(contadorPart > 1000) {
          contadorPart -= 1000;
          new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 10, 1500);
        }
      }
      else {
        if(contadorPart > 400) {
          contadorPart -= 400;
          new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 15, 1500);
          new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 15, 1500);
          new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 15, 1500);
        }
      }
    }

    //Si tiene el efecto de ralentizacion, se actualiza
    if(ralentizacion > 0) {
      ralentizacion -= elapsedTime;

      //Genera particulas
      float posH = coordsNuevas[0];
      float tamH = sprite->getGlobalPixelSize()[0];

      posH += rand() % (int)tamH - tamH/2.0;
      new ParticulaDireccional({posH, coordsNuevas[1]}, {93, 22, 51, 255}, 20.0, 500, {0, 1}, 0.1, 0.3);
    }

    if(!moverGanchobool){
      actualizarAnimacion(elapsedTime);
    } 
  }
}

//Update que se ejecuta cuando el jugador ha muerto. Hace una pequenya animacion antes de morir.
void Jugador::updateMuerto(int elapsedTime) {
  //Reproduce la animacion de muerte
  animacion->playOnce(ANIM_MUERTE, elapsedTime);

  if(animacion->getFinished()) {
    //Al terminar la animacion oscurece la pantalla
    VentanaRender::Instancia()->fadeIn(10);
  }
}

void Jugador::actualizarAnimacion(float dt){

    if(this->estadoAnimacion == JugadorEstados::Quieto){

      this->animacion->play("Quieto",dt);

    }else if(this->estadoAnimacion == JugadorEstados::Derecha){
       this->animacion->play("Derecha",dt);

    }else if(this->estadoAnimacion == JugadorEstados::Izquierda){
       this->animacion->play("Izquierda",dt);

    }else if(this->estadoAnimacion == JugadorEstados::Salto){
       this->animacion->play("Salto",dt);

    }
}
//Se comprueban las colisines en las cuatro direcciones del personaje con el mapa, las plataformas y puertas
void Jugador::comprobarColisiones(){
  EstadoPartida *estado               = EstadoPartida::Instancia();
  map<vector<int>, int>* colisionMap  = estado->getSalaActual()->getMapaColisiones();
  vector<int> tamMapa                 = estado->getSalaActual()->getTamanyoMapa();
  int tamTiles                        = estado->getSalaActual()->getConstTamTiles();

  vector<int> posComprobar = {-1,-1};
  bool tocandoSuelo = false;
  int puerta = -1;

  //Id del tile a comprobar
  int idCol         = estado->getSalaActual()->getIdColision(1);
  int idPlat        = estado->getSalaActual()->getIdColision(2);
  int idPArriba     = estado->getSalaActual()->getIdColision(3);
  int idPDerecha    = estado->getSalaActual()->getIdColision(4);
  int idPAbajo      = estado->getSalaActual()->getIdColision(5);
  int idPIzquierda  = estado->getSalaActual()->getIdColision(6);

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
            if(tile->second == idPDerecha && estado->getSalaActual()->existePuerta(1)){
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
            if(tile->second == idPAbajo && estado->getSalaActual()->existePuerta(2)){
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
            if(tile->second == idPIzquierda && estado->getSalaActual()->existePuerta(3)){
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
            if(tile->second == idPArriba && estado->getSalaActual()->existePuerta(0)){
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
  }
  else {
    coordsNuevas    = coordsPuerta;
    coordsAntiguas  = coordsPuerta;
    velocidad       = {0.0, 0.0};
    setDisparo(false);
  }
  
  //Con las colisiones recogidas se escoge que puerta ha tocado y se cambia la sala
  //Se asignan las coordenadas del jugador a las del punto de aparicion de la puerta correspondiente
  if(puerta != -1){
    VentanaRender *ventana = VentanaRender::Instancia();
    ventana->fadeIn(25);
    estado->setCambiandoSala(puerta);
    cambiandoSala = true;
    setDisparo(false);
  }
  else {
    cambiandoSala = false;
  }
}

//Dibuja al jugador
void Jugador::dibujar(float percentTick){
    if(tiempoDetenido && posDetenerTiempo[0] != -1) {
      //Guarda el color para volver a ponerlo despues
      vector<int> color = sprite->getColor();
      
      sprite->setColor({100, 100, 100, 200});
      sprite->setPosition(posDetenerTiempo[0], posDetenerTiempo[1]);
      sprite->draw();

      sprite->setColor(color);
    }

    if(muerto) {
      //Si el jugador esta muerto no realiza la interpolacion
      sprite->setPosition(coordsNuevas[0], coordsNuevas[1]);
    }
    else {
      float coordX = coordsAntiguas[0]*(1-percentTick) + coordsNuevas[0]*percentTick;
      float coordY = coordsAntiguas[1]*(1-percentTick) + coordsNuevas[1]*percentTick;

      sprite->setPosition(coordX, coordY);

      //Comprueba si esta usando la maza, en cuyo caso la dibuja
      if(mazaActivada) {
        spriteMaza->draw();
      }
    }
    
    sprite->draw();
}

//Resetear el reloj que controla el salto del personaje
void Jugador::resetearSalto(){
    relojSalto->restart();
}

//Metodo de salto
void Jugador::saltar(){
  if(saltando==true && relojSalto->getElapsedTime().segundos()<TIEMPO_SALTO){

    velocidad[1] = (-1)*getVelMax(1)*(1-(relojSalto->getElapsedTime().segundos()/TIEMPO_SALTO));
    saltoCoyote = 0;
  }
  else {
    saltando = false;
  }
}

//Devuelve si el jugador puede bajar por las plataformas
int Jugador::getPuedeBajar(){
  return puedeBajar;
}

//Devuelve la posicion nueva del jugador
vector<float> Jugador::getPosNueva(){
  return coordsNuevas;
}

//Devuelve la referencia al sprite del jugador
Sprite* Jugador::getSprite(){
  return sprite;
}

//Actualiza la vida del jugador segun el danyo que se le pasa y actualiza el hud
void Jugador::danyarVida(int dmg){
  EstadoPartida* estado = EstadoPartida::Instancia();
  //Comprueba que no es invulnerable
  if(invulnerable == 0 && inmuneGolpes <= 0) {
    if(escudo>=dmg){
      //Si tiene escudo suficiente lo danya
      escudo -= dmg;
      hud->setEscudo(escudo);
    }else if(escudo > 0){
      //Si tiene algo de escudo lo destruye y pierde algo de vida
      int resto = dmg - escudo;
      escudo = 0;
      vida -= resto;
      hud->setEscudo(escudo);
      hud->setVida(vida);
    }else{
      //Si no tiene escudo le resta salud
      vida -= dmg;
      hud->setVida(vida);
    }

    //Al recibir danyo, lo vuelve invulnerable durante un tiempo
    invulnerable = INVULNERABILIDAD_BASE;
    if(inventario->poseeObjeto(10)) {
      invulnerable += INVULNERABILIDAD_BASE*2;
    }

    //Activa el efecto de algun objeto
    if(inventario->poseeObjeto(2)) {
      //Genera una explosion al recibir danyo
      Sprite* s = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
      s->setScale(10.0, 10.0);
      Explosion* ex = new Explosion(0, 100, s, nullptr, coordsNuevas, 160.0, 0, 0);
      estado->anyadirExplosion(ex);
    }
    if(inventario->poseeObjeto(8)) {
      //Si tiene poca vida aumenta el danyo
      if(vida <= 2) {
        arma->setDanyoExtra(10);
      }
    }
  }
  else if(invulnerable == 0) {
    //Si tiene inmunidad ante golpes, la reduce y genera particulas que avisan de ello
    inmuneGolpes--;
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
    new ParticulaSimpleCuadrada(coordsNuevas, {255, 249, 161, 255}, 120, 700);
  }

  //Si el tiempo estaba detenido se reanuda
  if(tiempoDetenido) {
    reanudarTiempo();
  }

  //Si se queda sin vida, se acaba la partida
  if(vida <= 0){ 
    if(inventario->poseeObjeto(12)) {
      //El objeto permite revivir al jugador con la mitad de vida
      setVidaMax(vidaMax/2);
      vida = 0;
      recuperaVida(vidaMax*2);
      //Elimina el objeto tras su uso
      inventario->eliminaObjeto(12);

      //Aparecen particulas para que el jugador vea lo sucedido
      for(int i=0; i<20; i++) {
        new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 120, 1500);
      }
    }
    else {
      muerto = true;
      Particula::eliminarParticulas();
      for(int i=0; i<8; i++)
        new ParticulaSimpleCuadrada(coordsNuevas, {255, 0, 0, 255}, 40.0, 1000);
    }
  }
}

//Matar jugador
void Jugador::setMuerto(bool b){
  muerto = b;
}

//Recupera salud sin que supere el limite
void Jugador::recuperaVida(int v) {
  vida += v;
  //Comprueba si supera el limite
  if(vida > vidaMax*2) {
    vida = vidaMax*2;
  }
  //Actualiza el hud
  hud->setVida(vida);

  //Si posee objetos que dependen de la salud se modifican
  if(vida > 2) {
    arma->setDanyoExtra(0);
  }
}

//Anyade la cantidad de escudo pasada por parametro a la cantidad actual
void Jugador::aumentarEscudo(int e) {
  escudo += e;
  //Actualiza el hud
  hud->setEscudo(escudo);
}

//Devuelve en segundos el temporizador de la habilidad del jugador
float Jugador::getCooldownHabilidad(){
  float porcentajeCarga = (float)cargaTiempo/((float)cargaTiempoMax);
  return porcentajeCarga;
}

//Devuelve la vida del jugador
int Jugador::getVida(){
  return vida;
}
//Devuelve el escudo del jugador
int Jugador::getEscudo(){
  return escudo;
} 
//Devuelve la vida maxima del jugador                                      
int Jugador::getVidaMax(){
  return vidaMax;
}
//Devuelve la barra de robo de vida
float Jugador::getRoboVida(){
  return roboVida;
}
//Devuelve el estado de la barra de carga del poder
int Jugador::getCargaTiempo(){
  return cargaTiempo;
}
//Devuelve el tiempo de carga maximo de la barra del poder
int Jugador::getCargaTiempoMax(){
  return cargaTiempoMax;
}

Arma* Jugador::getArma(){
  return arma;
}

Inventario* Jugador::getInventario(){
  return inventario;
}

//Recupera la referencia al hud desde el estadoPartida
void Jugador::recuperarHud() {
  EstadoPartida* estado = EstadoPartida::Instancia();
  hud = estado->getHud();
}

//Aumenta el robo de vida y comprueba si aumenta su salud
void Jugador::aumentarRoboVida(float r) {
  roboVida += r/FRACCION_ROBO_VIDA;
  //Si tiene toda la vida, tan solo recuperara hasta la mitad de la barra
  if(vida == vidaMax*2) {
    roboVida = min(roboVida, ROBO_VIDA_MAX/2.0f);
  }

  //Si alcanza el maximo, recupera un punto de salud
  if(roboVida >= ROBO_VIDA_MAX) {
    recuperaVida(1);
    roboVida -= ROBO_VIDA_MAX;

    if(inventario->poseeObjeto(11)) {
      int random = rand() % 100;

      if(random < PROB_RECUPERAR_MAS_VIDA) {
        recuperaVida(1);
      }
    }
  }

  hud->setRoboVida((int)roboVida);
}

void Jugador::setPosicion(vector<float> pos, bool antiguas){
  coordsNuevas = pos;
  if(antiguas){
    coordsAntiguas = pos;
  }
  //Actualiza el sprite
  sprite->setPosition(pos[0], pos[1]);

  //Actualiza la posicion del arma
  arma->setPosArma(pos, 0, true);
}

void Jugador::setSpawn(vector<float> spawn){
  coordsPuerta = spawn;
}

//Actualiza el estado del jugador en tiempo detenido y llama al metodo de estado partida
void Jugador::detenerTiempo() {
  if(!tiempoDetenido) {
    EstadoPartida* estado = EstadoPartida::Instancia();
    tiempoDetenido = true;
    estado->detenerTiempo();
    relojHabilidad->restart();

    if(inventario->poseeObjeto(9)) {
      posDetenerTiempo = coordsNuevas;
    }
  }
}

//Actualiza el estado del jugador en tiempo detenido y llama al metodo de estado partida
void Jugador::reanudarTiempo() {
  if(tiempoDetenido) {
    EstadoPartida* estado = EstadoPartida::Instancia();
    tiempoDetenido = false;
    estado->reanudarTiempo();
    relojHabilidad->restart();

    if(inventario->poseeObjeto(9)) {
      Textura* t = estado->getTexturas(4);
      Sprite* s = new Sprite(t, 0, 0, 32, 32);
      s->setScale(8.0, 8.0);
      Explosion* e = new Explosion(0, 50, s, nullptr, posDetenerTiempo, 126.0, 0, 0);
      estado->anyadirExplosion(e);
    }

    posDetenerTiempo = {-1.0, -1.0};
  }
}

//Devuelve si el jugador ha detenido el tiempo
bool Jugador::getTiempoDetenido() {
  return tiempoDetenido;
}

void Jugador::setArma(Arma* nueva){
  arma = nueva;
  inventario->nuevaArma(&nueva);
}


void Jugador::setVidaMax(int vM){
  int recuperar = max(vM - vidaMax, 0);
  vidaMax = vM;
  recuperaVida(recuperar*2);
  hud->setVidaMax(vidaMax);
}

void Jugador::setHabilidadMax(int hM){
  cargaTiempoMax  = hM;
  cargaTiempo     = hM;
}

void Jugador::setHabilidadVel(int hV){
  cargaTiempoVel = hV;
}

Hud* Jugador::getHud(){
  return hud;
}

void Jugador::setInmuneGolpes(int golpes) {
  inmuneGolpes = max(inmuneGolpes, golpes);
}

//Se llama al entrar en una sala por primera vez en la mazmorra
void Jugador::entradoEnNuevaSala() {
  if(inventario != nullptr && inventario->poseeObjeto(3)) {
    setInmuneGolpes(1);
  }
  setDisparo(false);

}

int Jugador::getCargaTiempoVel() {
  return cargaTiempoVel;
}

void Jugador::setCargaTiempoVel(int vel) {
  if(vel > 0) {
    cargaTiempoVel = vel;
  }
}

//Devuelve las coordenadas del portal de desafio por el que entro el personje
vector<int> Jugador::getCoordsPortal(){
  return coordsPortal;
}

void Jugador::setCoordsPortal(vector<int> c){
  coordsPortal = c;
}

void Jugador::moverGancho(){
  EstadoPartida *estado = EstadoPartida::Instancia();
  vector<float> posJugador=estado->getJugador()->getPosNueva();
  vector<float> direccion=gancho->getPosition();
  float dist=sqrt(pow((gancho->getPosition()[0] - posJugador[0]), 2) + pow((gancho->getPosition()[1]-posJugador[1]),2));
  if(dist>60){
  moveVec[0]=direccion[0]-posJugador[0];
    moveVec[1]=direccion[1]-posJugador[1];
    VecLength=sqrt(pow(moveVec[0],2)+pow(moveVec[1],2)); 
    moveVec[0] /= VecLength;
    moveVec[1] /= VecLength;

    this->velocidad[0]= moveVec[0]*700;
    this->velocidad[1] = moveVec[1]*400;
    
      puedeBajar =true;

  }
    else{
      setDisparo(false);
      moverGanchobool=false;
    }
}
void Jugador::retroceso(){
     EstadoPartida *estado = EstadoPartida::Instancia();
    vector<float> posJugador=estado->getJugador()->getPosNueva();
    float dist=sqrt(pow((gancho->getPosition()[0] - posJugador[0]), 2) + pow((gancho->getPosition()[1]-posJugador[1]),2));
    
    if(dist<20){
      setDisparo(false);
      
    }

}

//Hace aparecer un objeto atemporal cercano al puntero, si lo hay
void Jugador::recuperarObjetoAtemporal() {
  //Recoge el objeto atemporal mas cercano al puntero, si lo hay
  VentanaRender* ventana = VentanaRender::Instancia();
  vector<float> posRaton = ventana->mapPixelToCoords(ventana->getPosicionRaton());
  float d = 0.0;
  ObjetoAtemporal* obj = EstadoPartida::Instancia()->getSalaActual()->getObjetoAtemporalCercano({posRaton[0], posRaton[1]}, &d);

  if(obj != nullptr && d < DISTANCIA_ATEMPORAL) {
    //Recupera la posicion del objeto y calcula la distancia al jugador
    vector<float> pos = obj->getPosicion();
    float distancia = sqrt(pow(pos[0]-coordsNuevas[0], 2) + pow(pos[1]-coordsNuevas[1], 2));

    if(distancia > DISTANCIA_ATEMPORAL) {
      //Si esta lo suficientemente lejos, trae el objeto
      obj->interactuar();

      if(tiempoDetenido) {
        //Al traer el objeto reanuda el tiempo
        reanudarTiempo();
      }
    }
  }
}

bool Jugador::getSaltando(){
  return saltando;
}

Gancho* Jugador::getGancho(){
      return gancho;
}


bool Jugador::getDisparo(){
      return disparo;
}

void Jugador::setDisparo(bool verfal){
    disparo = verfal;
}

bool Jugador::getMuerto() {
  return muerto;
}

bool Jugador::getLinterna() {
  return linterna;
}

void Jugador::setLinterna(bool l) {
  linterna = l;
}

bool Jugador::getMaza() {
  return maza;
}

void Jugador::setMaza(bool m) {
  maza = m;
}

int Jugador::getRalentizacion() {
  return ralentizacion;
}

void Jugador::setRalentizacion(int r) {
  ralentizacion = r;
}

/*Dado un int devuelve la velocidad maxima de movimiento del jugador teniendo en cuenta la 
* ralentizacion aplicada por los espiritus oscuros. Segun el parametro devuelve:
* 0 - Velocidad maxima horizontal
* 1 - Potencia salto
*/
float Jugador::getVelMax(int indice) {
  float v = 0.0;

  switch(indice) {
    case 0:
      v = VEL_MAX_H;
      if(ralentizacion > 0) {
        v *= 0.5;
      }
      break;
    case 1:
      v = POTENCIA_SALTO;
      if(ralentizacion > 0) {
        v *= 0.5;
      }
      break;
  }

  return v;
}

//Actualiza el movimiento de la maza y comprueba si rompe algun muro
void Jugador::actualizarMaza() {
  //Comprueba que esta usando la maza
  if(mazaActivada) {
    rotacionMaza+=7.5;
    //Calcula la posicion de la maza
    float dist = pow(8-8*(recuperacionMaza/(float)TIEMPO_USO_MAZA), 2);
    posMaza = {coordsNuevas[0]+dist+dirMaza[0]*dirMazamouse, coordsNuevas[1]+dist+dirMaza[1]};
    if(dirMaza[1]>=-30){
        dirMaza[0]-=6.5*dirMazamouse;

    }
    if(dirMaza[1]>=-26){
        dirMaza[1]+=1;

    }else{
     dirMaza[1]+=4;

    }

    spriteMaza->setPosition(posMaza[0], posMaza[1]);
    spriteMaza->setRotation(rotacionMaza*dirMazamouse);

    //Comprueba si colisiona con algun muro destructible
    vector<MuroDestructible*> muros = EstadoPartida::Instancia()->getSalaActual()->getMurosDestructibles();
    for(unsigned int i=0; i<muros.size(); i++) {
      //Si el sprite de la maza colisiona con el de un muro lo rompe
      if(spriteMaza->intersect(muros[i]->getSprite())) {
        muros[i]->romper();
      }
    }

    //Comprueba si ya ha pasado el tiempo de uso
    if(recuperacionMaza <= 0) {
      //Desactiva la maza y activa el tiempo de recuperacion
      mazaActivada = false;
      recuperacionMaza = TIEMPO_REC_MAZA;

      //Al llegar al punto maximo, danya a los enemigos con los que colisiona
      vector<Enemigo*>* enemigos = EstadoPartida::Instancia()->getSalaActual()->getEnemigos();
      //Recorre los enemigos de la sala
      for(unsigned int i=0; i<enemigos->size(); i++) {
        if((*enemigos)[i] != nullptr && (*enemigos)[i]->getSprite()->intersect(spriteMaza)) {
          //Si impacta algun enemigo lo danya
          (*enemigos)[i]->perderVida(DANYO_MAZA);
        }
      }
    }
  }
}

bool Jugador::getSalto(){
      return salto;
}