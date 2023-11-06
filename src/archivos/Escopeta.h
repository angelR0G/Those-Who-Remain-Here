#pragma once
#include "Arma.h"
#include "ProyectilRecto.h"
#include "Explosion.h"
#include<iostream>
using namespace std;

class Escopeta : public Arma{
    public:
        Escopeta(int, string, string, int, int, float, float, int, int, int, int, vector<int>, float);
        ~Escopeta();
        void disparar(int);
        void recargar();
        int getMunicion();
        void setMunicion(int);
        void disparos(float, float, int, bool, float);
        vector<string> getNombreRunas();
        vector<string> getDescRunasNormales();
        vector<string> getDescRunasGrandes();
        vector<int> getMaterialesRuna(int);
        vector<int> getRecorteSpriteRuna(int);

        vector<string> getTextoMejoras();
        vector<int> getMejorasArma();
        void setMejorasArma(int, int);
        vector<int> getMaterialesMejora(int, int);
    private:
        bool primera;
};