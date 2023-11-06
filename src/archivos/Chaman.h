#pragma once

#include "Enemigo.h"
#include "ParticulaSimpleCuadrada.h"
#include "Kamikaze.h"
#include "EstadoPartida.h"

enum EnemigoChaman {ChamanEsperando, ChamanGenerando};

class Chaman : public Enemigo {
    public:
        Chaman(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Chaman();

        void update(Tiempo);
        void reset();
    private:
        int contador;           //Milisegundos que lleva en el estado actual
        EnemigoChaman estado;   //Estado actual del chaman

        Animacion* animacion;
};