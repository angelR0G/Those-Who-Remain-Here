#include "ObjetoAtemporal.h"

const int TIEMPO_MAXIMO     = 5000;
const int TIEMPO_ESPERA     = 2000;
const string RUTA_TEX       = "resources/sprites/generales/atemporal.png";

Textura* ObjetoAtemporal::texAtemporal = 0;

//Constructor
ObjetoAtemporal::ObjetoAtemporal(Textura* tex, Sprite* s,vector<vector<int>> colE, map<vector<int>, int> colA):ElementoEntorno(tex, s, true) {
    //Crea la textura estatica la primera vez
    if(texAtemporal == 0) {
        texAtemporal = new Textura(RUTA_TEX);
    }

    //Inicializa el resto de variables
    habilitado      = true;
    activado        = false;
    animando        = false;
    tiempoActivo    = 0;
    colEliminar     = colE;
    colAnyadir      = colA;
    sala            = nullptr;

    //Crea el sprite y su animacion
    spriteAtemporal = new Sprite(texAtemporal, 0, 0, 32, 32);
    spriteAtemporal->setOrigin(16.0, 16.0);
    spriteAtemporal->setScale(1.0, 1.0);

    animacion       = new Animacion(spriteAtemporal);
    animacion->addAnimation("Activar", 100, 0, 0, 2, 2, 32, 32);
}

//Destructor
ObjetoAtemporal::~ObjetoAtemporal() {}

void ObjetoAtemporal::update(Tiempo t) {
    if(habilitado && !detenido) {
        if(animando) {
            //Reproduce la animacion al ser activado
            animacion->playOnce("Activar", t.milisegundos());

            tiempoActivo += t.milisegundos();

            //Actualiza los sprites
            float proporcion = min(tiempoActivo/800.0, 1.0);
            spriteAtemporal->setScale(8.0*proporcion, 8.0*proporcion);
            sprite->setColor({0, 255, 0, static_cast<int>(255*proporcion)});

            if(animacion->getFinished()) {
                //Activa el objeto al acabar la animacion
                animando        = false;
                activado        = true;
                tiempoActivo    = 0;

                //Devuelve el sprite a su estado original
                spriteAtemporal->setRect(0, 0, 32, 32);
                spriteAtemporal->setScale(1.0, 1.0);
                sprite->setColor({255, 255, 255, 255});
            }
        }
        else if(activado) {
            //Si esta activado, comprueba que no ha pasado el tiempo maximo
            tiempoActivo += t.milisegundos() + tiempoAntes.milisegundos();

            if(tiempoAntes.milisegundos() > 0) {
                tiempoAntes.setTiempoCero();
            }

            if(tiempoActivo >= TIEMPO_MAXIMO) {
                //Desactiva el objeto
                activado = false;
                sprite->setColor({0, 255, 0, 255});

                //Elimina las colisiones de la sala
                if(sala != nullptr) {
                    sala->eliminarColision(&colEliminar);
                }
            }
        }
        else if(!interactuable) {
            //Si no esta activo se comprueba si puede ser activado de nuevo
            tiempoActivo += t.milisegundos() + tiempoAntes.milisegundos();

            if(tiempoAntes.milisegundos() > 0) {
                tiempoAntes.setTiempoCero();
            }

            //Actualiza el sprite
            float proporcion = min((tiempoActivo-TIEMPO_MAXIMO)/(TIEMPO_ESPERA/2.0), 1.0);
            sprite->setColor({0, 255, 0, 255 - static_cast<int>(255*proporcion)});

            if(tiempoActivo >= TIEMPO_MAXIMO + TIEMPO_ESPERA) {
                //Hace que sea interactuable de nuevo
                interactuable = true;
                animacion->reset();
            }
        }

        //Hace que el sprite gire
        spriteAtemporal->setRotation(spriteAtemporal->getRotation()+1.0);
        relojDetenible.restart();
    }
}

//Trae el objeto de otra linea temporal
void ObjetoAtemporal::interactuar() {
    if(interactuable) {
        //Si se puede interactuar, se activa el objeto
        interactuable   = false;
        tiempoActivo    = 0;
        animando        = true;

        //Anyade las colisiones a la sala
        if(sala != nullptr) {
            sala->anyadirColision(&colAnyadir);
        }
    }
}

//Dibuja los sprites
void ObjetoAtemporal::dibujar() {
    //Comprueba que esta habilitado
    if(habilitado) {
        //Dibuja los sprites correspondientes segun el estado
        if(animando) {
            sprite->draw();
            spriteAtemporal->draw();
        }
        else if(activado || !interactuable) {
            sprite->draw();
        }
        else if(interactuable){
            spriteAtemporal->draw();
        }
    }
}

//Actualiza la posicion del objeto
void ObjetoAtemporal::setPosicion(vector<float> pos) {
    ElementoEntorno::setPosicion(pos);

    if(pos.size() == 2) {
        spriteAtemporal->setPosition(pos[0], pos[1]);
    }
}

//Devuelve si el objeto esta habilitado
bool ObjetoAtemporal::getHabilitado() {
    return habilitado;
}

//Actualiza la sala en la que se encuentra
void ObjetoAtemporal::setSala(Sala* s) {
    sala = s;
}