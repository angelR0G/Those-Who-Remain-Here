#include "PortalJefe.h"

const float DIST_INTER      = 200.0;

PortalJefe::PortalJefe(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true){
    distInteraccion = DIST_INTER;
    interactuado = false;
    
    animacion = new Animacion(sprite);
    animacion->addAnimation("PortalJefeIdle", 30.0, 0, 0, 3, 4, 100, 100);
}

PortalJefe::~PortalJefe(){
}

void PortalJefe::update(Tiempo elapsedTime){
    animacion->play("PortalJefeIdle", elapsedTime.milisegundos());
}

void PortalJefe::interactuar(){
    interactuado = true;
}

bool PortalJefe::getInteractuando(){
    return interactuado;
}