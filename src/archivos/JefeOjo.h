#pragma once

#include <vector>
#include "Enemigo.h"
#include "EstadoPartida.h"
#include "ProyectilRayoOjo.h"

using namespace std;

enum EstadoJefeOjo {JefeOjoEsperando, JefeOjoAtaque1, JefeOjoAtaque2, JefeOjoPreparaRayos, JefeOjoDerrotado};

class JefeOjo : public Enemigo {
    public:
        JefeOjo();

        ~JefeOjo();

        void update(Tiempo);
        void dibujar(float, int);
        void perderVida(int);
        void setPosicion(vector<float>);
        void comprobarColisiones();
    private:
        int ataque1;                                //Cantidad de veces que realiza el primer ataque desde el ultimo ataque potente
        int ataque2;                                //Cantidad de veces que realiza el segundo ataque desde el ultimo ataque potente
        bool enfadado;                              //Indica si ha perdido mas de la mitad de la vida
        int contador;                               //Contador para controlar el tiempo entre ataques
        int contadorRayos;
        int numDisparos;                            //Cantidad de veces que ha creado proyectiles durante el ataque
        int tamCirculoBalas;
        vector<float> direccBalas;
        float desplazamiento;
        int direccMov;                              //Direccion del movimiento true derecha
        int contadorLanzaPiedras;                   //Contador para controlar el tiempo entre el spawn de las piedras y cuando se mueven
        bool terminaPiedras;                        //Indica si ha terminado de lanzar las piedras
        bool piedrasLanzadas;                       //Indica si se han lanzado las piedras
        bool preparandoRayos;                       //Indica si esta preparando el ataque de los rayos
        bool barreraColocada;                       //Indica si se ha colocado la barrera al empezar el combate
        vector<vector<int>> barreraInvisible;       //Vector con las posiciones en las que habra una barrera hasta derrotar al jefe
        EstadoJefeOjo estado;                       //Estado actual del jefe
        vector<Textura*> texturas;                  //Vector con las texturas que necesita el jefe
        vector<Sprite*> spritesPiedras;

        Fuente* fuente;                             //Fuente y texto para dibujar el nombre
        Texto* textoNombre;
        Textura* texVida;                           //Textura y sprites de la barra de salud del jefe
        vector<Sprite*> spritesVida;

        Animacion* animacion;
};