#pragma once

#include "ElementoEntorno.h"
using namespace std;

class PortalSalaADesafio : public ElementoEntorno{
    public:
        PortalSalaADesafio(Textura*, Sprite*);
        ~PortalSalaADesafio();
        void update(Tiempo);
        void interactuar();
        bool getInteractuando();
    private:
        bool interactuado;

};