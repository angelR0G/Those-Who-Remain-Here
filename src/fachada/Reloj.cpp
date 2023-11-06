#include "Reloj.h"

using namespace std;

//Constructor
Reloj::Reloj() {
    reloj.restart();
}

//Destructor
Reloj::~Reloj() {}

//Recoge el tiempo actual del reloj
Tiempo Reloj::getElapsedTime(){
    Tiempo time;
    time.setTiempo(reloj.getElapsedTime());

    return time;
}

//Reinicia el reloj y devuelve el tiempo
Tiempo Reloj::restart(){
    Tiempo time;
    time.setTiempo(reloj.restart());

    return time;
}