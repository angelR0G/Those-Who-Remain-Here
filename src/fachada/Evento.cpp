#include "Evento.h"

using namespace std;

//Creador
Evento::Evento(){
}

//Destructor
Evento::~Evento(){

}

//Obtener evento
sf::Event* Evento::getEvento(){
    return &evento;
}

//Obtener EventoTipo
EventoTipo Evento::getTipo(){
    EventoTipo res;
    switch (evento.type) {
        case sf::Event::Closed:
            res = EventoTipo::Closed;
            break;
        case sf::Event::KeyPressed:
            res = EventoTipo::KeyPressed;
            break;
        case sf::Event::KeyReleased:
            res = EventoTipo::KeyReleased;
            break;
        case sf::Event::MouseButtonPressed:
            res = EventoTipo::MouseButtonPressed;
            break;
        case sf::Event::MouseButtonReleased:
            res = EventoTipo::MouseButtonReleased;
            break;
        case sf::Event::MouseMoved:
            res = EventoTipo::MouseMoved;
            break;
        case sf::Event::Resized:
            res = EventoTipo::Resized;
            break;
        default:
            res = EventoTipo::Tipo_Defecto;
            break;
    }

    return res;
}

//Obtener EventoTecla
EventoTecla Evento::getTecla(){
    EventoTecla res;
    if(evento.KeyPressed){
        switch (evento.key.code) {
        case sf::Keyboard::Left:
            res = EventoTecla::Left;
            break;
        case sf::Keyboard::Down:
            res = EventoTecla::Down;
            break;
        case sf::Keyboard::Right:
            res = EventoTecla::Right;
            break;
        case sf::Keyboard::Up:
            res = EventoTecla::Up;
            break;
        case sf::Keyboard::A:
            res = EventoTecla::A;
            break;
        case sf::Keyboard::S:
            res = EventoTecla::S;
            break;
        case sf::Keyboard::D:
            res = EventoTecla::D;
            break;
        case sf::Keyboard::W:
            res = EventoTecla::W;
            break;
        case sf::Keyboard::Space:
            res = EventoTecla::Space;
            break;
        case sf::Keyboard::Escape:
            res = EventoTecla::Esc;
            break;
        case sf::Keyboard::Tab:
            res = EventoTecla::Tab;
            break;
        case sf::Keyboard::Q:
            res = EventoTecla::Q;
            break;
        case sf::Keyboard::F:
            res = EventoTecla::F;
            break;
        case sf::Keyboard::E:
            res = EventoTecla::E;
            break;
        case sf::Keyboard::R:
            res = EventoTecla::R;
            break;
        case sf::Keyboard::Return:
            res = EventoTecla::Enter;
            break;
        case sf::Keyboard::Z:
            res = EventoTecla::Z;
            break;
        default:
            res = EventoTecla::Tecla_Defecto;
            break;
        }
    }

    return res;

}

//Obtener EventoRaton
EventoRaton Evento::getRaton(){
    EventoRaton res;
    if(evento.MouseButtonPressed){
        switch (evento.mouseButton.button) {
        case sf::Mouse::Left:
            res = EventoRaton::Raton_Left;
            break;
        case sf::Mouse::Right:
            res = EventoRaton::Raton_Right;
            break;
        case sf::Mouse::Middle:
            res = EventoRaton::Middle;
            break;
        default:
            res = EventoRaton::Raton_Defecto;
            break;
        }
    }

    return res;
}