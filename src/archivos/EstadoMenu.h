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

#define MAX_MENU 4
#define MAX_ITEMS 5

using namespace std;


class EstadoMenu : public EstadoJuego{
    public:
        static EstadoMenu* Instancia();
        ~EstadoMenu();

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

    protected:
        EstadoMenu();                                           //Constructor del menu

    private:
        static EstadoMenu* pEstadoMenu;                         //Referencia al estado

        int tiempoRetraso;                                      //Tiempo de actualizacion update
        int seleccionItemIndex;                                 //Opcion seleccionada
        Fuente* fuente;                                         //Fuente del menu
        Texto* menu[MAX_MENU][MAX_ITEMS];                       //Opciones del menu
        Texto* titulo;                                          //Titulo
        Sprite* fondo;                                          //Fondo
        Textura* fondotx;                                       //Textura Fondo
        vector<int> col_sel,col_normal,col_aura;                //Colores de opciones
        vector<int> numItems;                                   //Numero de items en cada submenu (principal, opciones)
        bool iniciandoPartida;

        //Control de menus
        int menuActivo;                                         //Menu activo

        //Otros menus
        Sprite* fondo2;                                         //Fondo2
        Textura* fondo2tx;                                      //Textura Fondo2

        //Controles
        Sprite* controlesS;                                     //Sprite controles
        Textura* controlesT;                                    //Textura controles

        //Creditos
        Texto* cred[5];                                         //Creditos
        Texto* controles[10];                                   //Texto controles
};