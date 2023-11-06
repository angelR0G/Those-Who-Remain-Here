#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "../fachada/Textura.h"




using namespace std;

class ProyectilOndaVital : public Proyectil{
    public:
        ProyectilOndaVital(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>);

        ~ProyectilOndaVital();
        void draw(float, int);
        void update(Tiempo);
        bool comprobarColisionMapa(Sprite*);
    private:
        vector<int> runasElementales;
        vector<Sprite*> hilo;
        vector<float> direcRope;
        vector<float> posEst;
        Textura* texCuerdesita;
        float contador;
        int cambio;
};