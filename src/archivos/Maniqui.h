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

enum EnemigoManiqui {ManiquiIdle,ManiquiGolpe};

class Maniqui : public Enemigo{
    public:
        Maniqui(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Maniqui();

        void perderVida(int);

        void update(Tiempo);
    private: 
        int maxvida, vidaactu, contador;
        Animacion* anime;
};