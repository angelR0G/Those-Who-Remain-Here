#include "Objeto.h"

using namespace std;
vector<int> Objeto::IDobjetos;
//Constructor recibe la id, el nombre, la descripcion y la ruta de la textura
Objeto::Objeto(int id, string nombre, string descripcion, string ruta){

    //Asignamos la id, nombre y descripcion
    this->id            =   id;
    this->nombre        =   nombre;
    this->descripcion   =   descripcion;

    //creamos la textura y el sprite
    textura             =   new Textura(ruta);
    spriteObjeto        =   new Sprite(textura, 0,0,32,32);

    //Asignamos parametros a la textura
    spriteObjeto->setOrigin(32/2, 32/2);
    spriteObjeto->setScale(2.f, 2.f);
    
    //Guardamos la id en el vector para saber que este objeto ya ha sido creado
    idObjetoUsada(id);
}

//Destructor
Objeto::~Objeto(){
    delete textura;
    delete spriteObjeto;
}

//Devuelve el nombre del objeto
string Objeto::getNombre(){
    return nombre;
}

//Devuelve la descripcion del objeto
string Objeto::getDescripcion(){
    return descripcion;
}

//Devuelve el sprite del objeto
Sprite* Objeto::getSprite(){
    return spriteObjeto;
}

//Devuelve la id del objeto
int Objeto::getId(){
    return id;
}

//Devuelve el vector de objetos ya creados
vector<int> Objeto::getCreados(){
    return IDobjetos;
}

void Objeto::idObjetoUsada(int id) {
    bool yaIncluida = false;

    //Recorre el vector de ids para comprobar que no esta ya agregada
    for(unsigned int i=0; i<IDobjetos.size() && !yaIncluida; i++) {
        if(IDobjetos[i] == id) {
            yaIncluida = true;
        }
    }

    //Si no lo esta, anyade la id
    if(!yaIncluida) {
        IDobjetos.push_back(id);
    }
}

void Objeto::reiniciarPoolObjetos() {
    IDobjetos.clear();
}