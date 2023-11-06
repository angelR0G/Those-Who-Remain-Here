#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "Jugador.h"
#include "Enemigo.h"

using namespace std;

class ProyectilExplosivo : public Proyectil{
    public:
        ProyectilExplosivo(int, int, Sprite*, Sprite*, Sprite*, vector<float>, vector<float>, int, float, int, int, vector<int>, int);

        ~ProyectilExplosivo();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        Sprite* spriteExplosion;
        Sprite* spriteVeneno;
        vector<int> runasElementales;
        float radio;
        int gas;
        int dobleExplosion;
};