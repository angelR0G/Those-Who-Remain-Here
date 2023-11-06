#pragma once

#include "Material.h"
#include <string>

using namespace std;

class FabricaMateriales{

    public:

        //Manera de generar el objeto Fabrica singleton
        static FabricaMateriales* Instancia();

        //Destructor
        ~FabricaMateriales();

        //Con este metodo se crea el material deseado
        vector<Material*> creaMaterial();

        //Devolver info
        string getNombre(int);
        string getDescripcion(int);

        //Con este metodo se recupera informacion del material deseado
        string getRutaPorId(int);

    private:

        //Constructor privado
        FabricaMateriales();

        //Para comprobar si el objeto esta construido
        static FabricaMateriales* pFabricamateriales;

};