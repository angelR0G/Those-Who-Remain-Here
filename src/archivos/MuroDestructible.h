#pragma once

#include "ElementoEntorno.h"
#include "Animacion.h"
#include "EstadoPartida.h"

class MuroDestructible : public ElementoEntorno {
    public:
        MuroDestructible(Textura*, Sprite*);

        ~MuroDestructible();

        void update(Tiempo);
        void romper();
        void dibujar();
        bool getDestruido();
        Sprite* getSprite();
    private:
        bool destruido;                     //Indica si el muro ha sido destruido
        bool animacionTerminada;            //Indica si ha terminado la animacion cuando es destruido
        vector<int> tileOcupado;            //Tile que ocupa el muro
        Animacion* animacion;
};