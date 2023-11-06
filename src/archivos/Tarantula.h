#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "ProyectilBoomerang.h"



enum EnemigoTarantula {TarantulaIzquierda,TarantulaDerecha};

class Tarantula : public Enemigo{
    public:
        Tarantula(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Tarantula();

        void comprobarColisiones();

        void update(Tiempo);
    private: 
        int desplazamiento;             //Indica el desplazamiento del sprite
        bool enRango;                   //Indica si el jugador ha entrado en el rango de ataque
        bool dir;                       //Indica en que direccion se mueve el enemigo: false izquierda, true derecha
        bool caer;                      //Indica si se cae
        int contador;
        float cont;


        vector<float> ladoColisiones;
        Animacion* animeTarantula;
        void saltar(float);
       // bool saltando;
        Reloj* relojSalto;
        int caida;
        float despla;
        
};