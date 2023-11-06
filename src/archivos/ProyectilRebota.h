#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilRebota : public Proyectil{
    public:
        ProyectilRebota(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>);

        ~ProyectilRebota();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        vector<int> runasElementales;
        int rebotesMaximos;                 //Cantidad de rebotes que dara el proyectil antes de desaparecer
        int rebotesActuales;                //Cantidad de rebotes que restantes
};