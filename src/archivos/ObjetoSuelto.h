#pragma once

#include "ObjetoSala.h"
#include "FabricaObjetos.h"
#include "EstadoPartida.h"

class ObjetoSuelto : public ObjetoSala {
    public:
        ObjetoSuelto(vector<float>, Sprite*, Textura*, int);

        ~ObjetoSuelto();

        void recoger(Inventario*);

        void mostrarInformacion();
    
    private:
        int idObjeto;               //Numero identificador del objeto
        string nombre;              //Nombre del objeto
        string descripcion;         //Descripcion del objeto

        Texto* texto;               //Texto a mostrar
        Texto* textod;
        float an_f;                 //Anchura objeto
        float al_f;                 //Altura objeto

        ArrayVertices* marcoN;
};