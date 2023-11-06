#include "Trucos.h"

const float ESCALA = 2.5;

using namespace std;

Trucos::Trucos(Textura* tex) : PNJ() {

    //Se crea el sprite del PNJ que guarda la partida
    spritePNJ = new Sprite(tex, 0, 2*28, 16, 28);
    spritePNJ->setOrigin(16/2, 31/2);
    spritePNJ->setScale(ESCALA, ESCALA);

    //Se coloca en su posicion
    posicion = {17*32,33*32};
    spritePNJ->setPosition(posicion[0], posicion[1]);

    //Se crea la animacion
    animacion = new Animacion(spritePNJ);
    animacion->addAnimation("PNJQuietoTrucos",150.f,0,2,3,2,16,28);
    estadoAnimacion = PNJEstados::PNJ_Quieto;

    //Se prepara el texto para el cartel que se muestra cuando se acerca el jugador
    texto = new Texto("");
    texto->setFuente(fuente);
    texto->setTamanyo(20);
    texto->setEscala(1,1);
    texto->setColorTexto({255, 255, 255, 255});
}

Trucos::~Trucos(){

}

void Trucos::dibujar(){
    spritePNJ->draw();
}

void Trucos::mostrarCartel(){
    texto->setString("Trucos");
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
    marco[1]->setScale(4.25, 1.0);
    marco[5]->setScale(4.25, 1.0);
    texto->setPosicion(posicion[0]-anchotxt/2, posicion[1]-36-12-altotxt/2);
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->draw();
    }
    texto->draw();
}

void Trucos::update(int elapsedTime){
    if(estadoAnimacion == PNJEstados::PNJ_Quieto){
        animacion->play("PNJQuietoTrucos", elapsedTime);
    }
}

void Trucos::interactuar(){
    EstadoPueblo *estado = EstadoPueblo::Instancia();
    interactuando= true;
    estado->getJugadorPueblo()->setInteracutando(interactuando);
    estado->cambiarEstado(3);
}