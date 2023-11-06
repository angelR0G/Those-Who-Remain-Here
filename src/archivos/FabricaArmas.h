#pragma once

#include "Pistola.h"
#include "Escopeta.h"
#include "Metralleta.h"
#include "LanzaCohetes.h"
#include <string>

using namespace std;

class FabricaArmas{
    public:
        static FabricaArmas* Instancia();

        ~FabricaArmas();
        class Arma* fabricarArma(int);    //Metodo que crea un arma
        string getRuta(int);
        string getNombre(int);
        int getDanyo(int);
        int getAlcance(int);
        float getCadencia(int);
        float getRecarga(int);
        int getMunicion(int);
        int getPrecicion(int);
        int getRunasNorm(int);
        int getRunasGran(int);
        vector<int> getRecorte(int);
        float getEscala(int);
        int getTotalArmas();
    protected:
        FabricaArmas();        //Constructor de la fabrica de armas
    private:
        static FabricaArmas* pFabricaArmas;   //Referencia a la fabrica
};