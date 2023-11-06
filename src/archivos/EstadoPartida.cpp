#include "EstadoPartida.h"
#include "JefeRana.h"

using namespace std;

const float DIST_MENSAJE = 40.0;
const int TAM_INPUT = 19;
const vector<string> ZONA = {"Plantilla_Area1.tmx", "Plantilla_Area2.tmx", "Plantilla_Area3.tmx"};
const vector<string> PUERTA_TAPIADA = {"PuertaTapiada1.png", "PuertaTapiada2.png", "PuertaTapiada3.png"};
const string TILESET = "tiles.png";
const vector<vector<int>> CAM_CENTRADA_JEFE = {{},{832, 1024},{320, 760}};

const string SALA_DESAFIO  = "SalaDesafio_1.tmx";
const vector<int> TAM_TILE = {8, 8}; 

const vector<string> SALAS_TUTORIAL = {"resources/niveles/tutorial/Tutorial_1.tmx", "resources/niveles/tutorial/Tutorial_2.tmx", "resources/niveles/tutorial/Tutorial_3.tmx", "resources/niveles/tutorial/Tutorial_4.tmx", "resources/niveles/tutorial/Tutorial_5.tmx", "resources/niveles/tutorial/Tutorial_6.tmx", "resources/niveles/tutorial/Tutorial_7.tmx"};

//Constantes para sonidos y musica
const vector<string> MUSICA_ZONA = {"resources/sonidos/Area1.ogg", "resources/sonidos/Area2.ogg", "resources/sonidos/Area3.ogg"};
const int VOL_MUSICA = 30;

//Vectores con el efecto de cada mejora del personaje
const vector<int> MEJORA_VIDA = {3, 4, 5, 6, 7, 8};
const vector<int> MEJORA_HMAX = {5500, 6000, 6750, 7500, 8750};
const vector<int> MEJORA_GANC = {32, 64, 96, 128};
const vector<int> MEJORA_HVEL = {115, 130, 150};
//Se inicializa el puntero de la instancia
EstadoPartida *EstadoPartida::pEstadoPartida = 0;

/*Se crea la instancia de EstadoPartida
* 
*/
EstadoPartida* EstadoPartida::Instancia(){
    if(pEstadoPartida == 0){   //Primera llamada
        pEstadoPartida = new EstadoPartida(); //Se crea la instancia de estadoPartida
    }
    return pEstadoPartida;
}

//Constructor de la clase
EstadoPartida::EstadoPartida(){
    VentanaRender* ventana = VentanaRender::Instancia();

    vista = new Vista(0, 0, ventana->getSize()[0], ventana->getSize()[1]);
    cambiandoSala = -1;
    siguienteArea = 1;

    jugador     = nullptr;
    hud         = nullptr;
    mira        = nullptr;
    salaActual  = nullptr;
    salaDesafio = nullptr;
    salaJefe    = nullptr;
    mejoras = {-1, -1, -1, -1};
    sonidos = new ControladorSonido();
}

//Destructor
EstadoPartida::~EstadoPartida() {
 
    //Borra proyectiles
    for(unsigned int i=0; i<almacenProyectiles.size(); i++) {
        if(almacenProyectiles[i] != nullptr) {
            delete almacenProyectiles[i];
        }
    }
    almacenProyectiles.clear();

    //Borra explosiones
    for(unsigned int i=0; i<almacenExplosiones.size(); i++) {
        if(almacenExplosiones[i] != nullptr) {
            delete almacenExplosiones[i];
        }
    }
    almacenExplosiones.clear();

    //Borra nubes de veneno
    for(unsigned int i=0; i<almacenNubeVeneno.size(); i++) {
        if(almacenNubeVeneno[i] != nullptr) {
            delete almacenNubeVeneno[i];
        }
    }
    almacenNubeVeneno.clear();

    while(texs.size() > 0) {
        if(texs[texs.size()-1] != nullptr) {
            delete texs[texs.size()-1];
        }
        texs.pop_back();
    }

    for(unsigned int i=0; i<mazmorra.size(); i++) {
        for(unsigned int j=0; j<mazmorra[i].size(); j++) {
            if(mazmorra[i][j] != nullptr) {
                delete mazmorra[i][j];
            }
        }
        mazmorra[i].clear();
    }
    mazmorra.clear();

    delete vista;
    delete mira;
    delete jugador;
    delete hud;
    delete salaDesafio;
    delete salaJefe;
    delete sonidos;
}

//Renderiza el juego con interpolacion entre el estado actual y el anterior
void EstadoPartida::dibujar(float percentTick, int update_tick_time){
    VentanaRender *ventana = VentanaRender::Instancia();

    if(ventana->getFade()) {
        percentTick = ventana->getTiempoFade()/((float)update_tick_time);
    }

    if(jugador->getMuerto()) {
        //Si el jugador esta muerto, solo se dibuja el
        jugador->dibujar(percentTick);
    }
    else {
        //Se dibuja la sala
        salaActual->dibujar(percentTick, update_tick_time);

        //Se dibuja al personaje
        jugador->dibujar(percentTick);

        //Se dibujan las balas
        for(unsigned int i = 0; i < almacenProyectiles.size(); i++){
            if(almacenProyectiles[i] != nullptr){
                almacenProyectiles[i]->draw(percentTick, update_tick_time);
            }
        }
        for(unsigned int i = 0; i < almacenExplosiones.size(); i++){
            if(almacenExplosiones[i] != nullptr){
                almacenExplosiones[i]->dibujar();
            }
        }
        for(unsigned int i = 0; i < almacenNubeVeneno.size(); i++){
            if(almacenNubeVeneno[i] != nullptr){
                almacenNubeVeneno[i]->dibujar();
            }
        }
        //dibuja gancho
        if(jugador->getDisparo()){
            jugador->getGancho()->draw(percentTick, update_tick_time);
        }
        //Se dibuja el arma
        jugador->getArma()->dibujar(percentTick, update_tick_time);

        //Se dibuja marco
        float distancia = 0;
        ObjetoSala* objetom = salaActual->getObjetoSalaCercano(jugador->getPosNueva(),&distancia);
        if(objetom != nullptr && distancia < DIST_MENSAJE){
            objetom->mostrarInformacion();
        }

        PilaEstados *pilaEstados = PilaEstados::Instancia();
        
        //Coloca la camara teniendo en cuenta si esta en la sala de un jefe
        bool centrarEnJugador = false;
        if(salaActual == salaJefe && siguienteArea-1 >= 0 && siguienteArea-1<(int)CAM_CENTRADA_JEFE.size()) {
            if(!salaActual->getSuperada() && CAM_CENTRADA_JEFE[siguienteArea-1].size() == 2) {
                centroVista = salaActual->centrarCamara(CAM_CENTRADA_JEFE[siguienteArea-1][0], CAM_CENTRADA_JEFE[siguienteArea-1][1]);
            }
            else
                centrarEnJugador    = true;
        }
        else
            centrarEnJugador        = true;
        
        if(centrarEnJugador) {
            //Se coloca la vista centrada en las coordenadas del jugador
            vector<float> coords = jugador->getSprite()->getPosition();
            centroVista = salaActual->centrarCamara(coords[0], coords[1]);
        }
        vista->setCentro(centroVista[0], centroVista[1]);
        

        vector<float> ratonPos = ventana->mapPixelToCoords(ventana->getPosicionRaton());
        mira->setPosition(ratonPos[0], ratonPos[1]);

        ventana->setVista(vista);

        //Se dibuja el hud(vida, minimapa)
        hud->dibujar();
    }
    
    ventana->dibujar(mira);
    
}

//Actualiza el juego
void EstadoPartida::update(Tiempo timeElapsed, bool input[TAM_INPUT]){
    VentanaRender *ventana = VentanaRender::Instancia();
    if(!ventana->getFade()){
        if(ventana->getPantallaNegro()){   //Cambio de sala
            if(cambiandoSala!=-1){
                //Borra proyectiles
                for(unsigned int i=0; i<almacenProyectiles.size(); i++) {
                    if(almacenProyectiles[i] != nullptr) {
                        delete almacenProyectiles[i];
                        almacenProyectiles[i] = nullptr;
                    }
                }
                for(unsigned int i=0; i<almacenExplosiones.size(); i++) {
                    if(almacenExplosiones[i] != nullptr) {
                        delete almacenExplosiones[i];
                        almacenExplosiones[i] = nullptr;
                    }
                }
                for(unsigned int i=0; i<almacenNubeVeneno.size(); i++) {
                    if(almacenNubeVeneno[i] != nullptr) {
                        delete almacenNubeVeneno[i];
                        almacenNubeVeneno[i] = nullptr;
                    }
                }
                
                vector<int> spawn  = cambiarSala(cambiandoSala);
                if(spawn[0] != -1 && spawn[1] != -1){
                    jugador->setPosicion({(float)spawn[0], (float)spawn[1]}, true);
                    jugador->setSpawn({(float)spawn[0], (float)spawn[1]});
                    cambiandoSala = -1;
                }
            }
            else if(jugador->getMuerto()) {
                //Si el jugador ha muerto vuelve al pueblo
                cambiarEstado(2);
            }
            ventana->fadeOut(25);
        }else{  //Ciclo normal
            if(jugador->getMuerto()) {
                //Si el jugador esta muerto, solo se actualiza el
                jugador->update(timeElapsed.milisegundos(), input);
            }
            else {
                //Si el jugador no esta muerto, se actualiza todo con normalidad
                jugador->update(timeElapsed.milisegundos(), input);
                jugador->getArma()->update(jugador->getPosNueva(), timeElapsed.milisegundos());

                salaActual->update(timeElapsed);
                if(jugador->getDisparo()){
                    jugador->getGancho()->update();
                    if(jugador->getGancho()->getColision()){
                        jugador->moverGancho();
                
                        }
                    if(jugador->getGancho()->getVuelta()){
                        jugador->retroceso();
                        }
                    }
                for(unsigned int i = 0; i < almacenProyectiles.size(); i++){
                    if(almacenProyectiles[i] != nullptr){
                        if(almacenProyectiles[i]->getDestruir()){
                            eliminarProyectil(almacenProyectiles[i]);
                        }else{
                            almacenProyectiles[i]->update(timeElapsed);
                        }
                        
                    }
                }

                for(unsigned int i = 0; i < almacenExplosiones.size(); i++){
                    if(almacenExplosiones[i] != nullptr){
                        if(almacenExplosiones[i]->getDestruir()){
                            eliminarExplosion(almacenExplosiones[i]);
                        }else{
                            almacenExplosiones[i]->update(timeElapsed);
                        }
                    }
                }

                for(unsigned int i = 0; i < almacenNubeVeneno.size(); i++){
                    if(almacenNubeVeneno[i] != nullptr){
                        if(almacenNubeVeneno[i]->getDestruir()){
                            eliminarNubeVeneno(almacenNubeVeneno[i]);
                        }else{
                            almacenNubeVeneno[i]->update(timeElapsed);
                        }
                    }
                }
            }
            
        } 
        
    }
}


//Llama al metodo detenible de todos los objetos que se puedan detener
void EstadoPartida::detenerTiempo(){
    //Detiene los elementos de la sala
    salaActual->detenerSala();

    //Recorre los proyectiles de la sala y los detiene
    for(unsigned int i=0; i<almacenProyectiles.size(); i++) {
        if(almacenProyectiles[i] != nullptr) {
            almacenProyectiles[i]->detener();
        }
    }
    for(unsigned int i=0; i<almacenExplosiones.size(); i++) {
        if(almacenExplosiones[i] != nullptr) {
            almacenExplosiones[i]->detener();
        }
    }
    for(unsigned int i=0; i<almacenNubeVeneno.size(); i++) {
        if(almacenNubeVeneno[i] != nullptr) {
            almacenNubeVeneno[i]->detener();
        }
    }
}

//Llama al metodo reanudar de todos los objetos que se puedan detener
void EstadoPartida::reanudarTiempo(){
    //Reanuda los elementos de la sala
    salaActual->reanudarSala();

    //Recorre los proyectiles de la sala y los detiene
    for(unsigned int i=0; i<almacenProyectiles.size(); i++) {
        if(almacenProyectiles[i] != nullptr) {
            almacenProyectiles[i]->reanudar();
        }
    }
    for(unsigned int i=0; i<almacenExplosiones.size(); i++) {
        if(almacenExplosiones[i] != nullptr) {
            almacenExplosiones[i]->reanudar();
        }
    }
    for(unsigned int i=0; i<almacenNubeVeneno.size(); i++) {
        if(almacenNubeVeneno[i] != nullptr) {
            almacenNubeVeneno[i]->reanudar();
        }
    }
}

void EstadoPartida::inicializar(Jugador* j, int area){
    //Reinicia el estado por si fuera necesario
    reiniciarEstadoPartida();

    //Recupera la partida guardada
    Guardado *guar = Guardado::Instancia();

    mejoras[0] = guar->getMejoraSalud();
    mejoras[1] = guar->getMejoraPoderMax();
    mejoras[2] = guar->getMejoraDistGancho();
    mejoras[3] = guar->getMejoraPoderVel();
    esencias   = guar->getEsencias();
    bool linterna   = guar->getLinterna();
    bool maza       = guar->getMaza();
    tutorialHecho   = guar->getTutorial();

    //Se asigna el area en la que se encuentra el jugador de la mazmorra
    area = max(min(area, (int)ZONA.size()-1), 0);
    sonidos->pararTodaMusica();
    //Se anyade la musica correspondiente a cada area
    if(!sonidos->existeMusica("musicaArea")){
        sonidos->addMusica("musicaArea", MUSICA_ZONA[area], VOL_MUSICA);
    }else{
        sonidos->removeMusica("musicaArea");
        sonidos->addMusica("musicaArea", MUSICA_ZONA[area], VOL_MUSICA);
    }
    sonidos->playMusica("musicaArea");

    //Crea las texturas globales
    texs.push_back(new Textura("resources/niveles/"+TILESET));
    texs.push_back(new Textura("resources/niveles/"+PUERTA_TAPIADA[area]));
    texs.push_back(new Textura("resources/sprites/armas/balas.png"));
    texs.push_back(new Textura("resources/sprites/armas/puntero.png"));
    texs.push_back(new Textura("resources/sprites/armas/explosion.png"));
    texs.push_back(new Textura("resources/sprites/armas/veneno.png"));
    mira = new Sprite(texs[3], 0, 0, 64, 64);
    mira->setOrigin(64/2, 64/2);
    VentanaRender::Instancia()->setMira(false);
    
    //Crea el jugador y el hud
    if(j != nullptr){
        if(jugador != nullptr && jugador != j) {
            delete jugador;
        }
        jugador = j;
    }else{
        if(jugador != nullptr) {
            delete jugador;
        }
        jugador = new Jugador();
    }
    hud = new Hud(); 
    jugador->recuperarHud();
    EstadoInventario* estadoInventario = EstadoInventario::Instancia();
    estadoInventario->inicializar();

    
    if(mejoras[0] != 0){
        jugador->setVidaMax(MEJORA_VIDA[mejoras[0]-1]);
    }
    if(mejoras[1] != 0){
        jugador->setHabilidadMax(MEJORA_HMAX[mejoras[1]-1]);
    }
    if(mejoras[3] != 0){
        jugador->setHabilidadVel(MEJORA_HVEL[mejoras[3]-1]);
    }
    if(mejoras[2] != 0){
        jugador->getGancho()->setDistancia(MEJORA_GANC[mejoras[2]-1]);
    }
    if(linterna) {
        jugador->setLinterna(true);
    }
    if(maza) {
        jugador->setMaza(true);
    }
    
    //Si no se ha completado el tutorial se genera la mazmorra del tutorial
    if(!tutorialHecho){
        cargarTutorial();
    }else{
        //Genera la mazmorra
        
        GeneradorMazmorras gMazmorras("resources/niveles/"+ZONA[area]);
        bool repetir = false;
        do {
            //Genera la mazmorra hasta que sale correctamente
            if(gMazmorras.generarMazmorra()) {
                repetir = false;
            }
            else {
                //La mazmorra no cumple con los requisitos, por lo que se repite el proceso
                repetir = true;
                gMazmorras.resetearMazmorra();
            }
        }while(repetir);

        //Se recupera si se ha seleccionado un jefe en los trucos
        EstadoTrucos *eTrucos = EstadoTrucos::Instancia();
        int jefeSeleccionado = eTrucos->getJefeSeleccionado();
        bool vidaInf = eTrucos->getVidaInf();
        if(vidaInf){
            jugador->setInmuneGolpes(INT_MAX);
        }

        gMazmorras.setTexturas(texs[0], texs[1]);
        mazmorra = gMazmorras.getMazmorra();

        //Si no se selecciona ningun jefe en los trucos se recoge la sala de inicio de la mazmorra
        //Flujo normal sin trucos
        if(jefeSeleccionado == -1){
            salaInicio = gMazmorras.getSalaInicial();
            posSalaActual = salaInicio;
            salaActual = mazmorra[posSalaActual[0]][posSalaActual[1]];

            //Coloca al jugador en el punto de aparicion
            vector<int> coords = salaActual->getCoordsRecompensa();
            jugador->setPosicion({(float)coords[0]*32, (float)coords[1]*32}, true);
            jugador->setSpawn({(float)coords[0]*32, (float)coords[1]*32});
        }

        jugador->getInventario()->setNivelMateriales(gMazmorras.getNivelMateriales());
        siguienteArea = gMazmorras.getSigArea();

        //Crea la sala del jefe
        salaJefe    = Sala::crearSalaJefe(siguienteArea);

        //Si el jugador ha seleccionado un truco para ir a la sala del jefe directamente se asigna la sala actual (la de aparicion), a la del jefe para que aparezca en esta
        if(jefeSeleccionado > -1){
            salaActual = salaJefe;
            salaInicio = gMazmorras.getSalaInicial();
            posSalaActual = salaInicio;
            //Coloca al jugador en el punto de aparicion
            vector<int> coords = salaActual->getCoordsAparicion(2);
            jugador->setPosicion({(float)coords[0], (float)coords[1]}, true);
            jugador->setSpawn({(float)coords[0], (float)coords[1]});
            EstadoTrucos::Instancia()->resetJefe();
        }
    }
    salaActual->comprobarEstadoSala();

    //Crea la sala de desafios
    salaDesafio = new Sala("resources/niveles/" + SALA_DESAFIO, texs[0], TAM_TILE, texs[1]);

    //Portal dentro de la sala de desafio
    Textura* tP1 = new Textura("resources/sprites/generales/atemporal.png");
    Sprite* sP1 = new Sprite(tP1, 0, 0, 32, 32);
    portalDesafio = new PortalDesafio(tP1, sP1);
    
    vector<float> coordsAux = {(float)salaDesafio->getCoordsRecompensa()[0]*32+16, (float)salaDesafio->getCoordsRecompensa()[1]*32+16};
    portalDesafio->setPosicion(coordsAux);

    //Recoge la seleccion del jugador de los trucos, si ha pulsado sala desafio se asigna a la sala actual(la de aparicion), la sala de desafio para que aparezca en ella
    int salaDesafioSelecc = EstadoTrucos::Instancia()->getExtraSeleccionado();
    if(salaDesafioSelecc != -1){    //Ha seleccionado la opcion de ir a la sala de desafio
        Textura* tP = new Textura("resources/sprites/generales/atemporal.png");
        Sprite* sP = new Sprite(tP, 0, 0, 32, 32);
        ElementoEntorno* portalADesafio = new PortalSalaADesafio(tP, sP);
        portalADesafio->setPosicion({11*32, 17*32});
        salaActual->anyadirElementoEntorno(portalADesafio);
    }
}

//Vuelve a reiniciar las variables creadas al inicializar la mazmorra
void EstadoPartida::reiniciarEstadoPartida() {
    //Borra proyectiles
    for(unsigned int i=0; i<almacenProyectiles.size(); i++) {
        if(almacenProyectiles[i] != nullptr) {
            delete almacenProyectiles[i];
            almacenProyectiles[i] = nullptr;
        }
    }
    for(unsigned int i=0; i<almacenExplosiones.size(); i++) {
        if(almacenExplosiones[i] != nullptr) {
            delete almacenExplosiones[i];
            almacenExplosiones[i] = nullptr;
        }
    }
    for(unsigned int i=0; i<almacenNubeVeneno.size(); i++) {
        if(almacenNubeVeneno[i] != nullptr) {
            delete almacenNubeVeneno[i];
            almacenNubeVeneno[i] = nullptr;
        }
    }

    while(texs.size() > 0) {
        if(texs[texs.size()-1] != nullptr) {
            delete texs[texs.size()-1];
        }
        texs.pop_back();
    }

    for(unsigned int i=0; i<mazmorra.size(); i++) {
        for(unsigned int j=0; j<mazmorra[i].size(); j++) {
            if(mazmorra[i][j] != nullptr) {
                delete mazmorra[i][j];
            }
        }
        mazmorra[i].clear();
    }
    mazmorra.clear();

    if(salaDesafio != nullptr){
        delete salaDesafio;
        salaDesafio = nullptr;
    }

    if(salaJefe != nullptr){
        delete salaJefe;
        salaJefe = nullptr;
    }
    
    if(mira != nullptr) {
        delete mira;
        mira = nullptr;
    }

    if(hud != nullptr) {
        delete hud;
        hud = nullptr;
    }

    salaActual = nullptr;
    cambiandoSala = -1;
}

//Anyade la referencia a una bala en el array de balas
void EstadoPartida::anyadirProyectil(Proyectil* p){
    bool hueco = false;
    //Busca si hay algun nullptr en el array de balas
    for(unsigned int i=0; i<almacenProyectiles.size() && !hueco; i++) {
        //Si lo hay mete ahi la referencia a la nueva bala
        if(almacenProyectiles[i] == nullptr) {
            hueco = true;
            almacenProyectiles[i] = p;
        }
    }

    //Si el array esta completo anyade la referencia al final
    if(!hueco) {
        almacenProyectiles.push_back(p);
    }
}

//Borra la referencia a la bala pasada por parametro del array de balas
void EstadoPartida::eliminarProyectil(Proyectil* p){
    bool encontrada = false;

    //Recorre el array de balas buscando la bala pasada por parametro
    for(unsigned int i=0; i<almacenProyectiles.size() && !encontrada; i++) {
        //Si la encuentra hace que apunte a nullptr
        if(p == almacenProyectiles[i]) {
            encontrada = true;
            delete almacenProyectiles[i];
            almacenProyectiles[i] = nullptr;
        }
    }
}

//Devuelve el tamanyo del array de balas
int EstadoPartida::getAlmacenProyectilesSize(){
    return almacenProyectiles.size();
}


//Anyade la referencia de una explosion al array de explosiones
void EstadoPartida::anyadirExplosion(Explosion* e){
    bool hueco = false;
    //Busca si hay algun nullptr en el array de balas
    for(unsigned int i=0; i<almacenExplosiones.size() && !hueco; i++) {
        //Si lo hay mete ahi la referencia a la nueva explosion
        if(almacenExplosiones[i] == nullptr) {
            hueco = true;
            almacenExplosiones[i] = e;
        }
    }

    //Si el array esta completo anyade la referencia al final
    if(!hueco) {
        almacenExplosiones.push_back(e);
    }
}

//Borra la referencia a la explosion pasada por parametro del array de explosiones
void EstadoPartida::eliminarExplosion(Explosion* e){
    bool encontrada = false;

    //Recorre el array de balas buscando la bala pasada por parametro
    for(unsigned int i=0; i<almacenExplosiones.size() && !encontrada; i++) {
        //Si la encuentra hace que apunte a nullptr
        if(e == almacenExplosiones[i]) {
            encontrada = true;
            delete almacenExplosiones[i];
            almacenExplosiones[i] = nullptr;
        }
    }
}

//Devuelve el tamanyo del array de explosiones
int EstadoPartida::getAlmacenExplosionesSize(){
    return almacenExplosiones.size();
}

//Anyade la referencia de una explosion al array de nubes
void EstadoPartida::anyadirNubeVeneno(NubeVeneno* v){
    bool hueco = false;
    //Busca si hay algun nullptr en el array de balas
    for(unsigned int i=0; i<almacenNubeVeneno.size() && !hueco; i++) {
        //Si lo hay mete ahi la referencia a la nueva nube
        if(almacenNubeVeneno[i] == nullptr) {
            hueco = true;
            almacenNubeVeneno[i] = v;
        }
    }

    //Si el array esta completo anyade la referencia al final
    if(!hueco) {
        almacenNubeVeneno.push_back(v);
    }
}

//Borra la referencia a la explosion pasada por parametro del array de explosiones
void EstadoPartida::eliminarNubeVeneno(NubeVeneno* v){
    bool encontrada = false;

    //Recorre el array de nubes buscando la bala pasada por parametro
    for(unsigned int i=0; i<almacenNubeVeneno.size() && !encontrada; i++) {
        //Si la encuentra hace que apunte a nullptr
        if(v == almacenNubeVeneno[i]) {
            encontrada = true;
            delete almacenNubeVeneno[i];
            almacenNubeVeneno[i] = nullptr;
        }
    }
}

//Devuelve el tamanyo del array de nubes
int EstadoPartida::getAlmacenNubeVenenoSize(){
    return almacenNubeVeneno.size();
}

//Devuelve la referencia del jugador
Jugador* EstadoPartida::getJugador(){
    return jugador;
}

//Devuelve la referencia de la sala actual
Sala* EstadoPartida::getSalaActual(){
    return salaActual;
}
vector<vector<Sala*>>* EstadoPartida::getSalas(){
    return &mazmorra;
}

Hud* EstadoPartida::getHud(){
    return hud;
}

void EstadoPartida::cambiarEstado(int valor){
    PilaEstados* pilaEstados = PilaEstados::Instancia();
    if(valor == 1) {
        EstadoInventario* eInventario = EstadoInventario::Instancia();
        
        
        //Cambia el estado actual al estado Inventario
        pilaEstados->anyadirEstado(eInventario);
        eInventario->resetTiempoEspera();

        VentanaRender::Instancia()->setMira(true);
    }
    if(valor == 2){
        //Borra las particulas
        Particula::eliminarParticulas();
        sonidos->pararTodaMusica();
        //Cambia al estado pueblo
        EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
        pilaEstados->modificarUltimoEstado(ePueblo);
        ePueblo->inicializar();

        //En el tutorial, como la sala del jefe forma parte de la mazmorra es necesario actualizar el puntero
        bool encontrada = false;
        for(unsigned int i=0; i<mazmorra.size() && !encontrada; i++) {
            for(unsigned int j=0; j<mazmorra[i].size() && !encontrada; j++) {
                if(mazmorra[i][j] == salaActual) {
                    if(salaActual == salaJefe) {
                        salaJefe = nullptr;
                    }
                    
                    delete salaActual;
                    salaActual = nullptr;
                    mazmorra[i][j] = nullptr;
                    encontrada = true;                    
                }
            }
        }
    }
    if(valor == 3) {
        EstadoMenuJuego* eMenu = EstadoMenuJuego::Instancia();
        
        //Cambia el estado actual al estado Menu
        pilaEstados->anyadirEstado(eMenu);
        eMenu->inicializar();

        VentanaRender::Instancia()->setMira(true);
    }
}

//Cambia la sala en la que se encuentra el jugador segun la puerta que atraviese
//Le pasa al jugador la posicion de spawn de la sala
//Si se pasa un 4 el jugador viaja a la sala de desafio
//Si se pasa un 5 en la direccion el jugador vuelve a las coordenasdas de la mazmorra antes de una sala de desafio
vector<int> EstadoPartida::cambiarSala(int direccion){
    int salaX = posSalaActual[0];
    int salaY = posSalaActual[1];
    vector<int> coords = {-1, -1};

    if(salaJefe == salaActual){
        if(direccion == 1){
            //Otorga al jugador la recompensa por derrotar al jefe
            Guardado* guardado = Guardado::Instancia();

            //Vuelve al pueblo
            VentanaRender* ventana = VentanaRender::Instancia();
            ventana->fadeIn(20);
            cambiarEstado(2);
            switch(siguienteArea) {
                case 1: 
                    if(!guardado->getGancho()){
                        guardado->setGancho(true);
                        EstadoTexto *eTexto = EstadoTexto::Instancia();
                        PilaEstados::Instancia()->anyadirEstado(eTexto);
                        eTexto->inicializar(2);
                    }
                        
                    break;
                case 2: 
                    if(!guardado->getMaza()){
                        guardado->setMaza(true);
                        EstadoTexto *eTexto = EstadoTexto::Instancia();
                        PilaEstados::Instancia()->anyadirEstado(eTexto);
                        eTexto->inicializar(3);
                    }       
                    break;
                case 3: 
                    if(!guardado->getLinterna()){
                        guardado->setLinterna(true);
                        EstadoTexto *eTexto = EstadoTexto::Instancia();
                        PilaEstados::Instancia()->anyadirEstado(eTexto);
                        eTexto->inicializar(4);
                    } 
                    break;
            }
            guardado->guardarPartida();

        }
        else if(direccion == 0){
            //Inicia una nueva mazmorra en la siguiente area
            VentanaRender* ventana = VentanaRender::Instancia();
            ventana->fadeIn(20);
            //Si es la ultima zona lleva al personaje al pueblo y pone historia
            if(siguienteArea >= (int)ZONA.size()){
                VentanaRender* ventana = VentanaRender::Instancia();
                ventana->fadeIn(20);
                cambiarEstado(2);

                EstadoTexto *eTexto = EstadoTexto::Instancia();
                PilaEstados::Instancia()->anyadirEstado(eTexto);
                eTexto->inicializar(5);
            }
            inicializar(jugador, siguienteArea);
        }
    }else{
        if(salaActual->existePuerta(direccion)){
            //Comprueba si el tiempo esta detenido
            if(jugador->getTiempoDetenido()) {
                jugador->reanudarTiempo();
            }

            //Actualiza la sala a la que va el jugador segun la direccion
            switch (direccion)
            {
            case 0: 
                //Puerta de arriba
                salaY++;
                break;
            case 1: 
                //Puerta de la derecha
                salaX++;
                break;
            case 2: 
                //Puerta de abajo
                salaY--;
                break;
            case 3: 
                //Puerta de la izquierda
                salaX--;
                break;
            }
            salaActual = mazmorra[salaX][salaY];
            salaActual->comprobarEstadoSala();
            posSalaActual[0] = salaX;
            posSalaActual[1] = salaY;

            //Resetea los enemigos de la sala para que no te ataquen al entrar
            resetEnemigos();

            //Borra las particulas
            Particula::eliminarParticulas();

            coords = salaActual->getCoordsAparicion((direccion+2)%4);
        }else{
            //Cambia al jugador a la sala de desafio
            if(direccion == 4){
                salaActual = salaDesafio;
                salaActual->anyadirElementoEntorno(portalDesafio);

                coords = salaActual->getCoordsAparicion(2);
            }
            //Cambia al jugador a la sala en la que se encontraba antes del desafio
            if(direccion == 5){
                salaActual = mazmorra[salaX][salaY];
                salaActual->comprobarEstadoSala();

                //Resetea los enemigos de la sala para que no te ataquen al entrar
                resetEnemigos();

                coords = jugador->getCoordsPortal();
            }

            //Cambia al jugador a la sala del jefe
            if(direccion == 6){
                salaActual = salaJefe;

                coords = salaActual->getCoordsAparicion(2);
            }

            //Borra las particulas
            Particula::eliminarParticulas();
        }
    }

    return coords;
}

Vista* EstadoPartida::getVista(){
    return vista;
}

//Devuelve la posion del array de texturas correspondiente a la opcion pasada
//0->tileset del mapa
//1->textura puerta tapiada
//2->textura de las balas
//3->textura del puntero
//4->textura de la explosion
//5->textura del veneno
Textura* EstadoPartida::getTexturas(int opc){
    return texs[opc];
}

void EstadoPartida::setCambiandoSala(int direc){
    cambiandoSala = direc;
}

//Reinicia el estado de los enemigos al atacar y moverse
void EstadoPartida::resetEnemigos() {
    vector<Enemigo*>* enemigos = salaActual->getEnemigos();
    for(unsigned int i=0; i<enemigos->size(); i++) {
        if((*enemigos)[i] != nullptr) {
            (*enemigos)[i]->reset();
        }
    }
}

Sala* EstadoPartida::getSalaDesafio(){
    return salaDesafio;
}

void EstadoPartida::setSalaActual(Sala* s){
    salaActual = s;
}

ElementoEntorno* EstadoPartida::getPortalDesafio(){
    return portalDesafio;
}

//Metodo que genera la mazmorra del tutorial
void EstadoPartida::cargarTutorial(){
    //Se crean las salas de la mazmorra del tutorial y se añaden a la matriz de la mazmorra
    Sala* tut1 = new Sala(SALAS_TUTORIAL[0], texs[0], TAM_TILE, texs[1]);
    Sala* tut2 = new Sala(SALAS_TUTORIAL[1], texs[0], TAM_TILE, texs[1]);
    Sala* tut3 = new Sala(SALAS_TUTORIAL[2], texs[0], TAM_TILE, texs[1]);
    Sala* tut4 = new Sala(SALAS_TUTORIAL[3], texs[0], TAM_TILE, texs[1]);
    Sala* tut5 = new Sala(SALAS_TUTORIAL[4], texs[0], TAM_TILE, texs[1]);
    Sala* tut6 = new Sala(SALAS_TUTORIAL[5], texs[0], TAM_TILE, texs[1]);
    Sala* tut7 = new Sala(SALAS_TUTORIAL[6], texs[0], TAM_TILE, texs[1]);
    mazmorra = {{nullptr, tut1, nullptr, nullptr, nullptr, nullptr},
                {nullptr, tut2, nullptr, nullptr, nullptr, nullptr},
                {tut4, tut3, nullptr, nullptr, nullptr, nullptr},
                {tut5, nullptr, nullptr, nullptr, nullptr, nullptr},
                {tut7, nullptr, nullptr, nullptr, nullptr, nullptr},
                {tut6, nullptr, nullptr, nullptr, nullptr, nullptr}};
    posSalaActual = {0, 1};
    salaActual = tut1;
    salaJefe = tut6;
    jugador->getInventario()->setNivelMateriales(1);
    //Coloca al jugador en el punto de aparicion
    vector<int> coords = salaActual->getCoordsAparicion(3);
    int tamTile = salaActual->getConstTamTiles();
    jugador->setPosicion({(float)coords[0], (float)coords[1]}, true);
    jugador->setSpawn({(float)coords[0], (float)coords[1]});
    siguienteArea = 0;

    //Se genera el enemigo de pruebas para el tutorial 4
    Textura* tM = new Textura("resources/sprites/pnjs/maniqui.png");
    Sprite* sM = new Sprite(tM, 0, 0, 14, 18);
    sM->setOrigin(14/2, 18/2 - 2);
    sM->setScale(-3, 3);
    Enemigo* maniqui = new Maniqui(500, 1, 0, 0, 0, {}, tM, sM);
    maniqui->setPosicion({18.0f*tamTile, 18.0f*tamTile});
    tut4->anyadirEnemigo(maniqui);

    //Crea el cofre para el tutorial 5
    FabricaElementosEntorno* fEntorno = FabricaElementosEntorno::Instancia();
    ElementoEntorno* cofre = fEntorno->fabricar(4);
    cofre->setPosicion({18.0f*tamTile, 18.0f*tamTile});
    tut5->anyadirElementoEntorno(cofre);

    //Se crean elementos con sprites de las teclas y textos
    Textura *tt1 = new Textura("resources/sprites/generales/controles/mov.png");
    Sprite *st1 = new Sprite(tt1, 0, 0, 70, 10);
    st1->setScale(3, 3);
    ElementoEntorno *et1 = new ElementoEntorno(tt1, st1, false);
    et1->setPosicion({9.f*tamTile, 9.f*tamTile});
    tut1->anyadirElementoEntorno(et1);

    Textura *tt2 = new Textura("resources/sprites/generales/controles/salto.png");
    Sprite *st2 = new Sprite(tt2, 0, 0, 93, 10);
    st2->setScale(3, 3);
    ElementoEntorno *et2 = new ElementoEntorno(tt2, st2, false);
    et2->setPosicion({9.f*tamTile, 12.f*tamTile});
    tut2->anyadirElementoEntorno(et2);

    Textura *tt3 = new Textura("resources/sprites/generales/controles/bajar.png");
    Sprite *st3 = new Sprite(tt3, 0, 0, 40, 10);
    st3->setScale(3, 3);
    ElementoEntorno *et3 = new ElementoEntorno(tt3, st3, false);
    et3->setPosicion({18.f*tamTile, 11.f*tamTile});
    tut3->anyadirElementoEntorno(et3);

    Textura *tt4 = new Textura("resources/sprites/generales/controles/disparar.png");
    Sprite *st4 = new Sprite(tt4, 0, 0, 140, 60);
    st4->setScale(2, 2);
    ElementoEntorno *et4 = new ElementoEntorno(tt4, st4, false);
    et4->setPosicion({14.f*tamTile, 11.f*tamTile});
    tut4->anyadirElementoEntorno(et4);

    Textura *tt5 = new Textura("resources/sprites/generales/controles/cofre.png");
    Sprite *st5 = new Sprite(tt5, 0, 0, 200, 70);
    st5->setScale(3, 3);
    ElementoEntorno *et5 = new ElementoEntorno(tt5, st5, false);
    et5->setPosicion({0.f*tamTile, 0.f*tamTile});
    tut5->anyadirElementoEntorno(et5);

    Textura *tt6 = new Textura("resources/sprites/generales/controles/pararTiempo.png");
    Sprite *st6 = new Sprite(tt6, 0, 0, 99, 10);
    st6->setScale(3, 3);
    ElementoEntorno *et6 = new ElementoEntorno(tt6, st6, false);
    et6->setPosicion({13.f*tamTile, 9.f*tamTile});
    tut7->anyadirElementoEntorno(et6);
    ElementoEntorno *recup = new RecuperadorVida(nullptr, nullptr);
    tut7->anyadirElementoEntorno(recup);

    Textura *tt7 = new Textura("resources/sprites/generales/controles/objetosTemporales.png");
    Sprite *st7 = new Sprite(tt7, 0, 0, 200, 40);
    st7->setScale(3, 3);
    ElementoEntorno *et7 = new ElementoEntorno(tt7, st7, false);
    et7->setPosicion({3.f*tamTile, 7.f*tamTile});
    tut6->anyadirElementoEntorno(et7);

}