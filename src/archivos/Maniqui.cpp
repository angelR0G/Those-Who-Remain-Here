#include "Maniqui.h"

const int TIEMPO_SANGRADO   = 200;

//Constructor
Maniqui::Maniqui(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
  contador = 0;
  maxvida = vida;
  vidaactu = vida;
  anime = new Animacion(sp);
  anime->addAnimation("ManiquiIdle", 400.0, 0, 0, 3, 0, 14, 18);
  anime->addAnimation("ManiquiGolpe", 40.0, 0, 1, 3, 0, 14, 18);
}
void Maniqui::update(Tiempo dt){
    Enemigo::update(dt);

    if(vida!=vidaactu && contador == 0)
      contador = 20;

    if(contador != 0){
      anime->play("ManiquiGolpe",dt.milisegundos());
      contador--;
    }
    else
      anime->play("ManiquiIdle",dt.milisegundos());
    
    vidaactu = vida;
}

//Maniqui no muere, se sube al maximo su vida
void Maniqui::perderVida(int danyo){
    vida-=danyo;

    if(vida <= 0) {
      vida = maxvida;
    }

    sangrando = TIEMPO_SANGRADO;
}

Maniqui::~Maniqui(){

}