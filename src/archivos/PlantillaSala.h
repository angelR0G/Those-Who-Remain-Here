#pragma once

#include <string>

using namespace std;

class PlantillaSala {
    public:
        PlantillaSala(string, int, int, int, int);

        ~PlantillaSala();

        bool puertaNormal(int);
        bool puertaCualquiera(int);
        string getNombre();
        int getNumPuertas();

    private:
        string nombre;          //Nombre del archivo de la sala
        int arriba;             //Indica si hay puerta arriba
        int derecha;            //   "   "   "    "    a la derecha
        int abajo;              //   "   "   "    "    abajo
        int izquierda;          //   "   "   "    "    a la izquierda
};