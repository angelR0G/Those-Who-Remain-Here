#include "Textura.h"

using namespace std;

//Constructor
Textura::Textura(string ruta) {
    textura = new sf::Texture();
    if (!textura->loadFromFile(ruta)) {
        cout << "Error cargando la imagen " << ruta;
        exit(0);
    }
}

//Destructor
Textura::~Textura() {
    delete textura;
}

//Devuelve el valor de textura (sf::Texture)
sf::Texture* Textura::getTextura() {
    return textura;
}

//Devuelve el tamanyo de la textura
vector<int> Textura::getSize() {
    sf::Vector2u tam = textura->getSize();

    return {(int)tam.x, (int)tam.y};
}