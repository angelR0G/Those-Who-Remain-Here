#include "NubeVeneno.h"

const int TIEMPO_NUBE  = 5000;

//Se le pasa el sprite de la nube de veneno, la posicion en la que aparece, el danyo que hace al jugador, el danyo que hace a los enemigos, el radio y el tiempo que dura en ms. Si se le pasa un tiempo menor que 0 emplea el tiempo por defecto.

NubeVeneno::NubeVeneno(Sprite* s, vector<float> pos, int dJ, int dE, int r, int t){
    spriteVeneno = s;
    posicion     = pos;
    danyoJugador = dJ;
    danyoEnemigo = dE;
    tiempo       = 0;
    maxTiempo    = 0;
    destruir     = false;
    duracion     = t;
    radio        = r;
    anim         = new Animacion(spriteVeneno);
    anim->addAnimation("Veneno", 200.0, 0, 0, 0, 2, 64, 64);
    spriteVeneno->setOrigin(64/2, 64/2);
    spriteVeneno->setColor({63, 171, 41, 100});
}

NubeVeneno::~NubeVeneno(){
    delete anim;
    delete spriteVeneno;
    posicion.clear();

}

void NubeVeneno::update(Tiempo elapsedTime){
    if(!detenido){
        anim->play("Veneno", elapsedTime.milisegundos());
        tiempo += elapsedTime.milisegundos();
        maxTiempo += elapsedTime.milisegundos();

        spriteVeneno->setPosition(posicion[0], posicion[1]);
        spriteVeneno->setScale(radio*2/64, radio*2/64);

        bool colision = false;
        EstadoPartida *estado = EstadoPartida::Instancia();
        Jugador* jugador = estado->getJugador();

        if(tiempo>=1000){
            tiempo-=1000;

            vector<float> diff = {abs(spriteVeneno->getPosition()[0]-jugador->getPosNueva()[0]), abs(spriteVeneno->getPosition()[1]-jugador->getPosNueva()[1])};
            float dist = sqrt(pow(diff[0], 2)+pow(diff[1], 2));
            //Comprueba si colisiona con el jugador
            if(danyoJugador > 0 &&  dist <= radio) {
                //Danya al jugador y elimina el proyectil
                jugador->danyarVida(danyoJugador);
                colision = true;
            }
            else if(!detenido && danyoEnemigo > 0) {        //Comprueba si colisiona con algun enemigo
                //Recupera los enemigos del nivel y comprueba si el proyectil impacta en alguno
                vector<Enemigo*>* enemigos = estado->getSalaActual()->getEnemigos();
                for(unsigned int i=0; i<enemigos->size(); i++) {
                    vector<float> diff = {abs(spriteVeneno->getPosition()[0]-(*enemigos)[i]->getSprite()->getPosition()[0]), abs(spriteVeneno->getPosition()[1]-(*enemigos)[i]->getSprite()->getPosition()[1])};
                    float dist = sqrt(pow(diff[0], 2)+pow(diff[1], 2));
                    //Comprueba si el sprite del enemigo intersecta con el del proyectil
                    if(dist <= radio) {
                        //Danya al enemigo y elimina el proyectil
                        colision = true;
                        (*enemigos)[i]->perderVida(danyoEnemigo);
                        colision = true;

                        //Aumenta el robo de vida del jugador
                        jugador->aumentarRoboVida(danyoEnemigo);
                    }
                }
            }
        }

        if(maxTiempo>= getDuracion()){
            destruir = true;
        }
    }

}

void NubeVeneno::dibujar(){
    spriteVeneno->draw();
}

bool NubeVeneno::getDestruir(){
    return destruir;
}

int NubeVeneno::getDuracion(){
    int d = 0;
    if(duracion<0){
        d = TIEMPO_NUBE;
    }else{
        d = duracion;
    }
    return d;
}