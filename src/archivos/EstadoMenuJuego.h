#pragma once

#include <iostream>

#include "EstadoJuego.h"
#include "EstadoPartida.h"
#include "EstadoPueblo.h"
#include "PilaEstados.h"
#include "Guardado.h"

//Librerias facade
#include "../fachada/Tiempo.h"
#include "../fachada/Fuente.h"
#include "../fachada/Texto.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/Evento.h"
#include "../fachada/VentanaRender.h"

#define MAX_ITEMS_SUB 3

using namespace std;


class EstadoMenuJuego : public EstadoJuego{
    public:
        static EstadoMenuJuego* Instancia();
        ~EstadoMenuJuego();

        void arriba();                                          //Moverse arriba con teclado
        void abajo();                                           //Moverse abajo con teclado
        bool raton(float x, float y);                           //Moverse con raton
        void reset(int num);                                    //Cambio de seleccion
        void hover(int num);                                    //Pasar raton por encima
        int  getItemPresionado() { return seleccionItemIndex;}  //Devolver opcion seleccionada
        void aceptar();                                         //Accion a realizar al activar opcion

        void update(Tiempo, bool[19]);
        void dibujar(float, int);
        void cambiarEstado(int);

        void inicializar();

    protected:
        EstadoMenuJuego();                                           //Constructor del menu

    private:
        static EstadoMenuJuego* pEstadoMenuJuego;                         //Referencia al estado

        int tiempoRetraso;                                      //Tiempo de actualizacion update
        int seleccionItemIndex;                                 //Opcion seleccionada
        Fuente* fuente;                                         //Fuente del menu
        Texto* menu[MAX_ITEMS_SUB];                       //Opciones del menu
        Texto* titulo;                                          //Titulo
        Sprite* fondo;                                          //Fondo
        Textura* fondotx;                                       //Textura Fondo
        vector<int> col_sel,col_normal,col_aura;                //Colores de opciones
        int cambiarA;

        float ancho, alto;

        float despVistaX, despVistaY;
};