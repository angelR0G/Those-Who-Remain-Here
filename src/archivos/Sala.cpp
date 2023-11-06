#include "Sala.h"

const int tamTiles              = 32;       //Tamanyo en pixeles que va a tener cada tile en pantalla
const int tamPatron             = 504;      //Cantidad de tiles del tilesheet. Necesario para calcular el id real de las colisiones.

const int idPuertaArriba        = 135;
const int idPuertaDerecha       = 137;
const int idPuertaAbajo         = 159;
const int idPuertaIzquierda     = 161;

const int idSpawnArriba         = 140;
const int idSpawnDerecha        = 142;
const int idSpawnAbajo          = 164;
const int idSpawnIzquierda      = 166;

const int idCoordsRecompensa    = 69;

const vector<int> TAM_TILE      = {8, 8}; 

//Constantes de efectos de objetos
const int DANYO_GAS_ENEMIGO     = 12;
const int PROB_EXPLOTAR_ENEMIGO = 5;

using namespace std;

Sala::Sala(string rutaSala, Textura* tilesheet, vector<int> tamTile, Textura* pTapiadas):Escenario(rutaSala, tilesheet, tamTile) {
    //Valores constantes
    map<int, int> idEnemigos      = {{49, 0}, {73, 1}, {97, 2}, {121, 3}, {145, 4}, {169, 5},
                                    {50, 6}, {74, 7}, {98, 8}, {122, 9}, {146, 10}, {170, 11},
                                    {51, 12}, {75, 13}};
    map<int, int> idObjAtemporal  = {{56, 0}, {80, 1}, {104, 2}, {128, 3}, {152, 4}, {176, 5},
                                    {57, 6}, {81, 7}, {105, 8}, {129, 9}, {153, 10}, {177, 11},
                                    {58, 12}, {82, 13}, {106, 14}, {130, 15}, {154, 16}, {178, 17},
                                    {59, 18}};
    map<int, int> idElemEntorno   = {{83, 0}, {107, 1}, {131, 2}, {155, 3}, {179, 7}, {60, 8},
                                    {84, 9}, {108, 4}, {132, 10}};

    //Guardo el puntero a la textura de las puertas tapiadas
    texSpritesTapiadas = pTapiadas;

    //Creo un lector de XML para leer la informacion de la sala
    LectorXML lector;
    if(lector.abrirArchivo(rutaSala)) {
        //Busca las propiedades en el fichero
        if(!lector.siguienteHijo("properties") || !lector.siguienteHijo("property")) {
            exit(0);
        }
        //Recorre las propiedades buscando las puertas

        bool buscando = true;
        do {
            //Comprueba si el elemento actual son las puertas y lo guarda
            if(lector.getAttribute("name").compare("EXITS") == 0) {
                //Guarda integers que indican donde hay salidas
                string auxString = lector.getAttribute("value");
                
                //Divide el numero en 4 valores
                for(int i = 0; i<4; i++) {
                    char valorI = auxString.at(i);
                    int puerta = valorI-'0';
                    
                    //Anyade un valor al vector por cada puerta
                    puertas.push_back(puerta);
                }
                buscando = false;
            }
            else {
                //Pasa al siguiente elemento
                if(!lector.siguienteHermano("property")) {
                    //Si no lo hay es que algo va mal
                    exit(0);
                }
            }
        } while(buscando);

        lector.volverARaiz();
        //Busca el mapa del escenario
        if(!lector.siguienteHijo("layer")) {
            exit(0);
        }

        buscando = true;
        do {
            //Comprueba si el elemento actual es el mapa o las colisiones y lo guarda
            if(lector.getAttribute("name").compare("Logica") == 0) {
                buscando        = false;
                coordsPuertas   = {{}, {}, {}, {}};     //Inicializa el vector con 4 posiciones vacias
                coordsAparicion = {{}, {}, {}, {}};

                FabricaEnemigos* fEnemigos              = FabricaEnemigos::Instancia();
                FabricaObjetosAtemporales* fAtemporal   = FabricaObjetosAtemporales::Instancia();
                FabricaElementosEntorno* fElemEntorno   = FabricaElementosEntorno::Instancia();
                EstadoPartida* estado                   = EstadoPartida::Instancia();

                //Comprueba que puede acceder al texto con la informacion
                if(!lector.siguienteHijo("data")) {
                    exit(0);
                }
                string cadenaMapa = lector.getElementText();
                
                //Con el objeto stringstream recorre el string
                stringstream sstream(cadenaMapa);
                string stringFila;
                int fila = 0;
                vector<int> coord = {-1, -1};

                //Recorre el texto por linea
                while(getline(sstream, stringFila, '\n')) {
                    stringstream sstreamFila(stringFila);
                    string valor;
                    int col = 0;

                    //Recorre los valores de la fila separados por coma
                    while(getline(sstreamFila, valor, ',')) {
                        if(valor.compare("0") != 0) {
                            int idValor = stoi(valor)-tamPatron-1;
                            coord[0] = col;
                            coord[1] = fila;

                            //Comprueba el valor de la id encontrada y procede segun a que pertenece
                            switch(idValor) {
                                case idPuertaArriba:        coordsPuertas[0]    = coord; break;
                                case idPuertaDerecha:       coordsPuertas[1]    = coord; break;
                                case idPuertaAbajo:         coordsPuertas[2]    = coord; break;
                                case idPuertaIzquierda:     coordsPuertas[3]    = coord; break;

                                case idSpawnArriba:         coordsAparicion[0]  = coord; break;
                                case idSpawnDerecha:        coordsAparicion[1]  = coord; break;
                                case idSpawnAbajo:          coordsAparicion[2]  = coord; break;
                                case idSpawnIzquierda:      coordsAparicion[3]  = coord; break;

                                case idCoordsRecompensa:    coordsRecompensa    = coord; break;

                                default:
                                    auto id = idEnemigos.find(idValor);

                                    if(id != idEnemigos.end()) {
                                        //Se crea el enemigo con la id correspondiente (factoria)
                                        Enemigo* enemigoActual = fEnemigos->fabricarEnemigo(id->second);
                                        if(enemigoActual != nullptr) {
                                            enemigoActual->setPosicion({(float)col*tamTiles, (float)fila*tamTiles});
                                            enemigoActual->setInventario(estado->getJugador()->getInventario());
                                            enemigos.push_back(enemigoActual);
                                        }
                                    }
                                    else {
                                        id = idObjAtemporal.find(idValor);

                                        if(id != idObjAtemporal.end()) {
                                            //Se crea el objeto con la id correspondiente
                                            ObjetoAtemporal* objetoActual = fAtemporal->fabricar(id->second, {col, fila});
                                            if(objetoActual != nullptr) {
                                                objetoActual->setSala(this);
                                                objAtemporales.push_back(objetoActual);
                                            }
                                        }
                                        else {
                                            id = idElemEntorno.find(idValor);

                                            if(id != idElemEntorno.end()) {
                                                //Se crea el elemento del entorno con la id correspondiente
                                                ElementoEntorno* elemActual = fElemEntorno->fabricar(id->second);
                                                if(elemActual != nullptr) {
                                                    elemActual->setPosicion({(float)col*tamTiles, (float)fila*tamTiles});
                                                    elementosEntorno.push_back(elemActual);
                                                }
                                            }
                                        }
                                    }
                                    break;
                            }
                        }

                        //Aumenta el valor de la columna en 1
                        col++;
                    }
                    
                    //Aumenta el valor de la fila en 1
                    if(col > 0) {
                        fila++;
                    }
                }
                coord.clear();
            }
            else {
                //Pasa al siguiente elemento
                if(!lector.siguienteHermano("layer")) {
                    //Si no lo hay es que algo va mal
                    exit(0);
                }
            }
        } while(buscando);
    }
    else {
        exit(0);
    }

    //Inicializa el resto de variables
    recompensa      = -1;
    descubierta     = false;
    superada        = false;
    puertasTapiadas = {false, false, false, false};
    spritesTapiadas = {};
}

//Destructor
Sala::~Sala() {
    //Recorre vectores que tengan objetos que borrar
    while(enemigos.size() > 0) {
        if(enemigos[enemigos.size()-1] != nullptr) {delete enemigos[enemigos.size()-1];}

        enemigos.pop_back();
    }
    while(objetosSoltados.size() > 0) {
        if(objetosSoltados[objetosSoltados.size()-1] != nullptr) {delete objetosSoltados[objetosSoltados.size()-1];}

        objetosSoltados.pop_back();
    }
    while(spritesTapiadas.size() > 0) {
        if(spritesTapiadas[spritesTapiadas.size()-1] != nullptr) {delete spritesTapiadas[spritesTapiadas.size()-1];}

        spritesTapiadas.pop_back();
    }
    while(elementosEntorno.size() > 0) {
        if(elementosEntorno[elementosEntorno.size()-1] != nullptr) {delete elementosEntorno[elementosEntorno.size()-1];}

        elementosEntorno.pop_back();
    }
    while(objAtemporales.size() > 0) {
        if(objAtemporales[objAtemporales.size()-1] != nullptr) {delete objAtemporales[objAtemporales.size()-1];}

        objAtemporales.pop_back();
    }

    puertas.clear();
    puertasTapiadas.clear();
    coordsPuertas.clear();
    coordsAparicion.clear();
    coordsRecompensa.clear();
    elementosEntorno.clear();

    texSpritesTapiadas = nullptr;
}

/*Comprueba y actualiza el estado de la sala. Si no quedan enemigos la sala pasara a estar superada
*y si no ha sido descubierta pasara a estarlo.
*/
void Sala::comprobarEstadoSala() {
    if(!superada && enemigos.size() == 0) {
        superada = true;
        generarRecompensa();
    }
    if(!descubierta) {
        Jugador* jugador = EstadoPartida::Instancia()->getJugador();
        if(jugador != nullptr) {
            //Avisa al jugador de que ha entrado en una sala nueva
            jugador->entradoEnNuevaSala();
        }
        descubierta = true;
    }
}

/*Dada una posicion, indica si hay una puerta no tapiada en dicha posicion. Las posiciones son:
*0 - Arriba
*1 - Derecha
*2 - Abajo
*3 - Izquierda
*/
bool Sala::existePuerta(int pos) {
    bool salida = false;

    //Comprueba en el vector de salidas si hay puerta en esa posicion
    if(pos >= 0 && pos <= 3 && puertas[pos] > 0 && !puertasTapiadas[pos]) {
        salida = true;
    }

    return salida;
}

//Actualiza las puertas tapiadas
void Sala::setPuertasTapiadas(vector<bool> pT) {
    //Actualiza el vector de puertas tapiadas
    puertasTapiadas = pT;

    //Elimina los sprites actualmente almacenados, si los hay
    while(spritesTapiadas.size() > 0) {
        if(spritesTapiadas[spritesTapiadas.size()-1] != nullptr) {delete spritesTapiadas[spritesTapiadas.size()-1];}

        spritesTapiadas.pop_back();
    }

    //Recorre el vector con las cuatro puertas
    for(int i=0; i<4; i++) {
        //Comprueba si existe una salida y si esta tapiada
        if(puertas[i] != 0 && puertasTapiadas[i]) {

            //Crea el sprite de la puerta a partir de la textura
            Sprite *sp = new Sprite(texSpritesTapiadas, 0, 0, texSpritesTapiadas->getSize()[0], texSpritesTapiadas->getSize()[1]);
            switch(i) {
                case 0:
                    sp->setOrigin(0, 0);
                    break;
                case 1:
                    sp->setOrigin(0, tamanyoTile[1]);
                    break;
                case 2:
                    sp->setOrigin(6*tamanyoTile[0], tamanyoTile[1]);
                    break;
                case 3:
                    sp->setOrigin(6*tamanyoTile[0], 0);
                    break;
            }
            sp->setScale(4, 4);

            //Orienta la puerta y la coloca en su lugar
            sp->setRotation(90*i);
            sp->setPosition(coordsPuertas[i][0]*tamTiles, coordsPuertas[i][1]*tamTiles);

            //Anyade el sprite al vector
            spritesTapiadas.push_back(sp);
        }
    }
}

/*Dada una matriz de salas, actualiza las puertas tapiadas de cada una de ellas segun su posicion 
*y las conexiones con las salas adyacentes.
*/
void Sala::actualizarPuertasTapiadas(vector<vector<Sala*>>* area) {
    //Recorre la matriz con las salas
    for(unsigned int i=0; i<area->size(); i++) {
        for(unsigned int j=0; j<(*area)[i].size(); j++) {
            if((*area)[i][j] != nullptr) {
                vector<bool> block;

                //Comprueba la sala que esta por encima
                if(j<(*area)[i].size()-1 && (*area)[i][j+1] != nullptr && (*area)[i][j+1]->existePuerta(2)) {
                    block.push_back(false);
                }
                else {
                    block.push_back(true);
                }

                //Comprueba la sala que esta a la derecha
                if(i<area->size()-1 && (*area)[i+1][j] != nullptr && (*area)[i+1][j]->existePuerta(3)) {
                    block.push_back(false);
                }
                else {
                    block.push_back(true);
                }

                //Comprueba la sala que esta por debajo
                if(j>0 && (*area)[i][j-1] != nullptr && (*area)[i][j-1]->existePuerta(0)) {
                    block.push_back(false);
                }
                else {
                    block.push_back(true);
                }

                //Comprueba la sala que esta a la izquierda
                if(i>0 && (*area)[i-1][j] != nullptr && (*area)[i-1][j]->existePuerta(1)) {
                    block.push_back(false);
                }
                else {
                    block.push_back(true);
                }
                
                //Actualiza las puertas bloqueadas de la sala
                (*area)[i][j]->setPuertasTapiadas(block);
                block.clear();
            }
        }
    }
}

//Dibuja la sala y todo lo que contiene
void Sala::dibujar(float percentTick, int update_milisegundos) {
    Escenario::dibujar();
    dibujarPuertasTapiadas();
    dibujarElementosEntorno();
    dibujarObjetosAtemporales();
    dibujarObjetosSoltados(percentTick);
    dibujarEnemigos(percentTick, update_milisegundos);
}

//Actualiza todo lo que haya en la sala
void Sala::update(Tiempo tiempo) {
    updateElementosEntorno(tiempo);
    updateObjetosAtemporales(tiempo);
    updateObjetosSoltados(tiempo);
    updateEnemigos(tiempo);
}

//Dibuja las puertas tapiadas
void Sala::dibujarPuertasTapiadas() {
    //Recupera la instancia de VentanaRender
    VentanaRender* ventana = VentanaRender::Instancia();

    //Recorre el vector de sprites y envia sus direcciones a VentanaRender para que los dibuje
    for(unsigned int i=0; i<spritesTapiadas.size(); i++) {
        ventana->dibujar(spritesTapiadas[i]);
    }
}

//Dibuja los enemigos
void Sala::dibujarEnemigos(float percent, int update_milisegundos) {
    //Recorre el vector de enemigos
    for(unsigned int i=0; i<enemigos.size(); i++) {
        if(enemigos[i] != nullptr && !enemigos[i]->getMuerto()) {
            //Dibuja el enemigo
            enemigos[i]->dibujar(percent, update_milisegundos);
        }
    }
}

//Dibuja los objetos soltados
void Sala::dibujarObjetosSoltados(float percentTick) {
    for(unsigned int i=0; i<objetosSoltados.size(); i++) {
        if(objetosSoltados[i] != nullptr && !objetosSoltados[i]->getRecogido()) {
            //Dibuja el objeto
            objetosSoltados[i]->dibujar(percentTick);
        }
    }
}

//Dibuja los elementos de la sala
void Sala::dibujarElementosEntorno() {
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        if(elementosEntorno[i] != nullptr) {
            //Dibuja el elemento
            elementosEntorno[i]->dibujar();
        }
    }
}

//Dibuja los objetos de otras lineas temporales
void Sala::dibujarObjetosAtemporales() {
    //Recorre el vector de objetos de otras lineas temporales
    for(unsigned int i=0; i<objAtemporales.size(); i++) {
        if(objAtemporales[i] != nullptr && objAtemporales[i]->getHabilitado()) {
            //Indica al enemigo que se actualice
            objAtemporales[i]->dibujar();
        }
    }
}

//Actualiza los objetos sueltos en la sala
void Sala::updateObjetosSoltados(Tiempo elapsedTime) {
    //Recorre el vector de objetos
    for(unsigned int i=0; i<objetosSoltados.size(); i++) {
        if(objetosSoltados[i] == nullptr || objetosSoltados[i]->getRecogido()) {
            //Si el objeto ya no esta lo elimina del vector
            objetosSoltados.erase(objetosSoltados.begin()+i);
            i--;
        }
        else {
            //Actualiza el objeto
            objetosSoltados[i]->update(elapsedTime, &mapaColisiones, tamTiles);
        }
    }
}

//Actualiza los elementos de la sala
void Sala::updateElementosEntorno(Tiempo elapsedTime) {
    //Recorre el vector de elementos del entorno
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        if(elementosEntorno[i] != nullptr) {
            //Actualiza el elemento
            elementosEntorno[i]->update(elapsedTime);
        }
    }
}

//Actualiza el estado de los enemigos
void Sala::updateEnemigos(Tiempo elapsedTime) {
    //Recorre el vector de enemigos
    for(unsigned int i=0; i<enemigos.size(); i++) {
        if(enemigos[i] != nullptr && !enemigos[i]->getMuerto()) {
            //Indica al enemigo que se actualice
            enemigos[i]->update(elapsedTime);
        }
        else {
            if(enemigos[i]->getMuerto()) {
                int idMat = enemigos[i]->getMaterialRecompensa();
                if(idMat != -1) {
                    //Crea el material con la id recibida del enemigo
                    Textura* tex = new Textura(FabricaMateriales::Instancia()->getRutaPorId(idMat));
                    Sprite* s = new Sprite(tex, 0, 0, 32, 32);
                    ObjetoSala* mat = new MaterialSuelto(enemigos[i]->getPosicion(), s, tex, idMat, 1);
                    
                    //Anyade el material a la sala
                    anyadirObjetoSala(mat);
                }

                //Realiza otros efectos si el jugador posee un objeto
                if(enemigos[i]->inventarioPoseeObjeto(6)) {
                    //Genera una nube de gas en la posicion en la que muere el enemigo
                    Textura* t = EstadoPartida::Instancia()->getTexturas(5);
                    Sprite* s = new Sprite(t, 0, 0, 64, 64);
                    NubeVeneno* nube = new NubeVeneno(s, enemigos[i]->getPosicion(), 0, DANYO_GAS_ENEMIGO, 200, 20000);
                    EstadoPartida::Instancia()->anyadirNubeVeneno(nube);
                }
                if(enemigos[i]->inventarioPoseeObjeto(7)) {
                    //Existe la posibilidad de que otro enemigo explote
                    int random = rand() % 100;
                    if(random < PROB_EXPLOTAR_ENEMIGO && enemigos.size() > 1) {
                        //Elige otro enemigo aleatoriamente
                        int j = rand() % enemigos.size();
                        if(j == (int)i || enemigos[j] == nullptr) {
                            j++;
                        }
                        if(j < (int)enemigos.size() && enemigos[j] != nullptr) {
                            //Tras asegurarse de que existe genera la explosion
                            Textura* t = EstadoPartida::Instancia()->getTexturas(4);
                            Sprite* s = new Sprite(t, 0, 0, 32, 32);
                            s->setScale(6.0, 6.0);
                            Explosion* explosion = new Explosion(0, 150, s, nullptr, enemigos[j]->getPosicion(), 96.0, 0, 0);
                            EstadoPartida::Instancia()->anyadirExplosion(explosion);
                        }
                    }
                }
            }
            enemigos.erase(enemigos.begin()+i);
            i--;
            comprobarEstadoSala();
        }
    }
}

//Actualiza el estado de los objetos de otras lineas temporales
void Sala::updateObjetosAtemporales(Tiempo elapsedTime) {
    //Recorre el vector de objetos de otras lineas temporales
    for(unsigned int i=0; i<objAtemporales.size(); i++) {
        if(objAtemporales[i] != nullptr && objAtemporales[i]->getHabilitado()) {
            //Indica al enemigo que se actualice
            objAtemporales[i]->update(elapsedTime);
        }
        else {
            objAtemporales.erase(objAtemporales.begin()+i);
            i--;
        }
    }
}

//Detiene todo lo detenible en la sala
void Sala::detenerSala() {
    //Recorre el vector de enemigos
    for(unsigned int i=0; i<enemigos.size(); i++) {
        if(enemigos[i] != nullptr) {
            //Detiene al enemigo
            enemigos[i]->detener();
        }
    }

    //Recorre el vector de objetos atemporales
    for(unsigned int i=0; i<objAtemporales.size(); i++) {
        if(objAtemporales[i] != nullptr) {
            //Detiene el objeto
            objAtemporales[i]->detener();
        }
    }

    //Recorre el vector de objetos atemporales
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        if(elementosEntorno[i] != nullptr) {
            //Detiene el elemento del entorno
            elementosEntorno[i]->detener();
        }
    }
}

//Reanuda todo lo detenibl en la sala
void Sala::reanudarSala() {
    //Recorre el vector de enemigos
    for(unsigned int i=0; i<enemigos.size(); i++) {
        if(enemigos[i] != nullptr) {
            //Reanuda al enemigo
            enemigos[i]->reanudar();
        }
    }

    //Recorre el vector de objetos atemporales
    for(unsigned int i=0; i<objAtemporales.size(); i++) {
        if(objAtemporales[i] != nullptr) {
            //Reanuda el objeto
            objAtemporales[i]->reanudar();
        }
    }

    //Recorre el vector de objetos atemporales
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        if(elementosEntorno[i] != nullptr) {
            //Reanuda el elemento del entorno
            elementosEntorno[i]->reanudar();
        }
    }
}

//Devuelve el vector de enemigos
vector<Enemigo*>* Sala::getEnemigos() {
    return &enemigos;
}

//Devuelve si hay recompensa en la sala
int Sala::getRecompensa(){
    return recompensa;
}

//Devuelve si la sala ha sido superada
bool Sala::getSuperada(){
    return superada;
}

//Devuelve si la sala ha sido descubierta
bool Sala::getDescubierta(){
    return descubierta;
}

//Devuelve las coordenadas de aparicion de una de las puertas
vector<int> Sala::getCoordsAparicion(int dir) {
    vector<int> coords = {-1, -1};
    if(dir >= 0 && dir <= 3) {
        coords[0] = coordsAparicion[dir][0]*tamTiles;
        coords[1] = coordsAparicion[dir][1]*tamTiles;
    }
    return coords;
}

vector<int> Sala::getCoordsRecompensa(){
    vector<int> coords = coordsRecompensa;
    if(coords.size() != 2) {
        coords = {-1, -1};
    }
    return coords;
}

//Elimina el objeto de la sala para que no aparezca mas
bool Sala::eliminarObjetoSala(ObjetoSala* obj) {
    bool encontrado = false;
    for(unsigned int i=0; i<objetosSoltados.size() && !encontrado; i++) {
        //Comprueba si el objeto que busca se encuentra en el vector
        if(objetosSoltados[i] == obj) {
            encontrado = true;

            //Elimina el objeto del vector
            delete objetosSoltados[i];
            objetosSoltados[i] = nullptr;
            objetosSoltados.erase(objetosSoltados.begin()+i);
        }
    }

    return encontrado;
}

//Anyade un objeto suelto nuevo a la sala
void Sala::anyadirObjetoSala(ObjetoSala* obj) {
    objetosSoltados.push_back(obj);
}

/*Devuelve la referencia al objeto de la sala mas cercano a la posicion pasada por parametro.
*Como segundo parametro recibe un puntero a float donde guardara la distancia.
*/
ObjetoSala* Sala::getObjetoSalaCercano(vector<float> ref, float* menorDist) {
    ObjetoSala* obj = nullptr;
    *menorDist = -1.0;

    //Recorre el vector de objetos
    for(unsigned int i=0; i<objetosSoltados.size(); i++) {
        if(objetosSoltados[i] != nullptr) {
            //Recupera la posicion del objeto y calcula la distancia al punto de referencia
            vector<float> pos = objetosSoltados[i]->getPosicion();
            float distancia = sqrt(pow(pos[0]-ref[0], 2) + pow(pos[1]-ref[1], 2));

            //Si no ha guardado ningun objeto o este se encuentra mas cerca que el guardado, lo actualiza
            if(*menorDist == -1.0 || distancia < *menorDist) {
                *menorDist = distancia;
                obj = objetosSoltados[i];
            }
        }
    }

    return obj;
}

//Anyade un elemento del entorno a la sala
void Sala::anyadirElementoEntorno(ElementoEntorno* elem) {
    elementosEntorno.push_back(elem);
}

/*Devuelve la referencia al elemento interactuable de la sala mas cercano a la posicion pasada por parametro.
*Como segundo parametro recibe un puntero a float donde guardara la distancia.
*/
ElementoEntorno* Sala::getElementoInteractuableCercano(vector<float> ref, float* menorDist) {
    ElementoEntorno* elem = nullptr;
    *menorDist = -1.0;

    //Recorre el vector de objetos
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        if(elementosEntorno[i] != nullptr && elementosEntorno[i]->getInteractuable()) {
            //Recupera la posicion del objeto y calcula la distancia al punto de referencia
            vector<float> pos = elementosEntorno[i]->getPosicion();
            float distancia = sqrt(pow(pos[0]-ref[0], 2) + pow(pos[1]-ref[1], 2));

            //Si no ha guardado ningun objeto o este se encuentra mas cerca que el guardado, lo actualiza
            if(*menorDist == -1.0 || distancia < *menorDist) {
                if(distancia <= elementosEntorno[i]->getDistInteraccion()) {
                    *menorDist = distancia;
                    elem = elementosEntorno[i];
                }
            }
        }
    }

    return elem;
}

//Anyade un objeto de otra linea temporal a la sala
void Sala::anyadirObjAtemporal(ObjetoAtemporal* obj) {
    objAtemporales.push_back(obj);
}

//Eliminar un objeto de otra linea temporal a la sala
bool Sala::eliminarObjAtemporal(ObjetoAtemporal* obj) {
    bool encontrado = false;
    for(unsigned int i=0; i<objAtemporales.size() && !encontrado; i++) {
        //Comprueba si el objeto que busca se encuentra en el vector
        if(objAtemporales[i] == obj) {
            encontrado = true;

            //Elimina el objeto del vector
            delete objAtemporales[i];
            objAtemporales[i] = nullptr;
            objAtemporales.erase(objAtemporales.begin()+i);
        }
    }

    return encontrado;
}

/*Devuelve la referencia al objeto atemporal de la sala mas cercano a la posicion pasada por parametro.
*Como segundo parametro recibe un puntero a float donde guardara la distancia.
*/
ObjetoAtemporal* Sala::getObjetoAtemporalCercano(vector<float> ref, float* menorDist) {
    ObjetoAtemporal* obj = nullptr;
    *menorDist = -1.0;

    //Recorre el vector de objetos
    for(unsigned int i=0; i<objAtemporales.size(); i++) {
        if(objAtemporales[i] != nullptr && objAtemporales[i]->getInteractuable()) {
            //Recupera la posicion del objeto y calcula la distancia al punto de referencia
            vector<float> pos = objAtemporales[i]->getPosicion();
            float distancia = sqrt(pow(pos[0]-ref[0], 2) + pow(pos[1]-ref[1], 2));

            //Si no ha guardado ningun objeto o este se encuentra mas cerca que el guardado, lo actualiza
            if(*menorDist == -1.0 || distancia < *menorDist) {
                *menorDist = distancia;
                obj = objAtemporales[i];
            }
        }
    }

    return obj;
}

//Incluye el enemigo en la sala
void Sala::anyadirEnemigo(Enemigo* e) {
    if(e != nullptr) {
        e->setInventario(EstadoPartida::Instancia()->getJugador()->getInventario());
        enemigos.push_back(e);
    }
}

//Comprueba si se ha interactuado con un portalSalaADesafio
//Si es asi se envia al jugador a la sala de desafio.
void Sala::entrarPortal(){
    for(unsigned int i = 0; i < elementosEntorno.size(); i++){
        if(dynamic_cast<PortalSalaADesafio*>(elementosEntorno[i])){
            if(dynamic_cast<PortalSalaADesafio*>(elementosEntorno[i])->getInteractuando()){
                EstadoPartida *estado = EstadoPartida::Instancia();
                //Se cambia la sala actual del personaje por la de desafio y se anyade el portal de desafio
                vector<int> aux = {(int)elementosEntorno[i]->getPosicion()[0], (int)elementosEntorno[i]->getPosicion()[1]};
                estado->getJugador()->setCoordsPortal(aux);
                estado->getJugador()->setDisparo(false);

                VentanaRender* ventana = VentanaRender::Instancia();
                ventana->fadeIn(20);
                estado->setCambiandoSala(4);
            }
        }
        if(dynamic_cast<PortalJefe*>(elementosEntorno[i])){
            if(dynamic_cast<PortalJefe*>(elementosEntorno[i])->getInteractuando()){
                EstadoPartida *estado = EstadoPartida::Instancia();
                //Se cambia la sala actual del personaje por la de desafio y se anyade el portal de desafio
                vector<int> aux = {(int)elementosEntorno[i]->getPosicion()[0], (int)elementosEntorno[i]->getPosicion()[1]};
                estado->getJugador()->setCoordsPortal(aux);
                estado->getJugador()->setDisparo(false);

                VentanaRender* ventana = VentanaRender::Instancia();
                ventana->fadeIn(20);
                estado->setCambiandoSala(6);
            }
        }
    }
}

//Genera la recompensa en la posicion guardado. Dependiendo de la ID, generara diferentes cosas
void Sala::generarRecompensa() {
    //Comprueba que la sala esta superada
    if(superada && coordsRecompensa.size()==2) {
        FabricaElementosEntorno* fab    = FabricaElementosEntorno::Instancia();
        ElementoEntorno* elemRecompensa = nullptr;
        vector<float> posFinalRec       = {(float)coordsRecompensa[0]*tamTiles, (float)coordsRecompensa[1]*tamTiles};

        switch(recompensa) {
            case 1:
                //Genera un cofre
                elemRecompensa = fab->fabricar(4);
                elemRecompensa->setPosicion(posFinalRec);
                anyadirElementoEntorno(elemRecompensa);
                break;
            case 2:
                //Genera un portal a sala de desafio
                elemRecompensa = fab->fabricar(5);
                elemRecompensa->setPosicion(posFinalRec);
                anyadirElementoEntorno(elemRecompensa);
                break;
            case 3:
                //Genera un portal a la sala del jefe
                elemRecompensa = fab->fabricar(6);
                elemRecompensa->setPosicion(posFinalRec);
                anyadirElementoEntorno(elemRecompensa);
        }

        if(elemRecompensa != nullptr) {
            posFinalRec[0] += (float)(tamTiles/2.0);
            posFinalRec[1] += (float)(tamTiles/2.0);

            //Genera particulas donde va a aparecer la recompensa
            for(int i=12; i>0; i--) {
                new ParticulaSimpleCuadrada(posFinalRec, {255, 238, 0, 255}, 30, 1200);
            }
        }
    }
}

//Actualiza la id de la recompensa de la sala
void Sala::setRecompensa(int id) {
    recompensa = id;
}

//Recorre los elementos de la sala en busca de cofres e indica si hay alguno por abrir
bool Sala::cofresPorAbrir() {
    bool cofreSinAbrir = false;

    //Recorre el vector de objetos
    for(unsigned int i=0; i<elementosEntorno.size() && !cofreSinAbrir; i++) {
        if(elementosEntorno[i] != nullptr && dynamic_cast<Cofre*>(elementosEntorno[i])) {
            //Comprueba si se ha abierto
            if(dynamic_cast<Cofre*>(elementosEntorno[i])->getAbierto() == false) {
                cofreSinAbrir = true;
            }
        }
    }

    return cofreSinAbrir;
}

//Crea una sala de jefe de las que hay predefinidas. El int determina la sala a crear.
Sala* Sala::crearSalaJefe(int id) {
    Sala* salaJefe          = nullptr;
    EstadoPartida* estP     = EstadoPartida::Instancia();
    FabricaEnemigos* fab    = FabricaEnemigos::Instancia();
    Enemigo* e              = nullptr;
    switch(id) {
        case 1:
            //Crea la sala del jefe rana
            salaJefe = new Sala("resources/niveles/SalaJefeRana.tmx", estP->getTexturas(0), TAM_TILE, estP->getTexturas(1));
            e = fab->fabricarJefe(1);
            e->setPosicion({38.0*32.0, 24.0*32.0});
            salaJefe->anyadirEnemigo(e);
            break;
        case 2:
            //Crea la sala del jefe ojo
            salaJefe = new Sala("resources/niveles/SalaJefeOjo.tmx", estP->getTexturas(0), TAM_TILE, estP->getTexturas(1));
            e = fab->fabricarJefe(2);
            e->setPosicion({23.0*32.0, 13.0*32.0});
            salaJefe->anyadirEnemigo(e);
            break;
        case 3:
            //Crea la sala del jefe caballero
            salaJefe = new Sala("resources/niveles/SalaJefeCaballero.tmx", estP->getTexturas(0), TAM_TILE, estP->getTexturas(1));
            e = fab->fabricarJefe(3);
            e->setPosicion({27.0*32.0, 8.0*32.0});
            salaJefe->anyadirEnemigo(e);
            break;
    }

    return salaJefe;
}

//Devuelve un vector con todos los muros destructibles de la sala
vector<MuroDestructible*> Sala::getMurosDestructibles() {
    vector<MuroDestructible*> muros;

    //Recorre todos los elementos del entorno de la sala
    for(unsigned int i=0; i<elementosEntorno.size(); i++) {
        //Comprueba si es un muro destructible y si no esta roto
        if(dynamic_cast<MuroDestructible*>(elementosEntorno[i]) && dynamic_cast<MuroDestructible*>(elementosEntorno[i])->getDestruido() == false) {
            muros.push_back(dynamic_cast<MuroDestructible*>(elementosEntorno[i]));
        }
    }

    return muros;
}