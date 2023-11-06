#pragma once

#include "Enemigo.h"
#include "ProyectilRecto.h"

class Aguila : public Enemigo {
    public:
        Aguila(int, int, int, int, int, vector<int>, Textura*, Sprite*);

        ~Aguila();

        void comprobarColisiones();
        void update(Tiempo);
        void reset();
        void perderVida(int);
        void modificarDireccion();
    private:
        int danyado;                    //Milisegundos restantes de sprint tras sufrir danyo
        int contador;                   //Tiempo desde el ultimo disparo
        bool direccion;                 //Indica la direccion hacia la que se mueve
        float velocidadActual;          //Velocidad de movimiento actual
        int xUltimoCambio;              //Guarda la coordenada X del ultimo cambio de direccion

        Animacion* animacion;
};