#pragma once

#include "Animacion.h"
//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Sprite.h"
#include "../fachada/Textura.h"

enum PNJEstados{
    PNJ_Quieto      = 0,
    PNJ_Izquierda   = 1,
    PNJ_Derecha     = 2
};

class PNJ{
    public:
        PNJ();
        virtual ~PNJ();
        virtual void dibujar();
        virtual void update(int);
        virtual void interactuar() = 0;
        Sprite* getSprite();
        vector<int> getPosicion();
        virtual void mostrarCartel() = 0;
        bool getInteractuando();
    protected:
        vector<int> posicion;       //Posicion del PNJ
        Sprite* spritePNJ;          //Sprite del PNJ
        bool interactuando;         //Indica si el personaje esta interactuando con el PNJ
        Textura* texMarco;          //Texturas para hacer el marco del dialogo
        vector<Sprite*>  marco;     //Vector con los sprites del marco
        Fuente* fuente;
        Texto* texto;
        //variables animacion
        int estadoAnimacion;
        Animacion* animacion;
};
