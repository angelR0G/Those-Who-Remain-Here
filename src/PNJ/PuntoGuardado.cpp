#include "PuntoGuardado.h"

const float ESCALA = 2.5;

const int TIEMPO_MOSTRAR = 50;

using namespace std;

PuntoGuardado::PuntoGuardado(Textura* tex) : PNJ() {

    //Se crea el sprite del PNJ que guarda la partida
    spritePNJ = new Sprite(tex, 0, 0, 16, 28);
    spritePNJ->setOrigin(16/2, 31/2);
    spritePNJ->setScale(ESCALA, ESCALA);

    //Se coloca en su posicion
    posicion = {6*32,33*32};
    spritePNJ->setPosition(posicion[0], posicion[1]);

    //Se crea la animacion
    animacion = new Animacion(spritePNJ);
    animacion->addAnimation("PNJQuietoSave",150.f,0,0,3,0,16,28);
    estadoAnimacion = PNJEstados::PNJ_Quieto;

    //Se prepara el texto para el cartel que se muestra cuando se acerca el jugador
    texto = new Texto("");
    texto->setFuente(fuente);
    texto->setTamanyo(20);
    texto->setEscala(1,1);
    texto->setColorTexto({255, 255, 255, 255});
    tiempoMostrar = 0;
}

PuntoGuardado::~PuntoGuardado(){

}

//Se dibuja el PNJ
void PuntoGuardado::dibujar(){
    spritePNJ->draw();
}

//Cuando el personaje esta cerca se muestra un cartel
void PuntoGuardado::mostrarCartel(){
    if(interactuando){
        if(tiempoMostrar<TIEMPO_MOSTRAR){
            texto->setString("Guardando");
        }else{
            texto->setString("Guardar");
        }
    }else{
        texto->setString("Guardar");
    }
    
    VentanaRender* ventana = VentanaRender::Instancia();
    int ancho = ventana->getSize()[0];
    int alto = ventana->getSize()[1];

    int anchotxt = texto->getAncho();
    int altotxt = texto->getAlto();
    texto->setEscala(ancho/1920,alto/1080);

    marco[0]->setPosition(posicion[0]-8-anchotxt/2, posicion[1]-36-32);//Esq superior izq
    marco[1]->setPosition(posicion[0]+8-anchotxt/2, posicion[1]-36-32);//Lado superior
    marco[2]->setPosition(posicion[0]-6+anchotxt/2, posicion[1]-36-32);//Esq superior dere
    marco[3]->setPosition(posicion[0]-6+anchotxt/2, posicion[1]-36-16);//Lado dere
    marco[4]->setPosition(posicion[0]-6+anchotxt/2, posicion[1]-36);//Esq abajo dere
    marco[5]->setPosition(posicion[0]+8-anchotxt/2, posicion[1]-36);//Lado abajo
    marco[6]->setPosition(posicion[0]-8-anchotxt/2, posicion[1]-36);//Esq abajo izq
    marco[7]->setPosition(posicion[0]-8-anchotxt/2, posicion[1]-36-16);//Lado izq
    marco[1]->setScale((anchotxt*0.95)/16, 1.0);
    marco[5]->setScale((anchotxt*0.95)/16, 1.0);
    texto->setPosicion(posicion[0]-anchotxt/2, posicion[1]-36-14-altotxt/2);
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->draw();
    }
    texto->draw();
}

void PuntoGuardado::update(int elapsedTime){
    if(interactuando){
        if(tiempoMostrar<TIEMPO_MOSTRAR){
            tiempoMostrar++;
        }else{
            tiempoMostrar=0;
            interactuando = false;
        }
    }
    if(estadoAnimacion == PNJEstados::PNJ_Quieto){
        animacion->play("PNJQuietoSave", elapsedTime);
    }
}

//Estando cerca, si se interactua con el PNJ se guarda la partida
void PuntoGuardado::interactuar(){
    //Guarda la partida
    Guardado* guar = Guardado::Instancia();
    interactuando = true;
    guar->guardarPartida();
}