#pragma once

#include "ElementoEntorno.h"
#include "EstadoPartida.h"

using namespace std;

class RecuperadorVida: public ElementoEntorno{
    public:
        RecuperadorVida(Textura*, Sprite*);

        ~RecuperadorVida();

        void interactuar();
        void update(Tiempo);
        void dibujar();
    private:
};