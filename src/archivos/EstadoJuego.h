#pragma once

#include <iostream>
#include "../fachada/Tiempo.h"

using namespace std;

class EstadoJuego {
    public:
        virtual void update(Tiempo, bool[]) = 0;
        virtual void dibujar(float, int) = 0;
        virtual void cambiarEstado(int) = 0;
    protected:
};