#pragma once

#include <iostream>
#include "EstadoJuego.h"
#include "EstadoPartida.h"
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

//Hereda de EstadoJuego
class EstadoInventario: public EstadoJuego{
    public:

        //Metodo para crear instancia, este llama al constructor que es privado
        static EstadoInventario* Instancia();

        //Destructor
        ~EstadoInventario();

        //Metodo para controlar movimiento del raton
        bool raton(float, float);

        //Sirve para los clicks y abrir/cerrar inventario
        void update(Tiempo, bool[19]);

        //Llama al metodo dibujar de inventario
        void dibujar(float, int);

        //Cambio entre estados
        void cambiarEstado(int);

        //WIP
        void setSpritePuntero(Sprite*);
        void resetTiempoEspera();
        void inicializar();
    private:

        //Constructor privado singleton
        EstadoInventario();
        
        //Puntero a objeto EstadoInventario, static para que solo haya uno
        static EstadoInventario* pEstadoInventario;
        class Inventario* inventario;
        Sprite* puntero;
        int tiempoEspera;                           //Tiempo que debe pasar antes de poder cerrar el inventario
};