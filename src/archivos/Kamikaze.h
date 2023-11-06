#pragma once

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ParticulaSimpleCuadrada.h"

enum EnemigoKamikaze {KamikazeApareciendo, KamikazeEsperando, KamikazePersiguiendo};

class Kamikaze : public Enemigo {
    public:
        Kamikaze(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Kamikaze();

        void update(Tiempo);
        void reset();
    private:
        int contador;                   //Milisegundos que ha permanecido en el estado actual
        int tiempoALaVista;             //Veces seguidas que el jugador estaba a la vista
        float escala;                   //Escala a aplicar al sprite segun la direccion en la que mira
        EnemigoKamikaze estado;         //Estado actual del enemigo

        Animacion* animacion;
};