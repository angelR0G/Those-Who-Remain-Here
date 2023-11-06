#include <iostream>
#include <math.h>

#include "PilaEstados.h"
#include "Particula.h"
//Librerias facade
#include "fachada/VentanaRender.h"
#include "fachada/Evento.h"
#include "fachada/Reloj.h"
#include "fachada/Tiempo.h"

const int UPDATE_TICK_TIME = 40;
const int FRAMERATE = 60;
const int ANCHO_VENTANA = 1920;
const int ALTO_VENTANA = 1080;
const int TAM_INPUT = 20;

void inputs(bool (*input)[TAM_INPUT]){
    VentanaRender *ventana = VentanaRender::Instancia();
    Evento *evento = new Evento();
    while(ventana->pollEvento(evento)){
        switch (evento->getTipo())
        {
            case EventoTipo::Closed:
                ventana->cerrar();
                break;
            case EventoTipo::KeyPressed:
                switch (evento->getTecla()) {
                    case EventoTecla::Left:
                    (*input)[0] = true;
                    break;
                    case EventoTecla::Down:
                    (*input)[1] = true;
                    break;
                    case EventoTecla::Right:
                    (*input)[2] = true;
                    break;
                    case EventoTecla::Up:
                    (*input)[3] = true;
                    break;
                    case EventoTecla::A:
                    (*input)[4] = true;
                    break;
                    case EventoTecla::S:
                    (*input)[5] = true;
                    break;
                    case EventoTecla::D:
                    (*input)[6] = true;
                    break;
                    case EventoTecla::W:
                    (*input)[7] = true;
                    break;
                    case EventoTecla::Space:
                    (*input)[8] = true;
                    break;
                    case EventoTecla::Esc:
                    (*input)[9] = true;
                    break;
                    case EventoTecla::Tab:
                    (*input)[10] = true;
                    break;
                    case EventoTecla::Q:
                    (*input)[11] = true;
                    break;
                    case EventoTecla::F:
                    (*input)[12] = true;
                    break;
                    case EventoTecla::E:
                    (*input)[13] = true;
                    break;
                    case EventoTecla::R:
                    (*input)[14] = true;
                    break;
                    case EventoTecla::Enter:
                    (*input)[15] = true;
                    break;
                    case EventoTecla::Z:
                    (*input)[19] = true;
                    break;
                }
                break;
            case EventoTipo::KeyReleased:
                switch (evento->getTecla()) {
                    case EventoTecla::Left:
                    (*input)[0] = false;
                    break;
                    case EventoTecla::Down:
                    (*input)[1] = false;
                    break;
                    case EventoTecla::Right:
                    (*input)[2] = false;
                    break;
                    case EventoTecla::Up:
                    (*input)[3] = false;
                    break;
                    case EventoTecla::A:
                    (*input)[4] = false;
                    break;
                    case EventoTecla::S:
                    (*input)[5] = false;
                    break;
                    case EventoTecla::D:
                    (*input)[6] = false;
                    break;
                    case EventoTecla::W:
                    (*input)[7] = false;
                    break;
                    case EventoTecla::Space:
                    (*input)[8] = false;
                    break;
                    case EventoTecla::Esc:
                    (*input)[9] = false;
                    break;
                    case EventoTecla::Tab:
                    (*input)[10] = false;
                    break;
                    case EventoTecla::Q:
                    (*input)[11] = false;
                    break;
                    case EventoTecla::F:
                    (*input)[12] = false;
                    break;
                    case EventoTecla::E:
                    (*input)[13] = false;
                    break;
                    case EventoTecla::R:
                    (*input)[14] = false;
                    break;
                    case EventoTecla::Enter:
                    (*input)[15] = false;
                    break;
                    case EventoTecla::Z:
                    (*input)[19] = false;
                    break;
                }
                break;
            case EventoTipo::MouseButtonPressed:
                switch (evento->getRaton())
                {
                    case EventoRaton::Raton_Left:
                        (*input)[16] = true;
                        break;
                    case EventoRaton::Raton_Right:
                        (*input)[17] = true;
                        break;
                    case EventoRaton::Middle:
                        (*input)[18] = true;
                        break;
                    default:
                        break;
                }
                break;
            case EventoTipo::MouseButtonReleased:
                switch (evento->getRaton())
                {
                    case EventoRaton::Raton_Left:
                        (*input)[16] = false;
                        break;
                    case EventoRaton::Raton_Right:
                        (*input)[17] = false;
                        break;
                    case EventoRaton::Middle:
                        (*input)[18] = false;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}


int main() {
    VentanaRender *ventana = VentanaRender::Instancia(ANCHO_VENTANA, ALTO_VENTANA, "Those Who Remain Here");
    ventana->setFramerate(FRAMERATE);
    Reloj updateReloj;
    bool input[TAM_INPUT];   //Vector que contiene los inputs del juego
    for(unsigned i = 0; i<TAM_INPUT;i++){
        input[i] = false;
    }

    //Creacion del vector de estados
    PilaEstados* estados = PilaEstados::Instancia();

    //Empieza con el estado del menu principal
    estados->anyadirEstado(EstadoMenu::Instancia());
    

    Tiempo timeElapsed;
    while(ventana->isOpen()){
        inputs(&input);

        //Update
        if(updateReloj.getElapsedTime().milisegundos()>UPDATE_TICK_TIME){
            timeElapsed = updateReloj.restart();
            //Solo actualiza los estados si no esta haciendo fade
            if(!ventana->getFade()) {
                estados->update(timeElapsed, input);
            }
            else {
                //Si esta haciendo fade actualiza el valor
                ventana->actualizarFade();
            }

            if(estados->getEstadoActivo() != nullptr && estados->getEstadoActivo() != EstadoInventario::Instancia()) {
                //Si no esta en el inventario, actualiza las particulas
                Particula::updateParticulas(timeElapsed);

                //Si existe el jugador, actualiza su posicion en las particulas
                if(EstadoPartida::Instancia() != nullptr && EstadoPartida::Instancia()->getJugador() != nullptr)
                    Particula::setPosJugador(EstadoPartida::Instancia()->getJugador()->getPosNueva());
            }
        }

        //Render
        ventana->limpiar();
        if(!ventana->getPantallaNegro()) {
            float percentTick = std::min(1.f, (float)updateReloj.getElapsedTime().milisegundos()/UPDATE_TICK_TIME);
            estados->dibujar(percentTick, UPDATE_TICK_TIME);
            
            if(estados->getEstadoActivo() != nullptr && estados->getEstadoActivo() != EstadoInventario::Instancia()) {
                //Si no esta en el inventario, dibuja las particulas
                Particula::dibujarParticulas(percentTick);
            }
        }
        //Dibuja el fade de la ventana, si lo tiene
        ventana->dibujarFade();
        ventana->mostrar();
    }
    return 0;
}
