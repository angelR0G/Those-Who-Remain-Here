#pragma once

#include "Particula.h"
#include <math.h>

class ParticulaSimpleCuadrada : public Particula {
    public:
        ParticulaSimpleCuadrada(vector<float>, vector<int>, float, int);

        ~ParticulaSimpleCuadrada();

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