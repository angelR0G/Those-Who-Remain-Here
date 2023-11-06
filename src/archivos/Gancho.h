#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


//Librerias facade
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/VentanaRender.h"

using namespace std;

class Gancho {
    public:
      
        Gancho();
        ~Gancho();
        void crear();
        void update( );
        void draw(float, int);
        void comprobarColisiones();
        vector<float> getPosition();
        bool getColision();
        bool getVuelta();
        int colisionar(vector<int>);
        int getCooldown();
        void setDistancia(float);




    private:    
        Sprite* proy;
        Textura* tex;
        Sprite* cuerdesita;
        vector<Sprite*> hilo;
        Textura* texCuerdesita;
        vector<float> vel;
        vector<float> dirGan;
        vector<float>  dirMouse;
        vector<float>  mouse;
        vector<float>  moveVec;
        vector<float> posicion;                    
        vector<float> posicionAnterior;             
        float VecLength;
        vector<float>  direcRope;
        float DirLength;
        bool colision;
        bool vuelta;
        float distAnt;
        float dis;
        float cont;
        float distGancho;
        float cooldown;


};