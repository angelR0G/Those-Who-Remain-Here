#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "Evento.h"
#include "Tiempo.h"
#include "Vista.h"
#include "Sprite.h"
#include "Texto.h"
#include "ArrayVertices.h"

using namespace std;

class VentanaRender {
    public:
        static VentanaRender* Instancia(int, int, string);
        static VentanaRender* Instancia();
        ~VentanaRender();
        void setVista(Vista*);
        void limpiar();
        void mostrar();
        void dibujar(class Sprite*);
        void dibujar(class Texto*);
        void dibujar(class ArrayVertices*);
        bool isOpen();
        void cerrar();
        bool pollEvento(Evento*);
        vector<float> mapPixelToCoords(vector<int>);
        void setFramerate(unsigned int);
        vector<unsigned int> getSize();
        vector<unsigned int> getSizeReal();
        vector<int> getPosicionRaton();
        bool getFade();
        bool getPantallaNegro();
        void fadeIn(int);
        void fadeOut(int);
        void actualizarFade();
        void dibujarFade();
        void setMira(bool);
        void setTiempoFade(int);
        int getTiempoFade();
        vector<float> getCentroVista();
        vector<float> getSizeVista();

    protected:
        VentanaRender(int, int, string);
    private:
        sf::RenderWindow ventana;               //RenderWindow de SFML
        static VentanaRender* pventanaRender;   //Referencia a la ventana
        int ancho;                              //Ancho de la ventana
        int alto;                               //Alto de la ventana
        string nombre;                          //Nombre de la ventana de juego
        bool fade;                              //Indica si se esta haciendo fade
        bool pantallaNegro;                     //Indica si la pantalla esta en negro

        Textura* texFundido;                    //Textura para el fundido
        Sprite* spriteFundido;                  //Imagen en negro para hacer el fundido
        int estadoFundido;                      //Valor alpha del fundido de 0 a 255
        int velFundido;                         //Velocidad a la que se hace el fundido
        int tiempoFade;                         //Tiempo en el que se realiza el fade
};