#pragma once

#include "ElementoEntorno.h"
#include "Animacion.h"

using namespace std;

class PortalJefe : public ElementoEntorno{
    public:
        PortalJefe(Textura*, Sprite*);
        ~PortalJefe();
        void update(Tiempo);
        void interactuar();
        bool getInteractuando();
    private:
        bool interactuado;
        Animacion* animacion;
};