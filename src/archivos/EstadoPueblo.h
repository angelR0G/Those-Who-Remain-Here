#pragma once

#include "EstadoJuego.h"
#include "EstadoTienda.h"
#include "EstadoTrucos.h"
#include "JugadorPueblo.h"
#include "Escenario.h"
#include "../PNJ/Tendero.h"
#include "../PNJ/PuntoGuardado.h"
#include "../PNJ/Trucos.h"
#include "../PNJ/Tutorial.h"
#include "ControladorSonido.h"

//Librerias facade
#include "../fachada/VentanaRender.h"

using namespace std;

class EstadoPueblo: public EstadoJuego{
    public:
        static EstadoPueblo* Instancia();
        ~EstadoPueblo();
        void update(Tiempo, bool[]);
        void dibujar(float, int);
        void cambiarEstado(int);
        void inicializar();
        class JugadorPueblo* getJugadorPueblo();
        class Jugador* getJugador();
        Escenario* getPueblo();
        void setEntrandoMazmorra(bool);
        void resetear();
        PNJ* getPNJCercano(vector<float>, float*);

        //Para los trucos
        void setArmaSeleccionada(int);
        void setZonaSeleccionada(int);
        void setJefeSeleccionado(int);

    protected:
        EstadoPueblo();
    private:
        static EstadoPueblo* pEstadoPueblo;   //Referencia al estado
        class JugadorPueblo* jugadorPueblo;
        class Jugador* jugador;
        Textura* tileset;
        Escenario* pueblo;
        Vista* vista;
        vector<float> centroVista;              //Indica el centro de la vista
        bool interactuando;
        bool entrandoMazmorra;
        vector<Textura*> texPNJs;
        vector<PNJ*> pnjs;
        //vector<Plano*> planos;

        //Para los trucos
        int armaSeleccionada;
        int jefeSeleccionado;
        int zonaSeleccionada;

        int trucosActivados;

        ControladorSonido* sonidos; 

        
};