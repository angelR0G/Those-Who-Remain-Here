#pragma once

#include "../archivos/PNJ.h"
#include "../archivos/EstadoTienda.h"

class Tendero: public PNJ {
    public:
        Tendero(Textura*);
        ~Tendero();
        void dibujar();
        void update(int);
        void interactuar();
        void mostrarCartel();
    private:
};
