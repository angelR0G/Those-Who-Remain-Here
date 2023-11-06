#include "EstadoInventario.h"

const int TAM_INPUT         = 19;
const int TIEMPO_ESPERA     = 300;

using namespace std;

//Puntero para comprobar que no esté ya creado
EstadoInventario* EstadoInventario::pEstadoInventario = 0;

//Metodo para generar la instancia
EstadoInventario* EstadoInventario::Instancia(){
    if(pEstadoInventario == 0){   //Primera llamada
        pEstadoInventario = new EstadoInventario(); //Se crea la instancia de estadoInventario
    }
    return pEstadoInventario;
}

EstadoInventario::EstadoInventario(){}

void EstadoInventario::inicializar(){
    EstadoPartida* estadoP = EstadoPartida::Instancia();
    inventario             = estadoP->getJugador()->getInventario();
}

//Destructor
EstadoInventario::~EstadoInventario(){}

//Metodo para cambiar de un estado a otro
void EstadoInventario::cambiarEstado(int valor){
    if(valor == 1) {
        PilaEstados* pilaEstados = PilaEstados::Instancia();
        
        //Cambia el estado actual al estado partida
        pilaEstados->eliminarUltimoEstado();
        

        //Se reinicia la escala de los marcos
        vector<Sprite*> marco = Marco::Instancia()->getMarco();
        for(unsigned int i = 0; i < marco.size(); i++){
            marco[i]->setScale(1, 1);
        }

        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->setMira(false);
    }
}

//WIP
void EstadoInventario::setSpritePuntero(Sprite* s) {
    puntero = s;
}

//Metodo que dibuja el inventario en pantalla
void EstadoInventario::dibujar(float fNU, int iNU){
    inventario->dibujar();
}

//Update para controlar los clicks del raton, tambien para abrir y cerrar el inventario
void EstadoInventario::update(Tiempo tNU, bool input[TAM_INPUT]){
    VentanaRender* ventana = VentanaRender::Instancia();
    if(!ventana->getFade()) {
        vector<float> pos  = ventana->mapPixelToCoords(ventana->getPosicionRaton());
        if(raton(pos[0], pos[1])){
            if(input[16]){
                inventario->clickRaton(pos[0], pos[1]);
            }
        }
        if(input[10] && tiempoEspera <= 0){
            cambiarEstado(1);
        }

        if(tiempoEspera > 0) {
            tiempoEspera -= tNU.milisegundos();
        }
        inventario->actualizarTemporizador(tNU.milisegundos());
    }
}

//Metodo para cuando se mueva el raton
bool EstadoInventario::raton(float x, float y){
    inventario->posicionRaton(x, y);
    return true;
}

//Reinicia el tiempo de cierre del inventario
void EstadoInventario::resetTiempoEspera() {
    tiempoEspera = TIEMPO_ESPERA;
}

