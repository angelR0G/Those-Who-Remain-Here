#include "Tutorial.h"

const float ESCALA = 2.5;

using namespace std;

Tutorial::Tutorial(Textura* tex) : PNJ() {

    //Se crea el sprite del PNJ que lleva al personaje al tutorial
    spritePNJ = new Sprite(tex, 0, 0, 14, 18);
    spritePNJ->setOrigin(14/2, 18/2 -5);
    spritePNJ->setScale(ESCALA, ESCALA);

    //Se coloca en su posicion
    posicion = {11*32,22*32};
    spritePNJ->setPosition(posicion[0], posicion[1]);

    //Se crea la animacion
    animacion = new Animacion(spritePNJ);
    animacion->addAnimation("PNJQuietoTutorial",100.f,0,0,3,0,14,18);
    estadoAnimacion = PNJEstados::PNJ_Quieto;

    //Se prepara el texto para el cartel que se muestra cuando se acerca el jugador
    texto = new Texto("");
    texto->setFuente(fuente);
    texto->setTamanyo(20);
    texto->setEscala(1,1);
    texto->setColorTexto({255, 255, 255, 255});
}

Tutorial::~Tutorial(){

}

//Se dibuja el PNJ
void Tutorial::dibujar(){
    spritePNJ->draw();
}

//Cuando el personaje esta cerca se muestra un cartel
void Tutorial::mostrarCartel(){

    texto->setString("Tutorial");
    
    
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

void Tutorial::update(int elapsedTime){
    if(estadoAnimacion == PNJEstados::PNJ_Quieto){
        animacion->play("PNJQuietoTutorial", elapsedTime);
    }
}

//Estando cerca, si se interactua con el PNJ se cambia al tutorial
void Tutorial::interactuar(){
    Guardado *guardado = Guardado::Instancia();
    guardado->setTutorial(false);
    guardado->guardarPartida();
    EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
    ePueblo->cambiarEstado(1);
}