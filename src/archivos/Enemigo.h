#pragma once

#include <iostream>
#include <string>
#include <math.h>

#include "Detenible.h"
#include "Animacion.h"
#include "Guardado.h"
#include "ParticulaSimpleCuadrada.h"
#include "ParticulaEsencia.h"
#include "Inventario.h"
#include "ControladorSonido.h"


#include "../fachada/VentanaRender.h"
#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

using namespace std;

class Enemigo : public Detenible {
    public:
        Enemigo(int,int,int,int,int,vector<int>,Textura*,Sprite*);

        virtual ~Enemigo();

        Sprite* getSprite();
        virtual void dibujar(float, int);
        virtual void update(Tiempo);
        virtual void perderVida(int);
        virtual void setPosicion(vector<float>);
        void muerte();
        bool getMuerto();
        virtual void reset();
        int getVelocidad();
        void congelar(bool, int);
        void quemar(bool, int);
        void electrocutar(bool, int);
        void setInventario(Inventario*);
        int getMaterialRecompensa();
        vector<float> getPosicion();
        bool inventarioPoseeObjeto(int);
        
    protected:
        Animacion* animacion;
        Textura* tex;
        Sprite* sprite;                 //Sprite del enemigo
        int danyo;                      //Danyo que provoca el enemigo al jugador
        int vida;                       //Cantidad de vida que aguanta el enemigo
        int velocidad;                  //Velocidad a la que se mueve
        int tiempoRecuperacion;         //Tiempo que tarda en recuperarse del ataque
        bool muerto;
        int esenciasDerrotado;          //Cantidad de esencias que suelta al ser derrotado
        int sangrando;                  //Cantidad de milisegundos que el enemigo sangrara
        int tiempoArdiendo;             //Tiempo que pasa tras la ultima vez que sufre danyo por quemadura
        vector<int> congelado;          //Indica la ralentizacion sufrida y el tiempo que le queda
        vector<int> quemado;            //Indica el danyo que recibe por segundo y el tiempo que le queda
        vector<int> electrocutado;      //Indica el extra de danyo que recibe y el tiempo que le queda
        int contadorPart;               //Contador utilizado para la generacion de particulas
        Inventario* inventario;         //Referencia al inventario del jugador
        vector<int> idMatRecompensa;    //Id de los materiales que puede soltar al morir
        vector<int> colorSprite;        //Color original del sprite
        
        vector<float> posicion;
        vector<float> posicionAnterior;

        //sonido
        ControladorSonido* sonidos; 
};