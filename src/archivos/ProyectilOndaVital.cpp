#include "ProyectilOndaVital.h"

const int tamTiles = 32;

using namespace std;

/*Constructor. Recibe el danyo que inflinge al jugador y a los enemigos, el sprite, la posicion inicial,
*la direccion en la que se mueve, la velocidad, el alcanze y un vector con las runas elementales que tiene el arma.
* Pos 0->Quemadura: 0 no tiene la runa, 1 quemado leve, 2 quemado grave
* Pos 1->Congelacion: 0 no tiene la runa, 1 congelacion leve, 2 congelacion grave
* Pos 2->Electrico: 0 no tiene la runa, 1 electrocutado leve, 2 electrocutado grave 
*/
ProyectilOndaVital::ProyectilOndaVital(int dJ, int dE, Sprite* s, vector<float> pos, vector<float> dir, int vel, int a, vector<int> runas) : Proyectil(dJ, dE, s, pos, dir, vel, a) {
    direcRope.push_back(0);
    direcRope.push_back(0);
    posEst.push_back(0);
    posEst.push_back(0);
    direcRope = dir;
    posEst = pos;
    texCuerdesita     = new Textura("resources/sprites/armas/balas.png");
                destruir=false;
    sprite =nullptr;
    contador=0;
    cambio=0;

}

//Destructor
ProyectilOndaVital::~ProyectilOndaVital() {
    for(unsigned int i=0;i<hilo.size();i++){
        delete hilo[i];
    }
  
}

//Actualiza el proyectil
void ProyectilOndaVital::update(Tiempo elapsedTime) {
    bool colision = false;
    EstadoPartida* estado = EstadoPartida::Instancia();
    Jugador* jugador = estado->getJugador();
    if(contador>0.25){
        cambio++;
        if(cambio==4){
            cambio=0;
        }
        contador=0;
    }
    contador+=elapsedTime.segundos();
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        float dis=sqrt(pow((posicion[0]- posEst[0]), 2) + pow((posicion[1]-posEst[1]),2));

        
        //Guarda el estado actual como el anterior
        posicionAnterior = posicion;



        

        //Modifica la posicion actual
        posicion[0]           += direccion[0]*velocidad*elapsedTime.segundos();
        posicion[1]           += direccion[1]*velocidad*elapsedTime.segundos();



        if(dis>150){
            destruir=true;
        }


        //Resetea el reloj de Detenible y tiempoAntes
        relojDetenible.restart();
        tiempoAntes.setTiempoCero();
    }
    for(int i=0;i<hilo.size();i++){
       
        if(jugador->getSprite()->intersect(hilo[i])) {
        //Danya al jugador y elimina el proyectil
        jugador->danyarVida(1);
        colision = true;
    }
    }
    //Comprueba si colisiona con el jugador
   
    
    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        if(!colision) {
            
            //Comprueba si colisiona con el mapa
            for(int i=0; i<hilo.size()&&!colision;i++){
                if(hilo[i]!=nullptr){
                     destruir = comprobarColisionMapa(hilo[i]);
                }
               
            }
            
        }
        else {
            //Genera particulas al haber colisionado con un ser
            for(int i=0; i<5; i++) {
                new ParticulaDireccional(posicion, {255, 0, 0, 255}, 120.0, 500, direccion, 0.2, 0.5);
            }
        }

        //Comprueba que no ha llegado al alcance maximo
        if(!colision){
            alcance-= velocidad*elapsedTime.segundos();
            if(alcance <= 0){
                destruir = true;
            }
        }
    }

    //Si ha colisionado la destruye
    if(colision) {
        destruir = true;
    }
}

bool ProyectilOndaVital::comprobarColisionMapa(Sprite* si) {
    bool colision = false;
    EstadoPartida* estado = EstadoPartida::Instancia();

    //Recupera el mapa de colisiones del nivel
    map<vector<int>, int>* mapa = estado->getSalaActual()->getMapaColisiones();
    int m=(int)si->getPosition()[0];
    int n=(int)si->getPosition()[1];
    vector<int> pos = {m,n};
    int idPlataformas = estado->getSalaActual()->getIdColision(2);
    vector<int> posCalc = {pos[0]/tamTiles, pos[1]/tamTiles};

    //Comprueba si su posicion coincide con alguna colision del mapa
    if(mapa->find(posCalc) != mapa->end() && mapa->find(posCalc)->second != idPlataformas) {
        colision = true;
    }
    else {
        //Comprueba que no colisiona ninguna esquina del sprite
        vector<int> pixels = si->getGlobalPixelSize();
        vector<int> posDesp = {(pos[0]-pixels[0]/2)/tamTiles, (pos[1]-pixels[1]/2)/tamTiles};
        if(posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1]) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }
        
        posDesp = {(pos[0]+pixels[0]/2)/tamTiles, (pos[1]-pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }

        posDesp = {(pos[0]-pixels[0]/2)/tamTiles, (pos[1]+pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }

        posDesp = {(pos[0]+pixels[0]/2)/tamTiles, (pos[1]+pixels[1]/2)/tamTiles};
        if(!colision && (posCalc[0] != posDesp[0] || posCalc[1] != posDesp[1])) {
            if(mapa->find(posDesp) != mapa->end() && mapa->find(posDesp)->second != idPlataformas) {
                colision = true;
            }
        }
    }

    return colision;
}


void ProyectilOndaVital::draw(float percentTick, int update_tick_time) {
    VentanaRender* ventana = VentanaRender::Instancia();
     EstadoPartida *estado = EstadoPartida::Instancia();
    vector<float> posJugador=estado->getJugador()->getPosNueva();
    //Si el proyectil se ha detenido mantiene su estado
    if(detenido || percentTick < tiempoAntes.milisegundos()/update_tick_time) {
        percentTick = tiempoAntes.milisegundos()/update_tick_time;
    }
    
    //Calcula la posicion del sprite interpolando la posicion anterior y la nueva
    float x = posicionAnterior[0]*(1-percentTick) + posicion[0]*percentTick;
    float y = posicionAnterior[1]*(1-percentTick) + posicion[1]*percentTick;

    float dist= sqrt(pow(x-posEst[0],2)+pow(y-posEst[1],2));

    int numSprite= dist/16;
    float ultSprite=dist-numSprite*16;

    float DirLength=sqrt(pow(direcRope[0],2)+pow(direcRope[1],2)); 

    

    hilo.clear();
    for(int i=0;i<numSprite-1;i++){
        hilo.push_back(new Sprite(texCuerdesita,(14+cambio)*16, 1*16, 16, 16));
        hilo[i]->setPosition(posEst[0]+i*direcRope[0]*16,posEst[1]+i*direcRope[1]*16);
        hilo[i]->setScale(1,2);
        hilo[i]->setRotation(57.29578*atan2(-direcRope[0],direcRope[1])+90);
        float m=direcRope[0];
        float n=direcRope[1];
        n=57.29578*atan2(direcRope[0],direcRope[1]);
        m=direcRope[0];
    }

    hilo.push_back(new Sprite(texCuerdesita,(14+cambio)*16, 1*16, ultSprite, 16));


    hilo[hilo.size()-1]->setPosition(posEst[0]+(numSprite-1)*direcRope[0]*16,posEst[1]+(numSprite-1)*direcRope[1]*16);
    hilo[hilo.size()-1]->setRotation(57.29578*atan2(-direcRope[0],direcRope[1])+90); 

    hilo[hilo.size()-1]->setScale(1,2);

    hilo.push_back(new Sprite(texCuerdesita,(14+cambio)*16, 2*16, 16, 16));
    hilo[hilo.size()-1]->setScale(-1,2);

    hilo[hilo.size()-1]->setPosition(posEst[0]+(numSprite)*direcRope[0]*16,posEst[1]+(numSprite)*direcRope[1]*16);
    hilo[hilo.size()-1]->setRotation(57.29578*atan2(-direcRope[0],direcRope[1])+90); 
   

    //Actualiza la posicion del sprite y lo dibuja
    //sprite->setPosition(x, y);
  //  ventana->dibujar(sprite);

    for(unsigned int i=0;i < hilo.size();i++){
        float m=hilo[i]->getPosition()[0];
                float n=hilo[i]->getPosition()[1];

        ventana->dibujar(hilo[i]);
    }

}