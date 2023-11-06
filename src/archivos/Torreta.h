#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

enum EnemigoEstado {};

class Torreta : public Enemigo{
    public:
        Torreta(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Torreta();

        void update(Tiempo);
        void setPosicion(vector<float>);
    private: 
        float desplazamiento;           //Indica el desplazamiento del sprite
        bool levitando;                 //Indica si el sprite sube o baja
        bool enRango;                   //Indica si el jugador ha entrado en el rango de ataque
        int contadorDisparo;            //Milisegundos desde el ultimo disparo
        vector<float> posInicial;       //Posicion inicial del enemigo
};