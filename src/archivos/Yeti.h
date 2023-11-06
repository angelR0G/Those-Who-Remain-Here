#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ProyectilRebota.h"

#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

class Yeti : public Enemigo{
    public:
        Yeti(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Yeti();

        void comprobarColisiones();

        void update(Tiempo);
        void reset();
    private: 
        float desplazamiento;           //Indica el desplazamiento del sprite
        int disparosRestantes;          //Disparos que debe hacer antes de dejar de atacar
        bool disparando;                //Indica si el enemigo ha comenzado a disparar
        bool dir;                       //Indica en que direccion se mueve el enemigo: false izquierda, true derecha
        bool caer;                      //Indica si se cae
        int contador;

        vector<float> ladoColisiones;
        Animacion* animacion;
};