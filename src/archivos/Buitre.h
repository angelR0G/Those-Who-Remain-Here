#pragma once

#include "Enemigo.h"
#include "ProyectilRecto.h"

class Buitre : public Enemigo{
    public:
        Buitre(int, int, int, int, int, vector<int>, Textura*, Sprite*);
        ~Buitre();
        void comprobarColisiones();
        void update(Tiempo);
        void reset();
        void perderVida(int);
        void modificarDireccion();
        void setPosicion(vector<float>);
    private:
        int danyado;
        int contador;
        vector <float> posInicial;
        float distanciaCentro;
        float anguloGiro;
        Animacion* animacion;
};