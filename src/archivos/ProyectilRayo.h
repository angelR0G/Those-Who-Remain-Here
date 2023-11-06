#pragma once

#include "Proyectil.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "Jugador.h"
#include "Animacion.h"

using namespace std;
enum Rayo {Cargando, Dispara};

class ProyectilRayo : public Proyectil{
    public:
        ProyectilRayo(int, int, Sprite*, vector<float>, vector<float>, int, int, vector<int>);
        int cacahuete=0;
        ~ProyectilRayo();

        void update(Tiempo);
        bool comprobarColisionMapa();
    private:
        int contador;           //Milisegundos que lleva en el estado actual
          //Estado actual del chaman

        Animacion* animacion;
        Rayo estadoRayo;
};