#pragma once

#include "ObjetoSuelto.h"
#include "MaterialSuelto.h"
#include "ArmaSuelta.h"
#include "EscudoSuelto.h"
#include "FabricaObjetos.h"
#include "FabricaArmas.h"
#include "EstadoPartida.h"
#include "ElementoEntorno.h"
#include "Animacion.h"



using namespace std;

class Cofre : public ElementoEntorno {
    public:
        Cofre(Textura*, Sprite*);

        ~Cofre();

        void interactuar();
        void update(Tiempo);
        bool getAbierto();
    private:
        bool abierto;           //Indica si ya se ha abierto el cofre
        bool reproduciendoAnim; //Indica si se esta ejecutando la animacion de abrir
        Animacion* animacion;
        
};