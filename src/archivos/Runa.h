#pragma once

#include <iostream>
#include <string>
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"



using namespace std;

class Runa{
    public:
        //Constructor se le pasa un boolean que será true si la runa es mayor y false si es menor
        Runa(bool);
        ~Runa();
        //Sprite del hueco de la runa vacio se usa uno u otro si es mayor o menor
        Sprite* getSpriteGeneral();
        //Sprite de la mejora aplicada
        Sprite* getSpriteRuna();
        //Devuelve si la runa es mayor o menor
        bool esMayor();
        //Devuelve si esta activada
        bool activada();
        //Se pone como sprite de runa el que corresponde al area de recorte, se guarda el indice de mejora 0-2 elementales 3-5 especificas, se pone el activa a true
        void setRuna(vector<int>, int);
        //Devuelve el indice de la runa
        int getIndice();
        //Pone activado a false
        void desactivar();

    private:
        Textura* runas;
        Sprite* interfaz;
        Sprite* tipo;
        bool superior;
        bool activa;
        int indice;

};