#include "Gancho.h"
#include <math.h>

#include "EstadoPartida.h"


using namespace std;

Gancho::Gancho(){
    vel.push_back(0);
    vel.push_back(0);
    moveVec.push_back(0);
    moveVec.push_back(0);
    posicion.push_back(0);
    posicion.push_back(0);
    direcRope.push_back(0);
    direcRope.push_back(0);
    dirMouse.push_back(0);
    dirMouse.push_back(0);
    dirGan.push_back(0);
    dirGan.push_back(0);
    distAnt=0;
    cont=0;
   
    
    colision=false;

    //distancia del gancho 
    distGancho = 500.f;
    hilo.clear();
    tex     = new Textura("resources/sprites/armas/gancho.png");
    proy  = new Sprite(tex, 0, 0, 9, 11);
    proy->setOrigin(2.0, 1.0);
     //  proy->setOrigin(0,20);

    this->proy->setScale(2.f,2.f);

    texCuerdesita     = new Textura("resources/sprites/armas//cuerdesita.png");


    
    

    //cooldown del gancho

    cooldown = 5;

}

void Gancho::crear(){
    vel       = {0.0, 0.0};
    moveVec       = {0.0, 0.0};
    posicion       = {0.0, 0.0};
    direcRope       = {0.0, 0.0};
    dirMouse       = {0.0, 0.0};
   
    distAnt=0;
    cont=0;
    hilo.clear();
    colision=false;
    posicionAnterior.clear();
    vuelta=false;

    EstadoPartida *estado = EstadoPartida::Instancia();
    vector<float> posJugador=estado->getJugador()->getPosNueva();
    VentanaRender *ventana = VentanaRender::Instancia();
    mouse = ventana->mapPixelToCoords(ventana->getPosicionRaton());
    
    //mueve el gancho a la posicion del jugador


    //direccion del gancho hacia el raton  
    dirMouse[0]=(mouse[0]-posJugador[0]);
    dirMouse[1]=(mouse[1]-posJugador[1]);
    dirGan[0]=(dirMouse[0] / (static_cast<float>(sqrt(pow(dirMouse[0], 2) + pow(dirMouse[1], 2)))));
    dirGan[1]=(dirMouse[1] / (static_cast<float>(sqrt(pow(dirMouse[0], 2) + pow(dirMouse[1], 2)))));
    this->proy->setPosition(posJugador[0]+ dirGan[0]*30*3,posJugador[1]+ dirGan[1]*30*3);


    //velocidad del gancho

    vel[0] = dirGan[0] * 30;
    vel[1] = dirGan[1] * 30;

  
     
     posicion[0] =posJugador[0]+vel[0];
     posicion[1] = posJugador[1]+vel[1];

}

Gancho::~Gancho(){
    posicionAnterior.clear();
    posicion.clear();
    vel.clear();
    moveVec.clear();
    if(proy != nullptr){
        delete proy;
        proy = nullptr;
    }
}

void Gancho::update(){
    if(!vuelta){
        comprobarColisiones();
    }
    
    EstadoPartida *estado = EstadoPartida::Instancia();
    vector<float> posJugador=estado->getJugador()->getPosNueva();

/*
    if(colision){
        estado->getJugador()->moverGancho();
    }
    if(vuelta){
         estado->getJugador()->retroceso();
    }*/
    //distancia del gancho al jugador
       
            
    float distancia=sqrt(pow((proy->getPosition()[0] - posJugador[0]), 2) + pow((proy->getPosition()[1]-posJugador[1]),2));
    if(distancia >= distGancho){
        vuelta=true;        
    }
    if(vel[0]!=0&&vel[1]!=0){
        proy->setRotation(57.29578*atan2(-vel[0],vel[1]));
    }
           
    if(vuelta&&!colision){
    
        moveVec[0]=posJugador[0] - proy->getPosition()[0];
        moveVec[1]=posJugador[1] - proy->getPosition()[1];
        VecLength=sqrt(pow(moveVec[0],2)+pow(moveVec[1],2)); 
        moveVec[0] /= VecLength;
        moveVec[1] /= VecLength;

        vel[0] = moveVec[0]*40;
        vel[1] = moveVec[1]*40;
                  //poner condicion de q mientras esta en colision q no haga esto
        proy->setRotation(57.29578*atan2(vel[0],-vel[1]));
       }
       
    posicionAnterior = posicion;
    
    posicion[0] += vel[0];
    posicion[1] += vel[1];
   
    
}
void Gancho::comprobarColisiones(){
    int ladoColisiones=0;
    vector<int> efe={0,0};

    efe[0]=((int)(proy->getPosition()[0])/32);
    efe[1]=((int)(proy->getPosition()[1]+16)/32);

    ladoColisiones=colisionar(efe);

    if(ladoColisiones!=1){
        efe[0]=((int)(proy->getPosition()[0])/32);
        efe[1]=((int)(proy->getPosition()[1]-16)/32);
        
        ladoColisiones=colisionar(efe);
        
         if(ladoColisiones!=1){
            efe[0]=((int)(proy->getPosition()[0]+16)/32);
            efe[1]=((int)(proy->getPosition()[1])/32);
        
            ladoColisiones=colisionar(efe);
            if(ladoColisiones!=1){
                efe[0]=((int)(proy->getPosition()[0]-16)/32);
                efe[1]=((int)(proy->getPosition()[1])/32);
        
                 ladoColisiones=colisionar(efe);
                 
            }
         }


    }


    if(ladoColisiones==1){

    vel[0]=0;
    vel[1]=0;

    colision=true;
    vuelta=false;    
    }
}
int Gancho::colisionar( vector<int> efe){
    EstadoPartida *estado = EstadoPartida::Instancia();
    map<vector<int>, int>* colisionMap = estado->getSalaActual()->getMapaColisiones();

     //Id del tile a comprobar
    int idCol         = estado->getSalaActual()->getIdColision(1);
    int idPlat        = estado->getSalaActual()->getIdColision(2);
    int idPArriba     = estado->getSalaActual()->getIdColision(3);
    int idPDerecha    = estado->getSalaActual()->getIdColision(4);
    int idPAbajo      = estado->getSalaActual()->getIdColision(5);
    int idPIzquierda  = estado->getSalaActual()->getIdColision(6);
    auto tile = colisionMap->find(efe);
    int ladoColisiones=0;
    if(tile != colisionMap->end()){
        if(tile->second == idPlat){
        ladoColisiones = 0;
        }else if(tile->second == idCol){
        ladoColisiones= 1;
        }else  if(tile->second == idPDerecha && !estado->getSalaActual()->existePuerta(1)){
            ladoColisiones = 1;
        }else  if(tile->second == idPArriba && !estado->getSalaActual()->existePuerta(0)){
            ladoColisiones = 1;
        }else  if(tile->second == idPAbajo && !estado->getSalaActual()->existePuerta(2)){
            ladoColisiones = 1;
        }else  if(tile->second == idPIzquierda && !estado->getSalaActual()->existePuerta(3)){
            ladoColisiones = 1;
        }
    }
    return ladoColisiones;
}


void Gancho::draw(float percentTick, int update_tick_time) {
    VentanaRender* ventana = VentanaRender::Instancia();
     EstadoPartida *estado = EstadoPartida::Instancia();
    vector<float> posJugador=estado->getJugador()->getPosNueva();
  
    float x = posicionAnterior[0]*(1-percentTick) + posicion[0]*percentTick;
    float y = posicionAnterior[1]*(1-percentTick) + posicion[1]*percentTick;

  //  cuerda->setPosition(posJugador[0],posJugador[1]);
    distAnt=dis;
    dis=sqrt(pow((x- posJugador[0]), 2) + pow((y - posJugador[1]),2));

    if(distAnt==dis&&colision){
       
        cont++;
       
       //estado->getJugador()->setDisparo();
    }else{
        cont--;
    }
    if(cont>0){

         colision=false;
         estado->getJugador()->setDisparo(false);
    }

    direcRope[0]=(x-posJugador[0] );
    direcRope[1]=(y-posJugador[1] );
    DirLength=sqrt(pow(direcRope[0],2)+pow(direcRope[1],2)); 
    direcRope[0] /= DirLength;
    direcRope[1] /= DirLength;


    int numSprite= dis/8;
    float ultSprite=dis-numSprite*8;
    hilo.clear();
    for(int i=0;i<numSprite;i++){
        hilo.push_back(cuerdesita = new Sprite(texCuerdesita, 0, 0, 8, 8));

        hilo[i]->setPosition(posJugador[0]+i*direcRope[0]*8,posJugador[1]+i*direcRope[1]*8);
        hilo[i]->setRotation(57.29578*atan2(-direcRope[0],direcRope[1]));   


    }
        hilo.push_back(cuerdesita = new Sprite(texCuerdesita, 0, 0, 8, ultSprite));

        hilo[hilo.size()-1]->setPosition(posJugador[0]+numSprite*direcRope[0]*8,posJugador[1]+numSprite*direcRope[1]*8);
        hilo[hilo.size()-1]->setRotation(57.29578*atan2(-direcRope[0],direcRope[1]));   


   // cuerda->setRect(0,0,8,dis);
    
   
       
   // this->cuerda->setRotation(57.29578*atan2(-direcRope[0],direcRope[1]));   

    //Actualiza la posicion del sprite y lo dibuja
    proy->setPosition(x, y);
 //   ventana->dibujar(cuerda);
    for(int i=0;i<hilo.size();i++){
        ventana->dibujar(hilo[i]);
    }
    ventana->dibujar(proy);
}


vector<float> Gancho::getPosition(){
    return proy->getPosition();
}

bool Gancho::getVuelta(){
    return vuelta;
}

bool Gancho::getColision(){
    return colision;
}
int Gancho::getCooldown(){
    return cooldown;
}
void Gancho::setDistancia(float distancia){
    this->distGancho += distancia;
}