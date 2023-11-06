#include "Buitre.h"

const int TIEMPO_ENFADADO       = 1200;

//Constructor
Buitre::Buitre(int vida, int danyo, int velocidad, int tiempoRecuperacion, int esencias, vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    danyado =0;
    contador=0;
    distanciaCentro = 100;
    posInicial = {0, 0};
    anguloGiro = 0;
    animacion = new Animacion(sprite);
    animacion->addAnimation("BuitreChill", 100.0, 0, 1, 4, 1, 16, 16);
    animacion->addAnimation("BuitreDiablo", 60.0, 0, 1, 4, 1, 16, 16);
}

Buitre::~Buitre(){
    delete animacion;
}
//Establece la posicion inicial, tenemos en cuenta que esta desplazado respecto al centro para realizar los movimientos circulares
void Buitre::setPosicion(vector<float> pos){
    posInicial = pos;
    posicion = {posInicial[0]+distanciaCentro*cos(anguloGiro), posInicial[1]+distanciaCentro*sin(anguloGiro)};
    posicionAnterior = posicion;
}
void Buitre::update(Tiempo t){
    Enemigo::update(t);

    if(!detenido){
        contador += t.milisegundos() + tiempoAntes.milisegundos();
        if(danyado<=0){
            animacion->play("BuitreChill", t.milisegundos());
            anguloGiro = anguloGiro+getVelocidad()*t.segundos();
            if(anguloGiro>360){
                anguloGiro -= 360;
            }
            posicionAnterior = posicion;
            posicion = {posInicial[0]+distanciaCentro*cos(anguloGiro), posInicial[1]+distanciaCentro*sin(anguloGiro)};
            if(contador>= tiempoRecuperacion){
                contador-=tiempoRecuperacion;
                //Para que dispare 8 balas en 8 direcciones distintas
                for(int i = 0; i<8; i++){
                    Sprite* spDisparo = new Sprite(EstadoPartida::Instancia()->getTexturas(2), 273, 166, 14, 7);
                    spDisparo->setScale(2.0, 2.0);
                    float angulo = i*45.0;
                    spDisparo->setRotation(angulo);
                    angulo = angulo * M_PI/180;
                    vector<float> direccion = {cos(angulo), sin(angulo)};
                    Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, direccion, 180, 800, {});
                    EstadoPartida::Instancia()->anyadirProyectil(p);
                }
            }
            
        }
        else{
            animacion->play("BuitreDiablo", t.milisegundos());
            anguloGiro = anguloGiro+getVelocidad()*t.segundos();
            danyado -= t.milisegundos() + tiempoAntes.milisegundos();
            if(anguloGiro>360){
                anguloGiro -= 360;
            }
            posicionAnterior = posicion;
            posicion = {posInicial[0]+distanciaCentro*cos(anguloGiro), posInicial[1]+distanciaCentro*sin(anguloGiro)};
            if(contador>= tiempoRecuperacion/4){
                contador-=tiempoRecuperacion/4;
                //Si se enfada lo hace en el doble de direcciones y mas seguido durante un muy breve periodo de tiempo
                for(int i = 0; i<8; i++){
                    Sprite* spDisparo = new Sprite(EstadoPartida::Instancia()->getTexturas(2), 273, 166, 14, 7);
                    spDisparo->setScale(2.0, 2.0);
                    float angulo = i*45;
                    spDisparo->setRotation(angulo);
                    angulo = angulo * M_PI/180;
                    vector<float> direccion = {cos(angulo), sin(angulo)};
                    Proyectil* p = new ProyectilRecto(danyo, 0, spDisparo, posicion, direccion, 180, 800, {});
                    EstadoPartida::Instancia()->anyadirProyectil(p);
                }
            }
        }        
        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }
}

//Resetea el aguila
void Buitre::reset() {
    contador    = 0;
    danyado     = 0;
}

//Sobrecarga de perderVida ed Enemigo
void Buitre::perderVida(int d) {
    Enemigo::perderVida(d);
    //Al recibir danyo, el aguila se enfada
    if(danyado <= 0) {
        contador    = 0;
        danyado     = TIEMPO_ENFADADO;
    }
    else {
        danyado += TIEMPO_ENFADADO/2;
    }
}