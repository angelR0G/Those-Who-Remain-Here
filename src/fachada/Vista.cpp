#include "Vista.h"

using namespace std;

//Constructor
Vista::Vista(int x, int y, int ancho, int alto) {
    vista.setCenter(x, y);
    vista.setSize(ancho, alto);
    this->ancho = ancho;
    this->alto = alto;
}

//Destructor
Vista::~Vista() {}

//Coloca el viewport de la vista en las coordenadas x0, y0 y con un tamanyo de x1, y1
void Vista::setAreaVista(int x0, int y0, int x1, int y1){
    vista.setViewport(sf::FloatRect(x0, y0, x1, y1));
}

//Coloca el centro de la vista en las coordenadas x, y pasadas
void Vista::setCentro(float x, float y){
    vista.setCenter(x, y);
}

//Devuelve el objeto view
sf::View Vista::getVista(){
    return vista;
}

vector<float> Vista::getCentro(){
    vector<float> centro = {vista.getCenter().x, vista.getCenter().y};
    return centro;
}

//Hacer zoom en la vista
void Vista::setZoom(float f){
    //.zoom
    vista.setSize(ancho*f,alto*f);
}