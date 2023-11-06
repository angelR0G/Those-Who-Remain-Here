#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "EstadoPartida.h"
#include "../fachada/Sprite.h"
#include "../fachada/VentanaRender.h"
#include "Sala.h"

using namespace std;

class Hud{

    private:
        //Variables de clase para la barra de vida, escudo y habilidad, tambien los sprites necesarios
        int vida;               //Vida Actual
        int vidaMax;            //Vida Maxima
        int escudo;             //Escudo
        int roboVida;           //Cantidad necesaria para curarse
        bool visible;           //Muestra/oculta el minimapa
        Textura* tex;            //Texturas y sprites necesarios
        Sprite* sptVida;
        Sprite* sptMediaVida;
        Sprite* sptVidaCero;
        Sprite* sptEscudo;
        Sprite* sptContornoBarra;
        Sprite* sptBarraRobo;
        Sprite* sptTiempoListo;
        Sprite* sptTiempoCero;
        //Aqui van las necesarias para el minimapa
        Textura* texMap;
        vector<vector<class Sala*>>* salas;
        Sprite* sptMapa;
        Sprite* sptMapaRecompensa;
        Sprite* sptMapaConJugador;
        Sprite* sptMapaConBullet;
        Sprite* sptMapaConBoss;
        //Variables para mostrar municion del arma
        Fuente* fuente;
        Texto* textoMunicion;
        //Variables para las esencias
        /*
        static Texto* cantidadEsencias;
        static Textura* texEsencia;
        static Sprite* sptEsencia;
        */

    public:
        //Constructor
        Hud();
        //Destructor
        ~Hud();
        void dibujar();           //Dibuja la interfaz
        void setVisible(bool);    //Muestra/Oculta el minimapa
        void setVida(int);        //Actualiza la vida actual
        void setEscudo(int);      //Actualiza el escudo actual
        void setRoboVida(int);    //Actualiza el robo de vida y comprueba si hay que curarse
        void setVidaMax(int);     //Actualiza la vida maxima
        static void muestraEsencias();      //Muestra la cantidad de esencias actual

};