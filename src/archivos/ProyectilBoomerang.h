#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilBoomerang : public Proyectil{
    public:
        ProyectilBoomerang(int, int, Sprite*, vector<float>, vector<float>, int, vector<int>,int);

        ~ProyectilBoomerang();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        vector<int> runasElementales;
        bool regresando;                    //Indica si el proyectil ha tocado ya una pared
        int rotacion;
};