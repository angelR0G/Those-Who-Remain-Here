#pragma once

#include <iostream>

#include "Inventario.h"
#include "Arma.h"
#include "FabricaArmas.h"
#include "Hud.h"
#include "EstadoPartida.h"
#include "Animacion.h"
#include "Inventario.h"
#include "ObjetoSala.h"
#include "Gancho.h"
#include "ControladorSonido.h"
#include "ParticulaDireccional.h"

//Librerias facade
#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/Buffer.h"
#include "../fachada/Sonido.h"
using namespace std;
const int TAM_INPUT1 = 19;

//Enumerado con los estados del jugador para las animaciones
enum JugadorEstados {
    Quieto      = 0, 
    Izquierda   = 1, 
    Derecha     = 2, 
    Salto       = 3, 
    Caida       = 4
};

class Jugador {
    public:
        Jugador();
        ~Jugador();
        Jugador getJugador();

        //Para actualizar el HUD

        int getVida();                                              //Devuelve la vida del jugador
        int getEscudo();                                            //Devuelve el escudo del jugador
        int getVidaMax();                                           //Devuelve la vida maxima del jugador
        float getRoboVida();                                        //Devuelve la barra de robo de vida
        int getCargaTiempo();                                       //Devuelve el estado de la barra de carga del poder
        int getCargaTiempoMax();                                    //Devuelve el tiempo de carga maximo de la barra del poder

        void danyarVida(int);                                       //Actualiza la vida
        void recuperaVida(int);                                     //Actualiza la vida
        void aumentarEscudo(int);                                   //Anyade escudo a la cantidad actual

        Sprite* getSprite();                                        //Devuelve el sprite del jugador
        void update(int, bool[TAM_INPUT1]);                         //Update de jugador
        void updateMuerto(int);
        void dibujar(float);                                   //Metodo render del jugador
        void actualizarAnimacion(float);
        int getPuedeBajar();
        void comprobarColisiones();
        vector<float> getPosNueva();
        void resetearSalto();
        void saltar();
        float getCooldownHabilidad();                               //Devuelve el cooldown de la habilidad del personaje en segundos
        Arma* getArma();
        void setArma(Arma*);
        void setInmuneGolpes(int);
        void entradoEnNuevaSala();
        int getCargaTiempoVel();
        void setCargaTiempoVel(int);
        bool getMuerto();
        void recuperarObjetoAtemporal();
        bool getLinterna();
        void setLinterna(bool);
        int getRalentizacion();
        void setRalentizacion(int);
        float getVelMax(int);
        void setMaza(bool);
        bool getMaza();
        bool getSalto();
        void setMuerto(bool);


        //Devuelve el inventario
        Inventario* getInventario();
        void recuperarHud();

        void aumentarRoboVida(float);

        void setPosicion(vector<float>, bool);
        void setSpawn(vector<float>);
        vector<int> getCoordsPortal();
        void setCoordsPortal(vector<int>);

        void detenerTiempo();
        void reanudarTiempo();
        bool getTiempoDetenido();

        //Metodos para aplicar mejoras
        void setVidaMax(int);
        void setHabilidadMax(int);
        void setHabilidadVel(int);

        class Hud* getHud();

        bool getSaltando();


        //Gancho 
        Gancho* getGancho();
        bool getDisparo();        
        void moverGancho();
        void setDisparo(bool);
        void retroceso();
        float timer;

        void actualizarMaza();
       

    private:
        Gancho* gancho;
        vector<Gancho*> ganchos;
        bool disparo;
        bool moverGanchobool;
        vector<float>  moveVec;
        float VecLength;
        class Inventario* inventario;         //Referencia al inventario
        class Arma* arma;                     //Referencia al arma equipada
        class Hud* hud;                       //Referencia al hud
        
        Textura* tex;                         //Textura del personaje
        Textura* texMaza;                     //Textura de la maza
        Sprite* sprite;                       //Sprite del personaje
        Sprite* spriteMaza;                   //Sprite de la maza
        float rotacionMaza;
        float dirMazamouse;

        
        //Variables para el hud
        int vida;                             //Vida actual
        int escudo;                           //Escudo actual
        int vidaMax;                          //Vida maxima del pesonaje
        float roboVida;                       //Barra de robo de vida
        int cargaTiempo;                      //Cooldown de la habilidad
        int cargaTiempoMax;                   //Carga maxima de la habilidad
        int cargaTiempoVel;                   //Velocidad con la que se recarga la habilidad

        //Velocidad y coordenadas del personaje
        vector<float> velocidad;
        vector<float> coordsNuevas;
        vector<float> coordsAntiguas;
        vector<float> coordsPuerta;           //Si el personaje se sale del mapa vuelve a estas coords
        vector<int> coordsPortal;           //Coordenadas del portal a un desafio al que el jugador entra

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
        int esperaInteraccion;                  //Cantidad de milisegundos desde la ultima interaccion (Tecla E o TAB)
        int invulnerable;                       //Tiempo restante de invulnerabilidad al recibir danyo
        int inmuneGolpes;                       //Cantidad de golpes que puede recibir sin sufrir danyo
        int contadorPart;                       //Contador utilizado para generar particulas
        int recuperacionPoder;
        vector<float> posDetenerTiempo;         //Posicion en la que se ha detenido el tiempo por ultima vez
        bool muerto;
        bool linterna;                          //Indica si ha conseguido la linterna
        bool maza;                              //Indica si ha conseguido la maza
        bool mazaActivada;                      //Indica si esta usando la maza
        bool zoom;                              //Indica si se ha hecho zoom
        bool zoommantenido;                     //Indica si se esta manteniendo la tecla del zoom
        int ralentizacion;                      //Tiempo de ralentizacion del jugador
        vector<float> posMaza;                  //Posicion de la maza
        vector<float> dirMaza;                  //Direccion de la maza
        int recuperacionMaza;                   //Tiempo restante antes de poder usar la maza de nuevo
        
        //Timers para el salto y la habilidad
        Reloj* relojSalto;
        Reloj* relojHabilidad;

        //Para la animacion
        short estadoAnimacion;
        Animacion* animacion; 
        ControladorSonido* sonidos; 


};