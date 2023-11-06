#pragma once

#include "Animacion.h"
#include "Enemigo.h"
#include "EstadoPartida.h"
#include "../fachada/Sprite.h"

using namespace std;

class NubeVeneno : public Detenible{
    public:
        NubeVeneno(Sprite*, vector<float>, int, int, int, int);
        ~NubeVeneno();
        void update(Tiempo);
        void dibujar();
        bool getDestruir();
        int getDuracion();
    private:
        Sprite* spriteVeneno;           //Sprite con la nube de veneno
        vector<float> posicion;         //Posicion de la nube
        bool destruir;
        int maxTiempo;                  //Indica el tiempo en milisegundos que lleva la nube
        int tiempo;                     //Indica cuando hace danyo a los enemigos
        int danyoJugador;               //Danyo por segundo de la nube al jugador
        int danyoEnemigo;               //Danyo por segundo de la nube a los enemigos
        int duracion;                   //Duracion de la nube
        int radio;                      //Radio de la nube
        Animacion* anim;                //Para animar
};