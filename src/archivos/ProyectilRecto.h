#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilRecto : public Proyectil{
    public:
        ProyectilRecto(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>);

        ~ProyectilRecto();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        vector<int> runasElementales;
};