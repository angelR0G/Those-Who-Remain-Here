#pragma once

#include "../archivos/PNJ.h"
#include "../archivos/Guardado.h"
#include "../archivos/EstadoPueblo.h"

class Tutorial: public PNJ {
    public:
        Tutorial(Textura*);
        ~Tutorial();
        void dibujar();
        void update(int);
        void interactuar();
        void mostrarCartel();
    private:
};
