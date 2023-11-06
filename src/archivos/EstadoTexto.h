#pragma once

#include <iostream>

#include "PilaEstados.h"

//Librerias facade
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Fuente.h"
#include "../fachada/Texto.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/Evento.h"
#include "../fachada/VentanaRender.h"

using namespace std;

class EstadoTexto: public EstadoJuego{
    public:
        static EstadoTexto* Instancia();
        ~EstadoTexto();
        void update(Tiempo, bool[19]);
        void dibujar(float, int);
        void inicializar(int);
        void cambiarEstado(int);
    protected:
        EstadoTexto();
    private:
        static EstadoTexto* pEstadoTexto;

        ArrayVertices* fondo;                       //Array de vertices de fondo
        Textura* texTecla;                          //Texturas y sprites para el fondo y las teclas
        Sprite* spriteTecla;

        Fuente* fuente;
        Texto* texto;                               //Texto que se muestra en pantalla
        vector<string> vStrings;                    //Vector con todos los strings a mostrar
        int stringActual;                           //Indice que indica el string que esta siendo mostrado
        int stringTotales;                          //Cantidad de strings a mostrar
        int caracteresMostrados;                    //Indica la cantidad de caracteres actualmente visibles en el string mostrado
        bool stringMostrado;                        //Indica si el string se ha terminado de mostrar
        bool teclaMantenida;                        //Indica si la tecla esta siendo mantenida para evitar que se salte todo el texto
};