#pragma once

#include "Enemigo.h"
#include "ParticulaSimpleCuadrada.h"
#include "EstadoPartida.h"

enum EnemigoHechicero {HechiceroEsperando, HechiceroGenerando};

class Hechicero : public Enemigo {
    public:
        Hechicero(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Hechicero();

        void update(Tiempo);
        void reset();
    private:
        int contador;           //Milisegundos que lleva en el estado actual
        bool disparo;           //Bala generada
        bool enRango;           //Jugador en rango
        bool cargado;           //Bala cargada
        EnemigoHechicero estado;   //Estado actual del hechicero

        Animacion* animacion;
        Proyectil* p;

        //Info bala
        vector<float> posproy; //Posicion proyectil
};