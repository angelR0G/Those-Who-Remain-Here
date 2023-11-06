#pragma once

#include "ElementoEntorno.h"
#include "EstadoPartida.h"
#include "ProyectilRecto.h"

class Trampa : public ElementoEntorno {
    public:
        Trampa(Textura*, Sprite*, int);

        ~Trampa();

        void update(Tiempo t);
    private:
        int contador;                   //Milisegundos que han pasado desde el ultimo disparo
        vector<float> direccion;        //Direccion de los disparos
        Animacion* animacion;
};