#pragma once

#include "../fachada/Reloj.h"
#include "../fachada/Tiempo.h"

class Detenible {
    public:
        Detenible();

        ~Detenible();

        void detener();
        void reanudar();
        void reiniciarTiempoAntes();
        bool getDetenido();

    protected:
        Reloj relojDetenible;           //Reloj que utilizan los objetos detenibles
        Tiempo tiempoAntes;             //Tiempo que ha pasado antes de detener el tiempo
        bool detenido;                  //Indica si el objeto esta detenido
};