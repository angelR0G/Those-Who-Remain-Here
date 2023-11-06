#include "PNJ.h"


const int TILE_MARCO = 16;
using namespace std;

PNJ::PNJ(){
    fuente = new Fuente("resources/sprites/generales/Chonkly.otf");
    interactuando = false;
    texMarco = new Textura("resources/sprites/generales/bordes.png");
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 2*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 10*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 5*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 13*TILE_MARCO, TILE_MARCO, TILE_MARCO));
}

PNJ::~PNJ(){
    
    posicion.clear();
    delete spritePNJ;
    delete animacion;

    while(marco.size() > 0) {
        if(marco[marco.size()-1] != nullptr) {
            delete marco[marco.size()-1];
        }
        marco.pop_back();
    }
    delete texMarco;
    delete texto;
    delete fuente;
}

void PNJ::dibujar(){
    spritePNJ->draw();
}

void PNJ::update(int elapsedTime){
    /*if(estadoAnimacion == PNJEstados::PNJ_Quieto){
        animacion->play("PNJQuieto", elapsedTime);
    }*/
}

vector<int> PNJ::getPosicion(){
    return posicion;
}

Sprite* PNJ::getSprite(){
    return spritePNJ;
}

//Devuelve si el jugador esta interactuando con un PNJ
bool PNJ::getInteractuando(){
    return interactuando;
}
