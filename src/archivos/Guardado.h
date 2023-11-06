#pragma once

#include <string>

#include "../tinyxml2/LectorXML.h"
#include "../tinyxml2/EscritorXML.h"

using namespace std;

class Guardado {
    public:
        static Guardado* Instancia();

        ~Guardado();

        bool guardarPartida();
        bool cargarPartida();

        int getEsencias();
        int getMejoraSalud();
        int getMejoraPoderMax();
        int getMejoraPoderVel();
        int getMejoraDistGancho();
        bool getGancho();
        bool getMaza();
        bool getLinterna();
        bool getTutorial();

        void setEsencias(int);
        void setMejoraSalud(int);
        void setMejoraPoderMax(int);
        void setMejoraPoderVel(int);
        void setMejoraDistGancho(int);
        void setGancho(bool);
        void setMaza(bool);
        void setLinterna(bool);
        void setTutorial(bool);

    private:
        Guardado();
        static Guardado* pGuardado;

        int esencias;               //Moneda del juego

        int mejoraSalud;            //Cantidad de mejoras de salud maxima adquirida
        int mejoraPoderMax;         //Cantidad de mejoras de duracion maxima del poder
        int mejoraPoderVel;         //Cantidad de mejoras de velocidad de recuperacion del poder
        int mejoraDistGancho;       //Cantidad de mejoras de distancia maxima del gancho

        bool gancho;                //Indica si se ha desbloqueado el gancho
        bool maza;                  //Indica si se ha desbloqueado la maza
        bool linterna;              //Indica si se ha desbloqueado la linterna

        bool tutorial;              //Indica si ha realizado el tutorial o no
};