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

enum EnemigoGoblin {GoblinIzquierda,GoblinDerecha};

class Goblin : public Enemigo{
    public:
        Goblin(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Goblin();

        void comprobarColisiones();

        void update(Tiempo);
    private: 
        int desplazamiento;             //Indica el desplazamiento del sprite
        bool enRango;                   //Indica si el jugador ha entrado en el rango de ataque
        bool dir;                       //Indica en que direccion se mueve el enemigo: false izquierda, true derecha
        bool caer;                      //Indica si se cae
        int contador;

        vector<float> ladoColisiones;
        Animacion* animeGoblin;
};