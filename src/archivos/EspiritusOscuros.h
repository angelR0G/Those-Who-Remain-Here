#pragma once

#include "ElementoEntorno.h"
#include "Animacion.h"
#include "EstadoPartida.h"
#include "ParticulaSimpleCuadrada.h"

class EspiritusOscuros : public ElementoEntorno {
    public:
        EspiritusOscuros(Textura*, Sprite*);

        ~EspiritusOscuros();

        void update(Tiempo);
    private:
        bool apagado;                           //Indica si el jugador se ha acercado con una linterna
        int contador;                           //Controla el tiempo entre comprobaciones
        Animacion* animacion;
};