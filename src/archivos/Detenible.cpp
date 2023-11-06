#include "Detenible.h"

//Constructor
Detenible::Detenible() {
    relojDetenible.restart();
    detenido = false;
}

//Destructor
Detenible::~Detenible() {}

//Pone el tiempo antes de detener a 0
void Detenible::reiniciarTiempoAntes() {
    tiempoAntes.setTiempoCero();
}

//Devuelve si el objeto esta detenido
bool Detenible::getDetenido() {
    return detenido;
}

//Recoge el tiempo del reloj y actualiza el estado del objeto
void Detenible::detener() {
    if(!detenido) {
        detenido        = true;
        tiempoAntes     = relojDetenible.getElapsedTime();
    }
}

//Reinicia el reloj y actualiza el estado del objeto
void Detenible::reanudar() {
    if(detenido) {
        detenido = false;
        relojDetenible.restart();
    }
}