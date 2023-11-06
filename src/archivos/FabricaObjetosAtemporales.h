#pragma once

#include <map>
#include <vector>

#include "ObjetoAtemporal.h"

#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

using namespace std;

class FabricaObjetosAtemporales{

    public:
        static FabricaObjetosAtemporales* Instancia();

        ~FabricaObjetosAtemporales();

        class ObjetoAtemporal* fabricar(int, vector<int>);
    protected:
        FabricaObjetosAtemporales();
    private:
        static FabricaObjetosAtemporales* pFabrica;
};