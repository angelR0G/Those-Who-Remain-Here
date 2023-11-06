#include "ArmaSuelta.h"

const int ESPERA_RECOGER    = 500;

//Constructor. Ademas de los parametros de ObjetoSala, recibe la id del material y la cantidad que contiene
ArmaSuelta::ArmaSuelta(vector<float> p, Sprite* s, Textura* t, int id):ObjetoSala(p, s, t) {
    idArma          = id;
    tiempoEspera    = ESPERA_RECOGER;

    FabricaArmas* fabrica   = FabricaArmas::Instancia();
    arma        = fabrica->fabricarArma(id);
    nombre      = fabrica->getNombre(id);

    danyo       = to_string(fabrica->getDanyo(id));
    municion    = to_string(fabrica->getMunicion(id));

    //Me aseguro de que la escala es correcta
    float esc = fabrica->getEscala(idArma);
    sprite->setScale(esc, esc);


    //Crea el marco
    marcoN = new ArrayVertices(4, 4);

    marcoN->setArrayColor(0, 0, 0, 100);

    texto = new Texto(nombre);
    textod = new Texto(danyo);
    textom = new Texto(municion);

    Fuente* fuente=new Fuente("resources/sprites/generales/Chonkly.otf");
    vector<int> col_text =    {255,255,  0,255};

    texto->setFuente(fuente);
    texto->setTamanyo(20);
    texto->setEscala(1,1);
    texto->setColorTexto(col_text);

    textod->setFuente(fuente);
    textod->setTamanyo(20);
    textod->setEscala(1,1);
    textod->setColorTexto(col_text);

    textom->setFuente(fuente);
    textom->setTamanyo(20);
    textom->setEscala(1,1);
    textom->setColorTexto(col_text);

    an_f = s->getGlobalPixelSize()[0];
    al_f = s->getGlobalPixelSize()[1];
}

//Destructor
ArmaSuelta::~ArmaSuelta() {
    delete arma;
}

//Actualiza el estado del arma suelta
void ArmaSuelta::update(Tiempo t, map<vector<int>, int>* mapa, int tamTiles) {
    ObjetoSala::update(t, mapa, tamTiles);
    //Si hay un tiempo de espera, se resta el tiempo que ha pasado en milisegundos
    if(tiempoEspera > 0) {
        tiempoEspera -= t.milisegundos();
    }
}

//Sustituye el arma del jugador por esta
void ArmaSuelta::recoger(Inventario* inv) {
    if(tiempoEspera <= 0) {
        tiempoEspera = ESPERA_RECOGER;

        FabricaArmas* fab = FabricaArmas::Instancia();
        Jugador* jugador = EstadoPartida::Instancia()->getJugador();

        //Recupera el arma del jugador, que sera soltada
        Arma* armaSoltada = jugador->getArma();

        //Modifica el arma del jugador
        jugador->setArma(arma);

        //Modifica los valores para la nueva arma soltada
        idArma  = armaSoltada->getId();

        delete sprite;
        delete textura;

        textura     = new Textura("resources/sprites/armas/" + fab->getRuta(idArma));
        vector<int> rect = fab->getRecorte(idArma);
        sprite      = new Sprite(textura, 0, 0, rect[0], rect[1]);
        float esc = fab->getEscala(idArma);
        sprite->setOrigin(rect[0]/2.0, rect[1]/2.0);
        sprite->setScale(esc, esc);
        sprite->setPosition(posicion[0], posicion[1]);

        arma        = armaSoltada;
        nombre      = fab->getNombre(idArma);

        //Actualiza las varibles de movimiento para que pegue un salto
        colocado = false;
        velocidadActual = -180.0;
    }
}

//Mostrar informacion de arma
void ArmaSuelta::mostrarInformacion(){
    VentanaRender* ventana = VentanaRender::Instancia();

    int ancho = ventana->getSize()[0];
    int alto = ventana->getSize()[1];
    
    tipo = "Arma: ";

    if(dynamic_cast<Escopeta*>(arma)){
        tipo = "Escopeta: ";
    }else if(dynamic_cast<Pistola*>(arma)){
        tipo = "Pistola: ";
    }else if(dynamic_cast<Metralleta*>(arma)){
        tipo = "Metralleta: ";
    }else if(dynamic_cast<LanzaCohetes*>(arma)){
        tipo = "Lanzacohetes: ";
    }

    danyo       = to_string((int)arma->getDanyoTotal());
    municion    = to_string(arma->getMunicion());

    texto->setString(tipo+nombre);
    textod->setString("Danyo: "+danyo);
    textom->setString("Balas: "+municion);
    
    int anchotxt = texto->getAncho();
    //int altotxt = texto->getAlto();

    if(texto->getAncho() < textod->getAncho()){
        anchotxt = textod->getAncho();
    }else if(texto->getAncho() < textom->getAncho()){
        anchotxt = textom->getAncho();
    }

    texto->setEscala(ancho/1920,alto/1080);
    textod->setEscala(ancho/1920,alto/1080);
    textom->setEscala(ancho/1920,alto/1080);

    marcoN->setVertexPosition(0, (int)posicion[0] - anchotxt/2 - anchotxt/8, (int)posicion[1] - al_f*2          - texto->getAlto() - textod->getAlto() - textom->getAlto());
    marcoN->setVertexPosition(1, (int)posicion[0] + anchotxt/2 + anchotxt/8, (int)posicion[1] - al_f*2          - texto->getAlto() - textod->getAlto() - textom->getAlto());
    marcoN->setVertexPosition(2, (int)posicion[0] + anchotxt/2 + anchotxt/8, (int)posicion[1] - al_f/2 - al_f/8);
    marcoN->setVertexPosition(3, (int)posicion[0] - anchotxt/2 - anchotxt/8, (int)posicion[1] - al_f/2 - al_f/8);

    //Se posicionan textos uno encima de otro
    texto->setPosicion( posicion[0]-texto->getAncho()/2 ,posicion[1]-2*al_f-texto->getAlto()-textod->getAlto()-textom->getAlto());
    textod->setPosicion(posicion[0]-textod->getAncho()/2,posicion[1]-2*al_f-textod->getAlto());
    textom->setPosicion(posicion[0]-textom->getAncho()/2,posicion[1]-2*al_f);

    ventana->dibujar(marcoN);

    ventana->dibujar(texto);
    ventana->dibujar(textod);
    ventana->dibujar(textom);
    
    vector<int> tam = marcoN->getBounds();
    int marcoX = -(marcoN->getVertexPosition(1)[0]-marcoN->getVertexPosition(0)[0])/2 + posicion[0];
    int marcoY = -(marcoN->getVertexPosition(1)[1]-marcoN->getVertexPosition(0)[1])   + posicion[1] -2*al_f-texto->getAlto()-textod->getAlto()-textom->getAlto();
    //Coloca el marco del mensaje modal
    marco[0]->setPosition(marcoX         , marcoY);//Esq superior izq
    marco[1]->setPosition(marcoX+tam[0]/2, marcoY);//Lado superior
    marco[2]->setPosition(marcoX+tam[0]  , marcoY);//Esq superior dere
    marco[3]->setPosition(marcoX+tam[0]  , marcoY+tam[1]/2);//Lado dere
    marco[4]->setPosition(marcoX+tam[0]  , marcoY+tam[1]);//Esq abajo dere
    marco[5]->setPosition(marcoX+tam[0]/2, marcoY+tam[1]);//Lado abajo
    marco[6]->setPosition(marcoX         , marcoY+tam[1]);//Esq abajo izq
    marco[7]->setPosition(marcoX         , marcoY+tam[1]/2);//Lado izq
    marco[1]->setScale((tam[0]*0.95)/16, 1.0);
    marco[3]->setScale(1.0, (tam[1]*0.9)/16);
    marco[5]->setScale((tam[0]*0.95)/16, 1.0);
    marco[7]->setScale(1.0, (tam[1]*0.9)/16);
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->draw();
    }
    
}