#pragma once

#include "ElementoEntorno.h"
#include "EstadoPartida.h"
#include "ProyectilBulletHell.h"
#include "Animacion.h"

using namespace std;

class PortalDesafio : public ElementoEntorno{
    public:
        PortalDesafio(Textura*, Sprite*);
        ~PortalDesafio();
        void update(Tiempo);
        void dibujar();
        void interactuar();
        bool getInteractuando();
        void generarPatrones(int);
        void generarRecompensa();
    private:
        Textura* texGenerador;          //Textura del portal que genera las balas
        Animacion* animPortal;
        
        //Sprites y animacion para cada portal
        Sprite* generadorBalasNaranja;
        Sprite* generadorBalasAzul;
        Sprite* generadorBalasVerde;
        Sprite* generadorBalasLila;

        Animacion* animNaranja;
        Animacion* animAzul;
        Animacion* animVerde;
        Animacion* animLila;

        vector<bool> mostrarGeneradores;    //Indica cuando se puede dibujar cada uno de los generadores

        vector<float> posElemento;
        bool interactuado;          //Indica si se ha interactuado
        bool activado;              //Indica si se ha activado la sala de desafio
        bool acabado;               //Indica si se ha terminado el desafio
        bool quitarPortal;
        bool primeraVez;
        bool reproduciendoAnim;

        int tamCirculoBalas;        //Numero de balas simultaneas que se disparan en cada update

        int tiempoActivo;           //Tiempo que lleva la sala activada
        int tiempoMaxActivo;        //Duracion del desafio
        int tiempoEntreBalas;       //Cooldown de las balas entre disparos
        int cadenciaBalas;          //Tiempo en ms que hay entre disparo y disparo
        int tiempoEmpezar;          //Temporizador para retardar el inicio del desafio
        int numeroCambios;          //Indica cuantas veces ha cambiado el patron

        Textura* texturaBalas;      //Textura de las balas
        bool recompensaGenerada;    //Indica si se han generado los cofres de la recompensa

        int numRand;                //Numero aleatorio para elegir el bullethell

        //Variables para ir cambiando la posicion y el angulo de las balas
        float angulo;
        vector<float> direcc;
        float desplazamientoX;
        float desplazamientoY;
        int cambioX;
        int cambioY;
        float increBalas;
        float increVel;
        ControladorSonido* contrSonido;

};