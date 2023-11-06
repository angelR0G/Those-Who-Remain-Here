#pragma once
#include "FabricaObjetos.h"
#include <iostream>
#include <string>
#include "../fachada/Sprite.h"

using namespace std;

class Objeto{

    public:

        //Constructor por defecto
        Objeto(int, string, string, string);

        //Destructor
        ~Objeto();

        //Devuelve el nombre del objeto
        string getNombre();

        //Devuelve la descripcion asociada al objeto
        string getDescripcion();

        //Devuelve el sprite del objeto
        Sprite* getSprite();

        //Devuelve la id del objeto
        int getId();

        //Devuelve vector con las id de los objetos ya creados
        static vector<int> getCreados();

        //Impide que se creen objetos con la id indicada
        static void idObjetoUsada(int);

        //Vacia el vector de id de objetos usados para poder volver a crearlos todos
        static void reiniciarPoolObjetos();

    private:

        //Identificador del objeto
        int id;

        //Nombre
        string nombre;

        //Descripcion
        string descripcion;

        //Textura utilizada para el objeto
        Textura* textura;

        //Sprite utilizado para el objeto
        Sprite* spriteObjeto;

        //Cuando se inicie una run se generaran todos los objetos y se guardara aqui un puntero a cada uno de ellos
        static vector<int> IDobjetos;

};