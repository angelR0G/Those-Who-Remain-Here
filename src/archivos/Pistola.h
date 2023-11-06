#pragma once
#include "Arma.h"
#include "ProyectilRecto.h"
#include "ProyectilPerforante.h"
using namespace std;

class Pistola : public Arma{
    public:
        Pistola(int, string, string, int, int, float, float, int, int, int, int, vector<int>, float);
        ~Pistola();
        void disparar(int);
        void recargar();
        int getMunicion();
        void setMunicion(int);
        vector<string> getNombreRunas();
        vector<string> getDescRunasNormales();
        vector<string> getDescRunasGrandes();
        vector<int> getMaterialesRuna(int);
        vector<int> getRecorteSpriteRuna(int);

        vector<string> getTextoMejoras();
        vector<int> getMejorasArma();
        void setMejorasArma(int, int);
        vector<int> getMaterialesMejora(int, int);
};