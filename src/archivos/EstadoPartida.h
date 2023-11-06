#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "EstadoJuego.h"
#include "EstadoInventario.h"
#include "EstadoMenuJuego.h"
#include "Sala.h"
#include "Proyectil.h"
#include "Explosion.h"
#include "NubeVeneno.h"
#include "Jugador.h"
#include "Hud.h"
#include "GeneradorMazmorras.h"
#include "Particula.h"
#include "PortalSalaADesafio.h"
#include "PortalDesafio.h"
#include "PortalJefe.h"

#include "Objeto.h"
#include "FabricaObjetos.h"
#include "Material.h"
#include "FabricaMateriales.h"

#include "FabricaElementosEntorno.h"
#include "RecuperadorVida.h"

//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Tiempo.h"

using namespace std;

class EstadoPartida: public EstadoJuego{

    public:
        static EstadoPartida* Instancia();

        ~EstadoPartida();

        void dibujar(float, int);                           //Render con interpolacion entre frames
        void update(Tiempo, bool[19]);                      //Update del juego

        void detenerTiempo();                               //
        void reanudarTiempo();                              //
        void inicializar(Jugador*, int);                    //
        void anyadirProyectil(Proyectil*);                  //
        void eliminarProyectil(Proyectil*);                 //
        int getAlmacenProyectilesSize();                    //

        void anyadirExplosion(class Explosion*);
        void eliminarExplosion(class Explosion*);
        int getAlmacenExplosionesSize();

        void anyadirNubeVeneno(class NubeVeneno*);
        void eliminarNubeVeneno(class NubeVeneno*);
        int getAlmacenNubeVenenoSize();

        class Jugador* getJugador();                        //Devuelve la referencia al jugador
        Sala* getSalaActual();                              //Devuelve la referencia a la sala actual
        vector<vector<Sala*>>* getSalas();                  //Devuelve la referencia al vector con todas las salas de la mazmorra
        class Hud* getHud();                                //Devuelve la referencia del hud
        void update();
        void dibujar();
        void input();
        void cambiarEstado(int);
        vector<int> cambiarSala(int);
        Vista* getVista();
        Textura* getTexturas(int);
        void setCambiandoSala(int);
        void resetEnemigos();

        void reiniciarEstadoPartida();
        Sala* getSalaDesafio();
        void setSalaActual(Sala*);
        ElementoEntorno* getPortalDesafio();
        void cambioSala();
        void cargarTutorial();

    protected:
        EstadoPartida();                        //Constructor del estado de la partida
    private:
        static EstadoPartida* pEstadoPartida;   //Referencia al estado
        Vista* vista;                           //Vista de la camara
        vector<float> centroVista;              //Indica el centro de la vista

        vector<Textura*> texs;                  //Vector con texturas creadas por el estado
        Sprite* mira;
        class Jugador* jugador;                 //Referencia al jugador
        vector<vector<Sala*>> mazmorra;         //Conjunto de salas que conforman la mazmorra
        vector<int> salaInicio;                 //Posicion de la sala de inicio
        Sala* salaActual;                       //Sala donde se encuentra el jugador
        vector<int> posSalaActual;              //Posicion de la sala actual
        vector<Proyectil*> almacenProyectiles;  //Vector con todos los proyectiles disparados en la sala
        vector<Explosion*> almacenExplosiones;  //Vector con todas las explosiones de la sala
        vector<NubeVeneno*> almacenNubeVeneno;  //Vector con todas las nubes de veneno de la sala
        vector<int> armasDesbloqueadas;         //Armas desbloqueadas
        class Hud* hud;                         //Referencia al hud

        int cambiandoSala;
        int siguienteArea;
        vector<int> mejoras;                    //Vector con las mejoras compradas
        int esencias;
        Sala* salaDesafio;
        Sala* salaJefe;
        ElementoEntorno* portalDesafio;
        bool tutorialHecho;                     //Indica si se ha completado el tutorial

        ControladorSonido* sonidos;
};