#pragma once

#include "../archivos/PNJ.h"
#include "../archivos/Guardado.h"

class PuntoGuardado: public PNJ {
    public:
        PuntoGuardado(Textura*);
        ~PuntoGuardado();
        void dibujar();
        void update(int);
        void interactuar();
        void mostrarCartel();
    private:
    int tiempoMostrar;
};
