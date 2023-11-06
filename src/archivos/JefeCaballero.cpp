#include "JefeCaballero.h"

const int VIDA              = 16000;
const int DANYO             = 2;
const int VELOCIDAD_PROY    = 300;
const int TIEMPO_REC        = 8000;
const int ESENCIAS          = 8000;
const vector<int> MAT       = {};
const string RUTA_TEX_JEFE  = "resources/sprites/jefes/CaballeroHielo.png";
const string RUTA_TEX_DISP  = "resources/sprites/jefes/Carambano.png";
const string RUTA_FONT      = "resources/sprites/generales/Chonkly.otf";
const string RUTA_HUD       = "resources/sprites/personaje/icons.png";
const int TAM_TILE          = 32;

//Constantes ataques y estado
const int TIEMPO_MAX_TANGIBLE       = 30000;
const int TIEMPO_MAX_TANG_ENFADADO  = 20000;
const int TIEMPO_CREACION_ENEMIGOS  = 2000;
const int TIEMPO_DISPARANDO         = 4000;
const int CANTIDAD_DISPAROS         = 8;
const float VEL_DISPAROS            = 300.0;

vector<int> RANGO_DISP_X            = {4, 50};
vector<int> RANGO_DISP_Y            = {9, 28};
vector<int> ID_ENEMIGOS             = {0, 8, 11, 12};
vector<vector<int>> POS_ENEMIGOS    = {{12, 14}, {4, 25}, {48, 14}, {42, 29}, {23, 26}};
vector<vector<int>> POS_ESPIRITUS   = {{18, 25}, {22, 25}, {26, 25}, {30, 25}, {34, 25}, {48, 24}, 
                                        {41, 28}, {44, 28}, {10, 28}, {13, 28}, {5, 24}, {13, 13}, {41, 13}};

//Constantes para las animaciones
const string ANIM_IDLE          = "JefeCaballeroIdle";
const string ANIM_INTANGIBLE    = "JefeCaballeroIntangible";
const string ANIM_INTANG_IN     = "JefeCaballeroDesaparece";
const string ANIM_INTANG_OUT    = "JefeCaballeroAparece";
const string ANIM_ATAQUE_IN     = "JefeCaballeroComienzaAtaque";
const string ANIM_ATAQUE_OUT    = "JefeCaballeroParaAtaque";
const string ANIM_MUERTE        = "JefeCaballeroMuerte";

//Constantes barra de vida
const float ESCALA_VIDA_Y       = 8.0;
const float ESCALA_VIDA_X       = 20.0;
const float ESCALA_VIDA_EXTREMO = 6.0;
const float DESPLAZAMIENTO_Y    = 90.0;
const int TAM_FRAG_VIDA         = 23;

JefeCaballero::JefeCaballero():Enemigo(VIDA, DANYO, VELOCIDAD_PROY, TIEMPO_REC, ESENCIAS, MAT, nullptr, nullptr) {
    tex     = new Textura(RUTA_TEX_JEFE);
    sprite  = new Sprite(tex, 0, 0, 64, 64);
    sprite->setOrigin(32.0, 16.0);
    sprite->setScale(3.0, 3.0);

    //Inicializa las variables del jefe
    enfadado            = false;
    intangible          = false;
    disparoPreparado    = false;
    espiritusGenerados  = false;
    disparosRestantes   = CANTIDAD_DISPAROS;
    contador            = 5000;
    contadorIntangible  = TIEMPO_MAX_TANGIBLE;
    vidaPerdida         = 0;
    estado              = JefeCaballeroEsperando;

    //Inicializa la barrera invisible que impide salir del combate
    barreraInvisible    = {{25, 5}, {26, 5}, {27, 5}, {28, 5}};
    barreraColocada     = false;

    //Carga la textura de los disparos y de la vida y la fuente del texto
    texDisparos = new Textura(RUTA_TEX_DISP);
    texVida     = new Textura(RUTA_HUD);
    fuente      = new Fuente(RUTA_FONT);
    textoNombre = new Texto("Melog, Guardian de la Sima Escarpada");
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

    //Crea las animaciones del jefe
    animacion = new Animacion(sprite);
    animacion->addAnimation(ANIM_IDLE, 200.0, 0, 0, 5, 0, 64, 64);
    animacion->addAnimation(ANIM_ATAQUE_IN, 120.0, 0, 1, 5, 1, 64, 64);
    animacion->addAnimation(ANIM_ATAQUE_OUT, 120.0, 0, 2, 5, 2, 64, 64);
    animacion->addAnimation(ANIM_INTANG_IN, 150.0, 0, 3, 2, 3, 64, 64);
    animacion->addAnimation(ANIM_INTANG_OUT, 150.0, 3, 3, 5, 3, 64, 64);
    animacion->addAnimation(ANIM_INTANGIBLE, 90.0, 0, 4, 5, 4, 64, 64);
    animacion->addAnimation(ANIM_MUERTE, 200.0, 0, 5, 5, 6, 64, 64);
}

//Destructor
JefeCaballero::~JefeCaballero() {
    delete animacion;
    delete texVida;
    delete fuente;
    delete textoNombre;
    delete texDisparos;

    while(spritesVida.size() > 0) {
        if(spritesVida[spritesVida.size()-1] != nullptr) {
            delete spritesVida[spritesVida.size()-1];
        }
        spritesVida.pop_back();
    }
}

//Actualiza el jefe dependiendo de su estado
void JefeCaballero::update(Tiempo t) {
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
        bool reproducirAnimIntangible = false;
        //Suma el tiempo que ha pasado
        contador += t.milisegundos() + tiempoAntes.milisegundos();

        //Resta el tiempo al contador de intangibilidad
        contadorIntangible -= t.milisegundos() + tiempoAntes.milisegundos();
        contadorIntangible = max(contadorIntangible, 0);

        //Si el enemigo esta intangible, comprueba si quedan enemigos en la sala
        if(intangible) {
            vector<Enemigo*>* enemigos = EstadoPartida::Instancia()->getSalaActual()->getEnemigos();

            //Si tan solo esta el jefe, vuelve a ser tangible
            if(enemigos->size() == 1) {
                contadorIntangible = 0;
            }
        }

        //Si termina el contador cambia el estado de intangibilidad
        if(contadorIntangible == 0 && !intangible) {
            estado = JefeCaballeroInvocando;
            contador = 0;
            contadorIntangible = 100000;
        }
        else if(contadorIntangible == 0 && intangible) {
            activarIntangible(!intangible, t.milisegundos());
        }
        else {
            //Comprueba el estado del enemigo
            switch(estado) {
                case JefeCaballeroEsperando:
                    //Si no es intangible reproduce la animacion correspondiente
                    if(intangible) 
                        reproducirAnimIntangible = true;
                    else 
                        animacion->play(ANIM_IDLE, t.milisegundos());

                    //Al pasar el tiempo de recuperacion, comienza a crear proyectiles
                    if(contador > tiempoRecuperacion) {
                        estado              = JefeCaballeroDisparando;
                        contador            = 0;
                        disparoPreparado    = false;
                        if(enfadado)
                            disparosRestantes   = CANTIDAD_DISPAROS*2;
                        else 
                            disparosRestantes   = CANTIDAD_DISPAROS;
                    }
                    break;
                case JefeCaballeroInvocando:{
                    reproducirAnimIntangible = false;
                    
                    //Genera particulas en las posiciones donde aparecen enemigos
                    vector<float> dirP  = {static_cast<float>(cos(anguloParticulas*M_PI/180)), 
                                        static_cast<float>(sin(anguloParticulas*M_PI/180))};
                    for(unsigned int i=0; i<POS_ENEMIGOS.size(); i++) {
                        vector<float> posActual = {POS_ENEMIGOS[i][0]*(float)TAM_TILE, POS_ENEMIGOS[i][1]*(float)TAM_TILE};
                        new ParticulaDireccional(posActual, {71, 204, 242, 255}, 120.0, 800, dirP, 0, 0);
                    }
                    anguloParticulas = (int)(anguloParticulas + 12) % 360;

                    //Al terminar la creacion de enemigos, vuelve al estado esperando
                    if(contador > TIEMPO_CREACION_ENEMIGOS) {
                        activarIntangible(!intangible, t.milisegundos());
                    }
                    }
                    break;
                case JefeCaballeroDisparando:
                    if(!disparoPreparado && disparosRestantes > 0) {        //No ha comenzado a disparar
                        //Si no es intangible reproduce la animacion correspondiente
                        if(intangible) {
                            reproducirAnimIntangible    = true;
                            disparoPreparado            = true;
                        }
                        else {
                            animacion->playOnce(ANIM_ATAQUE_IN, t.milisegundos());

                            //Al terminarla comienza a disparar
                            if(animacion->getFinished()) {
                                disparoPreparado        = true;
                                contador                = 0;
                                animacion->reset();
                            }
                        }
                    }
                    else if(disparoPreparado && disparosRestantes > 0) {    //Esta disparando
                        if(contador > TIEMPO_DISPARANDO/CANTIDAD_DISPAROS || (enfadado && (contador > TIEMPO_DISPARANDO/(CANTIDAD_DISPAROS*2)))) {
                            //Al pasar el tiempo entre disparos, crea un nuevo disparo
                            contador = 0;
                            generarDisparo(rand()%2, rand()%2);
                            if(enfadado) {
                                //Hace un disparo extra
                                generarDisparo(rand()%2, rand()%2);
                            }
                            disparosRestantes--;
                        }

                        //Si es intangible sigue reproduciendo la animacion
                        if(intangible) {
                            reproducirAnimIntangible    = true;
                        }
                    }
                    else {                                                  //Ha terminado de disparar
                        if(intangible) {
                            reproducirAnimIntangible    = true;
                            disparoPreparado        = false;
                            contador                = 0;
                            estado                  = JefeCaballeroEsperando;
                        }
                        else {
                            animacion->playOnce(ANIM_ATAQUE_OUT, t.milisegundos());

                            //Al terminarla comienza a disparar
                            if(animacion->getFinished()) {
                                disparoPreparado        = false;
                                contador                = 0;
                                estado                  = JefeCaballeroEsperando;
                                animacion->reset();
                            }
                        }
                    }
                    break;
                case JefeCaballeroDerrotado:
                    if(contador >= 2000) {
                        //Reproduce las animacion de muerte tras 2 segundos
                        animacion->playOnce(ANIM_MUERTE, t.milisegundos());

                        //Comprueba cuando termina
                        if(animacion->getFinished()) {
                            muerto = true;
                            //Elimina la barrera que impide evitar al jefe
                            Sala* sala = EstadoPartida::Instancia()->getSalaActual();
                            sala->eliminarColision(&barreraInvisible);
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

            //Reproduce la animacion de intangible si se encuentra en este estado
            if(reproducirAnimIntangible)
                animacion->play(ANIM_INTANGIBLE, t.milisegundos());
        }

        //Reinicia el tiempo antes de detener si es necesario y el reloj
        relojDetenible.restart();
        if(tiempoAntes.milisegundos()>0)
            reiniciarTiempoAntes();
    }

    if(!intangible && estado != JefeCaballeroDerrotado) {
        //Comprueba que el jugador no ha chocado con el jefe
        Jugador* jugador = EstadoPartida::Instancia()->getJugador();
        if(sprite->intersect(jugador->getSprite())) {
            jugador->danyarVida(danyo);
        }
    }
}

//Sobrecarga del metodo perderVida de Enemgio. Comprueba si ha perdido la mitad de la vida para enfurecerse.
void JefeCaballero::perderVida(int d) {
    //Si el jefe esta intangible no recibe danyo
    if(!intangible) {
        //Comprueba si el jefe muere al perder vida
        if(vida - d > 0) {
            Enemigo::perderVida(d);
            if(vida <= 0) {
                //Evita problemas
                muerto = false;
                vida = 1;
            }

            //Si no es intangible suma a la cantidad de vida perdida
            if(!intangible) {
                vidaPerdida += d;

                if(vidaPerdida >= VIDA/4) {
                    //Si ha perdido al menos 1/4 de la vida, se vuelve intangible
                    contadorIntangible = 0;
                    vidaPerdida = 0;
                }
            }

            //Si pierde la mitad de la vida se enfurece
            if(vida < VIDA/2) {
                enfadado = true;
                //Ademas generara espiritus oscuros si aun no lo ha hecho
                if(!espiritusGenerados) {
                    generarEspiritusOscuros();
                    espiritusGenerados = true;
                }
            }
        }
        else if(!intangible && estado != JefeCaballeroDerrotado){
            //Cambia el estado a derrotado
            estado = JefeCaballeroDerrotado;

            //Genera particulas rojas
            vector<int> tam = sprite->getGlobalPixelSize();
            for(int i=0; i<20; i++) {
                int posX = rand() % tam[0] - tam[0]/2 + posicion[0];
                int posY = posicion[1] - rand() % tam[1];
                new ParticulaSimpleCuadrada({(float)posX, (float)posY}, {255, 0, 0, 255}, 30, 5000);
            }
            animacion->reset();
            contador = 0;
        }
    }
}

//Dibuja el jefe
void JefeCaballero::dibujar(float percent, int updateMilisegundos) {
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

//Hace que el jefe se vuelva intangible o que vuelva a su estado original
void JefeCaballero::activarIntangible(bool activar, int milisegundos) {
    //Comprueba que no es ya intangible y quiere serlo
    if(!intangible && activar) {
        //Reproduce la animacion
        animacion->playOnce(ANIM_INTANG_IN, milisegundos);

        if(animacion->getFinished()) {
            //Al terminar la animacion se vuelve intangible y genera los enemigos
            intangible          = true;
            contador            = 0;
            estado              = JefeCaballeroEsperando;
            vidaPerdida         = 0;
            animacion->reset();
            generarEnemigos();
        }
    }
    else if(intangible && !activar) {
        //Ya es intangible y quiere dejar de serlo
        //Reproduce la animacion
        animacion->playOnce(ANIM_INTANG_OUT, milisegundos);

        if(animacion->getFinished()) {
            //Al terminar la animacion se vuelve intangible
            intangible          = false;
            if(enfadado)
                contadorIntangible  = TIEMPO_MAX_TANG_ENFADADO;
            else 
                contadorIntangible  = TIEMPO_MAX_TANGIBLE;
            vidaPerdida          = 0;
            animacion->reset();
        }
    }
}

//Genera enemigos en las posiciones guardadas
void JefeCaballero::generarEnemigos() {
    //Comprueba que al menos puede generar un tipo de enemigo
    if(ID_ENEMIGOS.size() > 0) {
        FabricaEnemigos* fabrica    = FabricaEnemigos::Instancia();
        Sala* sala                  = EstadoPartida::Instancia()->getSalaActual();

        //Recorre el vector de posiciones, generando un enemigo en cada una de ellas
        for(unsigned int i=0; i<POS_ENEMIGOS.size(); i++) {
            //Coge una id aleatoria
            int id = rand() % ID_ENEMIGOS.size();

            //Crea el enemigo, le asigna la posicion y lo anyade a la sala
            Enemigo* e = fabrica->fabricarEnemigo(ID_ENEMIGOS[id]);
            if(e != nullptr) {
                e->setPosicion({POS_ENEMIGOS[i][0]*(float)TAM_TILE, POS_ENEMIGOS[i][1]*(float)TAM_TILE});
                sala->anyadirEnemigo(e);
            }
        }
    }
}

//Genera espiritus oscuros en algunas posiciones de la sala para obstaculizar al jugador
void JefeCaballero::generarEspiritusOscuros() {
    FabricaElementosEntorno* fabrica    = FabricaElementosEntorno::Instancia();
    Sala* sala                          = EstadoPartida::Instancia()->getSalaActual();
    //Recorre el vector con las posiciones donde apareceran
    for(unsigned int i=0; i<POS_ESPIRITUS.size(); i++) {
        //Crea el espiritu y lo anyade a la sala
        ElementoEntorno* elem = fabrica->fabricar(7);
        if(elem != nullptr) {
            elem->setPosicion({POS_ESPIRITUS[i][0]*(float)TAM_TILE, POS_ESPIRITUS[i][1]*(float)TAM_TILE});
            sala->anyadirElementoEntorno(elem);
        }
    }
}

//Genera un disparo que recorre toda la sala, recibe dos parametros que determinan la direccion y el sentido
void JefeCaballero::generarDisparo(bool direccion, bool sentido) {
    //Crea el sprite del disparo
    Sprite* s = new Sprite(texDisparos, 0, 0, 25, 9);
    s->setScale(2.5, 2.5);

    //Calcula la rotacion
    float angulo = 0.0;
    if(direccion)
        angulo += 90.0;
    if(sentido)
        angulo += 180.0;
    s->setRotation(angulo);

    //Calcula una posicion inicial aleatoria y la direccion del disparo
    vector<int> tamSala = EstadoPartida::Instancia()->getSalaActual()->getTamanyoMapa();
    vector<float> dirP = {0, 0};
    vector<float> posI = {0, 0};
    if(direccion) {                 //El disparo viaja verticalmente
        dirP[0] = 0;
        posI[0] = (rand() % (RANGO_DISP_X[1]-RANGO_DISP_X[0]+1) + RANGO_DISP_X[0])*(float)TAM_TILE;

        if(sentido) {
            dirP[1] = -1.0;
            posI[1] = (tamSala[1]-1)*(float)TAM_TILE;
        }
        else {
            dirP[1] = 1.0;
            posI[1] = (float)TAM_TILE;
        }
    }
    else {                          //El disparo viaja horizontalmente
        dirP[1] = 0;
        posI[1] = (rand() % (RANGO_DISP_Y[1]-RANGO_DISP_Y[0]+1) + RANGO_DISP_Y[0])*(float)TAM_TILE;

        if(sentido) {
            dirP[0] = -1.0;
            posI[0] = (tamSala[0]-42)*(float)TAM_TILE;
        }
        else {
            dirP[0] = 1.0;
            posI[0] = (float)TAM_TILE;
        }
    }

    //Crea el proyectil
    Proyectil* p = new ProyectilBulletHell(danyo, s, posI, dirP, VEL_DISPAROS);
    EstadoPartida::Instancia()->anyadirProyectil(p);
}