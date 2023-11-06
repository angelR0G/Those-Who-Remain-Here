#pragma once

#include <vector>
#include "Enemigo.h"
#include "ProyectilBulletHell.h"
#include "EstadoPartida.h"

using namespace std;

enum EstadoJefeRana {JefeRanaEsperando, JefeRanaAtaque1, JefeRanaAtaque2, JefeRanaAtaque3, JefeRanaAbreBoca, JefeRanaAtaqueLengua, JefeRanaCierraBoca, JefeRanaDerrotado};

class JefeRana : public Enemigo {
    public:
        JefeRana();

        ~JefeRana();

        void update(Tiempo);
        void dibujar(float, int);
        void perderVida(int);
        void comprobarColisionPuntaLengua(vector<float>, float);
        void setPosicion(vector<float>);
    private:
        int ataque1;                                //Cantidad de veces que realiza el primer ataque desde el ultimo ataque potente
        int ataque2;                                //Cantidad de veces que realiza el segundo ataque desde el ultimo ataque potente
        int ataque3;                                //Cantidad de veces que realiza el tercer ataque desde el ultimo ataque potente
        bool enfadado;                              //Indica si ha perdido mas de la mitad de la vida
        int contador;                               //Contador para controlar el tiempo entre ataques
        int numDisparos;                            //Cantidad de veces que ha creado proyectiles durante el ataque
        bool colisionLengua;                        //Indica si la lengua a colisionado con algo
        vector<vector<int>> barreraInvisible;       //Vector con las posiciones en las que habra una barrera hasta derrotar al jefe
        bool barreraColocada;                       //Indica si se ha colocado la barrera al empezar el combate
        EstadoJefeRana estado;                      //Estado actual del jefe
        vector<Textura*> texturas;                  //Vector con las texturas que necesita el jefe
        vector<Sprite*> spritesAtaques;             //Vector con los sprites que crea para atacar

        Fuente* fuente;                             //Fuente y texto para dibujar el nombre
        Texto* textoNombre;
        Textura* texVida;                           //Textura y sprites de la barra de salud del jefe
        vector<Sprite*> spritesVida;

        Animacion* animacion;
};