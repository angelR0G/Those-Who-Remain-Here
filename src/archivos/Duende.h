#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Enemigo.h"
#include "EstadoPartida.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

enum EnemigoDuende {DuendeCaminando, DuendeEnRango, DuendePreparandoAtaque, DuendeAtacando};

class Duende : public Enemigo{
    public:
        Duende(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Duende();

        void comprobarColisiones();

        void update(Tiempo);
    private: 
        int desplazamiento;             //Indica el desplazamiento del sprite
        bool direccionMov;              //Indica en que direccion se mueve el enemigo: false izquierda, true derecha
        bool cayendo;                   //Indica si esta cayendo
        bool sueloIzquierda;            //Indica si hay suelo en dichas direcciones
        bool sueloDerecha;
        int contador;                   //Contador para los milisegundos
        int balasRestantes;             //Cantidad de balas que tiene por disparar
        EnemigoDuende estado;           

        vector<float> ladoColisiones;
        Animacion* animacionDuende;
};