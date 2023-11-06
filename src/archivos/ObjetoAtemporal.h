#pragma once

#include <map>

#include "ElementoEntorno.h"
#include "Detenible.h"
#include "Animacion.h"
#include "Sala.h"

using namespace std;

class ObjetoAtemporal : public ElementoEntorno{
    public:
        ObjetoAtemporal(Textura*, Sprite*, vector<vector<int>>, map<vector<int>, int>);

        ~ObjetoAtemporal();

        void update(Tiempo);
        void dibujar();
        void interactuar();
        void setPosicion(vector<float>);
        bool getHabilitado();
        void setSala(class Sala*);
        static void borrarTextura();
    private:
        Animacion* animacion;
        Sprite* spriteAtemporal;            //Sprite del objeto antes de ser traido de otra linea temporal
        bool habilitado;                    //Indica si debera aparecer o no
        bool activado;                      //Indica si esta recuperando un objeto en este momento
        int tiempoActivo;                   //Cantidad de milisegundos pasados desde la ultima activacion
        bool animando;                      //Indica si se esta reproduciendo la animacion
        vector<vector<int>> colEliminar;    //Posiciones donde se eliminaran las colisiones
        map<vector<int>, int> colAnyadir;   //Posiciones y tipo de colisiones que genera el objeto
        class Sala* sala;                         //Sala en la que se encuentra el objeto

        static Textura* texAtemporal;       //Textura de todos los objetos atemporales
};