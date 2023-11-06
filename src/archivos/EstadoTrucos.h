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
#include "../fachada/VentanaRender.h"

using namespace std;

class EstadoTrucos: public EstadoJuego{
    public:
        static EstadoTrucos* Instancia();
        ~EstadoTrucos();
        void resetearTrucos();
        bool hoverRaton(float, float, bool*);
        void update(Tiempo, bool[19]);
        void dibujar(float, int);
        void cambiarEstado(int);
        void resetTiempoEspera();
        void resetJefe();
        int getZonaSeleccionada();
        int getJefeSeleccionado();
        int getExtraSeleccionado();
        bool getVidaInf();
    protected:
        EstadoTrucos();
    private:
        static EstadoTrucos* pEstadoTrucos;
        vector<Textura*> texs;
        vector<Sprite*> sprites;
        Fuente *fuente;
        vector<Texto*> textos;
        int tiempoEspera;                           //Tiempo que debe pasar antes de poder cerrar la tienda
        int hover;
        int seleccionado;
        int zonaSeleccionada;
        int jefeSeleccionado;
        int armaSeleccionada;
        int extraSeleccionado;
        bool vidaInfinita;

        //Para los marcos
        vector<Sprite*> marco;
};