#pragma once

#include "../fachada/Sprite.h"
#include "../fachada/ArrayVertices.h"
#include "../fachada/Tiempo.h"

using namespace std;

class Particula {
    public: 
        Particula();

        virtual ~Particula();

        virtual void update(Tiempo) = 0;
        virtual void dibujar(float) = 0;
        bool getDestruir();
        static void setPosJugador(vector<float>);

        static void updateParticulas(Tiempo);
        static void dibujarParticulas(float);
        static void eliminarParticulas();
    protected:
        static vector<Particula*> vParticulas;      //Almacena todas las particulas creadas
        bool destruir;                              //Indica si la particula debe ser destruida
        static vector<float> posJugador;            //Posicion del jugador
};