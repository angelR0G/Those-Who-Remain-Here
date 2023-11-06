#include "PortalSalaADesafio.h"

const float DIST_INTER      = 80.0;

PortalSalaADesafio::PortalSalaADesafio(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true){
    distInteraccion = DIST_INTER;
    interactuado = false;
    sprite->setOrigin(32/2, 32/2);
    sprite->setScale(2, 2);
    sprite->setColor({255, 137, 0, 200});
}

PortalSalaADesafio::~PortalSalaADesafio(){
}

void PortalSalaADesafio::update(Tiempo elapsedTime){
    sprite->setRotation(sprite->getRotation()+3.0);
}

void PortalSalaADesafio::interactuar(){
    interactuado = true;
}

bool PortalSalaADesafio::getInteractuando(){
    return interactuado;
}