#pragma once

#include <map>
#include <vector>

#include "Trampa.h"
#include "Cofre.h"
#include "PortalSalaADesafio.h"
#include "PortalJefe.h"
#include "EspiritusOscuros.h"
#include "MuroOscuro.h"
#include "MuroDestructible.h"
#include "CumuloMateriales.h"

#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

using namespace std;

class FabricaElementosEntorno{

    public:
        static FabricaElementosEntorno* Instancia();

        ~FabricaElementosEntorno();

        class ElementoEntorno* fabricar(int);
    protected:
        FabricaElementosEntorno();
    private:
        static FabricaElementosEntorno* pFabrica;
};