#include "Texto.h"


using namespace std;

//Constructor
Texto::Texto(string t){
    texto = new sf::Text();
    texto->setString(t);
    texto->setOutlineThickness(1);
}

//Destructor
Texto::~Texto(){}

//Cambiar string
void Texto::setString(string t){
    texto->setString(t);
}

//Cambiar fuente
void Texto::setFuente(Fuente* f){
    fuente = f;
    texto->setFont(*f->getFuente());
}

//Cambiar color texto
void Texto::setColorTexto(vector<int> col){
    texto->setFillColor(sf::Color(col[0], col[1], col[2], col[3]));
}

//Cambiar color contorno
void Texto::setColorContorno(vector<int> col){
    texto->setOutlineColor(sf::Color(col[0], col[1], col[2], col[3]));
}

//Cambiar grosor del contorno
void Texto::setGrosorContorno(float grosor){
    texto->setOutlineThickness(grosor);
}


//Cambiar posicion
void Texto::setPosicion(float x, float y){
    texto->setPosition(x,y);
}

//Cambiar tamanyo
void Texto::setTamanyo(int tam){
    texto->setCharacterSize(tam);
}

//Poner escala
void Texto::setEscala(float escalaX, float escalaY){
    texto->setScale(escalaX, escalaY);
}

//Dibujar texto
void Texto::draw(){
    VentanaRender* ventana = VentanaRender::Instancia();

    ventana->dibujar(this);
}

//Devuelve el texto
sf::Text* Texto::getTexto(){
    return texto;
}

//Devuelve el ancho global del contenedor del texto
float Texto::getAncho(){
    return texto->getGlobalBounds().width;
}

//Devuelve el alto global del contenedor del texto
float Texto::getAlto(){
    return texto->getGlobalBounds().height;
}

//Devuelve si las coordenadas pasadas por parametro
//se encuentran dentro del contenedor de texto
bool Texto::getContains(float x, float y){
    return texto->getGlobalBounds().contains(x,y);
}