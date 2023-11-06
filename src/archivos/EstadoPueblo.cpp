#include "EstadoPueblo.h"

using namespace std;

const int TAM_INPUT         = 19;

const string TILESET        = "tiles.png";
const string RUTA_PUEBLO    = "Pueblo.tmx";
const vector<int> TAM_TILE  = {8, 8};

//Distancia a la que se muestra el mensaje del PNJ
const float DIST_MENSAJE    = 40.0;

//Volumen de la musica del pueblo
const int VOL_MUSICA = 30;

//Se inicializa el puntero de la instancia
EstadoPueblo *EstadoPueblo::pEstadoPueblo = 0;

/*Se crea la instancia de estadopueblo
* 
*/
EstadoPueblo* EstadoPueblo::Instancia(){
    if(pEstadoPueblo == 0){   //Primera llamada
        pEstadoPueblo = new EstadoPueblo(); //Se crea la instancia de EstadoPueblo
    }
    return pEstadoPueblo;
}

//Constructor
EstadoPueblo::EstadoPueblo(){
    VentanaRender* ventana = VentanaRender::Instancia();
    interactuando = false;
    vista = new Vista(0, 0, ventana->getSize()[0], ventana->getSize()[1]);

    tileset = nullptr;
    pueblo  = nullptr;
    jugadorPueblo = nullptr;
    jugador = nullptr;

    //Para los trucos
    armaSeleccionada = -1;
    zonaSeleccionada = -1;
    jefeSeleccionado = -1;
    trucosActivados = false;
    sonidos = new ControladorSonido();
    sonidos->addMusica("musicaPueblo", "resources/sonidos/Pueblo.ogg", VOL_MUSICA);

}

//Destructor
EstadoPueblo::~EstadoPueblo(){
    //Borra el array de texturas
    while(texPNJs.size() > 0) {
        if(texPNJs[texPNJs.size()-1] != nullptr) {
            delete texPNJs[texPNJs.size()-1];
        }
        texPNJs.pop_back();
    }

    delete tileset;
    delete pueblo;
    delete sonidos;
}

//Inicializa el pueblo
void EstadoPueblo::inicializar(){
    resetear();
    Guardado *guardado = Guardado::Instancia();
    guardado->setTutorial(true);
    guardado->guardarPartida();
    if(tileset == nullptr) {
        tileset = new Textura("resources/niveles/"+TILESET);
    }
    if(pueblo == nullptr){
        pueblo = new Escenario("resources/niveles/"+RUTA_PUEBLO, tileset, TAM_TILE);
    }
    //Se crean las texturas para todos los PNJ del pueblo
    texPNJs.push_back(new Textura("resources/sprites/pnjs/warlock.png"));
    texPNJs.push_back(new Textura("resources/sprites/pnjs/magos.png"));
    texPNJs.push_back(new Textura("resources/sprites/pnjs/magos.png"));
    texPNJs.push_back(new Textura("resources/sprites/pnjs/maniqui.png"));
    //Se crean los PNJs y se agregan al vector de pnj del pueblo
    pnjs.push_back(new Tendero(texPNJs[0]));
    pnjs.push_back(new PuntoGuardado(texPNJs[1]));
    pnjs.push_back(new Tutorial(texPNJs[3]));
    
    jugadorPueblo = new JugadorPueblo();
    vector<int> coords = {55, 22};
    jugadorPueblo->setPosicion({(float)coords[0]*32, (float)coords[1]*32}, true);
    jugadorPueblo->setSpawn({(float)coords[0]*32, (float)coords[1]*32});
    entrandoMazmorra = false;
    VentanaRender::Instancia()->setMira(false);

    //Se crea el jugador
    jugador = new Jugador();

    //Reinicia la pool de objetos al entrar al pueblo
    Objeto::reiniciarPoolObjetos();

    //Se activa la musica del pueblo
    sonidos->playMusica("musicaPueblo");
}

void EstadoPueblo::resetear(){
    if(jugadorPueblo != nullptr){
        delete jugadorPueblo;
        jugadorPueblo = nullptr;
    }
    //Se borran los pnj y sus texturas
    while(pnjs.size() > 0) {
        if(pnjs[pnjs.size()-1] != nullptr) {
            delete pnjs[pnjs.size()-1];
        }
        pnjs.pop_back();
    }
    while(texPNJs.size() > 0) {
        if(texPNJs[texPNJs.size()-1] != nullptr) {
            delete texPNJs[texPNJs.size()-1];
        }
        texPNJs.pop_back();
    }
    trucosActivados = false;
}

//Render del pueblo
void EstadoPueblo::dibujar(float percentTick, int update_tick_time){
    VentanaRender *ventana = VentanaRender::Instancia();
    if(ventana->getFade()) {
        percentTick = ventana->getTiempoFade()/((float)update_tick_time);
    }

    //Se dibuja al personaje
    jugadorPueblo->dibujar(percentTick);

    //Se dibuja el pueblo
    pueblo->dibujar();

    //Se dibujan los PNJs
    for(unsigned int i = 0; i < pnjs.size(); i++){
        pnjs[i]->dibujar();
    }

    //Se dibuja al personaje
    ventana->dibujar(jugadorPueblo->getSprite());

    //Mostrar cartel del PNJ mas cercano al personaje
    float dist = 0;
    PNJ* pnjCercano = getPNJCercano(jugadorPueblo->getPosNueva(), &dist);
    if(pnjCercano != nullptr && dist < DIST_MENSAJE){
        pnjCercano->mostrarCartel();
    }

    PilaEstados *pEstados = PilaEstados::Instancia();
    if(pEstados->getEstadoActivo() == EstadoPueblo::Instancia()){
        //Se coloca la vista centrada en las coordenadas del jugadorPueblo
        vector<float> coords = jugadorPueblo->getSprite()->getPosition();
        centroVista = pueblo->centrarCamara(coords[0], coords[1]);
        vista->setCentro(centroVista[0], centroVista[1]);
        ventana->setVista(vista);
    }
}

//Actualiza el pueblo
void EstadoPueblo::update(Tiempo timeElapsed, bool input[TAM_INPUT]){
    VentanaRender* ventana = VentanaRender::Instancia();
    if(!trucosActivados && input[11] && input[17]){
        trucosActivados = true;
        pnjs.push_back(new Trucos(texPNJs[2]));
    }
    if(!ventana->getFade() && !ventana->getPantallaNegro()) {
        //Update normal
        jugadorPueblo->update(timeElapsed.milisegundos(), input);
        //Se dibujan los PNJs
        for(unsigned int i = 0; i < pnjs.size(); i++){
            pnjs[i]->update(timeElapsed.milisegundos());
        }
    }
    else if(!ventana->getFade()){
        if(entrandoMazmorra) {
            cambiarEstado(1);
        }
        else {
            ventana->fadeOut(25);
        }
    }
}

//Cambia entre estados
void EstadoPueblo::cambiarEstado(int valor){
    PilaEstados* pilaEstados = PilaEstados::Instancia();
    if(valor == 1) {
        //Se para la musica del pueblo cuando entras en la mazmorra
        sonidos->stopMusica("musicaPueblo");

        EstadoPartida* ePartida = EstadoPartida::Instancia();
        
        //Cambia el estado actual al estado de la partida
        pilaEstados->modificarUltimoEstado(ePartida);

        if(!trucosActivados){
            EstadoTrucos::Instancia()->resetearTrucos();
        }

        FabricaArmas *fabricaArma = FabricaArmas::Instancia();
        jugador->setArma(fabricaArma->fabricarArma(0));
        if(armaSeleccionada >= 0){
            jugador->setArma(fabricaArma->fabricarArma(armaSeleccionada));
        }
        int zonaSeleccionada = EstadoTrucos::Instancia()->getZonaSeleccionada();
        int jefeSeleccionado = EstadoTrucos::Instancia()->getJefeSeleccionado();
        if(zonaSeleccionada == -1){
            zonaSeleccionada = 3;
        }
        if(jefeSeleccionado != -1){
            zonaSeleccionada = jefeSeleccionado - 3;
        }
        //Inicializa la partida
        ePartida->inicializar(jugador, zonaSeleccionada-3);
    }
    if(valor == 2){
        EstadoTienda* eTienda = EstadoTienda::Instancia();
        
        //Cambia el estado actual al estado tienda
        pilaEstados->anyadirEstado(eTienda);
        eTienda->resetTiempoEspera();
        eTienda->cargarMejoras();

        VentanaRender::Instancia()->setMira(true);
    }
    if(valor == 3){
        EstadoTrucos* eTrucos = EstadoTrucos::Instancia();
        
        //Cambia el estado actual al estado tienda
        pilaEstados->anyadirEstado(eTrucos);
        eTrucos->resetTiempoEspera();

        VentanaRender::Instancia()->setMira(true);
    }
}

//Devuelve la referencia al jugador del pueblo
JugadorPueblo* EstadoPueblo::getJugadorPueblo(){
    return jugadorPueblo;
}

//Devuelve la referencia al jugador
Jugador* EstadoPueblo::getJugador(){
    return jugador;
}


//Devuelve la referencia al pueblo
Escenario* EstadoPueblo::getPueblo(){
    return pueblo;
}

//Cambia el valor de entrandoMazmorra
void EstadoPueblo::setEntrandoMazmorra(bool b) {
    entrandoMazmorra = b;
}

//Devuelve la referencia al PNJ mas cercano al jugador para interactuar con el
//Se le pasa la posicion del jugador y un float donde se guarda la distancia
PNJ* EstadoPueblo::getPNJCercano(vector<float> posJ, float* menorDist){
    
    PNJ* pnj = nullptr;
    *menorDist = -1.0;

    //Recorre el array de pnj del pueblo
    for(unsigned int i=0; i < pnjs.size(); i++){
        if(pnjs[i]!= nullptr){
            vector<int> posPNJ = pnjs[i]->getPosicion();
            float distancia = sqrt(pow(posPNJ[0]-posJ[0], 2)+pow(posPNJ[1]-posJ[1], 2));
            if(*menorDist == -1.0 || distancia < *menorDist){
                *menorDist = distancia;
                pnj = pnjs[i];
            }
        }
    }
    return pnj;
}

void EstadoPueblo::setArmaSeleccionada(int a){
    armaSeleccionada = a;
}

void EstadoPueblo::setZonaSeleccionada(int z){
    zonaSeleccionada = z;
}

void EstadoPueblo::setJefeSeleccionado(int j){
    jefeSeleccionado = j;
}