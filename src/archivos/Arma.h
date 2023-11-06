#pragma once

#include <iostream>
#include <string>
#include <map>
//Librerias facade
#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"
#include "../fachada/VentanaRender.h"

#include "ControladorSonido.h"
#include "Runa.h"


using namespace std;

class Arma {
    public:
        //Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG, recorte del sprite
        Arma(int, string, string, int, int, float, float, int, float, int, int, vector<int>, float);
        virtual ~Arma();
        void dibujar(float, int);
        void update(vector<float>, int);
        virtual void disparar(int) = 0;
        virtual void recargar() = 0;
        virtual void setMunicion(int) = 0;
        void updateCoolDown(int);
        void coolDownRecarga(int);
        bool puedeDisparar();
        void setPosArma(vector<float>, int, bool);
        void vaciarCargador();
        virtual int getMunicion() = 0;
        int getMunicionRestante();
        string getNombre();
        int getId();
        vector<bool> getRunasPequenyas();
        vector<bool> getRunasGrandes();
        void setRunas(int, bool, bool);
        void setCooldown(bool);
        float getCooldown();
        float getCadencia();
        int getDanyoExtra();
        void setDanyoExtra(int);
        float getDanyoTotal();
        Sprite* getSprite();
        vector<Runa*> getRunas();

        //Metodos para recuperar la informacion de las runas
        virtual vector<string> getNombreRunas() = 0;
        virtual vector<string> getDescRunasNormales() = 0;
        virtual vector<string> getDescRunasGrandes() = 0;
        virtual vector<int> getMaterialesRuna(int) = 0;
        virtual vector<int> getRecorteSpriteRuna(int) = 0;
        virtual vector<string> getTextoMejoras() = 0;
        virtual vector<int> getMejorasArma() = 0;
        virtual void setMejorasArma(int, int) = 0;
        virtual vector<int> getMaterialesMejora(int, int) = 0;

        int getCantidadPequenyas();
        int getCantidadGrandes();

        //Metodos para las mejoras del arma
        int getMejorasDanyo();
        int getMejorasAlcance();
        int getMejorasCadencia();
        int getMejorasPrecision();
        int getMejorasRadio();
        int getMejorasRecarga();
    protected:
        Sprite* spriteArma;             //Sprite del arma
        Textura* texArma;               //Textura del arma
        Sprite* recargaIcono;           //Sprite del icono de recarga
        Textura* texRecarga;            //Textura del icono de recarga
        string nombre;                  //Nombre del arma
        int id;                         //Identificador del arma
        int increRot;                   //Incremento del angulo de giro para el sprite de recarga
        int danyo;                      //Danyo del arma
        int danyoExtra;                 //Porcentaje de danyo adicional que inflinge el arma
        int alcance;                    //Alcance maximo de las balas
        float cadencia;                 //Cadencia del arma
        float recarga;                  //Tiempo que tarda en recargar
        float cooldownRecarga;          //Cooldown del tiempo de recarga
        int municion;                   //Cantidad de municion del arma
        int municionOriginal;           //Cantidad de municion sin modificadores
        float cooldownAtaque;           //Cooldown entre disparos
        int balasDisparadas;            //Contador con las balas que quedan por disparar hasta tener que recargar
        float precision;                //Precision del arma
        int runaP;                      //Cantidad de runas pequenyas que tiene el arma
        int runaG;                      //Cantidad de runas grandes que tiene el arma
        vector<Runa*> runasArma; //Runas que tiene el arma
        bool recargando;                //Indica si el arma se esta recargando
        float escala;                   //Escala del esprite
        vector<float> posArma;          //Posciion actual del arma 
        vector<float> posAntiguasArma;  //Posicion antigua del arma para interpolar

        //Vectores con las mejoras activas del arma: Fuego, trueno, hielo, las 3 ultimas las especificas de cada una
        vector<bool> pequenyas;            //Mejoras en runas pequenyas
        vector<bool> grandes;              //Mejoras en runas grandes

        //Vector con las mejoras del arma:0->danyo, 1->alcance, 2->cadencia, 3->precision, 4->radio, 5->recarga
        vector<int> mejorasArma;

        //sonido
        ControladorSonido* sonidos; 


};