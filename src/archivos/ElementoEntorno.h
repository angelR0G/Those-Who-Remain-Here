#pragma once

#include <vector>
#include "Detenible.h"
#include "ControladorSonido.h"

#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/Tiempo.h"

using namespace std;

class ElementoEntorno : public Detenible{
    public:
        ElementoEntorno(Textura*, Sprite*, bool);

        virtual ~ElementoEntorno();

        virtual void update(Tiempo);
        virtual void dibujar();
        virtual void interactuar();
        vector<float> getPosicion();
        virtual void setPosicion(vector<float>);
        bool getInteractuable();
        float getDistInteraccion();
    protected:
        vector<float> posicion;         //Posicion en el mapa
        Textura* textura;
        Sprite* sprite;
        bool interactuable;             //Indica si el jugador puede interactuar con el elemento
        float distInteraccion;          //Distancia desde la que se puede interactuar con el elemento
        //sonido
        ControladorSonido* sonidos; 
};