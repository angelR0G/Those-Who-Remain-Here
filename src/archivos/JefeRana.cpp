#include "JefeRana.h"

const int VIDA          = 8000;
const int DANYO         = 1;
const int VELOCIDAD     = 800;
const int TIEMPO_REC    = 3000;
const int ESENCIAS      = 3000;
const vector<int> MAT   = {};
const string RUTA_TEX   = "resources/sprites/jefes/RanaTileset.png";
const string RUTA_TEX2  = "resources/sprites/jefes/Lengua.png";
const string RUTA_TEX3  = "resources/sprites/jefes/awa.png";
const string RUTA_TEX4  = "resources/sprites/jefes/LenguaRana.png";
const string RUTA_FONT  = "resources/sprites/generales/Chonkly.otf";
const string RUTA_HUD   = "resources/sprites/personaje/icons.png";
const int TAM_TILE      = 32;

//Constantes barra de vida
const float ESCALA_VIDA_Y       = 8.0;
const float ESCALA_VIDA_X       = 20.0;
const float ESCALA_VIDA_EXTREMO = 6.0;
const float DESPLAZAMIENTO_Y    = 90.0;
const int TAM_FRAG_VIDA         = 23;

//Constantes de ataques
const int TIEMPO_ENTRE_ATAQUES  = 700;
const int CANT_ATAQUES_TECHO    = 12;
const int CANT_ATAQUES_ABANICO  = 4;
const int CANT_DISPAROS_ABANICO = 9;
const float DESP_SPRITE_LENGUA  = -80.0;
const int TAM_FRAG_LENGUA       = 42;

//Constantes de la animaciones
const string ANIM_ESPERANDO     = "JefeRanaEsperando";
const string ANIM_ATAQUE_AGUA   = "JefeRanaAtaqueAgua";
const string ANIM_ABRE_BOCA     = "JefeRanaAbreBoca";
const string ANIM_CIERRA_BOCA   = "JefeRanaCierraBoca";
const string ANIM_MUERE         = "JefeRanaMuere";

//Constructor
JefeRana::JefeRana():Enemigo(VIDA, DANYO, VELOCIDAD, TIEMPO_REC, ESENCIAS, MAT, nullptr, nullptr) {
    tex     = new Textura(RUTA_TEX);
    sprite  = new Sprite(tex, 0, 0, 60, 60);
    sprite->setOrigin(29, 60);
    sprite->setScale(3.5, 3.5);

    ataque1         = 0;
    ataque2         = 0;
    ataque3         = 0;
    numDisparos     = 0;
    enfadado        = false;
    contador        = -500;
    colisionLengua  = false;
    estado          = JefeRanaEsperando;

    texturas.push_back(new Textura(RUTA_TEX2));
    texturas.push_back(new Textura(RUTA_TEX3));

    animacion = new Animacion(sprite);
    animacion->addAnimation(ANIM_ESPERANDO, 200.0, 0, 0, 5, 0, 60, 60);
    animacion->addAnimation(ANIM_ATAQUE_AGUA, 200.0, 0, 1, 5, 1, 60, 60);
    animacion->addAnimation(ANIM_ABRE_BOCA, 200.0, 0, 2, 5, 2, 60, 60);
    animacion->addAnimation(ANIM_CIERRA_BOCA, 200.0, 0, 3, 5, 3, 60, 60);
    animacion->addAnimation(ANIM_MUERE, 200.0, 0, 4, 5, 5, 60, 60);

    //Crea los sprites para el ataque de la lengua
    Sprite* puntaLengua = new Sprite(texturas[0], 0, 0, 11, 9);
    puntaLengua->setOrigin(2, 4);
    puntaLengua->setScale(3.0, 3.0);
    puntaLengua->setColor({255, 255, 255, 0});
    puntaLengua->setPosition(posicion[0], posicion[1]+DESP_SPRITE_LENGUA);
    spritesAtaques.push_back(puntaLengua);

    for(int i=0; i<=40*TAM_TILE/TAM_FRAG_LENGUA; i++) {
        Sprite* fragLengua = new Sprite(texturas[0], 1, 10, 14, 5);
        fragLengua->setOrigin(0, 2);
        fragLengua->setScale(3.0, 3.0);
        fragLengua->setColor({255, 255, 255, 0});
        fragLengua->setPosition(posicion[0], posicion[1]+DESP_SPRITE_LENGUA);
        spritesAtaques.push_back(fragLengua);
    }

    //Posiciones de la barrera invisible
    barreraInvisible    = {{41, 16}, {41, 17}, {41, 18}, {41, 19}, {41, 20}, {41, 21}, {41, 22}, {41, 23}, {41, 24}};
    barreraColocada     = false;

    //Carga el texto
    fuente      = new Fuente(RUTA_FONT);
    textoNombre = new Texto("Gorf, Guardian del Bosque de los Lamentos");
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
JefeRana::~JefeRana() {
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

    while(spritesAtaques.size() > 0) {
        if(spritesAtaques[spritesAtaques.size()-1] != nullptr) {
            delete spritesAtaques[spritesAtaques.size()-1];
        }
        spritesAtaques.pop_back();
    }

    while(texturas.size() > 0) {
        if(texturas[texturas.size()-1] != nullptr) {
            delete texturas[texturas.size()-1];
        }
        texturas.pop_back();
    }
}

//Actualiza el jefe
void JefeRana::update(Tiempo t) {
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

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();

        //Comprueba el estado del enemigo
        switch(estado) {
            case JefeRanaEsperando:
                //Actualiza la animacion
                animacion->play(ANIM_ESPERANDO, t.milisegundos());

                //Comprueba si ha pasado el tiempo entre ataques
                if(contador >= tiempoRecuperacion || (enfadado && contador >= tiempoRecuperacion/2)) {
                    bool ataca = false;
                    if(ataque1 + ataque2 + ataque3 >= 4) {
                        estado = JefeRanaAbreBoca;
                        ataque1 = 0;
                        ataque2 = 0;
                        ataque3 = 0;
                        ataca   = true;
                    }
                    else {
                        int random = rand() % 3;
                        switch(random) {
                            case 0:
                                if(ataque1 < 2) {
                                    estado = JefeRanaAtaque1;
                                    ataque1++;
                                    ataca = true;
                                }
                                break;
                            case 1:
                                if(ataque2 < 2) {
                                    estado = JefeRanaAtaque2;
                                    ataque2++;
                                    ataca = true;
                                }
                                break;
                            case 2:
                                if(ataque3 < 3) {
                                    estado = JefeRanaAtaque3;
                                    ataque3++;
                                    ataca = true;
                                }
                                break;
                        }
                    }

                    //Reinicia variables si va a atacar
                    if(ataca) {
                        contador = 0;
                        numDisparos = 0;
                    }
                }
                break;
            case JefeRanaAtaque1:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_AGUA, t.milisegundos());

                //Genera proyectiles en una posicion aleatoria
                if(contador >= TIEMPO_ENTRE_ATAQUES || (enfadado && contador >= TIEMPO_ENTRE_ATAQUES/2)) {
                    int random = rand() % 33 + 3;

                    for(int i=-3; i<4; i+=3) {
                        //Genera el proyectil y lo anyade a la sala
                        Sprite* s = new Sprite(texturas[1], 75, 146, 45, 25);
                        s->setOrigin(22, 12);
                        s->setScale(3.0, 3.0);
                        s->setRotation(90);
                        vector<float> pos = {static_cast<float>((random+i)*TAM_TILE), static_cast<float>(1*TAM_TILE)};
                        ProyectilBulletHell* p = new ProyectilBulletHell(DANYO, s, pos, {0, 1}, 420);
                        EstadoPartida::Instancia()->anyadirProyectil(p);
                    }

                    numDisparos++;
                    contador = 0;
                }

                if((!enfadado && numDisparos >= CANT_ATAQUES_TECHO) || numDisparos >= CANT_ATAQUES_TECHO*2) {
                    //Al terminar el ataque vuelve al estado Esperando
                    estado = JefeRanaEsperando;
                }
                break;
            case JefeRanaAtaque2:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_AGUA, t.milisegundos());

                //Comprueba que ha pasado el tiempo entre filas
                if(contador >= TIEMPO_ENTRE_ATAQUES*2.5) {
                    int random = rand() % CANT_DISPAROS_ABANICO;
                    float inicio = 3*TAM_TILE - TAM_TILE/2;

                    //Va generando proyectiles en filas dejando un hueco
                    for(unsigned int i=0; i<=CANT_DISPAROS_ABANICO; i++) {
                        if((int)i != random) {
                            //Genera el proyectil y lo anyade en la sala
                            Sprite* s = new Sprite(texturas[1], 75, 146, 45, 25);
                            s->setOrigin(22, 12);
                            s->setScale(3.0, 3.0);
                            s->setRotation(90);
                            ProyectilBulletHell* p = new ProyectilBulletHell(DANYO, s, {inicio, (float)1*TAM_TILE}, {0, 1}, 340);
                            EstadoPartida::Instancia()->anyadirProyectil(p);
                        }

                        //Incrementa la posicion del siguiente disparo
                        inicio += 30*TAM_TILE/CANT_DISPAROS_ABANICO;
                    }

                    numDisparos++;
                    contador = 0;
                }

                if((!enfadado && numDisparos >= CANT_ATAQUES_ABANICO) || numDisparos >= CANT_ATAQUES_ABANICO*2) {
                    //Al terminar el ataque vuelve al estado Esperando
                    estado = JefeRanaEsperando;
                }
                break;
            case JefeRanaAtaque3:
                //Actualiza la animacion
                animacion->play(ANIM_ATAQUE_AGUA, t.milisegundos());

                //Comprueba que ha pasado el tiempo entre proyectiles
                if(contador >= TIEMPO_ENTRE_ATAQUES*1.35 || (enfadado && contador >= TIEMPO_ENTRE_ATAQUES)) {
                    //Va generando proyectiles horizontales en posiciones aleatorias
                    bool posRandom = rand() % 2;
                    float posY = 0.0;
                    if(posRandom) {
                        posY = (float)24*TAM_TILE;
                    }
                    else {
                        posY = (float)22*TAM_TILE;
                    }

                    //Genera el proyectil y lo anyade en la sala
                    Sprite* s = new Sprite(texturas[1], 75, 146, 45, 25);
                    s->setOrigin(22, 12);
                    s->setScale(3.0, 3.0);
                    ProyectilBulletHell* p = new ProyectilBulletHell(DANYO, s, {(float)1*TAM_TILE, posY}, {1, 0}, 600);
                    EstadoPartida::Instancia()->anyadirProyectil(p);

                    numDisparos++;
                    contador = 0;
                }

                if((!enfadado && numDisparos >= CANT_ATAQUES_TECHO) || numDisparos >= CANT_ATAQUES_TECHO*2) {
                    //Al terminar el ataque vuelve al estado Esperando
                    estado = JefeRanaEsperando;
                }
                break;
            case JefeRanaAbreBoca:
                if(numDisparos <= CANT_DISPAROS_ABANICO-3) {
                    //Actualiza la animacion
                    animacion->play(ANIM_ATAQUE_AGUA, t.milisegundos());
                    float inicio = 3*TAM_TILE - TAM_TILE/2 + 30*numDisparos*TAM_TILE/CANT_DISPAROS_ABANICO;

                    //Genera el proyectil y lo anyade en la sala
                    Sprite* s = new Sprite(texturas[1], 75, 146, 45, 25);
                    s->setOrigin(22, 12);
                    s->setScale(3.0, 3.0);
                    s->setRotation(90);
                    ProyectilBulletHell* p = new ProyectilBulletHell(DANYO, s, {inicio, (float)1*TAM_TILE}, {0, 1}, 430);
                    EstadoPartida::Instancia()->anyadirProyectil(p);

                    numDisparos++;
                }
                else {
                    //Actualiza la animacion
                    animacion->playOnce(ANIM_ABRE_BOCA, t.milisegundos());

                    //Cuando termina la animacion, tira el ataque
                    if(animacion->getFinished()) {
                        animacion->reset();
                        estado          = JefeRanaAtaqueLengua;
                        colisionLengua  = false;
                        contador        = 2000;
                    }
                }
                break;
            case JefeRanaAtaqueLengua:
                if(contador >= tiempoRecuperacion) {
                    //Recupera la posicion del jugador
                    Jugador* jugador            = EstadoPartida::Instancia()->getJugador();
                    vector<float> posObjetivo   = jugador->getPosNueva();
                    vector<float> posLengua     = spritesAtaques[0]->getPosition();

                    //Calcula la direccion de la lengua
                    vector<float> direccion     = {posObjetivo[0]-posLengua[0], posObjetivo[1]-posLengua[1]};
                    float distancia             = sqrt(pow(direccion[0], 2) + pow(direccion[1], 2));
                    direccion[0] /= distancia;
                    direccion[1] /= distancia;

                    //Comprueba que la lengua no va a colisionar con nada en el proximo movimiento
                    comprobarColisionPuntaLengua(direccion, getVelocidad()*t.segundos());

                    //Si la lengua no colisiona con nada avanza
                    if(!colisionLengua) {
                        //Actualiza la punta de la lengua
                        if(spritesAtaques[0]->getColor()[3] == 0) {
                            spritesAtaques[0]->setColor({255, 255, 255, 255});
                        }
                        spritesAtaques[0]->setPosition(posLengua[0]+direccion[0]*getVelocidad()*t.segundos(),
                                                        posLengua[1]+direccion[1]*getVelocidad()*t.segundos());

                        //Calcula la orientacion de los sprites a partir del vector que va desde la boca al objetivo
                        vector<float> dirBoca = {spritesAtaques[0]->getPosition()[0] - posicion[0], 
                                                spritesAtaques[0]->getPosition()[1] - (posicion[1]+DESP_SPRITE_LENGUA)};
                        float angulo = atan2(dirBoca[1], dirBoca[0]);
                        angulo = angulo*180/M_PI - 180;

                        spritesAtaques[0]->setRotation(angulo);

                        //Realiza calculos para colocar los sprites en su posicion
                        distancia   = sqrt(pow(dirBoca[0], 2) + pow(dirBoca[1], 2));
                        dirBoca[0]  /= distancia;
                        dirBoca[1]  /= distancia;

                        //Actualiza el resto de sprites de la lengua
                        int fragActual = 1;
                        while(distancia > 0) {
                            if(spritesAtaques[fragActual]->getColor()[3] == 0) {
                                spritesAtaques[fragActual]->setColor({255, 255, 255, 255});
                            }

                            vector<float> posFragAnterior   = spritesAtaques[fragActual-1]->getPosition();
                            if(fragActual == 1) {
                                spritesAtaques[fragActual]->setPosition(posFragAnterior[0]-dirBoca[0]*27, 
                                                                        posFragAnterior[1]-dirBoca[1]*27);
                            }
                            else {
                                spritesAtaques[fragActual]->setPosition(posFragAnterior[0]-dirBoca[0]*TAM_FRAG_LENGUA, 
                                                                        posFragAnterior[1]-dirBoca[1]*TAM_FRAG_LENGUA);
                            }
                            spritesAtaques[fragActual]->setRotation(angulo);

                            fragActual++;
                            distancia -= TAM_FRAG_LENGUA;
                        }
                    }

                    //Si la lengua ha colisionado, vuelve
                    if(colisionLengua) {
                        estado = JefeRanaCierraBoca;
                    } 
                }
                break;
            case JefeRanaCierraBoca: {
                vector<float> posLengua     = spritesAtaques[0]->getPosition();

                //Calcula la direccion de la lengua
                vector<float> direccion     = {posicion[0]-posLengua[0], (posicion[1]+DESP_SPRITE_LENGUA)-posLengua[1]};
                float distancia             = sqrt(pow(direccion[0], 2) + pow(direccion[1], 2));
                direccion[0] /= distancia;
                direccion[1] /= distancia;

                //Si la lengua no ha llegado al enemigo retrocede
                if(distancia > 50.0) {
                    //Calcula la orientacion de los sprites a partir del vector que va hasta la boca
                    float angulo = atan2(direccion[1], direccion[0]);
                    angulo = angulo*180/M_PI;

                    //Actualiza la punta de la lengua
                    spritesAtaques[0]->setPosition(posLengua[0]+direccion[0]*getVelocidad()/2*t.segundos(),
                                                    posLengua[1]+direccion[1]*getVelocidad()/2*t.segundos());
                    spritesAtaques[0]->setRotation(angulo);

                    //Actualiza el resto de sprites de la lengua
                    int fragActual = 1;
                    while(fragActual < (int)spritesAtaques.size() && spritesAtaques[fragActual]->getColor()[3] == 255) {
                        vector<float> posFragAnterior   = spritesAtaques[fragActual-1]->getPosition();
                        if(fragActual == 1) {
                            spritesAtaques[fragActual]->setPosition(posFragAnterior[0]+direccion[0]*27, 
                                                                    posFragAnterior[1]+direccion[1]*27);
                        }
                        else {
                            spritesAtaques[fragActual]->setPosition(posFragAnterior[0]+direccion[0]*TAM_FRAG_LENGUA, 
                                                                    posFragAnterior[1]+direccion[1]*TAM_FRAG_LENGUA);
                        }
                        spritesAtaques[fragActual]->setRotation(angulo);

                        //Comprueba si ha alcanzado la boca
                        vector<float> posFragActual = spritesAtaques[fragActual]->getPosition();
                        float distFrag = sqrt(pow(posFragActual[0]-posicion[0], 2) + pow(posFragActual[1]-(posicion[1]+DESP_SPRITE_LENGUA), 2));

                        if(distFrag < 50.0) {
                            spritesAtaques[fragActual+1]->setColor({255, 255, 255, 0});
                        }

                        fragActual++;
                        distancia -= TAM_FRAG_LENGUA;
                    }
                }
                else {
                    if(spritesAtaques[0]->getColor()[3] == 255) {
                        //Modifico la opacidad de la punta de la lengua y el sprite conectado por si acaso
                        spritesAtaques[0]->setColor({255, 255, 255, 0});
                        spritesAtaques[1]->setColor({255, 255, 255, 0});
                    }
                    animacion->playOnce(ANIM_CIERRA_BOCA, t.milisegundos());

                    //Al acabar la animacion, vuelve al estado Esperando
                    if(animacion->getFinished()) {
                        animacion->reset();
                        estado      = JefeRanaEsperando;
                        contador    = 0;
                        numDisparos = 0;
                    }
                }
                }
                break;
            case JefeRanaDerrotado:
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
                        Sprite* s       = new Sprite(tex, 0, 0, 30, 13);
                        s->setScale(3.0, 3.0);
                        s->setOrigin(2, 0);
                        ElementoEntorno* lengua = new ElementoEntorno(tex, s, false);
                        lengua->setPosicion(posicion);
                        sala->anyadirElementoEntorno(lengua);
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

    if(estado != JefeRanaDerrotado) {
        //Comprueba que el jugador no ha chocado con el jefe
        Jugador* jugador = EstadoPartida::Instancia()->getJugador();
        if(sprite->intersect(jugador->getSprite())) {
            jugador->danyarVida(danyo);
        }

        if(estado == JefeRanaAtaqueLengua && !colisionLengua) {
            //Si esta atacando con la lengua, comprueba si colisiona con el jugador
            if(spritesAtaques[0]->intersect(jugador->getSprite())) {
                jugador->danyarVida(danyo*2);
                colisionLengua = true;
            }
        }
    }
}

//Sobrecarga del metodo perderVida de Enemgio. Comprueba si ha perdido la mitad de la vida para enfurecerse.
void JefeRana::perderVida(int d) {
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
    else if(estado != JefeRanaDerrotado){
        //Cambia el estado a derrotado
        estado = JefeRanaDerrotado;

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
void JefeRana::setPosicion(vector<float> nuevaPos) {
    Enemigo::setPosicion(nuevaPos);

    for(unsigned int i=0; i<spritesAtaques.size(); i++) {
        spritesAtaques[i]->setPosition(posicion[0], posicion[1] + DESP_SPRITE_LENGUA);
    }
}

//Comprueba si el sprite de la lengua colisiona con el entorno
void JefeRana::comprobarColisionPuntaLengua(vector<float> dir, float distancia) {
    //Comprueba que la variable de colision no indica ya que ha colisionado
    if(!colisionLengua) {
        //Recupera las colisiones de la sala y calcula la posicion a comprobar
        Sala* salaActual                = EstadoPartida::Instancia()->getSalaActual();
        map<vector<int>, int>* mapaCol  = salaActual->getMapaColisiones();
        int idCol                       = salaActual->getIdColision(1);

        vector<float> posInicial = spritesAtaques[0]->getPosition();
        vector<int> posComprobar;
        posComprobar.push_back(posInicial[0]/TAM_TILE);
        posComprobar.push_back(posInicial[1]/TAM_TILE);

        //Comprueba la posicion actual
        auto iterator = mapaCol->find(posComprobar);
        if(iterator != mapaCol->end() && iterator->second == idCol) {
            colisionLengua = true;
        }

        //Comprueba el camino hasta el objetivo
        for(float i=10.0; i<distancia && !colisionLengua; i+=10.0) {
            posComprobar[0] = (posInicial[0] + i*dir[0])/TAM_TILE;
            posComprobar[1] = (posInicial[1] + i*dir[1])/TAM_TILE;
            iterator = mapaCol->find(posComprobar);
            if(iterator != mapaCol->end() && iterator->second == idCol) {
                colisionLengua = true;
            }
        }

        //Comprueba la posicion final
        posComprobar[0] = (posInicial[0] + distancia*dir[0])/TAM_TILE;
        posComprobar[1] = (posInicial[1] + distancia*dir[1])/TAM_TILE;
        iterator = mapaCol->find(posComprobar);
        if(iterator != mapaCol->end() && iterator->second == idCol) {
            colisionLengua = true;
        }
    }
}

//Dibuja el jefe
void JefeRana::dibujar(float percent, int updateMilisegundos) {
    if(estado == JefeRanaAtaqueLengua || estado == JefeRanaCierraBoca) {
        bool continuarDibujando = true;

        //Recorre el vector con los sprites de la lengua
        for(unsigned int i=0; i<spritesAtaques.size() && continuarDibujando; i++) {
            if(spritesAtaques[i]->getColor()[3] == 0) {
                //Si no tiene opacidad significa que ya no es necesario seguir dibujando
                continuarDibujando = false;
            }
            else {
                //Dibuja el trozo de la lengu
                spritesAtaques[i]->draw();
            }
        }
    }

    Enemigo::dibujar(percent, updateMilisegundos);

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