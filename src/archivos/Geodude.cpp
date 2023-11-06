#include "Geodude.h"

const float DIST_DETECCION  = 1000.0;
const float DIST_ATAQUE     = 1000.0;

//Constructor
Geodude::Geodude(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias, vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
   tiempoRecuperacion = 5000;
    posInicial      = {0, 0};
    estadoGeodude = tranquilo;
    enRango = false;
    contadorDisparo = 0;
    animacionGeodude = new Animacion(sp);
    animacionGeodude->addAnimation("tranquilo",75.f,0,0,3,0,16,20);
    animacionGeodude->addAnimation("atacando",75.f,0,1,3,1,16,20);
}

void Geodude::update(Tiempo dt){
    Enemigo::update(dt);
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Actualiza la posicion
        posicionAnterior = posicion;
      
        switch (estadoGeodude)
        {
        case tranquilo:
            animacionGeodude->play("tranquilo", dt.milisegundos());

            break;
        
        case atacando:
            animacionGeodude->play("atacando", dt.milisegundos());
            break;
        }
        //Modifica la direccion en la que mira
        EstadoPartida* partida = EstadoPartida::Instancia();
        Jugador* j = partida->getJugador();
        vector<float> posJugador = j->getPosNueva();

        vector<float> escalaActual = sprite->getScale();
        if(posJugador[0] >= posicion[0]) {
            sprite->setScale(abs(escalaActual[0]), escalaActual[1]);
        }
        else if(escalaActual[0] > 0){
            sprite->setScale(escalaActual[0]*(-1.0f), escalaActual[1]);
        }
        float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
        //Genera particulas en la torreta
         if(enRango && distancia <= DIST_ATAQUE&&rand() % 2) {
                     int desp = rand() % 17 - 9;
                      new ParticulaDireccional({posicion[0]+desp*2, posicion[1]+10}, {255, 0, 255, 100}, 8.0, 1600, {0, 1}, 0.1, 0.2);
            }

        contadorDisparo += dt.milisegundos() + tiempoAntes.milisegundos();
        if(contadorDisparo > tiempoRecuperacion) {
            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if(((!enRango && distancia <= DIST_DETECCION) || (enRango && distancia <= DIST_ATAQUE))&&j->getSalto()) {
                //Actualiza el estado de la torreta a jugador en rango
                enRango = true;
                
                //Si esta cerca crea un proyectil
                estadoGeodude =atacando;
                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                Sprite* spDisparo = new Sprite(new Textura("resources/sprites/armas/nubeRayo.png"), 0, 0, 32, 32);
                Proyectil* p = new ProyectilRayo(danyo, 0, spDisparo, {posJugador[0],posJugador[1]-24}, {0,0}, 250, 1000, {});
                partida->anyadirProyectil(p);
                
            }
            else{
                //El jugador se ha alejado
                enRango = false;
                estadoGeodude= tranquilo;
            }

            contadorDisparo -= tiempoRecuperacion;
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
    
    
}

Geodude::~Geodude(){

}

//Actualiza la posicion de la torreta
void Geodude::setPosicion(vector<float> p) {
    Enemigo::setPosicion(p);
    if(p.size() == 2) {
        posInicial  = p;
    }
}