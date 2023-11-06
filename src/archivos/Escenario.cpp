#include "Escenario.h"

const int tamTiles                  = 32;       //Tamanyo en pixeles que va a tener cada tile en pantalla
const int tamPatron                 = 504;      //Cantidad de tiles del tilesheet. Necesario para calcular el id real de las colisiones.

const int idColision                = 65;       //Id de las colisiones en el mapa
const int idColisionPlataforma      = 67;       //Id de las plataformas en el mapa
const int idColisionPuertaArriba    = 207;      //Id de las colisiones con las puertas
const int idColisionPuertaDerecha   = 209;
const int idColisionPuertaAbajo     = 211;
const int idColisionPuertaIzquierda = 213;

//Constructor. Recibe la ruta del fichero XML con el mapa, un puntero a la textura del tilesheet y el tamanyo de los tiles
Escenario::Escenario(string rutaEscenario, Textura* pTilesheet, vector<int> tamTile) {

    //Guarda el tamanyo de los tiles
    tamanyoTile = tamTile;

    //Carga el tileset
    cargarTilesheet(pTilesheet);

    //Crea un objeto LectorXML para leer el archivo con el escenario
    LectorXML lector;
    if(lector.abrirArchivo(rutaEscenario)) {
        //Busca el elemento que contiene el nombre
        if(!lector.siguienteHijo("properties") || !lector.siguienteHijo("property")) {
            exit(0);
        }
        //Recorre las propiedades buscando el nombre del nivel
        bool buscando = true;
        do {
            //Comprueba si el elemento actual es el nombre y lo guarda
            if(lector.getAttribute("name").compare("LEVEL") == 0) {
                buscando = false;
                nombre = lector.getAttribute("value");
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
        bool buscandoMapa = true, buscandoColision = true;
        do {
            bool comprobandoColision = false;
            //Comprueba si el elemento actual es el mapa o las colisiones y lo guarda
            if(lector.getAttribute("name").compare("Fondo") == 0 || lector.getAttribute("name").compare("Solido") == 0) {
                if(lector.getAttribute("name").compare("Fondo") == 0) {
                    buscandoMapa = false;
                    //Guarda el tamanyo del mapa
                    tamanyoMapa.push_back(stoi(lector.getAttribute("width")));
                    tamanyoMapa.push_back(stoi(lector.getAttribute("height")));
                }
                else {
                    comprobandoColision = true;
                    buscandoColision = false;
                }

                if(!lector.siguienteHijo("data")) {
                    exit(0);
                }
                string cadenaMapa = lector.getElementText();
                
                //Con el objeto stringstream recorre el string
                stringstream sstream(cadenaMapa);
                string stringFila;
                int fila = 0;

                //Recorre el texto por linea
                while(getline(sstream, stringFila, '\n')) {
                    stringstream sstreamFila(stringFila);
                    string valor;
                    int col = 0;

                    //Recorre los valores de la fila separados por coma
                    while(getline(sstreamFila, valor, ',')) {
                        vector <int> coord = {col, fila};

                        //Guarda el valor en el mapa correspondiente
                        if(comprobandoColision) {
                            unsigned long v = stoul(valor)-tamPatron-1;
                            if( v == idColision || 
                                v == idColisionPlataforma || 
                                v == idColisionPuertaAbajo || 
                                v == idColisionPuertaArriba ||
                                v == idColisionPuertaDerecha ||
                                v == idColisionPuertaIzquierda){
                                mapaColisiones.insert({coord, v});
                            }
                        }
                        else {
                            almacenaSpriteMapa(coord, stoul(valor)-1);
                        }
                        coord.clear();

                        //Aumenta el valor de la columna en 1
                        col++;
                    }
                    
                    //Aumenta el valor de la fila en 1
                    if(col > 0) {
                        fila++;
                    }
                }
                lector.padre();
            }
            
            //Pasa al siguiente elemento
            if(!lector.siguienteHermano("layer") && (buscandoMapa || buscandoColision)) {
                //Si no lo hay es que algo va mal
                exit(0);
            }
        } while(buscandoMapa || buscandoColision);

        //Crea el array de vertices para el fondo
        fondo = new ArrayVertices(4, 4);
        fondo->setArrayColor(14, 7, 27, 255);
        fondo->setVertexPosition(0, -1920, -1080);
        fondo->setVertexPosition(1, tamanyoMapa[0]*tamTiles + 1920, -1080);
        fondo->setVertexPosition(2, tamanyoMapa[0]*tamTiles + 1920, tamanyoMapa[1]*tamTiles + 1080);
        fondo->setVertexPosition(3, -1920, tamanyoMapa[1]*tamTiles + 1080);
    }
    else {
        exit(0);
    }
}

//Destructor
Escenario::~Escenario() {
    for(auto i=mapa.begin(); i != mapa.end(); ++i) {
        delete mapa.find(i->first)->second;
    }
    mapa.clear();
    mapaColisiones.clear();
    idTiles.clear();
    tamanyoMapa.clear();
    tamanyoTile.clear();
    delete fondo;
    tilesheet = nullptr;
}

//Dada la posicion en el mapa y una id, guarda el Sprite con el tile que se dibujara en dicha posicion.
void Escenario::almacenaSpriteMapa(vector<int> coord, unsigned long id) {
    bool hflip = false;
    bool vflip = false;
    
    //Cojo el tile del tilesheet a partir del id
    auto iterator = idTiles.find(id);

    //Si no lo encuentra compruebo si la id tiene modificaciones
    if(iterator == idTiles.end()) {
        //Comprueba si el tile esta flipeado horizontalmente (ultimo bit == 1)
        unsigned long idCorrecta = id;
        if(idCorrecta >= pow(2, 31)) {
            idCorrecta = idCorrecta - pow(2, 31);
            hflip = true;
        }

        //Comprueba si el tile esta flipeado verticalmente (penultimo bit == 1)
        if(idCorrecta >= pow(2, 30)) {
            idCorrecta = idCorrecta - pow(2, 30);
            vflip = true;
        }

        //Vuelvo a buscar la id ya corregida
        iterator = idTiles.find((int)idCorrecta);
    }
    
    //Si encuentra un tile con la misma id lo dibuja
    if(iterator != idTiles.end()) {
        //Recoge las coordenadas de el tile en la textura
        vector<int> coordsTex = iterator->second;

        //Crea el sprite a partir de la textura y las coordenadas del area de recortado
        Sprite* s = new Sprite(tilesheet, coordsTex[0], coordsTex[1], tamanyoTile[0], tamanyoTile[1]);
        s->setOrigin(tamanyoTile[0] / 2, tamanyoTile[1] / 2);

        //Lo coloca segun su posicion
        s->setPosition(coord[0]*tamTiles + tamTiles/2, coord[1]*tamTiles + tamTiles/2);

        //Crea la escala a partir del valor de referencia
        float scale_x = tamTiles/(float)tamanyoTile[0], scale_y = tamTiles/(float)tamanyoTile[1];
        //Hago flip al tile si es necesario
        if(hflip) {
            scale_x *= -1;
        }
        if(vflip) {
            scale_y *= -1;
        }

        s->setScale(scale_x, scale_y);

        //Guarda el Sprite en el mapa
        mapa[coord] = s;
    }
}

//Utiliza la textura del puntero que recibe como tilesheet
void Escenario::cargarTilesheet(Textura* pTilesheet) {

    //Cargo la imagen con el tileset
    tilesheet = pTilesheet;

    //Calcula la cantidad de tiles a partir del tamanyo de los tiles y la textura
    vector<int> size = tilesheet->getSize();
    int x_size = size[0] / tamanyoTile[0];
    int y_size = size[1] / tamanyoTile[1];
    
    //Asigno las coordenadas de cada tile
    for(int i = 0; i < y_size; i++) {
        for(int j = 0; j < x_size; j++) {
            vector<int> aux;

            aux.push_back(j * tamanyoTile[0]);
            aux.push_back(i * tamanyoTile[1]);

            //Guardo las coordenadas de cada tile en el map
            idTiles[i*x_size + j] = aux;

            aux.clear();
        }
    }

    //Cambia la textura a los Sprites del mapa (si los hay)
    for(auto i=mapa.begin(); i!=mapa.end(); ++i) {
        i->second->setTextura(pTilesheet);
    }
}

/*Dada la posicion del jugador, devolvera un vector con la posicion de la camara centrada
*en el jugador y la sala al mismo tiempo.
*/
vector<float> Escenario::centrarCamara(float jugadorX, float jugadorY) {
    float posX = 0, posY = 0;
    VentanaRender* ventana = VentanaRender::Instancia();
    vector<float> tamVentana = ventana->getSizeVista();

    //Comprueba si la sala cabe en el ancho de la ventana
    if((unsigned int) tamanyoMapa[0] * tamTiles <= tamVentana[0]) {
        //Si es pequenya la centra en la pantalla
        posX = (float) (tamanyoMapa[0]/2 * tamTiles);
    }
    else {
        //Si es mas grande bloquea para que no se salga del borde
        posX = max(jugadorX, (float) tamVentana[0]/2);
        posX = min(posX, (float) tamanyoMapa[0] * tamTiles - tamVentana[0] / 2);
    }

    //Comprueba si la sala cabe en el alto de la ventana
    if((unsigned int) tamanyoMapa[1] * tamTiles <= tamVentana[1]) {
        //Si es pequenya la centra en la pantalla
        posY = (float) (tamanyoMapa[1]/2 * tamTiles);
    }
    else {
        //Si es mas grande bloquea para que no se salga del borde
        posY = max(jugadorY, (float) tamVentana[1] / 2);
        posY = min(posY, (float) tamanyoMapa[1] * tamTiles - tamVentana[1] / 2);
    }
    
    return {posX, posY};
}

//Dibuja el mapa en la ventana
void Escenario::dibujar() {
    VentanaRender* ventana = VentanaRender::Instancia();

    //Dibuja el fondo
    ventana->dibujar(fondo);

    //Recorre el mapa y dibuja los Sprite
    for(auto i = mapa.begin(); i!=mapa.end(); ++i) {
        ventana->dibujar(i->second);
    }
}

//Devuelve el tamanyo del mapa en tiles
vector<int> Escenario::getTamanyoMapa() {
    return tamanyoMapa;
}

//Devuelve el tamanyo real de los tiles en pixeles
vector<int> Escenario::getTamanyoTile() {
    return tamanyoTile;
}

//Devuelve una referencia al mapa de colisiones
map<vector<int>, int>* Escenario::getMapaColisiones() {
    return &mapaColisiones;
}

/*Dado un int, devuelve la id de un tipo de colisiones determinado en el mapa de colisiones.
*El tipo puede valer:
*1 - Id de las colisiones completas (valor devuelto por defecto)
*2 - Id de las plataformas
*3-6 - Id de las colisiones con las puertas
*/
int Escenario::getIdColision(int tipo) {
    int id = idColision;

    switch(tipo) {
        case 2: id = idColisionPlataforma;      break;
        case 3: id = idColisionPuertaArriba;    break;
        case 4: id = idColisionPuertaDerecha;   break;
        case 5: id = idColisionPuertaAbajo;     break;
        case 6: id = idColisionPuertaIzquierda; break;
    }

    return id;
}

//Devuelve el tamanyo final de los tiles en pantalla (constante)
int Escenario::getConstTamTiles() {
    return tamTiles;
}

//Dado un puntero a un mapa como el de las colisiones, anyade esas colisiones al escenario
void Escenario::anyadirColision(map<vector<int>, int>* nuevasCol) {
    for(auto i=nuevasCol->begin(); i != nuevasCol->end(); ++i) {
        int v = i->second;
        if( v == idColision || 
            v == idColisionPlataforma || 
            v == idColisionPuertaAbajo || 
            v == idColisionPuertaArriba ||
            v == idColisionPuertaDerecha ||
            v == idColisionPuertaIzquierda){
            mapaColisiones.insert({i->first, v});
        }
    }
}

//Dado un puntero a vector de posiciones, elimina todas esas posiciones del mapa de colisiones
void Escenario::eliminarColision(vector<vector<int>>* eliminar) {
    for(unsigned int i=0; i<eliminar->size(); i++) {
        if((*eliminar)[i].size() == 2) {
            mapaColisiones.erase((*eliminar)[i]);
        }
    }
}