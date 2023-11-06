#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilParabolico : public Proyectil{
    public:
        ProyectilParabolico(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>);

        ~ProyectilParabolico();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        vector<int> runasElementales;
        vector<float> velActual;            //Velocidad en cada eje
};