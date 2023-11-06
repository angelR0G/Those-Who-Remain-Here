#pragma once

#include "ObjetoSala.h"

class MaterialSuelto : public ObjetoSala {
    public:
        MaterialSuelto(vector<float>, Sprite*, Textura*, int, int);

        ~MaterialSuelto();

        void recoger(Inventario*);

        void mostrarInformacion();
    private:
        int idMaterial;         //Numero identificador del material
        int cantidad;           //Cantidad de material que contiene
        string nombre;          //Nombre del material
        string descripcion;     //Descripcion del material

        Texto* texto;               //Texto a mostrar
        Texto* textod;
        float an_f;                 //Anchura objeto
        float al_f;                 //Altura objeto

        ArrayVertices* marcoN;
};