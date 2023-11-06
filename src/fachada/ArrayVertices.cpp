#include "ArrayVertices.h"
using namespace std;

/*Crea el array de vertices con el tipo de primitiva y numero de vertices especificados
**Tipos de primitiva:
**0 - LineStrip
**1 - Triangles
**2 - TriangleFan
**3 - TriangleStrip
**4 - Quads
*/
ArrayVertices::ArrayVertices(int modo, int numVertices) {
    array.resize(numVertices);

    //Segun el valor del modo, especifica un tipo de primitiva
    switch(modo) {
        case 0:
            array.setPrimitiveType(sf::LineStrip);
            break;
        case 1:
            array.setPrimitiveType(sf::Triangles);
            break;
        case 2:
            array.setPrimitiveType(sf::TriangleFan);
            break;
        case 3:
            array.setPrimitiveType(sf::TriangleStrip);
            break;
        case 4:
            array.setPrimitiveType(sf::Quads);
            break;
        default:
            array.setPrimitiveType(sf::Points);
            break;
    }
}

//Destructor
ArrayVertices::~ArrayVertices() {
    array.clear();
}

//Recoge la posicion de un vertice. Devuelve un array vacio si no es posible.
vector<int> ArrayVertices::getVertexPosition(int vertice) {
    vector<int> posicion;

    //Comprueba que el indice del vertice se encuentra dentro del array
    if(vertice < (int)array.getVertexCount()) {
        posicion.push_back(array[vertice].position.x);
        posicion.push_back(array[vertice].position.y);
    }

    return posicion;
}

//Cambia el color del vertice especificado. Los valores rgba deben estar entre 0 y 255.
void ArrayVertices::setVertexColor(int vertice, int r, int g, int b, int a) {
    //Comprueba que el indice del vertice se encuentra dentro del array
    if(vertice < (int)array.getVertexCount()) {
        array[vertice].color = sf::Color(r, g, b, a);
    }
}

//Cambia el color de todos los vertices del array. Los valores rgba deben estar entre 0 y 255.
void ArrayVertices::setArrayColor(int r, int g, int b, int a) {
    //Recorre el array verice a vertice
    for(unsigned int i=0; i<array.getVertexCount(); i++) {
        array[i].color = sf::Color(r, g, b, a);
    }
}

//Cambia la posicion del vertice especificado.
void ArrayVertices::setVertexPosition(int vertice, int x, int y) {
    //Comprueba que el indice del vertice se encuentra dentro del array
    if(vertice < (int)array.getVertexCount()) {
        array[vertice].position = sf::Vector2f(x, y);
    }
}

//Cambia las coordenadas de textura del vertice especificado.
void ArrayVertices::setVertexTexCoord(int vertice, float u, float v) {
    //Comprueba que el indice del vertice se encuentra dentro del array
    if(vertice < (int)array.getVertexCount()) {
        array[vertice].texCoords = sf::Vector2f(u, v);
    }
}

//Devuelve el array de vertices de SFML
sf::VertexArray ArrayVertices::getArrayVertices() {
    return array;
}

//Devuelve el tamanyo en pixeles del array de vertices
vector<int> ArrayVertices::getBounds() {
    vector<int> pixelSize;
    pixelSize.push_back(array.getBounds().width);
    pixelSize.push_back(array.getBounds().height);

    return pixelSize;
}

//Dibuja el array de vertices
void ArrayVertices::draw() {
    VentanaRender* ventana = VentanaRender::Instancia();

    ventana->dibujar(this);
}