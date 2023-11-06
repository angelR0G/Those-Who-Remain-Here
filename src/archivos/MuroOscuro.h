#pragma once

#include <map>

#include "ElementoEntorno.h"
#include "Animacion.h"
#include "EstadoPartida.h"

class MuroOscuro : public ElementoEntorno {
    public:
        MuroOscuro(Textura*, Sprite*);

        ~MuroOscuro();

        void update(Tiempo);
    private:
        vector<vector<int>> colisiones;         //Vector con las coordenadas donde apareceran colisiones
        bool apagado;                           //Indica si el jugador se ha acercado con una linterna
        int contador;                           //Controla el tiempo entre comprobaciones
        Animacion* animacion;
};