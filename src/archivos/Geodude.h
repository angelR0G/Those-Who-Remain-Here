#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "ProyectilRayo.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

enum EnemigoGeodude {tranquilo,atacando};

class Geodude : public Enemigo{
    public:
        Geodude(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Geodude();

        void update(Tiempo);
        void setPosicion(vector<float>);
    private: 
        
        bool enRango;                   //Indica si el jugador ha entrado en el rango de ataque
        int contadorDisparo;            //Milisegundos desde el ultimo disparo
        vector<float> posInicial;       //Posicion inicial del enemigo
        Animacion* animacionGeodude;
        EnemigoGeodude estadoGeodude;
};