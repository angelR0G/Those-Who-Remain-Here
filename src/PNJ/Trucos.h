#pragma once

#include "../archivos/PNJ.h"
#include "../archivos/EstadoTrucos.h"

class Trucos: public PNJ {
    public:
        Trucos(Textura*);
        ~Trucos();
        void dibujar();
        void update(int);
        void interactuar();
        void mostrarCartel();
    private:
};
