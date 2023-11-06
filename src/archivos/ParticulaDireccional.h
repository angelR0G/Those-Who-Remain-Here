#pragma once

#include "Particula.h"
#include <math.h>

class ParticulaDireccional : public Particula {
    public:
        ParticulaDireccional(vector<float>, vector<int>, float, int, vector<float>, float, float);

        ~ParticulaDireccional();

        void update(Tiempo);
        void dibujar(float);
    private:
        ArrayVertices* particula;
        vector<float> posAnterior;          //Posiciones de los vertices de la particula
        vector<float> posNueva;
        vector<int> color;                  //Color de la particula
        int tamanyo;                        //Tamanyo de la particula
        int tiempoVida;                     //Tiempo que permanecera la particula
        vector<float> direccion;            //Direccion de desplazamiento de la particula
        float velocidad;                    //Velocidad de desplazamiento de la particula
};