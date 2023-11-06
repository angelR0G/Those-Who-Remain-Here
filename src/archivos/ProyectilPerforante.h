#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilPerforante : public Proyectil{
    public:
        ProyectilPerforante(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>, int);

        ~ProyectilPerforante();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        vector<int> runasElementales;
        bool perforando;                    //Indica si esta perforando a alguien
        int perforacionActual;              //Cantidad de veces que a traspasado a alguien
        int perforacionMaxima;              //Numero maximo de perforaciones antes de destruirse
};