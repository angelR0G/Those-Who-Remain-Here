#pragma once

#include "EstadoPartida.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class Explosion : public Detenible{
    public:
        Explosion(int, int, Sprite*, Sprite*, vector<float>, float, int, int);

        ~Explosion();

        void update(Tiempo);
        void dibujar();
        bool getDestruir();
        bool getTerminado();
    private:
        int danyoJugador;                           //Cantidad de danyo que provoca la explosion al jugador
        int danyoEnemigo;                           //Cantidad de danyo que provoca la explosion al enemigo
        Sprite* sprite;                             //Sprite de la explosion
        Sprite* spriteVeneno;                       //Sprite con la nube de veneno
        vector<float> posicion;                     //Posicion de la explosio
        bool destruir;                              //Indica si el objeto se va ha borrar
        float radio;                                //Radio de la explosion
        Animacion* anim;                            //Animacion de la explosion
        bool reproduciendoAnim;                     //Indica si la animacion se esta reproduciendo
        bool impacto;                               //La primera vez del update hace danyo la explosion
        int gas;                                    //Indica si tiene la mejora de la nube de gas, grande o pequenya
        int dobleExplosion;                         //Indica si tiene la mejora de doble explosion, grande o pequenya

        ControladorSonido *ctrSonido;
};