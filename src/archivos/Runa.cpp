#include"Runa.h"

//Constructor
Runa::Runa(bool mayor){
    
    /**
     * Se recoge la textura y se inician variables, si es mayor usa un textura diferente para el hueco de runa
     * 
     */
    runas   =   new Textura("resources/sprites/inventario/runaContenedor.png");
    activa    =   false;
    superior = mayor;
    indice = 0;

    if(mayor){
        interfaz = new Sprite(runas, 64, 0, 64, 64 );
        interfaz->setOrigin(64/2, 64/2);
    }
    else{
        interfaz = new Sprite(runas, 0, 0, 64, 64 );
        interfaz->setOrigin(64/2, 64/2);
    }
    tipo = nullptr;

}
//Destructor
Runa::~Runa(){
    delete runas;
    runas = nullptr;
    delete interfaz;
    interfaz = nullptr;
    delete tipo;
    tipo    = nullptr;

}

//Establece sprite de la runa que ocupa el hueco usando el area de recorte sobre la textura almacenada en el objeto, se guarda el indice correspondiente con el que se podra controlar si la mejora ya existe y demás
void Runa::setRuna(vector<int> recorte, int indice){
    activa = true;
    this->indice=indice;
    tipo = new Sprite(runas, recorte[0], recorte[1], 64, 64);
    tipo->setOrigin(64/2, 64/2);
}

/**
 * GETTERS de la clase
 **/
int Runa::getIndice(){
    return indice;
}
Sprite* Runa::getSpriteRuna(){
    return tipo;
}
Sprite* Runa::getSpriteGeneral(){
    return interfaz;
}
bool Runa::esMayor(){
    return superior;
}
bool Runa::activada(){
    return activa;
}
/**
 * SETTERS de la clase
 **/
void Runa::desactivar(){
    activa = false;
}