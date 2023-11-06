#include "PilaEstados.h"

PilaEstados* PilaEstados::pPilaEstados = 0;

PilaEstados* PilaEstados::Instancia() {
    if(pPilaEstados == 0) {
        pPilaEstados = new PilaEstados();
    }
    return pPilaEstados;
}

PilaEstados::PilaEstados() {
    relojDibujar = new Reloj();
}

PilaEstados::~PilaEstados() {
    estados.clear();
}

//Actualiza el ultimo estado de la pila
void PilaEstados::update(Tiempo timeElapsed, bool input[]) {
    estados[estados.size()-1]->update(timeElapsed, input);
    relojDibujar->restart();
}

//Manda a todos los estados a dibujar, empezando por el primero
void PilaEstados::dibujar(float percentTick, int updateTickTime) {
    for(iterator=0; iterator<estados.size(); iterator++) {
        if(iterator == estados.size()-1) {
            estados[iterator]->dibujar(percentTick, updateTickTime);
        }
        else {
            float percent = std::min(1.f, (float)tiemposDibujar[iterator].milisegundos()/updateTickTime);
            estados[iterator]->dibujar(percent, updateTickTime);
        }
    }
}

//Elimina el ultimo estado de la pila
void PilaEstados::eliminarUltimoEstado() {
    estados.pop_back();
    if(estados.size() > 0) {
        tiemposDibujar.pop_back();
    }
}

//Anyade un estado a la pila, sera el que reciba los inputs y haga el update
void PilaEstados::anyadirEstado(EstadoJuego* estado) {
    estados.push_back(estado);
    if(estados.size() > 1) {
        tiemposDibujar.push_back(relojDibujar->getElapsedTime());
    }
}

//Cambia el ultimo estado de la pila por el pasado por parametro
void PilaEstados::modificarUltimoEstado(EstadoJuego* estado) {
    if(estados.size() > 0) {
        estados[estados.size()-1] = estado;
    }
    else {
        estados.push_back(estado);
    }
}

//Devuelve el estado activo, el que esta en la cima de la pila de estados
EstadoJuego* PilaEstados::getEstadoActivo() {
    EstadoJuego* estado = nullptr;

    if(estados.size() > 0) {
        estado = estados[estados.size()-1];
    }

    return estado;
}