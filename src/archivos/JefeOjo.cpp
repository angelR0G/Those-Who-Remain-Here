#include "JefeOjo.h"

const int VIDA          = 12000;
const int DANYO         = 1;
const int TIEMPO_REC    = 3000;
const int ESENCIAS      = 6000;
const vector<int> MAT   = {};
const string RUTA_TEX   = "resources/sprites/jefes/OjoTileset.png";
const string RUTA_TEX2  = "resources/sprites/jefes/balasOjo.png";
const string RUTA_TEX4  = "resources/sprites/jefes/martilloOjo.png";
const string RUTA_FONT  = "resources/sprites/generales/Chonkly.otf";
const string RUTA_HUD   = "resources/sprites/personaje/icons.png";
const int TAM_TILE      = 32;
const int VELOCIDAD_MOV = 70;

//Constantes barra de vida
const float ESCALA_VIDA_Y       = 8.0;
const float ESCALA_VIDA_X       = 20.0;
const float ESCALA_VIDA_EXTREMO = 6.0;
const float DESPLAZAMIENTO_Y    = 90.0;
const int TAM_FRAG_VIDA         = 23;

//Constantes de ataques
const int TIEMPO_ENTRE_ATAQUES  = 700;
const int TIEMPO_ENTRE_ATAQUES_RAYO  = 20000;
const int TIEMPO_ENTRE_ATAQUES_ESPIRAL  = 800;
const int CANT_ATAQUES_ESPIRAL  = 5;
const int CANT_ATAQUES_PIEDRAS  = 3;
const int CANT_MAX_DISPAROS_PIEDRAS = 5;
const int TIEMPO_LANZAR_PIEDRAS = 1500;
const int CANT_ATAQUE_RAYOS     = 30;

const int TIEMPO_PREPARAR_RAYOS = 6000;
const int TIEMPO_ATAQUE_RAYOS  = 4000;

//Constantes de la animaciones
const string ANIM_ESPERANDO         = "JefeOjoEsperando";
const string ANIM_ATAQUE_PROY       = "JefeOjoAtaqueProy";
const string ANIM_REC_ATAQUE_PROY   = "JefeOjoRecAtaqueProy";
const string ANIM_ATAQUE_RAYOS      = "JefeOjoAtaqueRayos";
const string ANIM_MUERE             = "JefeOjoMuere";

//Constructor
JefeOjo::JefeOjo():Enemigo(VIDA, DANYO, VELOCIDAD_MOV, TIEMPO_REC, ESENCIAS, MAT, nullptr, nullptr) {
    tex     = new Textura(RUTA_TEX);
    sprite  = new Sprite(tex, 0, 0, 32, 32);
    sprite->setOrigin(16, 16);
    sprite->setScale(4.0, 4.0);

    ataque1         = 0;
    ataque2         = 0;
    numDisparos     = 0;
    enfadado        = false;
    contador        = -500;
    estado          = JefeOjoEsperando;
    tamCirculoBalas = 6;
    direccBalas     = {-1, -1};
    desplazamiento  = 0;
    terminaPiedras  = false;
    contadorLanzaPiedras = 0;
    piedrasLanzadas = false;
    preparandoRayos = false;
    contadorRayos   = 0;

    texturas.push_back(new Textura(RUTA_TEX2));
    /*texturas.push_back(new Textura(RUTA_TEX3));*/
    animacion = new Animacion(sprite);
    animacion->addAnimation(ANIM_ESPERANDO, 200.0, 0, 0, 5, 0, 32, 32);
    animacion->addAnimation(ANIM_ATAQUE_PROY, 200.0, 0, 1, 5, 1, 32, 32);
    animacion->addAnimation(ANIM_REC_ATAQUE_PROY, 200.0, 0, 2, 5, 2, 32, 32);
    animacion->addAnimation(ANIM_ATAQUE_RAYOS, 200.0, 0, 3, 5, 3, 32, 32);
    animacion->addAnimation(ANIM_MUERE, 200.0, 0, 5, 6, 6, 32, 32);

    //Posiciones de la barrera invisible
    barreraInvisible    = {{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}};
    barreraColocada     = false;

    //Carga el texto
    fuente      = new Fuente(RUTA_FONT);
    textoNombre = new Texto("Ocom, Guardian de la Caverna Polvorienta");
    textoNombre->setFuente(fuente);
    textoNombre->setTamanyo(36);

    //Carga los sprites de la barra de vida
    texVida     = new Textura(RUTA_HUD);
    Sprite* sBarraIz    = new Sprite(texVida, 65, 38, 3, 7);
    sBarraIz->setScale(ESCALA_VIDA_EXTREMO, ESCALA_VIDA_Y);
    Sprite* sBarra      = new Sprite(texVida, 68, 38, 40, 7);
    sBarra->setScale(ESCALA_VIDA_X, ESCALA_VIDA_Y);
    Sprite* sBarraDer   = new Sprite(texVida, 108, 38, 3, 7);
    sBarraDer->setScale(ESCALA_VIDA_EXTREMO, ESCALA_VIDA_Y);

    spritesVida.push_back(sBarraIz);
    spritesVida.push_back(sBarra);
    spritesVida.push_back(sBarraDer);

    Sprite* sSaludIz    = new Sprite(texVida, 123, 84, 2, 7);
    sSaludIz->setOrigin(2.0, 0.0);
    sSaludIz->setScale(ESCALA_VIDA_EXTREMO*(-1.0f), ESCALA_VIDA_Y);
    Sprite* sSaludDer   = new Sprite(texVida, 123, 84, 2, 7);
    sSaludDer->setScale(ESCALA_VIDA_EXTREMO, ESCALA_VIDA_Y);

    spritesVida.push_back(sSaludIz);

    int tamBarraSalud   = sBarra->getGlobalPixelSize()[0];
    for(int i=0; i<tamBarraSalud; i+=TAM_FRAG_VIDA) {
        Sprite* sSalud = new Sprite(texVida, 121, 52, TAM_FRAG_VIDA, 5);
        if(i + TAM_FRAG_VIDA > tamBarraSalud) {
            //Si es el ultimo sprite, se recorta para ajustarse
            sSalud->setRect(121, 52, tamBarraSalud-i, 5);
        }
        sSalud->setScale(1.0, ESCALA_VIDA_Y);
        spritesVida.push_back(sSalud);
    }

    spritesVida.push_back(sSaludDer);
}

//Destructor
JefeOjo::~JefeOjo() {
    delete animacion;
    delete fuente;
    delete textoNombre;
    delete texVida;

    while(spritesVida.size() > 0) {
        if(spritesVida[spritesVida.size()-1] != nullptr) {
            delete spritesVida[spritesVida.size()-1];
        }
        spritesVida.pop_back();
    }

    while(spritesPiedras.size() > 0) {
        if(spritesPiedras[spritesPiedras.size()-1] != nullptr) {
            delete spritesPiedras[spritesPiedras.size()-1];
        }
        spritesPiedras.pop_back();
    }

    while(texturas.size() > 0) {
        if(texturas[texturas.size()-1] != nullptr) {
            delete texturas[texturas.size()-1];
        }
        texturas.pop_back();
    }
    direccBalas.clear();
}

//Actualiza el jefe
void JefeOjo::update(Tiempo t) {
    Enemigo::update(t);
    
    
    //Al comenzar el combate coloca la barrera
    if(!barreraColocada) {
        Sala* sala  = EstadoPartida::Instancia()->getSalaActual();
        int idCol   = sala->getIdColision(1);
        map<vector<int>, int> colisionesBarrera;

        for(unsigned int i=0; i<barreraInvisible.size(); i++) {
            colisionesBarrera[barreraInvisible[i]] = idCol;
        }

        sala->anyadirColision(&colisionesBarrera);
    }

    //Comprueba si se ha detenido el tiempo
    if(!detenido) {
        
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();
        if(piedrasLanzadas){
            contadorLanzaPiedras += t.milisegundos() + tiempoAntes.milisegundos();
        }
        if(preparandoRayos){
            contadorRayos += t.milisegundos() + tiempoAntes.milisegundos();
        }
    
        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        float distNorm = 0;
        posicionAnterior = posicion;
        //Movimiento del boss
        if(direccMov){
            //Desplaza derecha
            desplazamiento = getVelocidad()*t.segundos();
        }else{
            //Desplaza izquierda
            desplazamiento = getVelocidad()*t.segundos()*(-1);
        }
        comprobarColisiones();
        posicion[0]+=desplazamiento;

        EstadoPartida* epartida = EstadoPartida::Instancia();
        vector<int> tamMapa = epartida->getSalaActual()->getTamanyoMapa();
        Jugador* j = epartida->getJugador();
        vector<float> posJugador = j->getPosNueva();
        float distancia = sqrt(pow(posJugador[0]-posicion[0], 2) + pow(posJugador[1]-posicion[1], 2));

        //Comprueba el estado del enemigo
        switch(estado) {
            case JefeOjoEsperando:
                //Actualiza la animacion
                animacion->play(ANIM_ESPERANDO, t.milisegundos());

                //Comprueba si ha pasado el tiempo entre ataques
                if(contador >= tiempoRecuperacion || (enfadado && contador >= tiempoRecuperacion/2)) {
                    bool ataca = false;
                    if(ataque1 + ataque2 >= 4) {
                        estado = JefeOjoPreparaRayos;
                        preparandoRayos = true;
                        contador = TIEMPO_ENTRE_ATAQUES_RAYO;
                        ataque1 = 0;
                        ataque2 = 0;
                        ataca   = true;
                    }
                    else if(!preparandoRayos){
                        int random = rand() % 2;
                        switch(random) {
                            case 0:
                                if(ataque1 < 2) {
                                    estado = JefeOjoAtaque1;
                                    ataque1++;
                                    ataca = true;
                                }
                                break;
                            case 1:
                                if(ataque2 < 2) {
                                    estado = JefeOjoAtaque2;
                                    ataque2++;
                                    ataca = true;
                                }
                                break;
                        }
                    }

                    //Reinicia variables si va a atacar
                    if(ataca) {
                        if(!preparandoRayos){
                            contador = 0;
                        }
                        numDisparos = 0;
                    }
                }
                break;
            case JefeOjoAtaque1:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_PROY, t.milisegundos());
                //Genera una arco de proyectiles donde el punto central apunta al jugador
                if(contador >= TIEMPO_ENTRE_ATAQUES_ESPIRAL || (enfadado && contador >= TIEMPO_ENTRE_ATAQUES_ESPIRAL/2)) {
                    
                    direccBalas = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                    for(int i=0; i<tamCirculoBalas; i++) {
                        //Genera el proyectil y lo anyade a la sala
                        Sprite* s = new Sprite(texturas[0], 0, 10, 10, 20);
                        s->setOrigin(5, 10);
                        s->setScale(3.0, 3.0);
                        
                        float aBala = atan2(direccBalas[1],direccBalas[0])*180/M_PI;
                        s->setRotation(aBala-90);

                        ProyectilRecto* p = new ProyectilRecto(DANYO, 0, s, posicion, direccBalas, 420, 10000, {});
                        EstadoPartida::Instancia()->anyadirProyectil(p);
                        aBala += 15;
                        aBala*=M_PI/180;
                        direccBalas[0] = cos(aBala);
                        direccBalas[1] = sin(aBala);
                        distNorm = (static_cast<float>(sqrt(pow(direccBalas[0], 2) + pow(direccBalas[1], 2))));
                        direccBalas[0] /= distNorm;
                        direccBalas[1] /= distNorm;
                    }
                    direccBalas = {(posJugador[0]-posicion[0])/distancia, (posJugador[1]-posicion[1])/distancia};
                    float aBala = atan2(direccBalas[1],direccBalas[0])*180/M_PI;
                    for(int i=0; i<tamCirculoBalas; i++) {
                        //Genera el proyectil y lo anyade a la sala
                        Sprite* s = new Sprite(texturas[0], 0, 10, 10, 20);
                        s->setOrigin(5, 10);
                        s->setScale(3.0, 3.0);
                        
                        aBala = atan2(direccBalas[1],direccBalas[0])*180/M_PI;
                        s->setRotation(aBala-90);

                        ProyectilRecto* p = new ProyectilRecto(DANYO, 0, s, posicion, direccBalas, 420, 10000, {});
                        EstadoPartida::Instancia()->anyadirProyectil(p);
                        aBala -= 15;
                        aBala*=M_PI/180;
                        direccBalas[0] = cos(aBala);
                        direccBalas[1] = sin(aBala);
                        distNorm = (static_cast<float>(sqrt(pow(direccBalas[0], 2) + pow(direccBalas[1], 2))));
                        direccBalas[0] /= distNorm;
                        direccBalas[1] /= distNorm;
                    }

                    numDisparos++;
                    contador = 0;
                }

                if((!enfadado && numDisparos >= CANT_ATAQUES_ESPIRAL) || numDisparos >= CANT_ATAQUES_ESPIRAL*2) {
                    //Al terminar el ataque vuelve al estado Esperando
                    estado = JefeOjoEsperando;
                }
                break;
            case JefeOjoAtaque2:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_PROY, t.milisegundos());

                //Comprueba que ha pasado el tiempo entre filas
                if(contador >= TIEMPO_ENTRE_ATAQUES*3.5) {
                    //Controla cuantas piedras spawnea durante el ataque dependiendo de si esta enfadado o no
                    int piedras = CANT_MAX_DISPAROS_PIEDRAS;
                    if(enfadado){
                        piedras+=2;
                    }
                    //Genera piedras en una posicion aleatoria del mapa
                    if(!piedrasLanzadas){
                        for(int i=0; i<=piedras; i++) {
                            int randomX = rand() % tamMapa[0];
                            int randomY = rand() % tamMapa[1];

                            //Genera el sprite de la piedra
                            Sprite* s = new Sprite(texturas[0], 0, 0, 20, 10);
                            s->setOrigin(10, 5);
                            s->setScale(3.0, 3.0);
                            s->setRotation(90);
                            s->setPosition(randomX*TAM_TILE, randomY*TAM_TILE);
                            spritesPiedras.push_back(s);
                        }
                    }
                    piedrasLanzadas = true;

                    if(contadorLanzaPiedras>=TIEMPO_LANZAR_PIEDRAS){
                        terminaPiedras = true;
                        contadorLanzaPiedras = 0;
                        piedrasLanzadas = false;
                        for(unsigned int i = 0; i < spritesPiedras.size(); i++){
                            float dist = sqrt(pow(posJugador[0]-spritesPiedras[i]->getPosition()[0], 2) + pow(posJugador[1]-spritesPiedras[i]->getPosition()[1], 2));
                            vector<float> direcc  = {(posJugador[0]-spritesPiedras[i]->getPosition()[0])/dist, (posJugador[1]-spritesPiedras[i]->getPosition()[1])/dist};
                            float anguloPiedras = atan2(direcc[1],direcc[0]) * 180/M_PI;
                            spritesPiedras[i]->setRotation(anguloPiedras);
                            ProyectilBulletHell* p = new ProyectilBulletHell(DANYO, spritesPiedras[i], spritesPiedras[i]->getPosition(), direcc, 400);
                            EstadoPartida::Instancia()->anyadirProyectil(p);
                        }
                        spritesPiedras.clear();
                        numDisparos++;
                    }

                    
                    contador = 0;
                }
                
                if((!enfadado && numDisparos >= CANT_ATAQUES_PIEDRAS) || numDisparos >= CANT_ATAQUES_PIEDRAS*1.5) {
                    //Al terminar el ataque vuelve al estado Esperando
                    estado = JefeOjoEsperando;
                    piedrasLanzadas = false;
                    contadorLanzaPiedras = 0;
                    terminaPiedras = false;
                }
                break;

            case JefeOjoPreparaRayos:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_RAYOS, t.milisegundos());
                //Comprueba que ha pasado el tiempo entre filas
                if(contador >= TIEMPO_ENTRE_ATAQUES_RAYO) {
                    int random = rand() % CANT_ATAQUE_RAYOS;
                    float inicio = 3*TAM_TILE - TAM_TILE/2;

                    //Va generando proyectiles en filas dejando un hueco
                    for(unsigned int i=0; i<=CANT_ATAQUE_RAYOS; i++) {
                        if((int)i != random && (int)i != random+1 && (int)i != random+2 && (int)i != random-1 && (int)i != random-2) {
                            //Genera el proyectil y lo anyade en la sala
                            Sprite* s = new Sprite(texturas[0], 10, 20, 10, 10);
                            s->setPosition(inicio, (float)1*TAM_TILE);
                            ProyectilRayoOjo* p = new ProyectilRayoOjo(2, 0, s, {inicio, (float)1*TAM_TILE}, {0, 1}, 340, TIEMPO_PREPARAR_RAYOS, TIEMPO_ATAQUE_RAYOS, {3,(float)(tamMapa[1]*32)-(6*32)});
                            EstadoPartida::Instancia()->anyadirProyectil(p);
                        }

                        //Incrementa la posicion del siguiente disparo
                        inicio += 60*TAM_TILE/CANT_ATAQUE_RAYOS;
                    }
                    preparandoRayos = true;
                    numDisparos++;
                    contador = 0;
                }

                if(contadorRayos >= (TIEMPO_PREPARAR_RAYOS + TIEMPO_ATAQUE_RAYOS)){
                    if(numDisparos >= 1) {
                        //Al terminar el ataque vuelve al estado Esperando
                        estado = JefeOjoEsperando;
                        preparandoRayos = false;
                        contador = 0;
                        contadorRayos = 0;
                    }
                }
                break;
            case JefeOjoDerrotado:
                if(contador >= 2000) {
                    //Reproduce las animacion de muerte tras 2 segundos
                    animacion->playOnce(ANIM_MUERE, t.milisegundos());

                    //Comprueba cuando termina
                    if(animacion->getFinished()) {
                        muerto = true;
                        //Elimina la barrera que impide evitar al jefe
                        Sala* sala = EstadoPartida::Instancia()->getSalaActual();
                        sala->eliminarColision(&barreraInvisible);

                        //Crea un elemento de entorno con el sprite de la lengua y lo coloca en la sala
                        Textura* tex    = new Textura(RUTA_TEX4);
                        Sprite* s       = new Sprite(tex, 0, 0, 40, 26);
                        s->setScale(3.0, 3.0);
                        s->setOrigin(2, 0);
                        ElementoEntorno* martillo = new ElementoEntorno(tex, s, false);
                        martillo->setPosicion(posicion);
                        sala->anyadirElementoEntorno(martillo);
                    }
                }
                else {
                    textoNombre->setColorTexto({255, 255, 255, static_cast<int>((1.0-contador/2000.0f)*255)});
                    spritesVida[0]->setColor({255, 255, 255, static_cast<int>((1.0-contador/2000.0f)*255)});
                    spritesVida[1]->setColor({255, 255, 255, static_cast<int>((1.0-contador/2000.0f)*255)});
                    spritesVida[2]->setColor({255, 255, 255, static_cast<int>((1.0-contador/2000.0f)*255)});
                }
                break;
        }
    }

    if(estado != JefeOjoDerrotado) {
        //Comprueba que el jugador no ha chocado con el jefe
        Jugador* jugador = EstadoPartida::Instancia()->getJugador();
        if(sprite->intersect(jugador->getSprite())) {
            jugador->danyarVida(danyo);
        }
    }
}

//Sobrecarga del metodo perderVida de Enemgio. Comprueba si ha perdido la mitad de la vida para enfurecerse.
void JefeOjo::perderVida(int d) {
    //Comprueba si el jefe muere al perder vida
    if(vida - d > 0) {
        Enemigo::perderVida(d);
        if(vida <= 0) {
            //Evita problemas
            muerto = false;
            vida = 1;
        }

        //Si pierde la mitad de la vida se enfurece
        if(vida < VIDA/2) {
            enfadado = true;
        }
    }
    else if(estado != JefeOjoDerrotado){
        //Cambia el estado a derrotado
        estado = JefeOjoDerrotado;

        //Genera particulas rojas
        vector<int> tam = sprite->getGlobalPixelSize();
        for(int i=0; i<20; i++) {
            int posX = rand() % tam[0] - tam[0]/2 + posicion[0];
            int posY = posicion[1] - rand() % tam[1];
            new ParticulaSimpleCuadrada({(float)posX, (float)posY}, {255, 0, 0, 255}, 30, 5000);
        }

        contador = 0;
    }
}

//Actualiza la posicon del enemigo y de los sprites de la lengua
void JefeOjo::setPosicion(vector<float> nuevaPos) {
    Enemigo::setPosicion(nuevaPos);
}

//Dibuja el jefe
void JefeOjo::dibujar(float percent, int updateMilisegundos) {
    Enemigo::dibujar(percent, updateMilisegundos);

    //Dibuja las piedras
    if(piedrasLanzadas){
        for(unsigned int i = 0; i < spritesPiedras.size(); i++){
            spritesPiedras[i]->draw();
        }
    }

    //Dibuja la salud
    vector<float> centroVista   = EstadoPartida::Instancia()->getVista()->getCentro();
    vector<unsigned int> aux    = VentanaRender::Instancia()->getSize();
    vector<int> tamVentana      = {(int)aux[0], (int)aux[1]};
    int tamBarraSalud   = spritesVida[1]->getGlobalPixelSize()[0];
    int pixelsVidaMax   = tamBarraSalud + spritesVida[3]->getGlobalPixelSize()[0]*2;
    int pixelsVida      = pixelsVidaMax*(vida/(float)VIDA);

    //Pone la posicion de la barra de vida
    spritesVida[0]->setPosition(centroVista[0] - tamBarraSalud/2 - spritesVida[0]->getGlobalPixelSize()[0],
                                centroVista[1] + tamVentana[1]/2 - DESPLAZAMIENTO_Y);
    spritesVida[1]->setPosition(centroVista[0] - tamBarraSalud/2, centroVista[1] + tamVentana[1]/2 - DESPLAZAMIENTO_Y);
    spritesVida[2]->setPosition(centroVista[0] + tamBarraSalud/2, centroVista[1] + tamVentana[1]/2 - DESPLAZAMIENTO_Y);
    spritesVida[0]->draw();
    spritesVida[1]->draw();
    spritesVida[2]->draw();

    //Pone la posicion de los sprites que rellenan la barra
    int desp = 1*ESCALA_VIDA_EXTREMO;
    for(unsigned int i=3; i<spritesVida.size() && pixelsVida > 0; i++) {
        spritesVida[i]->setPosition(centroVista[0] - tamBarraSalud/2 - spritesVida[0]->getGlobalPixelSize()[0] + desp,
                                    centroVista[1] + tamVentana[1]/2 - DESPLAZAMIENTO_Y + ESCALA_VIDA_Y);

        //Comprueba si se tiene que recortar
        if(i == 3 && pixelsVida < spritesVida[0]->getGlobalPixelSize()[0]) {
            spritesVida[i]->setRect(123+(2-pixelsVida/(int)ESCALA_VIDA_EXTREMO), 84, pixelsVida/(int)ESCALA_VIDA_EXTREMO, 5);
            spritesVida[i]->setOrigin(floor(pixelsVida/ESCALA_VIDA_EXTREMO), 0.0);
        }
        else if(pixelsVida > 0 && pixelsVida < spritesVida[i]->getGlobalPixelSize()[0]){
            if(i == spritesVida.size()-1) {
                spritesVida[i]->setRect(123, 84, pixelsVida/(int)ESCALA_VIDA_EXTREMO, 5);
            }
            else {
                spritesVida[i]->setRect(121, 52, pixelsVida, 5);
            }
        }

        spritesVida[i]->draw();
        if(i == 3 || i == spritesVida.size()-1) {
            desp        += 12;
            pixelsVida  -= 12;
        }
        else if(i == spritesVida.size()-2) {
            desp        += tamBarraSalud%TAM_FRAG_VIDA;
            pixelsVida  -= tamBarraSalud%TAM_FRAG_VIDA;
        }
        else {
            desp        += TAM_FRAG_VIDA;
            pixelsVida  -= TAM_FRAG_VIDA;
        }

        //Dibuja el nombre
        textoNombre->setPosicion(centroVista[0] - textoNombre->getAncho()/2, 
                                centroVista[1] + tamVentana[1]/2 - DESPLAZAMIENTO_Y - spritesVida[1]->getGlobalPixelSize()[1]);
        textoNombre->draw();
    }
    
}

void JefeOjo::comprobarColisiones(){
    EstadoPartida *estado               = EstadoPartida::Instancia();
    map<vector<int>, int>* colisionMap  = estado->getSalaActual()->getMapaColisiones();
    vector<int> tamMapa                 = estado->getSalaActual()->getTamanyoMapa();
    int tamTiles                        = estado->getSalaActual()->getConstTamTiles();
    vector<int> posComprobar = {-1,-1};

    //Id del tile a comprobar
    int idCol         = estado->getSalaActual()->getIdColision(1);
    //Comprueba que no se ha salido del mapa
    if(posicion[0]>=0 && posicion[0]/32<tamMapa[0] && posicion[1]>=0 && posicion[1]/32<tamMapa[1]) {
        //Parte central derecha
        posComprobar[0] = (posicion[0]+64)/tamTiles;
        posComprobar[1] = (posicion[1]+0)/tamTiles;
        auto tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol){
                direccMov = false;
            }
        }

        //Parte central izquierda
        posComprobar[0] = (posicion[0]-64)/tamTiles;
        posComprobar[1] = (posicion[1]+0)/tamTiles;
        tile = colisionMap->find(posComprobar);
        if(tile != colisionMap->end()){
            if(tile->second == idCol){
                direccMov = true;
            }
        }
    }
}