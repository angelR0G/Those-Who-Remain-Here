#pragma once

using namespace std;

#include "Proyectil.h"
#include "EstadoPartida.h"

enum ProyOjoEstados{PO_PreparaAtaque, PO_Ataque};

class ProyectilRayoOjo : public Proyectil{
    public:
        ProyectilRayoOjo(int, int, Sprite*, vector<float>, vector<float>, int, int, int, vector<float>);

        ~ProyectilRayoOjo();
        void update(Tiempo);
    private:
        int contador;
        ProyOjoEstados estados;
        int tiempoPreparar;
        int tiempoAtaque;
        int cooldownDanyo;
        vector<float> alcanceEscala;
};