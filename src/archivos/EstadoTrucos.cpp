#include "EstadoTrucos.h"

const int TAM_INPUT         = 19;
const int TIEMPO_ESPERA     = 300;
const int NUM_TITULOS       = 3;
const int NUM_ZONAS         = 3;
const int NUM_JEFES         = 3;
const int NUM_ARMAS         = 12;

//Precios mejoras
const vector<int> PRECIO_VIDA = {10, 20 , 40 , 100, 140, 200};
const vector<int> PRECIO_PMAX = {40, 100, 160, 200, 260};
const vector<int> PRECIO_GANC = {60, 120, 180, 300};
const vector<int> PRECIO_PVEL = {80, 180, 280};

const int ESPERA_INTER        = 300;  

//Colores texto
const vector<int> COLOR_HOVER = {255, 255, 0, 255};
const vector<int> COLOR_SELEC = {179, 144, 20, 255};
using namespace std;

EstadoTrucos* EstadoTrucos::pEstadoTrucos = 0;

EstadoTrucos* EstadoTrucos::Instancia(){
    if(pEstadoTrucos == 0){   //Primera llamada
        pEstadoTrucos = new EstadoTrucos(); //Se crea la instancia de EstadoTrucos
    }
    return pEstadoTrucos;
}

EstadoTrucos::EstadoTrucos(){
    tiempoEspera = ESPERA_INTER;
    texs.push_back(new Textura("resources/sprites/generales/negro.jpg"));
    texs.push_back(new Textura("resources/sprites/personaje/icons.png"));

    //Fondo
    sprites.push_back(new Sprite(texs[0], 0, 0, 10, 10));

    //Contenedor
    sprites.push_back(new Sprite(texs[1], 16*4, 0, 48, 16));
    //Relleno
    sprites.push_back(new Sprite(texs[1], 16*7.4, 16*10, 48, 12));

    //Inicializa los textos
    fuente = new Fuente("resources/sprites/generales/Chonkly.otf");
    textos.push_back(new Texto("Seleccion zona"));
    textos.push_back(new Texto("Seleccion jefe"));
    textos.push_back(new Texto("Seleccion arma"));

    textos.push_back(new Texto("Zona 1"));
    textos.push_back(new Texto("Zona 2"));
    textos.push_back(new Texto("Zona 3"));

    textos.push_back(new Texto("Jefe 1"));
    textos.push_back(new Texto("Jefe 2"));
    textos.push_back(new Texto("Jefe 3"));

    textos.push_back(new Texto("Pistola 1"));
    textos.push_back(new Texto("Pistola 2"));
    textos.push_back(new Texto("Pistola 3"));

    textos.push_back(new Texto("Escopeta 1"));
    textos.push_back(new Texto("Escopeta 2"));
    textos.push_back(new Texto("Escopeta 3"));

    textos.push_back(new Texto("Metralleta 1"));
    textos.push_back(new Texto("Metralleta 2"));
    textos.push_back(new Texto("Metralleta 3"));

    textos.push_back(new Texto("Lanzacohetes 1"));
    textos.push_back(new Texto("Lanzacohetes 2"));
    textos.push_back(new Texto("Lanzacohetes 3"));

    textos.push_back(new Texto("Anyadir esencias"));
    textos.push_back(new Texto("Anyadir materiales"));
    textos.push_back(new Texto("Sala desafio"));
    textos.push_back(new Texto("Vida inf"));
    
    for(unsigned int i = 0; i < textos.size(); i++){
        textos[i]->setFuente(fuente);
        textos[i]->setTamanyo(40);
        textos[i]->setEscala(1,1);
        textos[i]->setColorTexto({255, 255, 255, 255});
    }
    hover = -1;
    seleccionado = -1;
    zonaSeleccionada = -1;
    jefeSeleccionado = -1;
    armaSeleccionada = -1;
    extraSeleccionado = -1;
    vidaInfinita = false;
    
    marco = Marco::Instancia()->getMarco();
}

EstadoTrucos::~EstadoTrucos(){
    while(sprites.size() > 0) {
        if(sprites[sprites.size()-1] != nullptr) {
            delete sprites[sprites.size()-1];
        }
        sprites.pop_back();
    }
    while(texs.size() > 0) {
        if(texs[texs.size()-1] != nullptr) {
            delete texs[texs.size()-1];
        }
        texs.pop_back();
    }
    while(textos.size() > 0) {
        if(textos[textos.size()-1] != nullptr) {
            delete textos[textos.size()-1];
        }
        textos.pop_back();
    }
    delete fuente;
}

void EstadoTrucos::cambiarEstado(int valor){
    if(valor == 1) {
        PilaEstados* pilaEstados = PilaEstados::Instancia();
        
        //Cambia el estado actual al estado partida
        pilaEstados->eliminarUltimoEstado();
        EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
        ePueblo->setArmaSeleccionada(armaSeleccionada-NUM_TITULOS-NUM_ZONAS-NUM_JEFES);
        ePueblo->getJugadorPueblo()->setInteracutando(false);
        for(unsigned int i = 0; i < marco.size(); i++){
            marco[i]->setScale(1, 1);
        }

        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->setMira(false);
    }
}

void EstadoTrucos::dibujar(float fNU, int iNU){
    VentanaRender *ventana = VentanaRender::Instancia();
    vector<unsigned int> tamVentana = ventana->getSize();
    vector<float> centroVista = ventana->getCentroVista();
    float despVistaX          =centroVista[0] - (tamVentana[0]/2);
    float despVistaY          =centroVista[1] - (tamVentana[1]/2);

    sprites[0]->setColor({255, 255, 255, 200});
    sprites[0]->setScale(150, 80);
    sprites[0]->setPosition(despVistaX+200, despVistaY+150);

    //Dibuja el fondo y el marco de la tienda
    sprites[0]->draw();
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->setScale(5, 5);
    }
    marco[0]->setPosition(despVistaX+200, despVistaY+150);//Esq superior izq
    marco[1]->setPosition(despVistaX+200+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+150);//Lado superior
    marco[2]->setPosition(despVistaX+200+sprites[0]->getGlobalPixelSize()[0], despVistaY+150);//Esq superior dere
    marco[3]->setPosition(despVistaX+200+sprites[0]->getGlobalPixelSize()[0], despVistaY+150+sprites[0]->getGlobalPixelSize()[1]/2);//Lado dere
    marco[4]->setPosition(despVistaX+200+sprites[0]->getGlobalPixelSize()[0], despVistaY+150+sprites[0]->getGlobalPixelSize()[1]);//Esq abajo dere
    marco[5]->setPosition(despVistaX+200+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+150+sprites[0]->getGlobalPixelSize()[1]);//Lado abajo
    marco[6]->setPosition(despVistaX+200, despVistaY+150+sprites[0]->getGlobalPixelSize()[1]);//Esq abajo izq
    marco[7]->setPosition(despVistaX+200, despVistaY+150+sprites[0]->getGlobalPixelSize()[1]/2);//Lado izq

    marco[1]->setScale(90, 5.0);
    marco[3]->setScale(5.0, 46.0);
    marco[5]->setScale(90, 5.0);
    marco[7]->setScale(5.0, 46.0);
    for(unsigned int i = 0; i < 8; i++){
        marco[i]->draw();
    }

    //Se dibujan todos los textos
    //Se dibujan los titulos
    for(int i = 0; i < NUM_TITULOS; i++){
        textos[i]->setPosicion(despVistaX+200, despVistaY+200+i*150);
        textos[i]->draw();
    }

    //Se dibujan las zonas
    for(int i = NUM_TITULOS; i < NUM_TITULOS + NUM_ZONAS; i++){
        if(zonaSeleccionada != i){
            textos[i]->setGrosorContorno(0.0);
            textos[i]->setColorContorno({255, 255, 255, 255});
            textos[i]->setColorTexto({255, 255, 255, 255});
        }

        //Se hace el hover
        if(zonaSeleccionada == -1 && hover != -1){
            textos[hover]->setColorTexto(COLOR_HOVER);
        }

        //Se selecciona una zona
        if(zonaSeleccionada != -1){
            textos[zonaSeleccionada]->setGrosorContorno(2.0);
            textos[zonaSeleccionada]->setColorContorno(COLOR_SELEC);
        }
        textos[i]->setPosicion(despVistaX+200+(i-NUM_TITULOS)*200, despVistaY+270);
        textos[i]->draw();
    }

    //Se dibujan los jefes
    for(int i = NUM_TITULOS+NUM_ZONAS; i < NUM_TITULOS+NUM_ZONAS+NUM_JEFES; i++){
        if(jefeSeleccionado != i){
            textos[i]->setGrosorContorno(0.0);
            textos[i]->setColorContorno({255, 255, 255, 255});
            textos[i]->setColorTexto({255, 255, 255, 255});
        }

        //Se hace el hover
        if(jefeSeleccionado == -1 && hover != -1){
            textos[hover]->setColorTexto(COLOR_HOVER);
        }

        //Se selecciona un jefe
        if(jefeSeleccionado != -1){
            textos[jefeSeleccionado]->setGrosorContorno(2.0);
            textos[jefeSeleccionado]->setColorContorno(COLOR_SELEC);
        }
        textos[i]->setPosicion(despVistaX+200+(i-NUM_TITULOS-NUM_ZONAS)*200, despVistaY+430);
        textos[i]->draw();
    }

    //Se dibujan las armas
    int columna = 0;
    int fila    = 0;
    for(int i = NUM_TITULOS+NUM_ZONAS+NUM_JEFES; i < NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS; i++){
        if(armaSeleccionada != i){
            textos[i]->setGrosorContorno(0.0);
            textos[i]->setColorContorno({255, 255, 255, 255});
            textos[i]->setColorTexto({255, 255, 255, 255});
        }

        //Se hace el hover
        if(armaSeleccionada == -1 && hover != -1){
            textos[hover]->setColorTexto(COLOR_HOVER);
        }

        //Se selecciona un arma
        if(armaSeleccionada != -1){
            textos[armaSeleccionada]->setGrosorContorno(2.0);
            textos[armaSeleccionada]->setColorContorno(COLOR_SELEC);
        }
        textos[i]->setPosicion(despVistaX+200+columna*300, despVistaY+600+fila*60);
        textos[i]->draw();
        fila++;
        if(fila == 3){
            columna++;
            fila = 0;
        }
    }

    //Extras
    for(int i = NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS; i < NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS+4; i++){
        if(extraSeleccionado != i){
            textos[i]->setGrosorContorno(0.0);
            textos[i]->setColorContorno({255, 255, 255, 255});
            textos[i]->setColorTexto({255, 255, 255, 255});
        }
        //Se hace el hover
        if(hover != -1){
            textos[hover]->setColorTexto(COLOR_HOVER);
        }
        textos[i]->setPosicion(despVistaX+200+(i-NUM_TITULOS-NUM_ZONAS-NUM_JEFES-NUM_ARMAS)*420, despVistaY+860);
        textos[i]->draw();
    }
}

void EstadoTrucos::update(Tiempo tNU, bool input[TAM_INPUT]){
    
    VentanaRender* ventana = VentanaRender::Instancia();
    if(!ventana->getFade()) {
        vector<float> pos = ventana->mapPixelToCoords(ventana->getPosicionRaton());
        
        bool botonMejorar = false;
        //Si se pasa el raton por un texto
        if(hoverRaton(pos[0], pos[1], &botonMejorar)){
            
            //Selecciona una casilla
            if(input[16] && tiempoEspera <= 0){
                for(unsigned int i = NUM_TITULOS; i < textos.size(); i++){

                    //Se comprueba si las coordenadas del raton pasan por algun sprite, si es asi se guarda en hover
                    if(textos[i]->getContains(pos[0], pos[1])){
                        if(i < NUM_TITULOS+NUM_ZONAS){
                            zonaSeleccionada = i;
                            jefeSeleccionado = -1;
                            extraSeleccionado = -1;
                        }
                        if(i >= NUM_TITULOS+NUM_ZONAS && i < NUM_TITULOS+NUM_ZONAS+NUM_JEFES){
                            jefeSeleccionado = i;
                            zonaSeleccionada = -1;
                            extraSeleccionado = -1;
                        }
                        if(i >= NUM_TITULOS+NUM_ZONAS+NUM_JEFES && i < NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS){
                            armaSeleccionada = i;
                        }

                        //Truco esencias
                        if(i == NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS){
                            Guardado *guar = Guardado::Instancia();
                            guar->setEsencias(999999);
                            guar->guardarPartida();
                        }

                        //Truco anyadir materiales
                        if(i == NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS+1){
                            EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
                            
                            for(int i = 0; i < 13; i++){
                                if(ePueblo->getJugador()->getInventario()->cantidadMateriales(i) == 0){
                                    ePueblo->getJugador()->getInventario()->incrementaMaterial(i, 99);
                                }
                            }
                           
                        }

                        //Truco sala de desafio
                        if(i == NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS+2){
                            jefeSeleccionado = -1;
                            zonaSeleccionada = -1;
                            extraSeleccionado = 0;
                           
                        }

                        //Truco invulnerabilidad
                        if(i == NUM_TITULOS+NUM_ZONAS+NUM_JEFES+NUM_ARMAS+3){
                            vidaInfinita = true;
                        }
                    }
                }
                tiempoEspera = ESPERA_INTER;
            }
        }
        

        if((input[9] || input[13]) && tiempoEspera <= 0){
            cambiarEstado(1);
            tiempoEspera = ESPERA_INTER;
        }

        if(tiempoEspera > 0) {
            tiempoEspera -= tNU.milisegundos();
        }
    }
}

//Comprueba que sprite se encuentra en las coordenadas pasadas
bool EstadoTrucos::hoverRaton(float x, float y, bool* boton){
    bool pasa = false;
    for(unsigned int i = 3; i < textos.size(); i++){

        //Se comprueba si las coordenadas del raton pasan por algun sprite, si es asi se guarda en hover
        if(textos[i]->getContains(x, y)){
            hover = i;
            pasa = true;
            if(i == 4){
                *boton = true;
            }
        }
        
        if(!pasa){
            hover = -1;
        }
    }
    return pasa;
}

//Reinicia el tiempo de cierre de la interfaz
void EstadoTrucos::resetTiempoEspera() {
    tiempoEspera = TIEMPO_ESPERA;
}

void EstadoTrucos::resetearTrucos(){
    EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
    zonaSeleccionada = -1;
    armaSeleccionada = -1;
    jefeSeleccionado = -1;
    extraSeleccionado = -1;
    vidaInfinita = false;
    ePueblo->setArmaSeleccionada(armaSeleccionada);
    for(int i = 0; i < 13; i++){
        ePueblo->getJugador()->getInventario()->incrementaMaterial(i, 0);
    }
}

void EstadoTrucos::resetJefe(){
    jefeSeleccionado = -1;
}

//Metodo que devuelve la zona seleccionada en la interfaz
int EstadoTrucos::getZonaSeleccionada(){
    return zonaSeleccionada;
}

//Metodo que devuelve el jefe seleccionado en la interfaz
int EstadoTrucos::getJefeSeleccionado(){
    return jefeSeleccionado;
}

int EstadoTrucos::getExtraSeleccionado(){
    return extraSeleccionado;
}

bool EstadoTrucos::getVidaInf(){
    return vidaInfinita;
}