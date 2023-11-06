#include "Kamikaze.h"

const int TIEMPO_APARICION      = 1600;
const int TIEMPO_PERSECUCION    = 3000;
const int TIEMPO_MIN_AVISTA     = 3;
const float DISTANCIA_ATAQUE    = 420.0;
const float INCREM_DIST_COMPROB = 10.0;

//Constructor
Kamikaze::Kamikaze(int vida,int danyo,int velocidad,int tiempoRecuperacion,int esencias,vector<int> idMat, Textura* t, Sprite* sp):Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp) {
    contador        = 0;
    estado          = KamikazeApareciendo;
    escala          = 2.5;
    tiempoALaVista  = 0;

    sprite->setColor({255, 255, 255, 0});
    sprite->setScale(escala, escala);
    animacion = new Animacion(sprite);
    animacion->addAnimation("KamikazeLento", 200.0, 0, 0, 1, 1, 14, 14);
    animacion->addAnimation("KamikazeRapido", 30.0, 0, 0, 1, 1, 14, 14);
}

//Destructor
Kamikaze::~Kamikaze() {
    delete animacion;
}

//Actualiza el kamikaze
void Kamikaze::update(Tiempo t) {
    Enemigo::update(t);

    //Comprueba que no se ha detenido el tiempo
    if(!detenido) {
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        int opacidad = 0;
        switch(estado) {
            case KamikazeApareciendo:
                //Actualiza la transparencia del sprite
                opacidad = static_cast<int>((min(contador, TIEMPO_APARICION)/(float)TIEMPO_APARICION)*255);
                sprite->setColor({255, 255, 255, opacidad});

                if(contador >= TIEMPO_APARICION) {
                    contador = 0;
                    estado = KamikazeEsperando;
                    
                    //Genera particulas para indicar claramente que ya ha aparecido
                    for(int i = 10; i > 0; i--) {
                        new ParticulaSimpleCuadrada(posicion, {39, 66, 58, 255}, 30, 1000);
                    }                    
                }
                break;
            case KamikazeEsperando:
                animacion->play("KamikazeLento", t.milisegundos());

                if(contador >= tiempoRecuperacion) {
                    //Comprueba la distancia a la que se encuentra el jugador
                    Jugador* jugador = EstadoPartida::Instancia()->getJugador();
                    vector<float> posJugador = jugador->getPosNueva();

                    float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
                    if(distancia < DISTANCIA_ATAQUE) {
                        //Creo y recojo las variables necesarias
                        Sala* salaActual = EstadoPartida::Instancia()->getSalaActual();
                        int tamTiles                        = salaActual->getConstTamTiles();
                        map<vector<int>, int>* colisionMap  = salaActual->getMapaColisiones();
                        int idCol                           = salaActual->getIdColision(1);
                        vector<int> coordsActuales  = {(int)posicion[0]/tamTiles, (int)posicion[1]/tamTiles};
                        vector<float> dir           = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                        bool colision               = false;

                        //Si se encuentra a la distancia de ataque, comprueba si hay colisiones por enmedio
                        for(int i = 0; i < (int)distancia/INCREM_DIST_COMPROB && !colision; i++){
                            vector<int> coordsComprobar =  {(int)(posicion[0]+dir[0]*INCREM_DIST_COMPROB*i)/tamTiles, 
                                                            (int)(posicion[1]+dir[1]*INCREM_DIST_COMPROB*i)/tamTiles};

                            //Si la posicion que va ha comprobar es la misma que ha comprobado antes no hace nada
                            if(coordsComprobar[0] != coordsActuales[0] || coordsComprobar[1] != coordsActuales[1]){
                                //Busca en el mapa de colisiones si la posicion a comprobar tiene una colision
                                coordsActuales = coordsComprobar;
                                auto tile = colisionMap->find(coordsComprobar);
                                if(tile != colisionMap->end()){
                                    if(tile->second == idCol){
                                        //Si encuentra una colision deja de buscar
                                        colision = true;
                                    }
                                }
                            } 
                        }

                        if(colision) {                                      //Se mantiene en el estado actual
                            //Reinicia el contador para no hacerlo en cada update
                            contador        = 0;
                            tiempoALaVista  = 0;
                        }
                        else if(tiempoALaVista >= TIEMPO_MIN_AVISTA){       //Cambia a persiguiendo
                            contador    = 0;
                            estado      = KamikazePersiguiendo;
                        }
                        else {                                              //El jugador esta a la vista
                            tiempoALaVista++;
                        }
                    }
                }
                break;
            case KamikazePersiguiendo:
                animacion->play("KamikazeRapido", t.milisegundos());

                //Comprueba que todavia no ha pasado el tiempo antes de explotar
                if(contador < TIEMPO_PERSECUCION) {
                    //Comprueba la distancia a la que se encuentra el jugador
                    Jugador* jugador = EstadoPartida::Instancia()->getJugador();
                    vector<float> posJugador = jugador->getPosNueva();

                    float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));
                    if(distancia < DISTANCIA_ATAQUE) {
                        //Creo y recojo las variables necesarias
                        Sala* salaActual = EstadoPartida::Instancia()->getSalaActual();
                        int tamTiles                        = salaActual->getConstTamTiles();
                        map<vector<int>, int>* colisionMap  = salaActual->getMapaColisiones();
                        int idCol                           = salaActual->getIdColision(1);
                        vector<int> coordsActuales  = {(int)posicion[0]/tamTiles, (int)posicion[1]/tamTiles};
                        vector<float> dir           = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                        bool colision               = false;

                        //Si se encuentra a la distancia de ataque, comprueba si hay colisiones por enmedio
                        for(int i = 0; i < (int)distancia/INCREM_DIST_COMPROB && !colision; i++){
                            vector<int> coordsComprobar =  {(int)(posicion[0]+dir[0]*INCREM_DIST_COMPROB*i)/tamTiles, 
                                                            (int)(posicion[1]+dir[1]*INCREM_DIST_COMPROB*i)/tamTiles};

                            //Si la posicion que va ha comprobar es la misma que ha comprobado antes no hace nada
                            if(coordsComprobar[0] != coordsActuales[0] || coordsComprobar[1] != coordsActuales[1]){
                                //Busca en el mapa de colisiones si la posicion a comprobar tiene una colision
                                coordsActuales = coordsComprobar;
                                auto tile = colisionMap->find(coordsComprobar);
                                if(tile != colisionMap->end()){
                                    if(tile->second == idCol){
                                        //Si encuentra una colision deja de buscar
                                        colision = true;
                                    }
                                }
                            } 
                        }

                        if(!colision) {
                            //Se mueve hacia el jugador
                            posicionAnterior = posicion;

                            posicion[0] += dir[0]*getVelocidad()*(t.segundos()+tiempoAntes.segundos());
                            posicion[1] += dir[1]*getVelocidad()*(t.segundos()+tiempoAntes.segundos());
                        }

                        //Comprueba si el jugador esta a la derecha o a la izquierda del enemigo
                        if(posJugador[0] >= posicion[0]) {
                            escala = abs(escala);
                        }
                        else {
                            escala = -1*abs(escala);
                        }

                        //Comprueba si el jugador choca con el kamikaze, en cuyo caso explota
                        sprite->setPosition(posicion[0], posicion[1]);
                        if(sprite->intersect(jugador->getSprite())) {
                            //El kamikaze explota
                            muerto = true;
                            EstadoPartida* estado = EstadoPartida::Instancia();
                            Sprite* s = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
                            s->setScale(8.0, 8.0);
                            Explosion* ex = new Explosion(danyo, 0, s, nullptr, posicion, 128.0, 0, 0);
                            estado->anyadirExplosion(ex);
                        }
                    }

                    //Actualiza la direccion en la que mira el sprite
                    sprite->setScale(escala, abs(escala));

                    //Reinicia el tiempo antes de detener si es necesario y el reloj
                    relojDetenible.restart();
                    if(tiempoAntes.milisegundos()>0)
                        reiniciarTiempoAntes();
                }
                else {
                    //El kamikaze explota
                    muerto = true;
                    EstadoPartida* estado = EstadoPartida::Instancia();
                    Sprite* s = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
                    s->setScale(8.0, 8.0);
                    Explosion* ex = new Explosion(danyo, 0, s, nullptr, posicion, 128.0, 0, 0);
                    estado->anyadirExplosion(ex);
                }
                break;
        }
    }
}

//Resetea el kamikaze
void Kamikaze::reset() {
    Enemigo::reset();

    contador            = 0;
    tiempoALaVista      = 0;
    estado              = KamikazeApareciendo;
    posicionAnterior    = posicion;
    sprite->setColor({255, 255, 255, 0});
}