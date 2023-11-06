#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ProyectilParabolico.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

enum EnemigoEsqueleto {EsqueletoIzquierda,EsqueletoDerecha};

class Esqueleto : public Enemigo{
    public:
        Esqueleto(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Esqueleto();

        void update(Tiempo);
    private:
        int direccionMira;              //Indica hacia que direccion mira el enemigo
        bool enRango;                   //Indica si el jugador ha entrado en el rango de ataque
        int contador;
        Animacion* animeEsqueleto;
};