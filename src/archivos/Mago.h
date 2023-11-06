#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ParticulaDireccional.h"
#include "ProyectilOndaVital.h"

//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"


enum EnemigoMagician {chill,attack};

class Mago : public Enemigo{
    public:
        Mago(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Mago();

        void update(Tiempo);
        void setPosicion(vector<float>);
    private: 
        bool enRango;
        int contadorDisparo;            //Milisegundos desde el ultimo disparo
        vector<float> posInicial;       //Posicion inicial del enemigo
        vector<float> posicionRayo;                     //Posicion actual del proyectil
        Animacion* animacionMagician;
        EnemigoMagician estadoMagician;
};