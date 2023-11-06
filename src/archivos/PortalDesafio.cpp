#include "PortalDesafio.h"

const int TIEMPO_MAX        = 60000;       //ms
const int CAMBIO_PATRON     = 20000;       //Tiempo en el que cambia el patron del bullethell (ms)
const int TIEMPO_EMPEZAR    = 4000;        //ms
const int CADENCIA_BALAS    = 400;         //ms

const int NUM_PATRONES = 3;

const int VEL_BALAS = 260;

const int DANYO_BALAS = 1;

const vector<int> BALAS_NARANJAS    = {0, 2*16};
const vector<int> BALAS_AZULES      = {9*16 + 1, 2*16};
const vector<int> BALAS_VERDES      = {18*16 + 1, 2*16};
const vector<int> BALAS_LILAS       = {27*16 + 1, 2*16};

const float DIST_INTERACCION        = 80.0;

const int VOL_MUSICA                = 30;

//Se le pasa la textura del elemento y el sprite
PortalDesafio::PortalDesafio(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true){
    distInteraccion = DIST_INTERACCION;
    interactuado = false;
    acabado      = false;
    activado     = false;
    quitarPortal = false;
    primeraVez   = true;
    reproduciendoAnim = true;
    recompensaGenerada = false;

    //Origen y escala del portal
    sprite->setOrigin(32/2, 32/2);
    sprite->setScale(2, 2);
    sprite->setColor({255, 137, 0, 200});
    animPortal = new Animacion(sprite);
    animPortal->addAnimation("Explotar", 300.0f, 0, 0, 0, 2, 32, 32);

    texGenerador = new Textura("resources/sprites/generales/generadorBulletHell.png");

    generadorBalasNaranja = new Sprite(texGenerador, 0, 0, 32, 32);
    generadorBalasNaranja->setOrigin(16, 16);
    generadorBalasNaranja->setColor({240, 97, 26, 255});
    generadorBalasNaranja->setScale(2, 2);

    generadorBalasAzul = new Sprite(texGenerador, 0, 0, 32, 32);
    generadorBalasAzul->setOrigin(16, 16);
    generadorBalasAzul->setColor({33, 203, 222, 255});
    generadorBalasAzul->setScale(2, 2);

    generadorBalasVerde = new Sprite(texGenerador, 0, 0, 32, 32);
    generadorBalasVerde->setOrigin(16, 16);
    generadorBalasVerde->setColor({33, 222, 39, 255});
    generadorBalasVerde->setScale(2, 2);

    generadorBalasLila = new Sprite(texGenerador, 0, 0, 32, 32);
    generadorBalasLila->setOrigin(16, 16);
    generadorBalasLila->setColor({190, 77, 255, 255});
    generadorBalasLila->setScale(2, 2);

    animNaranja = new Animacion(generadorBalasNaranja);
    animNaranja->addAnimation("girar", 100.0f, 0, 0, 3, 3, 32, 32);
    animAzul    = new Animacion(generadorBalasAzul);
    animAzul->addAnimation("girar", 100.0f, 0, 0, 3, 3, 32, 32);
    animVerde   = new Animacion(generadorBalasVerde);
    animVerde->addAnimation("girar", 100.0f, 0, 0, 3, 3, 32, 32);
    animLila    = new Animacion(generadorBalasLila);
    animLila->addAnimation("girar", 100.0f, 0, 0, 3, 3, 32, 32);

    posElemento      = {getPosicion()[0], getPosicion()[1]};
    tiempoActivo     = 0;
    tiempoMaxActivo  = TIEMPO_MAX;
    tamCirculoBalas  = 1;
    tiempoEntreBalas = 0;
    cadenciaBalas    = CADENCIA_BALAS;
    tiempoEmpezar    = TIEMPO_EMPEZAR;
    numeroCambios    = 0;
    increBalas       = 0;
    increVel         = 0;
    texturaBalas = new Textura("resources/sprites/armas/balas.png");

    mostrarGeneradores = {false, false, false, false};

    numRand = rand() % NUM_PATRONES;

    angulo = 0.0;
    desplazamientoX = 0.0;
    desplazamientoY = 0.0;
    direcc = {1, 0};
    cambioX = 1;
    cambioY = 1;
    
    //Se crean los sonidos y la musica de la sala de desafio
    contrSonido = new ControladorSonido();
    contrSonido->addMusica("musicaBulletHell", "resources/sonidos/BulletHell.wav", VOL_MUSICA);
}

PortalDesafio::~PortalDesafio(){
    delete texturaBalas;
    posElemento.clear();
    direcc.clear();
    mostrarGeneradores.clear();
    delete generadorBalasNaranja;
    delete generadorBalasVerde;
    delete generadorBalasAzul;
    delete generadorBalasLila;
    delete texGenerador;
}

void PortalDesafio::dibujar(){
    if(!quitarPortal){
        sprite->draw();
    }
    if(mostrarGeneradores[0]){
        generadorBalasNaranja->draw();
    }
    if(mostrarGeneradores[1]){
        generadorBalasAzul->draw();
    }
    if(mostrarGeneradores[2]){
        generadorBalasVerde->draw();
    }
    if(mostrarGeneradores[3]){
        generadorBalasLila->draw();
    }
}

void PortalDesafio::update(Tiempo elapsedTime){
    //El juagador ha activado el desafio
    if(!detenido){
        if(activado){
            if(tiempoEmpezar > 0){
                tiempoEmpezar -=elapsedTime.milisegundos();
                if(tiempoEmpezar < 300){
                    if(reproduciendoAnim) {
                        sprite->setRotation(0.0);
                        animPortal->playOnce("Explotar", elapsedTime.milisegundos());

                        //Comprueba que ya ha terminado
                        if(animPortal->getFinished()) {
                            reproduciendoAnim = false;
                        }
                    }
                }else{
                    sprite->setRotation(sprite->getRotation()+15.0);
                }
            }else{
                quitarPortal = true;
                tiempoEmpezar = 0;
                tiempoActivo+=elapsedTime.milisegundos();
                
                animNaranja->play("girar", elapsedTime.milisegundos());
                animAzul->play("girar", elapsedTime.milisegundos());
                animVerde->play("girar", elapsedTime.milisegundos());
                animLila->play("girar", elapsedTime.milisegundos());

                //Si ya ha cambiado 3 veces de patron acaba el desafio y se generan los cofres
                if(numeroCambios == 3){
                    mostrarGeneradores = {false, false, false, false};
                    
                    //Cuando se completa el desafio
                    contrSonido->stopMusica("musicaBulletHell");
                    contrSonido->playMusica("musicaArea");
                    acabado = true;
                    activado = false;
                    interactuable = true;
                    quitarPortal = false;
                    if(!recompensaGenerada){
                        recompensaGenerada = true;
                        generarRecompensa();
                    }
                }
                
                if(primeraVez){
                    tiempoEntreBalas+=elapsedTime.milisegundos();
                    if(!acabado && tiempoEntreBalas >= cadenciaBalas){
                        tiempoEntreBalas -= cadenciaBalas;
                        generarPatrones(numRand*3 + numeroCambios);
                    }
                    //Se comprueba si ha pasado el tiempo necesario entre patrones y se incrementa la cantidad de cambios
                    if(tiempoActivo >= CAMBIO_PATRON){
                        primeraVez = false;
                        tiempoActivo     = 0;
                        tiempoEntreBalas = 0;
                        numeroCambios++;
                    }
                }else{
                    if(tiempoActivo >= TIEMPO_EMPEZAR){
                        tiempoEntreBalas+=elapsedTime.milisegundos();
                        if(!acabado && tiempoEntreBalas >= cadenciaBalas){
                            tiempoEntreBalas -= cadenciaBalas;
                            generarPatrones(numRand*3 + numeroCambios);
                        }
                        //Se comprueba si ha pasado el tiempo necesario entre patrones y se incrementa la cantidad de cambios
                        if(tiempoActivo >= CAMBIO_PATRON){
                            tiempoActivo     = 0;
                            tiempoEntreBalas = 0;
                            numeroCambios++;
                        }
                    }
                }
            }
        }else{
            sprite->setRotation(sprite->getRotation()+3.0);
        }
    }
}

//Cuando se interactua con el elemento del escenario
void PortalDesafio::interactuar(){
    EstadoPartida *estado = EstadoPartida::Instancia();
    
    //Empieza el desafio al interactuar con el elemento
    if(!activado){
        activado = true;
        interactuado = true;
        interactuable = false;
        contrSonido->playMusica("musicaBulletHell");
        contrSonido->stopMusica("musicaArea");
    }

    //Si se ha acabado el tiempo, y se interactua vuelve a la sala de la mazmorra
    if(acabado == true){
        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->fadeIn(20);
        estado->setCambiandoSala(5);
    }

}

void PortalDesafio::generarPatrones(int opc){
    mostrarGeneradores = {false, false, false, false};
    EstadoPartida* estado = EstadoPartida::Instancia();
    vector<float> tamMapa = {(float)estado->getSalaActual()->getTamanyoMapa()[0], (float)estado->getSalaActual()->getTamanyoMapa()[1]};
    int tamTileMapa = estado->getSalaActual()->getConstTamTiles();
    tamMapa[0] *= tamTileMapa;
    tamMapa[1] *= tamTileMapa;
    vector<float> posicionGenerador = {0.0, 0.0};
    Sprite* sProy = nullptr;
    Proyectil* p = nullptr;
    float distNorm = 0.0;
    float anguloDesp = 0.0;
    float nuevoAngulo = 0.0;
    float anguloInv = 0.0;
    float despXInv = 0.0;
    float despYInv = 0.0;
    switch (opc)
    {
        case 0:
            //Crea una espiral de balas
            tamCirculoBalas = 4;
            anguloDesp = 360/tamCirculoBalas;
            cadenciaBalas = 600;
            for(int i = 0; i < tamCirculoBalas; i++){
                //Primera espiral
                posicionGenerador = {26.0f*tamTileMapa, 6.0f*tamTileMapa};
                generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[0] = true;

                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);

                //Segunda espiral
                posicionGenerador = {12.0f*tamTileMapa, 20.0f*tamTileMapa};
                generadorBalasAzul->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[1] = true;

                sProy = new Sprite(texturaBalas, BALAS_AZULES[0], BALAS_AZULES[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);

                //Tercera espiral
                posicionGenerador = {40.0f*tamTileMapa, 20.0f*tamTileMapa};
                generadorBalasLila->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[3] = true;

                sProy = new Sprite(texturaBalas, BALAS_LILAS[0], BALAS_LILAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            angulo += 5;
            break;
        case 1:
            //Crea un circulo de balas y balas de forma horizontal
            cadenciaBalas = 1000;
            tamCirculoBalas = 20;
            anguloDesp = 360/tamCirculoBalas;
            for(int i = 0; i < tamCirculoBalas; i++){
                //Primera espiral
                posicionGenerador = {tamMapa[0]/2, tamMapa[1]/2};
                generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[0] = true;

                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                float a = nuevoAngulo*180/M_PI;
                float velPunta = abs(sin((4)*a*M_PI/360))*50;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc, VEL_BALAS - velPunta);
                estado->anyadirProyectil(p);
            }
            //Dispara desde la izquierda
            posicionGenerador = {32,32};

            sProy = new Sprite(texturaBalas, BALAS_VERDES[0], BALAS_VERDES[1], 16, 16);
            p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0], posicionGenerador[1]+desplazamientoY}, {1, 0},VEL_BALAS);
            estado->anyadirProyectil(p);

            if(posicionGenerador[1]+desplazamientoY >= tamMapa[1]){
                cambioY = -1;
            }
            if(posicionGenerador[1]+desplazamientoY <= 0){
                cambioY = 1;
            }
            desplazamientoY+=64*cambioY;

            //Dispara desde la derecha
            despYInv = -desplazamientoY+cambioY*(-20);
            posicionGenerador = {tamMapa[0]-32, tamMapa[1]-32};
            sProy = new Sprite(texturaBalas, BALAS_VERDES[0], BALAS_VERDES[1], 16, 16);
            p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0], posicionGenerador[1] +despYInv}, {-1, 0},VEL_BALAS);
            estado->anyadirProyectil(p);
            break;
        case 2:
            //Crea tres espirales de balas
            cadenciaBalas = 600;
            tamCirculoBalas = 6;
            anguloDesp = 360/tamCirculoBalas;
            for(int i = 0; i < tamCirculoBalas; i++){
                //Primera espiral
                posicionGenerador = {26.0f*tamTileMapa, 6.0f*tamTileMapa};
                generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[0] = true;

                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);

                //Segunda espiral
                posicionGenerador = {12.0f*tamTileMapa, 20.0f*tamTileMapa};
                generadorBalasAzul->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[1] = true;
                sProy = new Sprite(texturaBalas, BALAS_AZULES[0], BALAS_AZULES[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);

                //Tercera espiral
                posicionGenerador = {40.0f*tamTileMapa, 20.0f*tamTileMapa};
                generadorBalasLila->setPosition(posicionGenerador[0], posicionGenerador[1]);
                mostrarGeneradores[3] = true;

                sProy = new Sprite(texturaBalas, BALAS_LILAS[0], BALAS_LILAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            angulo += 5;
            break;
        case 3:
            //Crea dos espirales en sentidos contrarios y caen balas del techo
            tamCirculoBalas = 4;
            anguloDesp = 360/tamCirculoBalas;
            posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
            generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
            mostrarGeneradores[0] = true;
            for(int i = 0; i < tamCirculoBalas; i++){

                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            angulo += 5;
            anguloInv = -angulo;
            posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
            for(int i = 0; i < tamCirculoBalas; i++){
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = anguloInv + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }

            for(int i = 0; i < tamCirculoBalas/2; i++){
                //Dispara desde arriba
                posicionGenerador = {32,32};
                sProy = new Sprite(texturaBalas, BALAS_AZULES[0], BALAS_AZULES[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0]+desplazamientoX, posicionGenerador[1]}, {0, 1},VEL_BALAS);
                estado->anyadirProyectil(p);

                if(posicionGenerador[0]+desplazamientoX >= tamMapa[0]){
                    cambioX = -1;
                }
                if(posicionGenerador[0]+desplazamientoX <= 0){
                    cambioX = 1;
                }
                desplazamientoX+=128*cambioX;
            }
            break;

        case 4:
            generadorBalasLila->setPosition(tamMapa[0]/2.0f, tamMapa[1]/2.0f);
            mostrarGeneradores[3] = true;
            //Crea una barrera de balas horizontal
            cadenciaBalas = 2000;
            //Parte de arriba
            for(int i = 0; i < tamMapa[1]/64 - 3; i++){
                posicionGenerador = {32.0f,32.0f+tamTileMapa*i};
                sProy = new Sprite(texturaBalas, BALAS_LILAS[0], BALAS_LILAS[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, {1,0},VEL_BALAS/1.5);
                estado->anyadirProyectil(p);
            }
            //Parte de abajo
            for(int i = tamMapa[1]/64 - 3; i < tamMapa[1]/32; i++){
                posicionGenerador = {tamMapa[0],32.0f+tamTileMapa*i};
                sProy = new Sprite(texturaBalas, BALAS_VERDES[0], BALAS_VERDES[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, {-1,0},VEL_BALAS/1.5);
                estado->anyadirProyectil(p);
            }
            break;
        case 5:
            //Espiral central con balas desde abajo
            cadenciaBalas = 200;
            tamCirculoBalas = 8;
            anguloDesp = 360/tamCirculoBalas;
            posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
            generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
            mostrarGeneradores[0] = true;
            for(int i = 0; i < tamCirculoBalas; i++){
                //Primera espiral
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            angulo+=2;


            posicionGenerador = {32,tamMapa[1]-32};
            for(int i = 0; i < tamCirculoBalas/6; i++){
                //Dispara desde arriba
                sProy = new Sprite(texturaBalas, BALAS_AZULES[0], BALAS_AZULES[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0]+desplazamientoX, posicionGenerador[1]}, {0, -1},VEL_BALAS);
                estado->anyadirProyectil(p);

                if(posicionGenerador[0]+desplazamientoX >= tamMapa[0]){
                    cambioX = -1;
                }
                if(posicionGenerador[0]+desplazamientoX <= 0){
                    cambioX = 1;
                }
                desplazamientoX+=320*cambioX;
            }
            break;
        case 6:
            cadenciaBalas = 900;
            tamCirculoBalas = 1 + increBalas;
            generadorBalasNaranja->setPosition(tamMapa[0]/2.0f, tamMapa[1]/2.0f);
            mostrarGeneradores[0] = true;
            for(int i = 0; i < tamCirculoBalas; i++){
                //Dispara desde arriba
                posicionGenerador = {32,32};
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0]+desplazamientoX, posicionGenerador[1]}, {0, 1},VEL_BALAS);
                estado->anyadirProyectil(p);

                if(posicionGenerador[0]+desplazamientoX >= tamMapa[0]){
                    cambioX = -1;
                }
                if(posicionGenerador[0]+desplazamientoX <= 0){
                    cambioX = 1;
                }
                desplazamientoX+=200*cambioX;

                //Dispara desde la izquierda
                posicionGenerador = {32,32};
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0], posicionGenerador[1]+desplazamientoY}, {1, 0},VEL_BALAS);
                estado->anyadirProyectil(p);

                if(posicionGenerador[1]+desplazamientoY >= tamMapa[1]){
                    cambioY = -1;
                }
                if(posicionGenerador[1]+desplazamientoY <= 0){
                    cambioY = 1;
                }
                desplazamientoY+=64*cambioY;

                //Dispara desde abajo
                despXInv = desplazamientoX+cambioX*(-20);
                posicionGenerador = {32,tamMapa[1]-32};
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0]+despXInv, posicionGenerador[1]}, {0, -1},VEL_BALAS+50);
                estado->anyadirProyectil(p);

                //Dispara desde la derecha
                despYInv = desplazamientoY+cambioY*(-20);
                posicionGenerador = {tamMapa[0]-32, 32};
                sProy = new Sprite(texturaBalas, BALAS_VERDES[0], BALAS_VERDES[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, {posicionGenerador[0], posicionGenerador[1] +despYInv}, {-1, 0},VEL_BALAS+150);
                estado->anyadirProyectil(p);
            }
            increBalas += 0.15;
            break;
        case 7:
            //Crea dos espirales de balas en sentidos opuestos
            cadenciaBalas = 300;
            tamCirculoBalas = 6;
            anguloDesp = 360/tamCirculoBalas;
            posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
            generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
            mostrarGeneradores[0] = true;
            for(int i = 0; i < tamCirculoBalas; i++){
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            angulo += 5;

            //Para el otro lado
            anguloInv = -angulo;
            for(int i = 0; i < tamCirculoBalas; i++){
                posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = anguloInv + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS);
                estado->anyadirProyectil(p);
            }
            break;
        case 8:
            cadenciaBalas = 500;
            //Parte de la izquierda
            for(int i = 0; i < tamMapa[0]/128; i++){
                posicionGenerador = {(float)tamTileMapa*i,32.0f};
                sProy = new Sprite(texturaBalas, BALAS_LILAS[0], BALAS_LILAS[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, {0,1},VEL_BALAS/1.5);
                estado->anyadirProyectil(p);
            }
            //Parte de la derecha
            for(int i = 0; i < tamMapa[0]/128; i++){
                posicionGenerador = {(tamMapa[0]*3/4)+tamTileMapa*i,32.0f};
                sProy = new Sprite(texturaBalas, BALAS_VERDES[0], BALAS_VERDES[1], 16, 16);
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, {0,1},VEL_BALAS/1.5);
                estado->anyadirProyectil(p);
            }

            //Doble espiral
            tamCirculoBalas = 8;
            anguloDesp = 360/tamCirculoBalas;
            posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
            generadorBalasNaranja->setPosition(posicionGenerador[0], posicionGenerador[1]);
            mostrarGeneradores[0] = true;
            for(int i = 0; i < tamCirculoBalas; i++){
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = angulo + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS/1.2);
                estado->anyadirProyectil(p);
            }
            angulo += 5;

            //Para el otro lado
            anguloInv = -angulo;
            for(int i = 0; i < tamCirculoBalas; i++){
                posicionGenerador = {tamMapa[0]/2.0f, tamMapa[1]/2.0f};
                sProy = new Sprite(texturaBalas, BALAS_NARANJAS[0], BALAS_NARANJAS[1], 16, 16);

                //Se cambia la direccion
                nuevoAngulo = anguloInv + i*anguloDesp;
                nuevoAngulo*=M_PI/180;
                direcc[0] = cos(nuevoAngulo);
                direcc[1] = sin(nuevoAngulo);
                distNorm = (static_cast<float>(sqrt(pow(direcc[0], 2) + pow(direcc[1], 2))));
                direcc[0] /= distNorm;
                direcc[1] /= distNorm;
                p = new ProyectilBulletHell(DANYO_BALAS, sProy, posicionGenerador, direcc,VEL_BALAS/1.2);
                estado->anyadirProyectil(p);
            }
            break;
        default:
            break;
    }
}

void PortalDesafio::generarRecompensa(){
    EstadoPartida* estado = EstadoPartida::Instancia();
    int tamTile = estado->getSalaActual()->getConstTamTiles();
    Textura* tc = new Textura("resources/sprites/generales/Chests.png");
    Sprite* sc = new Sprite(tc, 0, 0, 48, 32);
    ElementoEntorno* cofre = new Cofre(tc, sc);
    cofre->setPosicion({19.0f*tamTile, 24.0f*tamTile});
    estado->getSalaActual()->anyadirElementoEntorno(cofre);
    Textura* tc2 = new Textura("resources/sprites/generales/Chests.png");
    Sprite* sc2 = new Sprite(tc2, 0, 0, 48, 32);
    ElementoEntorno* cofre2 = new Cofre(tc2, sc2);
    cofre2->setPosicion({22.0f*tamTile, 24.0f*tamTile});
    estado->getSalaActual()->anyadirElementoEntorno(cofre2);
    Textura* tc3 = new Textura("resources/sprites/generales/Chests.png");
    Sprite* sc3 = new Sprite(tc3, 0, 0, 48, 32);
    ElementoEntorno* cofre3 = new Cofre(tc3, sc3);
    cofre3->setPosicion({28.0f*tamTile, 24.0f*tamTile});
    estado->getSalaActual()->anyadirElementoEntorno(cofre3);
    Textura* tc4 = new Textura("resources/sprites/generales/Chests.png");
    Sprite* sc4 = new Sprite(tc4, 0, 0, 48, 32);
    ElementoEntorno* cofre4 = new Cofre(tc4, sc4);
    cofre4->setPosicion({31.0f*tamTile, 24.0f*tamTile});
    estado->getSalaActual()->anyadirElementoEntorno(cofre4);
}



bool PortalDesafio::getInteractuando(){
    return interactuado;
}