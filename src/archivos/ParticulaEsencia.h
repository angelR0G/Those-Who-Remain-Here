#pragma once

#include "Particula.h"
#include <math.h>

class ParticulaEsencia : public Particula {
    public:
        ParticulaEsencia(vector<float>, vector<int>, int);

        ~ParticulaEsencia();

        void update(Tiempo);
        void dibujar(float);
    private:
        ArrayVertices* particula;
        vector<float> posAnterior;          //Posiciones de los vertices de la particula
        vector<float> posNueva;
        vector<int> color;                  //Color de la particula
        int tamanyo;                        //Tamanyo de la particula
        int tiempoEstatico;                 //Tiempo que permaneceran en movimiento aleatorio
        int tiempoActual;
        vector<float> direccion;            //Direccion de desplazamiento de la particula
        float velocidad;                    //Velocidad de desplazamiento de la particula
};