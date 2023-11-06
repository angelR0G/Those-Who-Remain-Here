#pragma once

#include "ObjetoSala.h"
#include "FabricaArmas.h"
#include "EstadoPartida.h"

#include <iostream>
#include <cmath>

class ArmaSuelta : public ObjetoSala {
    public:
        ArmaSuelta(vector<float>, Sprite*, Textura*, int);

        ~ArmaSuelta();

        void update(Tiempo, map<vector<int>, int>*, int);
        void recoger(Inventario*);

        void mostrarInformacion();
    private:
        int idArma;                 //Numero identificador del arma
        Arma* arma;                 //Objeto Arma
        string nombre;              //Nombre del arma
        string tipo;                //Tipo del arma
        string danyo;               //Danyo del arma
        string municion;            //Municion del arma
        int tiempoEspera;           //Tiempo que ha pasado desde que se recoge el arma

        Texto* texto;               //Texto a mostrar
        Texto* textod;
        Texto* textom;
        float an_f;                 //Anchura arma
        float al_f;                 //Altura arma

        ArrayVertices* marcoN;
};