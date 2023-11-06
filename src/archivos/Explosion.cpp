#include "Explosion.h"

const float DOBLEEXP_PEQ = 0.3;
const float DOBLEEXP_GRA = 0.6;

const int   GAS_PEQ      = 5;
const int   GAS_GRA      = 10; 

//Constructor de la explosion se le pasa el danyo que hace al jugador y el que hace al enemigo
//El sprite de la explosion, el sprite de la nube de veneno, la posicion en la que va ha aparecer, el radio de explosion, si teine la mejora de la nube de gas y si tiene la mejora de doble explosion
Explosion::Explosion(int dJ, int dE, Sprite* s, Sprite* sV, vector<float> pos, float r, int g, int dobleEx){
    danyoJugador = dJ;
    danyoEnemigo = dE;
    sprite       = s;
    posicion     = pos;
    radio        = r;
    sprite->setOrigin(32/2, 32/2);
    anim           = new Animacion(sprite);
    anim->addAnimation("Explosion", 1.0, 0, 0, 4, 4, 32, 32);
    destruir       = false;
    reproduciendoAnim = true;
    impacto        = true;
    gas            = g;
    dobleExplosion = dobleEx;
    spriteVeneno   = sV;
    ctrSonido = new ControladorSonido();
    ctrSonido->addSonido("explota",new Buffer("resources/sonidos/Explosion.ogg"),60);
    
}

Explosion::~Explosion(){
    if(sprite!=nullptr){
        delete sprite;
    }
    delete anim;
    posicion.clear();
    delete ctrSonido;
}

//Update de la explosion
void Explosion::update(Tiempo elapsedTime){
    EstadoPartida *estado = EstadoPartida::Instancia();
    Sprite* espriteaux = nullptr;
    Explosion *exAux = nullptr;
    NubeVeneno* nube = nullptr;
    sprite->setPosition(posicion[0], posicion[1]);
    sprite->setScale(radio*2/32, radio*2/32);
    ctrSonido->play("explota");
    if(!detenido && reproduciendoAnim) {
        anim->playOnce("Explosion", elapsedTime.milisegundos());
        //Comprueba que ya ha terminado
        if(anim->getFinished()) {
            reproduciendoAnim = false;
            if(dobleExplosion == 1){
                espriteaux = new Sprite(*sprite);
                exAux = new Explosion(danyoJugador, danyoEnemigo*DOBLEEXP_PEQ, espriteaux, nullptr, posicion, radio, 0, 0);
                estado->anyadirExplosion(exAux);
            }else if(dobleExplosion == 2){
                espriteaux = new Sprite(*sprite);
                exAux = new Explosion(danyoJugador, danyoEnemigo*DOBLEEXP_GRA, espriteaux, nullptr, posicion, radio, 0, 0);
                estado->anyadirExplosion(exAux);
            }
            if(gas == 1){
                if(spriteVeneno != nullptr){
                    nube = new NubeVeneno(spriteVeneno, posicion, 0, GAS_PEQ, 200, -1);
                    estado->anyadirNubeVeneno(nube);
                }
            }else if(gas == 2){
                if(spriteVeneno != nullptr){
                    nube = new NubeVeneno(spriteVeneno, posicion, 0, GAS_GRA, 200, -1);
                    estado->anyadirNubeVeneno(nube);
                }
            }
            destruir = true;
        }
    }
    if(impacto){
        bool colision = false;
        impacto = false;
        EstadoPartida *estado = EstadoPartida::Instancia();
        Jugador* jugador = estado->getJugador();

        vector<float> diff = {abs(sprite->getPosition()[0]-jugador->getPosNueva()[0]), abs(sprite->getPosition()[1]-jugador->getPosNueva()[1])};
        float dist = sqrt(pow(diff[0], 2)+pow(diff[1], 2));
        //Comprueba si colisiona con el jugador
        if(danyoJugador > 0 &&  dist<= radio) {
            //Danya al jugador y elimina el proyectil
            jugador->danyarVida(danyoJugador);
            colision = true;
        }
        else if(!detenido && danyoEnemigo > 0) {        //Comprueba si colisiona con algun enemigo
            //Recupera los enemigos del nivel y comprueba si el proyectil impacta en alguno
            vector<Enemigo*>* enemigos = estado->getSalaActual()->getEnemigos();
            for(unsigned int i=0; i<enemigos->size() && !colision; i++) {
                vector<float> diff = {abs(sprite->getPosition()[0]-(*enemigos)[i]->getSprite()->getPosition()[0]), abs(sprite->getPosition()[1]-(*enemigos)[i]->getSprite()->getPosition()[1])};
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
}

void Explosion::dibujar(){
    sprite->draw();
}

bool Explosion::getDestruir(){
    return destruir;
}

bool Explosion::getTerminado(){
    return reproduciendoAnim;
}