#pragma once

#include <map>
#include <vector>

#include "../fachada/Sprite.h"
#include "../fachada/Textura.h"
#include "../fachada/Tiempo.h"
#include "Inventario.h"
#include "Marco.h"
using namespace std;

class ObjetoSala {
    public:
        ObjetoSala(vector<float>, Sprite*, Textura*);

        virtual ~ObjetoSala();

        virtual void recoger(Inventario*) = 0;
        virtual void mostrarInformacion() = 0;
        virtual void update(Tiempo, map<vector<int>, int>*, int);
        void dibujar(float);
        bool getRecogido();
        vector<float> getPosicion();

    protected:
        vector<float> posicion;                 //Posicion actual del objeto
        vector<float> posicionAnterior;         //Posicion anterior para interpolar
        float velocidadActual;                  //Velocidad a la que se mueve el objeto
        Sprite* sprite;
        Textura* textura;
        bool colocado;                          //Indica si ha tocado el suelo y, por lo tanto, ya no se mueve
        bool recogido;                          //Indica si el personaje ha recogido el objeto

        //Para los marcos
        Textura* texMarco;
        vector<Sprite*> marco;
};