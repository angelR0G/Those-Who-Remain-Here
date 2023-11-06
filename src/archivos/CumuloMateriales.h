#pragma once

#include "MaterialSuelto.h"
#include "EstadoPartida.h"
#include "ElementoEntorno.h"
#include "Animacion.h"

using namespace std;

class CumuloMateriales : public ElementoEntorno {
    public:
        CumuloMateriales(Textura*, Sprite*);

        ~CumuloMateriales();

        void interactuar();
        void update(Tiempo);
    private:
        bool abierto;           //Indica si ya se ha abierto el cofre
        bool reproduciendoAnim; //Indica si se esta ejecutando la animacion de abrir
        Animacion* animacion;
};