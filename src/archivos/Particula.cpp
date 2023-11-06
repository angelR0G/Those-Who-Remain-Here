#include "Particula.h"

vector<Particula*> Particula::vParticulas;
vector<float> Particula::posJugador = {0, 0};

Particula::Particula() {
    destruir    = false;

    //Busca una posicion vacia en el vector de particulas para introducirla
    bool encontrada = false;
    for(unsigned int i=0; i<vParticulas.size() && !encontrada; i++) {
        if(vParticulas[i] == nullptr) {
            encontrada      = true;
            vParticulas[i]  = this;
        }
    }
    //Si no la hay la anyade al final
    if(!encontrada) {
        vParticulas.push_back(this);
    }
}

Particula::~Particula() {}

//Actualiza todas las particulas
void Particula::updateParticulas(Tiempo t) {
    for(unsigned int i=0; i<vParticulas.size(); i++) {
        if(vParticulas[i] != nullptr) {
            if(vParticulas[i]->getDestruir()) {
                //Si se va a destruir, la elimina y vacia la posicion en el vector
                delete vParticulas[i];
                vParticulas[i] = nullptr;
            }
            else {
                vParticulas[i]->update(t);
            }
        }
    }
}

//Dibuja todas las particulas
void Particula::dibujarParticulas(float percent) {
    for(unsigned int i=0; i<vParticulas.size(); i++) {
        if(vParticulas[i] != nullptr && !vParticulas[i]->getDestruir()) {
            vParticulas[i]->dibujar(percent);
        }
    }
}

//Elimina todas las particulas, vaciando por completo las posiciones del vector
void Particula::eliminarParticulas() {
    for(unsigned int i=0; i<vParticulas.size(); i++) {
        if(vParticulas[i] != nullptr) {
            delete vParticulas[i];
            vParticulas[i] = nullptr;
        }
    }
}

//Devuelve si la particula esta lista para ser destruida
bool Particula::getDestruir() {
    return destruir;
}

//Actualiza la posicion del jugador
void Particula::setPosJugador(vector<float> p) {
    posJugador = p;
}