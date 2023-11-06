#pragma once

#include <iostream>


#include "EstadoPartida.h"
#include "Animacion.h"
#include "ObjetoSala.h"

//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
using namespace std;

//Enumerado con los estados del jugador para las animaciones
enum JugadorPuebloEstados {
    QuietoPueblo      = 0, 
    IzquierdaPueblo   = 1, 
    DerechaPueblo     = 2, 
    SaltoPueblo       = 3, 
    CaidaPueblo       = 4
};

class JugadorPueblo {
    public:
        JugadorPueblo();
        ~JugadorPueblo();
        JugadorPueblo getJugador();

        Sprite* getSprite();                                        //Devuelve el sprite del jugador
        void update(int, bool[]);                          //Update de jugador
        void dibujar(float);                                             //Metodo render del jugador
        void actualizarAnimacion(float);
        //void actualizarMovimientoVertical(float, bool[15]);
        int getPuedeBajar();
        void comprobarColisiones();
        vector<float> getPosNueva();
        void resetearSalto();
        void saltar();

        void setPosicion(vector<float>, bool);
        void setSpawn(vector<float>);

        void setInteracutando(bool);

    private:
        Textura* tex;                         //Textura del personaje
        Sprite* sprite;                       //Sprite del personaje

        //Velocidad y coordenadas del personaje
        vector<float> velocidad;
        vector<float> coordsNuevas;
        vector<float> coordsAntiguas;
        vector<float> coordsPuerta;           //Si el personaje se sale del mapa vuelve a estas coords

        //Lado con el que se ha chocado el pesonaje
        vector<float> ladoColisiones;

        float cooldown;
        bool disparado;
        bool salto;                            //Indica si el pesonaje puede saltar
        int saltoCoyote;                       //Cantidad de updates en los que se puede saltar aunque se salga de la plataforma
        bool saltando;                         //Inica si el personaje esta saltando
        float desaceleracion;                  //Desaceleracion del personaje cuanod dejas de moverte
        bool puedeBajar;                       //Indica si se pueden atravesar las plataformas
        bool tocaTecho;                        //Indica si esta tocando el techo
        bool tiempoDetenido;                   //Indica si el tiempo esta detenido
        bool cambiandoSala;
        int esperaInteraccion;                  //Cantidad de milisegundos desde la ultima interaccion (Tecla E)
        bool interactua;
        
        //Timers para el salto y la habilidad
        Reloj* relojSalto;
        //Reloj* relojHabilidad;

        //Para la animacion
        short estadoAnimacion;
        Animacion* animacion;
        ControladorSonido* sonidos; 
};