#pragma once

#include <vector>

#include "../fachada/Sprite.h"
#include "../fachada/Tiempo.h"
#include "Detenible.h"

using namespace std;

class Proyectil : public Detenible{
    public:
        Proyectil(int, int, Sprite*, vector<float>, vector<float>, int, int);

        virtual ~Proyectil();

        virtual void update(Tiempo) = 0;
        virtual void draw(float, int);
        bool getDestruir();
        void setVelocidad(int);
    protected:
        int danyoJugador;                           //Cantidad de danyo que provoca el proyectil al jugador
        int danyoEnemigo;                           //Cantidad de danyo que provoca el proyectil al enemigo
        Sprite* sprite;                             //Sprite del proyectil
        vector<float> posicion;                     //Posicion actual del proyectil
        vector<float> posicionAnterior;             //Posicion anterior para calcular interpolacion
        vector<float> direccion;                    //Direccion que sigue el proyectil
        int velocidad;                              //Velocidad a la que avanza el proyectil
        bool destruir;
        int alcance;                                //Alcance maximo de la bala
};