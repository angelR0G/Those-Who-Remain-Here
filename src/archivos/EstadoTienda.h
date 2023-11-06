#pragma once

#include <iostream>

#include "EstadoTienda.h"
#include "PilaEstados.h"

//Librerias facade
#include "../fachada/Tiempo.h"
#include "../fachada/Fuente.h"
#include "../fachada/Texto.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/Evento.h"
#include "../fachada/VentanaRender.h"

using namespace std;

class EstadoTienda: public EstadoJuego{
    public:
        static EstadoTienda* Instancia();
        ~EstadoTienda();
        bool hoverRaton(float, float, bool*);
        void update(Tiempo, bool[19]);
        void dibujar(float, int);
        void cambiarEstado(int);
        void resetTiempoEspera();
        void mostrarMejora();
        void cargarMejoras();
        bool mejorar(float, float);
    protected:
        EstadoTienda();
    private:
        static EstadoTienda* pEstadoTienda;
        vector<Textura*> texs;
        vector<Sprite*> sprites;
        Fuente *fuente;
        vector<Texto*> textos;
        vector<int> mejoras;
        int esencias;
        int tiempoEspera;                           //Tiempo que debe pasar antes de poder cerrar la tienda
        int hover;
        int seleccionado;
        int siguienteMejora;

        //Para los marcos
        Textura* texMarco;
        vector<Sprite*> marco;
};