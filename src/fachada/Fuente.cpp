#include "Fuente.h"

using namespace std;

//Constructor
Fuente::Fuente(string p){
    fuente = new sf::Font();
    if (!fuente->loadFromFile(p))
    {
        // error
        cout << "error" << endl;
    }
}

//Destructor
Fuente::~Fuente(){}

//Obtener font
sf::Font* Fuente::getFuente(){
    return fuente;
}