#pragma once

#include "Torreta.h"
#include "Goblin.h"
#include "Pajaro.h"
#include "Duende.h"
#include "Chaman.h"
#include "Kamikaze.h"
#include "Aguila.h"
#include "Maniqui.h"
#include "Esqueleto.h"
#include "Hechicero.h"
#include "Buitre.h"
#include "JefeRana.h"
#include "JefeOjo.h"
#include "JefeCaballero.h"
#include "Yeti.h"
#include "Mago.h"
#include "Geodude.h"
#include "Tarantula.h"

#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

using namespace std;

class FabricaEnemigos{

    public:
        static FabricaEnemigos* Instancia();

        ~FabricaEnemigos();

        class Enemigo* fabricarEnemigo(int);
        class Enemigo* fabricarJefe(int);
        class Enemigo* fabricarManiqui();
    protected:
        FabricaEnemigos();
    private:
        static FabricaEnemigos* pFabricaEnemigos;
};