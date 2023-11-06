#include "Hechicero.h"

const float DIST_DETECCION      = 500.0;
const int TIEMPO_GENERACION     = 1300;
const int TIEMPO_CARGA          = 1200;
const int TIEMPO_CAIDA          = 900;

//Constructor
Hechicero::Hechicero(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp) {
    contador = 0;
    disparo = false;
    enRango = false;
    cargado = false;
    estado = HechiceroEsperando;
    sprite->setPosition(posicion[0], posicion[1]);

    animacion = new Animacion(sprite);
    animacion->addAnimation("HechiceroEsperando", 100.f,0,1,3,1,24,24);
    animacion->addAnimation("HechiceroAtacando", 100.f,0,2,3,2,24,24);
}

//Destructor
Hechicero::~Hechicero() {
    delete animacion;
}

//Actualiza el enemigo
void Hechicero::update(Tiempo t) {
    Enemigo::update(t);

    //Comprueba que el enemigo no esta detenido
    if(!detenido) {
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        //Actua segun el estado
        switch(estado) {
            case HechiceroEsperando:
                //Reproduce la animacion
                animacion->play("HechiceroEsperando", t.milisegundos());

                if(contador >= tiempoRecuperacion) {
                    //Si ha pasado suficiente tiempo, comienza a generar un kamikaze
                    contador = 0;
                    estado = HechiceroGenerando;
                }
                break;
            case HechiceroGenerando:
                //Reproduce la animacion
                animacion->play("HechiceroAtacando", t.milisegundos());

                //Genera particulas
                new ParticulaSimpleCuadrada({posicion[0], posicion[1]-16}, {134, 189, 209, 180}, 10, 2000);

                if(disparo){
                    for(int i = 0; i < 2;i++){
                            new ParticulaSimpleCuadrada({posproy[0], posproy[1]}, {134, 189, 209, 180}, 30, 2000);
                    }
                }
                
                if(contador >= TIEMPO_GENERACION) {
                    //Si ha pasado suficiente tiempo, genera disparo
                    
                    
                    
                    EstadoPartida* partida = EstadoPartida::Instancia();
                    Jugador* j = partida->getJugador();
                    vector<float> posJugador = j->getPosNueva();

                    //Calcula la distancia a la que se encuentra el jugador
                    float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
                    if(!disparo && !enRango && distancia <= DIST_DETECCION) {
                        enRango = true;
                        contador = 0;
                        //Genera disparo de hielo
                        int torcer = rand() % 21 - 10;
                        //vector<float> dirDisp = {0, 1};

                        int distarriba = 60;
                        //Si el jugador esta saltando, aleja mas arriba el proyectil
                        if(j->getSaltando()){
                            distarriba*=2;
                        }

                        posproy = {posJugador[0]-torcer,posJugador[1]-distarriba};
                        /*//Disparo instantaneo
                        Sprite* spDisparo = new Sprite(partida->getTexturas(2), 16*16 + 2, 16*6 + 4, 10, 10);
                        //spDisparo->setOrigin(10/2, 10/2);
                        spDisparo->setScale(3,3);
                        vector<int> medida = spDisparo->getGlobalPixelSize();
                        
                        p = new ProyectilRecto(danyo, 0, spDisparo, posproy, dirDisp, 0, 1000, {});
                        partida->anyadirProyectil(p);*/
                        disparo = true;
                    }else{
                        //El jugador se ha alejado
                        enRango = false;
                    }
                }else{
                    if(!cargado && disparo && contador >= TIEMPO_CARGA){
                        EstadoPartida* partida = EstadoPartida::Instancia();
                        Sprite* spDisparo = new Sprite(partida->getTexturas(2), 16*16 + 2, 16*6 + 4, 10, 10);
                        spDisparo->setScale(3,3);
                        vector<int> medida = spDisparo->getGlobalPixelSize();
                        vector<float> dirDisp = {0, 1};


                        p = new ProyectilRecto(danyo, 0, spDisparo, posproy, dirDisp, 0, 1000, {});
                        partida->anyadirProyectil(p);//Disparo retrasado
                        cargado = true;
                    }
                    if(cargado && disparo && contador >= TIEMPO_CAIDA){
                        disparo = false;
                        p->setVelocidad(300);
                        estado = HechiceroEsperando;
                        cargado = false;
                    }
                }

                break;
        }
    }
}

//Resetea el enemigo
void Hechicero::reset() {
    Enemigo::reset();
    estado      = HechiceroEsperando;
    contador    = 0;
}