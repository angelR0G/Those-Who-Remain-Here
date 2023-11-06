#include "EscudoSuelto.h"

//Constructor. Recibe la posicion, el sprite y su textura.
EscudoSuelto::EscudoSuelto(vector<float> p, Sprite* s, Textura* t):ObjetoSala(p, s, t) {}

//Destructor
EscudoSuelto::~EscudoSuelto() {}

//Recoge el escudo y lo suma a la vida del jugador
void EscudoSuelto::recoger(Inventario* inv) {
    //Comprueba que no ha sido recogido
    if(!recogido) {
        recogido = true;

        Jugador* jugador = EstadoPartida::Instancia()->getJugador();
        jugador->aumentarEscudo(1);
    }
}

//Muestra informacion sobre el escudo
void EscudoSuelto::mostrarInformacion() {}