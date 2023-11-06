#include "Pajaro.h"

const float DISTANCIA_DETECCION     = 600.0;
const float DISTANCIA_ATAQUE        = 500.0;
const float DISTANCIA_MAX_ATACANDO  = 490.0;

const float COMPROBAR_CADA          = 5.0;
const float VELOCIDAD_ATAQUE        = 2.8;    
const float VELOCIDAD_SPRINT        = 6.0;

const int TIEMPO_UPDATE_COL       = 100;    //en ms
const int TIEMPO_VISTA            = 2000;   //Tiempo que tiene que detectar al personaje para atacarle en ms
const int TIEMPO_CAMBIO           = 1500;   //Tiempo que permanece en la misma direccion a no ser que se choque


//Constantes para calcular la posicion de las colisiones
const int COLISION_X        = 16;
const int COLISION_Y        = 16;

const int ESCALA            = 3; 

//Constructor
Pajaro::Pajaro(int vida, int danyo, int velocidad, int tiempoRecuperacion, int esencias, vector<int> idMat, Textura* t, Sprite* sp) : Enemigo(vida, danyo, velocidad, tiempoRecuperacion, esencias, idMat, t, sp){
    enRango = false;
    anim    = new Animacion(sp);
    anim->addAnimation("Normal", 100.0, 0, 1, 4, 1, 16, 16);
    anim->addAnimation("Ataque", 100.0, 0, 0, 4, 0, 16, 16);
    estadoPajaro = EstadosPajaro::Normal;
    colision = false;
    atacando = false;
    atacado  = false;
    reposo = false;
    tiempoVision = 0;
    tiempoEntreAct = 0;
    tiempoEntreCambios = TIEMPO_CAMBIO;
    cooldownAtaque = tiempoRecuperacion;
    ladoColisiones={-1, -1, -1, -1};
    direccMov = {0, 0};
    direccion = 1;
}

//Destructor
Pajaro::~Pajaro(){
    delete anim;
    posEmpezarAtaque.clear();
    ladoColisiones.clear();
    direccMov.clear();
}

//Update
void Pajaro::update(Tiempo elapsedTime){
    Enemigo::update(elapsedTime);
    tiempoEntreAct += elapsedTime.milisegundos();

    //Comprueba que no se haya detenido el tiempo
    if(!detenido){
        posicionAnterior = posicion;

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        EstadoPartida* partida = EstadoPartida::Instancia();
        Jugador* j = partida->getJugador();
        vector<float> posJugador = j->getPosNueva();

        //Se recuperan los datos para calcular las colisiones del enemigo
        map<vector<int>, int>* colisionMap  = partida->getSalaActual()->getMapaColisiones();
        vector<int> tamMapa                 = partida->getSalaActual()->getTamanyoMapa();
        int idCol                           = partida->getSalaActual()->getIdColision(1);

        //Calcula la distancia a la que se encuentra el jugador
        int distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));

        if((distancia <= DISTANCIA_DETECCION)) {
            vector<float> vectorD = {posJugador[0]-posicion[0], posJugador[1]-posicion[1]};
            
            vectorD = {vectorD[0]/distancia, vectorD[1]/distancia};
            vector<int> posOriginal = {(int)posicion[0]/32, (int)posicion[1]/32};
            colision = false;
            //Si esta dentro de la distancia de deteccion se comprueba si hay una pared delante
            if(tiempoEntreAct >= TIEMPO_UPDATE_COL){

                tiempoEntreAct -= TIEMPO_UPDATE_COL;
                for(int i = 0; i < distancia/COMPROBAR_CADA && !colision; i++){
                    vector<int> coordsComprobar = {(int)(posicion[0]+vectorD[0]*COMPROBAR_CADA*i)/32, (int)(posicion[1]+vectorD[1]*COMPROBAR_CADA*i)/32};
                    //Si la posicion que va ha comprobar es la misma en la que esta no hace nada
                    if(coordsComprobar != posOriginal){
                        //Busca en el mapa de colisiones si la direccion de vision del pajaro colisiona con una pared
                        posOriginal = coordsComprobar;
                        auto tile = colisionMap->find(coordsComprobar);
                        if(tile != colisionMap->end()){
                            if(tile->second == idCol){
                                colision = true;
                            }
                        }
                    } 
                }
            }
            //Si no colisiona la vision con ninguna pared
            if(!colision){
                //Se comprueba si el pajaro ha visto durante un tiempo al personaje
                tiempoVision += elapsedTime.milisegundos();
                cooldownAtaque += elapsedTime.milisegundos();
                if(!atacando && tiempoVision >= TIEMPO_VISTA){
                    tiempoVision -= TIEMPO_VISTA;
                    if(cooldownAtaque>= tiempoRecuperacion){
                        cooldownAtaque -= tiempoRecuperacion;
                        atacando = true;
                        posEmpezarAtaque = posicion;
                        //Actualiza el estado de la torreta a jugador en rango
                        enRango = true;
                        estadoPajaro = EstadosPajaro::Ataque;
                    }
                    
                }
                
                if(atacando){   //Si el pajaro esta atacando al jugador
                    reposo = false;
                    //Si el peronaje es detectado el enemigo se lanza hasta el personaje
                    if(distancia <= DISTANCIA_ATAQUE){
                        posicion[0] += vectorD[0]*getVelocidad()*VELOCIDAD_SPRINT*elapsedTime.segundos();
                        posicion[1] += vectorD[1]*getVelocidad()*VELOCIDAD_SPRINT*elapsedTime.segundos();
                    }else{
                        posicion[0] += vectorD[0]*getVelocidad()*VELOCIDAD_ATAQUE*elapsedTime.segundos();
                        posicion[1] += vectorD[1]*getVelocidad()*VELOCIDAD_ATAQUE*elapsedTime.segundos();
                    }
                    if(vectorD[0]< 0){
                        direccion = -1;
                    }else{
                        direccion = 1;
                    }
                    //Si el sprite del personaje intersecta con el del enemigo, el jugador pierde vida
                    if(j->getSprite()->intersect(sprite)){
                        j->danyarVida(danyo);
                        atacado = true;
                    }
                    
                    //Se calcula la distancia entre la posicion actual del enemigo y la posicion donde empezo el ataque
                    float distanciaAtaque = sqrt(pow(posicion[0]-posEmpezarAtaque[0], 2) + pow(posicion[1]-posEmpezarAtaque[1], 2));

                    //Si el pajaro y aha atacado al personaje o el pajaro ha excedido la distancia de ataque desde donde empezo
                    if(atacado || distanciaAtaque >= DISTANCIA_MAX_ATACANDO){
                        atacando = false;
                        atacado = false;
                        reposo = true;
                    }
                }else{
                    reposo = true;
                }
            }else{
                reposo = true;
            }
        }
        else{
            reposo = true;
        }
        tiempoEntreCambios += elapsedTime.milisegundos();
        if(reposo){
            //El jugador no esta en el rango de accion del enemigo, se mueve de forma aleatoria
            enRango = false;
            atacando = false;
            estadoPajaro = EstadosPajaro::Normal;
            int num = rand() % 8;
            int cambioDirec = rand() % 10;
            if(tiempoEntreCambios >= TIEMPO_CAMBIO && cambioDirec == 9){
                tiempoEntreCambios -= TIEMPO_CAMBIO;
                seleccionaDireccion(num);
            }
            colisionesMapa();
            if(direccMov[0] < 0){
                direccion = -1;
            }else{
                direccion = 1;
            }
            posicion[0] += direccMov[0]*getVelocidad()*elapsedTime.segundos();
            posicion[1] += direccMov[1]*getVelocidad()*elapsedTime.segundos();
        }
        //Dependiendo del estado del enemigo se intercambia la animacion
        if(estadoPajaro == EstadosPajaro::Normal){
            sprite->setScale(direccion*ESCALA, ESCALA);
            anim->play("Normal", elapsedTime.milisegundos());
        }else if(estadoPajaro == EstadosPajaro::Ataque){ 
            sprite->setScale(direccion*ESCALA, ESCALA);
            anim->play("Ataque", elapsedTime.milisegundos());
        }
    }
}

void Pajaro::colisionesMapa(){
    EstadoPartida *estado               = EstadoPartida::Instancia();
    map<vector<int>, int>* colisionMap  = estado->getSalaActual()->getMapaColisiones();
    vector<int> tamMapa                 = estado->getSalaActual()->getTamanyoMapa();
    int tamTiles                        = estado->getSalaActual()->getConstTamTiles();
    vector<int> posComprobar = {-1,-1};

    //Id del tile a comprobar
    int idCol         = estado->getSalaActual()->getIdColision(1);
    int idPArriba     = estado->getSalaActual()->getIdColision(3);
    int idPDerecha    = estado->getSalaActual()->getIdColision(4);
    int idPAbajo      = estado->getSalaActual()->getIdColision(5);
    int idPIzquierda  = estado->getSalaActual()->getIdColision(6);

    //Comprueba que no se ha salido del mapa
    if(posicion[0]>=0 && posicion[0]/32<tamMapa[0] && posicion[1]>=0 && posicion[1]/32<tamMapa[1]) {
        //Parte central derecha
        posComprobar[0] = (posicion[0]+COLISION_X)/tamTiles;
        posComprobar[1] = (posicion[1]+0)/tamTiles;
        auto tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol || tile->second == idPDerecha){
                ladoColisiones[0] = ((int)(posicion[0]+COLISION_X)%tamTiles)*(-1) +1;
                direccMov = {-1, 0};
            }
        }else{
            ladoColisiones[0] = 0;
        }

        //Parte central Abajo
        posComprobar[0] = (posicion[0]+0)/tamTiles;
        posComprobar[1] = (posicion[1]+COLISION_Y)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol || tile->second == idPAbajo){
                ladoColisiones[1] = ((int)(posicion[1]+COLISION_Y)%tamTiles)*(-1) +1;
                direccMov = {0, -1};
            }
        }else{
            ladoColisiones[1] = 0;
        }

        //Parte central izquierda
        posComprobar[0] = (posicion[0]-COLISION_X)/tamTiles;
        posComprobar[1] = (posicion[1]+0)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol || tile->second == idPIzquierda){
                ladoColisiones[2] = ((int)(posicion[0]-COLISION_X)%tamTiles) - 1;
                direccMov = {1, 0};
            }
        }else{
            ladoColisiones[2] = 0;
        }

        //Parte central arriba
        posComprobar[0] = (posicion[0]-0)/tamTiles;
        posComprobar[1] = (posicion[1]-COLISION_Y)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol || tile->second == idPArriba){
                ladoColisiones[3] = ((int)(posicion[1]-COLISION_Y)%tamTiles) - 1;
                direccMov = {0, 1};
            }
        }else{
            ladoColisiones[3] = 0;
        }

        int random = rand() % 7;
        //Colision izquierda
        if(ladoColisiones[2] != 0){
            posicion[0] += (tamTiles-ladoColisiones[2]);
            if(random != 0 && random != 7 && random != 6){
                seleccionaDireccion(random);
            }
            seleccionaDireccion(3);
        }

        //Colision derecha
        if(ladoColisiones[0] != 0){
            posicion[0] += ladoColisiones[0];
            if(random != 2 && random != 3 && random != 4){
                seleccionaDireccion(random);
            }
            seleccionaDireccion(7);
        }

        //Colision arriba
        if(ladoColisiones[3] != 0){
            posicion[1] += (tamTiles-ladoColisiones[3]);
            if(random != 0 && random != 1 && random != 2){
                seleccionaDireccion(random);
            }
            seleccionaDireccion(5);
        }
        
        //Colision abajo
        if(ladoColisiones[1] != 0){
            posicion[1]+=ladoColisiones[1];
            if(random != 6 && random != 5 && random != 4){
                seleccionaDireccion(random);
            }
            seleccionaDireccion(1);
        }
    }
}


void Pajaro::seleccionaDireccion(int r){
    switch (r)
    {
        case 0:
            direccMov = {-1, -1};
            break;
        case 1:
            direccMov = {0, -1};
            break;
        case 2:
            direccMov = {1, -1};
            break;
        case 3:
            direccMov = {1, 0};
            break;
        case 4:
            direccMov = {1, 1};
            break;
        case 5:
            direccMov = {0, 1};
            break;
        case 6:
            direccMov = {-1, 1};
            break;
        case 7:
            direccMov = {-1, 0};
            break;
        default:
            break;
    }
}