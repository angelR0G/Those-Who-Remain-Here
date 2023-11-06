#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "Jugador.h"

using namespace std;

class ProyectilBulletHell : public Proyectil{
    public:
        ProyectilBulletHell(int, Sprite*, vector<float>, vector<float>, int);

        ~ProyectilBulletHell();

        void update(Tiempo);
};