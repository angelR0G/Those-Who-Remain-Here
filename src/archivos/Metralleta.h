#pragma once
#include "Arma.h"
#include "ProyectilRecto.h"
#include<iostream>
#include <chrono>
using namespace std;

class Metralleta : public Arma{
    public:
        Metralleta(int, string, string, int, int, float, float, int, float, int, int, vector<int>, float);
        ~Metralleta();
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