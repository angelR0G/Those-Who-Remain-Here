#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "Animacion.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

using namespace std;

enum EstadosPajaro{
    Normal = 0,
    Ataque = 1
};

class Pajaro : public Enemigo{
    public:
        Pajaro(int, int, int, int, int, vector<int>, Textura*, Sprite*);
        ~Pajaro();
        void update(Tiempo);
        void colisionesMapa();
        void seleccionaDireccion(int);
    private:
        bool enRango;       //Indica si el jugador ha entrado en el rango de ataque del enemigo
        bool colision;
        bool atacando;      //Indica si el enemigo esta atacando
        bool atacado;
        bool reposo;
        Animacion* anim;    //Para la animacion
        int estadoPajaro;   //Estado para cambiar entre las animaciones de ataque y la normal
        int tiempoEntreAct; //Tiempo entre calculos de colisiones para detectar al personaje
        int tiempoVision;   //Tiempo que el pajaro ha visto al personaje
        int cooldownAtaque;
        int tiempoEntreCambios;
        vector<float> posEmpezarAtaque; //Posicion del pajaro antes de atacar, si se aleja mucho para de atacar al personaj
        vector<int> direccMov;
        vector<float> ladoColisiones;
        int direccion;
};