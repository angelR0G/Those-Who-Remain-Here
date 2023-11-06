#pragma once

#include <vector>

#include "EstadoPartida.h"
#include "EstadoMenu.h"
#include "EstadoTexto.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Reloj.h"

using namespace std;

class PilaEstados {
    public:
        static PilaEstados* Instancia();

        ~PilaEstados();

        void update(Tiempo, bool[]);
        void dibujar(float, int);
        void eliminarUltimoEstado();
        void anyadirEstado(EstadoJuego*);
        void modificarUltimoEstado(EstadoJuego*);
        EstadoJuego* getEstadoActivo();

    protected:
        PilaEstados();
    private:
        static PilaEstados* pPilaEstados;
        vector<EstadoJuego*> estados;           //Pila de estados
        unsigned int iterator;                  //Indice para recorrer los estados
        Reloj* relojDibujar;                    //Se utiliza para recuperar los tiempo para dibujar estados
        vector<Tiempo> tiemposDibujar;          //Vector con los tiempos en los que se deben dibujar los estados que no se encuentran en la cima de la pila
};