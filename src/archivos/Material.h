#pragma once
#include <string>
#include "../fachada/Sprite.h"

using namespace std;

class Material{

    public:

        //Constructor que será llamado por la fabrica de materiales
        Material(string, string, string);

        //Destructor
        ~Material();

        //Devuelve descripcion del material
        string getDescripcion();

        //Devuelve la cantidad de materiales de ese tipo que tenemos
        int getCantidad();

        //Devuelve el sprite del material
        Sprite* getSprite();

        //Devuelve el nombre del material
        string getNombre();

        //Se le pasa un int y aumenta la cantidad de materiales que tenemos
        void aumentarCantidad(int);

        //Se le pasa un int y reduce la cantidad de materiales que tenemos
        void reducirCantidad(int);

    private:

        //nombre del material
        string nombre;

        //descripcion
        string descripcion;

        //cantidad disponible
        int cantidad;

        //Textura empleada
        Textura* textura;

        //Sprite del material
        Sprite* spriteMaterial;

};