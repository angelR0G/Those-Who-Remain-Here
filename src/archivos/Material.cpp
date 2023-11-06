#include "Material.h"

using namespace std;

//Constructor, recibe nombre, descipcion y la ruta a la textura
Material::Material(string nombre, string descripcion, string ruta){

    //iniciamos la cantidad a 0 y asignamos nombre descripcion e id
    cantidad            =   0;
    this->nombre        =   nombre;
    this->descripcion   = descripcion;
    //creamos la textura y el sprite
    textura             =   new Textura(ruta);
    spriteMaterial      =   new Sprite(textura, 0,0,32,32);
    spriteMaterial->setOrigin(32/2, 32/2);
    spriteMaterial->setScale(2.f,2.f);

}

//Destructor
Material::~Material(){

    //Borramos punteros
    delete textura;
    delete spriteMaterial;
}

//Devuelve la cantidad del material
int Material::getCantidad(){
    return cantidad;
}

//Devuelve la descripcion del material
string Material::getDescripcion(){
    return descripcion;
}

//Devuelve el nombre del material
string Material::getNombre(){
    return nombre;
}

//Devuelve el sprite del material
Sprite* Material::getSprite(){
    return spriteMaterial;
}

//Aumenta la cantidad del material
void Material::aumentarCantidad(int c){
    cantidad    +=  c;
}

//Reduce la cantidad del material
void Material::reducirCantidad(int c){
    cantidad    -=  c;
}