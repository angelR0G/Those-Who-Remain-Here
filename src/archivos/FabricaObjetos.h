#pragma once

#include "Objeto.h"
#include <string>

using namespace std;

class FabricaObjetos{

    public:

        //Metodo para crear la fabrica de objetos singleton
        static FabricaObjetos* Instancia();

        //Desctructor
        ~FabricaObjetos();

        //metodo para crear un objeto concreto a partir de la id
        class Objeto* fabricarObjetoPorID(int);

        //metodo para recibir la informacion de un objeto
        string getDescripcion(int);
        string getNombre(int);
        string getRuta(int);

        //metodo que devuelve la id aleatoria de un objeto que se puede crear
        int getIDDisponible();
    private:

        //Constructor privado
        FabricaObjetos();

        //para saber si la fabrica esta creada
        static FabricaObjetos* pFabricaObjetos;
};