#include "Mago.h"

const float DIST_DETECCION  = 500.0;
const float DIST_ATAQUE     = 900.0;

//Constructor
Mago::Mago(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias, vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    //Inicializa su estado de manera aleatoria
    posicionRayo.push_back(0);

        posicionRayo.push_back(0);
    estadoMagician=chill;
    sprite->setOrigin(8,10);
    posInicial      = {0, 0};
    sprite->setScale(5,5);
    enRango = false;
    contadorDisparo = 0;
    animacionMagician = new Animacion(sp);
    
    animacionMagician->addAnimation("tranquilo",75.f,0,0,6,0,18,18);
    animacionMagician->addAnimation("atacando",75.f,0,1,4,1,18,18);
}

void Mago::update(Tiempo dt){
    Enemigo::update(dt);
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Actualiza la posicion
        posicionAnterior = posicion;
        
        switch (estadoMagician)
        {
        case chill:
            animacionMagician->play("tranquilo", dt.milisegundos());

            break;
        
        case attack:
            animacionMagician->play("atacando", dt.milisegundos());
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

        contadorDisparo += dt.milisegundos() + tiempoAntes.milisegundos();
        if(contadorDisparo > tiempoRecuperacion) {
            //Calcula la distancia a la que se encuentra el jugador
            float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
            if((!enRango && distancia <= DIST_DETECCION) || (enRango && distancia <= DIST_ATAQUE)) {
                //Actualiza el estado de la torreta a jugador en rango
                enRango = true;
                
                //Si esta cerca crea un proyectil
                                estadoMagician =attack;

                vector<float> dirDisp = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                Sprite* spDisparo = new Sprite(partida->getTexturas(2), 224, 16, 16, 16);
               
                posicionRayo[0]=posicion[0]+dirDisp[0]*40;
                                    posicionRayo[1]=posicion[1];
                                    float va=dirDisp[0];

                if(dirDisp[0]<0){
                   // posicionRayo[1]=posicion[1]+20;

                }
                Proyectil* p = new ProyectilOndaVital(danyo, 500, spDisparo, posicionRayo, dirDisp, 250, 1000, {});
                partida->anyadirProyectil(p);
            }
            else{
                //El jugador se ha alejado
                enRango = false;
                estadoMagician= chill;

            }

            contadorDisparo -= tiempoRecuperacion;
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
    
    
}

Mago::~Mago(){

}

//Actualiza la posicion de la torreta
void Mago::setPosicion(vector<float> p) {
    Enemigo::setPosicion(p);
    if(p.size() == 2) {
        posInicial  = p;
    }
}