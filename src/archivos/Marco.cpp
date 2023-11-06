#include "Marco.h"
const int TILE_MARCO        = 16; //Tamanyo para el marco

Marco* Marco::pMarco = 0;

Marco* Marco::Instancia(){
    if(pMarco == 0){   //Primera llamada
        pMarco = new Marco(); //Se crea la instancia de Marco
    }
    return pMarco;
}


Marco::Marco(){
    texMarco = new Textura("resources/sprites/generales/bordes.png");
    //Para los marcos
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 2*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 10*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 5*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 13*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->setOrigin(TILE_MARCO/2, TILE_MARCO/2);
    }
}

Marco::~Marco(){
    while(marco.size() > 0) {
        if(marco[marco.size()-1] != nullptr) {
            delete marco[marco.size()-1];
        }
        marco.pop_back();
    }
    delete texMarco;
}

vector<Sprite*> Marco::getMarco(){
    return marco;
}

Textura* Marco::getTextura(){
    return texMarco;
}