#pragma once

#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ProyectilBulletHell.h"
#include "ParticulaDireccional.h"

enum EstadoJefeCaballero {JefeCaballeroInvocando, JefeCaballeroEsperando, JefeCaballeroDisparando, JefeCaballeroDerrotado};

class JefeCaballero : public Enemigo {
    public:
        JefeCaballero();

        ~JefeCaballero();

        void update(Tiempo);
        void dibujar(float, int);
        void perderVida(int);
        void activarIntangible(bool, int);
        void generarEnemigos();
        void generarEspiritusOscuros();
        void generarDisparo(bool, bool);
    private:
        bool enfadado;                              //Indica si el enemigo se ha enfadado al perder suficiente vida
        int contador;                               //Milisegundos que lleva el enemigo en el estado actual
        int contadorIntangible;                     //Milisegundos restantes hasta volverse intangible
        bool intangible;                            //Indica si el enemigo puede ser alcanzado por las balas
        int vidaPerdida;                            //Cantidad de vida que ha perdido mientras es tangible
        float anguloParticulas;                     //Angulo con el que genera las particulas
        bool disparoPreparado;                      //Indica si ya puede disparar
        int disparosRestantes;                      //Cantidad de disparos restantes antes de cambiar de estado
        bool espiritusGenerados;                    //Indica si ya se han generado los espiritus oscuros en la sala
        EstadoJefeCaballero estado;                 //Estado actual del jefe
/*
        vector<int> idEnemigos;                     //ID de los enemigos que hace aparecer
        vector<vector<int>> posEnemigos;            //Posiciones en las que hace aparecer a los enemigos
    */
        vector<vector<int>> barreraInvisible;       //Vector con las posiciones en las que habra una barrera hasta derrotar al jefe
        bool barreraColocada;                       //Indica si se ha colocado la barrera al empezar el combate

        Textura* texDisparos;                       //Textura que utilizan los disparos del enemigo
        Fuente* fuente;                             //Fuente y texto para dibujar el nombre
        Texto* textoNombre;
        Textura* texVida;                           //Textura y sprites de la barra de salud del jefe
        vector<Sprite*> spritesVida;

        Animacion* animacion;
};