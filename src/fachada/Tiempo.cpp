#include "Tiempo.h"

using namespace std;

//Constructor
Tiempo::Tiempo() {}

//Destructor
Tiempo::~Tiempo() {}

//Especifica el tiempo
void Tiempo::setTiempo(sf::Time time) {
    tiempo = time;
}

//Devuelve el tiempo en segundos
float Tiempo::segundos(){

    return tiempo.asSeconds();
}

//Devuelve el tiempo en milisegundos
int Tiempo::milisegundos(){

    return tiempo.asMilliseconds();
}

//Pone el tiempo a 0
void Tiempo::setTiempoCero(){

    tiempo = sf::Time::Zero;
}