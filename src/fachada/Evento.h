#pragma once

#include "SFML/Graphics.hpp"

using namespace std;

//Enumerado tipo de evento
enum EventoTipo {
    Tipo_Defecto        = 0,
    Closed              = 1,
    KeyPressed          = 2,
    KeyReleased         = 3,
    MouseButtonPressed  = 4,
    MouseButtonReleased = 5,
    MouseMoved          = 6,
    Resized             = 7
};

//Enumerado tecla
enum EventoTecla {
    Tecla_Defecto = 0,
    Left          = 1,
    Down          = 2,
    Right         = 3,
    Up            = 4,
    A             = 5,
    S             = 6,
    D             = 7,
    W             = 8,
    Space         = 9,
    Esc           = 10,
    Tab           = 11,
    Q             = 12,
    F             = 13,
    E             = 14,
    R             = 15,
    Enter         = 16,
    Z             = 17
};

//Enumerado boton raton
enum EventoRaton {
    Raton_Defecto = 0,
    Raton_Left    = 1,
    Raton_Right   = 2,
    Middle        = 3
};

class Evento{
    public:
        Evento();
        ~Evento();

        sf::Event* getEvento();
        EventoTipo getTipo();
        EventoTecla getTecla();
        EventoRaton getRaton();
    
    private:
        sf::Event evento;
};
