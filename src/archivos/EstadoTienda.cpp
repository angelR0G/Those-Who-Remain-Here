#include "EstadoTienda.h"

const int TAM_INPUT         = 19;
const int TILE_MARCO        = 16; //Tamanyo para el marco
const int TIEMPO_ESPERA     = 300;
const int NUM_MEJORAS       = 4;

//Cantidad de mejoras disponibles
const int CANT_MEJORAS_SUMA = 18;
const int CANT_MEJORAS_VIDA = 6;
const int CANT_MEJORAS_PMAX = 5;
const int CANT_MEJORAS_PVEL = 3;
const int CANT_MEJORAS_GANC = 4;

//Precios mejoras
const vector<int> PRECIO_VIDA = {4000, 8000, 14000, 20000, 30000, 50000};
const vector<int> PRECIO_PMAX = {2000, 6000, 10000, 15000, 20000};
const vector<int> PRECIO_GANC = {2000, 5000, 8000, 15000};
const vector<int> PRECIO_PVEL = {5000, 10000, 15000};

const int ESPERA_INTER        = 300;  

//Colores texto
const vector<int> COLOR_HOVER = {255, 255, 0, 255};
const vector<int> COLOR_SELEC = {179, 144, 20, 255};
using namespace std;

EstadoTienda* EstadoTienda::pEstadoTienda = 0;

EstadoTienda* EstadoTienda::Instancia(){
    if(pEstadoTienda == 0){   //Primera llamada
        pEstadoTienda = new EstadoTienda(); //Se crea la instancia de EstadoTienda
    }
    return pEstadoTienda;
}

EstadoTienda::EstadoTienda(){
    mejoras  = {-1, -1, -1, -1};
    esencias = -1;
    siguienteMejora = 0;
    tiempoEspera = ESPERA_INTER;
    texs.push_back(new Textura("resources/sprites/generales/negro.jpg"));
    texs.push_back(new Textura("resources/sprites/personaje/icons.png"));

    //Fondo
    sprites.push_back(new Sprite(texs[0], 0, 0, 10, 10));

    //Contenedor
    sprites.push_back(new Sprite(texs[1], 16*4, 0, 48, 16));
    //Relleno
    sprites.push_back(new Sprite(texs[1], 16*7.4, 16*10, 48, 12));

    //Recupera las mejoras adquiridas
    cargarMejoras();

    //Inicializa los textos
    fuente = new Fuente("resources/sprites/generales/Chonkly.otf");
    textos.push_back(new Texto("Salud"));
    textos.push_back(new Texto("Control Tiempo Max"));
    textos.push_back(new Texto("Gancho"));
    textos.push_back(new Texto("Control Tiempo"));
    textos.push_back(new Texto(""));
    textos.push_back(new Texto(""));
    textos.push_back(new Texto("Esencias: "));
    textos.push_back(new Texto(""));
    for(unsigned int i = 0; i < textos.size(); i++){
        textos[i]->setFuente(fuente);
        textos[i]->setTamanyo(40);
        textos[i]->setEscala(1,1);
        textos[i]->setColorTexto({255, 255, 255, 255});
    }
    hover = -1;
    seleccionado = -1;
    
    //Para los marcos
    texMarco = new Textura("resources/sprites/generales/bordes.png");
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 2*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 10*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 5*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 13*TILE_MARCO, TILE_MARCO, TILE_MARCO));

    //Para el boton de mejora
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 9*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 6*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    marco.push_back(new Sprite(texMarco, 1*TILE_MARCO, 14*TILE_MARCO, TILE_MARCO, TILE_MARCO));
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->setOrigin(TILE_MARCO/2, TILE_MARCO/2);
    }
}

EstadoTienda::~EstadoTienda(){
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

    while(marco.size() > 0) {
        if(marco[marco.size()-1] != nullptr) {
            delete marco[marco.size()-1];
        }
        marco.pop_back();
    }
    delete texMarco;
    mejoras.clear();
}

void EstadoTienda::cambiarEstado(int valor){
    if(valor == 1) {
        PilaEstados* pilaEstados = PilaEstados::Instancia();
        
        //Cambia el estado actual al estado partida
        pilaEstados->eliminarUltimoEstado();
        EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
        ePueblo->getJugadorPueblo()->setInteracutando(false);

        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->setMira(false);
    }
}

void EstadoTienda::dibujar(float fNU, int iNU){
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
    for(unsigned int i = 0; i < CANT_MEJORAS_SUMA; i++){

        //Contenedores
        if(i<CANT_MEJORAS_VIDA){
            sprites[1]->setScale(2, 4);
            sprites[1]->setPosition(despVistaX+50+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+250+0*60);
            sprites[1]->draw();
            //Barras
            if(i<(unsigned int)mejoras[0]){
                sprites[2]->setScale(2, 4);
                sprites[2]->setPosition(despVistaX+48+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+242+0*60);
                sprites[2]->draw();
            }
        }

        //Contenedores
        if(i<CANT_MEJORAS_PMAX){
            sprites[1]->setScale(2, 4);
            sprites[1]->setPosition(despVistaX+50+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+250+1*60);
            sprites[1]->draw();
            //Barras
            if(i<(unsigned int)mejoras[1]){
                sprites[2]->setScale(2, 4);
                sprites[2]->setPosition(despVistaX+48+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+242+1*60);
                sprites[2]->draw();
            }
        }

        //Contenedores
        if(i<CANT_MEJORAS_GANC){
            sprites[1]->setScale(2, 4);
            sprites[1]->setPosition(despVistaX+50+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+250+2*60);
            sprites[1]->draw();
            //Barras
            if(i<(unsigned int)mejoras[2]){
                sprites[2]->setScale(2, 4);
                sprites[2]->setPosition(despVistaX+48+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+242+2*60);
                sprites[2]->draw();
            }
        }

        //Contenedores
        if(i<CANT_MEJORAS_PVEL){
            sprites[1]->setScale(2, 4);
            sprites[1]->setPosition(despVistaX+50+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+250+3*60);
            sprites[1]->draw();
            //Barras
            if(i<(unsigned int)mejoras[3]){
                sprites[2]->setScale(2, 4);
                sprites[2]->setPosition(despVistaX+48+i*120+sprites[0]->getGlobalPixelSize()[0]/2, despVistaY+242+3*60);
                sprites[2]->draw();
            }
        }
    }

    for(unsigned int i = 0; i <= NUM_MEJORAS; i++){
        //Resetea el color de los textos
        if((unsigned int)seleccionado != i){
            textos[i]->setGrosorContorno(0.0);
            textos[i]->setColorContorno({255, 255, 255, 255});
            textos[i]->setColorTexto({255, 255, 255, 255});
        }
        //Hace el hover
        if(seleccionado == -1 && hover != -1){
            textos[hover]->setColorTexto(COLOR_HOVER);
        }
        //Hay una mejora seleccionada
        if(seleccionado != -1){
            textos[seleccionado]->setGrosorContorno(2.0);
            textos[seleccionado]->setColorContorno(COLOR_SELEC);
            mostrarMejora();
        }
        if(i<NUM_MEJORAS){
            textos[i]->setPosicion(despVistaX+250, despVistaY+250+i*60);
            textos[i]->draw();
        }
    }
    textos[6]->setString("Esencias: " + to_string(esencias));
    textos[6]->setPosicion(despVistaX+(sprites[0]->getGlobalPixelSize()[0]/2), despVistaY+180);
    textos[6]->draw();
}

void EstadoTienda::cargarMejoras(){
    Guardado *guar = Guardado::Instancia();

    esencias   = guar->getEsencias();
    mejoras[0] = guar->getMejoraSalud();
    mejoras[1] = guar->getMejoraPoderMax();
    mejoras[2] = guar->getMejoraDistGancho();
    mejoras[3] = guar->getMejoraPoderVel();

}

void EstadoTienda::mostrarMejora(){
    VentanaRender *ventana = VentanaRender::Instancia();
    vector<unsigned int> tamVentana = ventana->getSize();
    vector<float> centroVista = ventana->getCentroVista();
    float despVistaX          =centroVista[0] - (tamVentana[0]/2);
    float despVistaY          =centroVista[1] - (tamVentana[1]/2);
    switch (seleccionado)
    {
        case 0:
            textos[5]->setString("Mejora de forma permanente la vida maxima +1");
            break;
        case 1:
            textos[5]->setString("Mejora de forma permanente la capacidad maxima de la habilidad");
            break;
        case 2:
            textos[5]->setString("Mejora de forma permanente el alcance del gancho");
            break;
        case 3:
            textos[5]->setString("Reduce de forma permanente el enfriamiento del control del tiempo");
            break;
        default:
            break;
    }
    vector<float> pos = ventana->mapPixelToCoords(ventana->getPosicionRaton());
    if(textos[4]->getContains(pos[0], pos[1])){
        textos[4]->setColorTexto(COLOR_HOVER);
        marco[8]->setColor(COLOR_HOVER);
        marco[9]->setColor(COLOR_HOVER);
        marco[10]->setColor(COLOR_HOVER);
        marco[11]->setColor(COLOR_HOVER);
    }else{
        textos[4]->setColorTexto({255, 255, 255, 255});
        marco[8]->setColor({255, 255, 255, 255});
        marco[9]->setColor({255, 255, 255, 255});
        marco[10]->setColor({255, 255, 255, 255});
        marco[11]->setColor({255, 255, 255, 255});
    }

    switch (seleccionado)
    {
        case 0:
            textos[7]->setString(to_string(PRECIO_VIDA[siguienteMejora-1]));     
            if(PRECIO_VIDA[siguienteMejora-1]>esencias){
                textos[7]->setColorTexto({255, 0, 0, 255});
            }else{
                textos[7]->setColorTexto({255, 255, 255, 255});
            } 
            break;
        case 1:
            textos[7]->setString(to_string(PRECIO_PMAX[siguienteMejora-1]));
            if(PRECIO_PMAX[siguienteMejora-1]>esencias){
                textos[7]->setColorTexto({255, 0, 0, 255});
            }else{
                textos[7]->setColorTexto({255, 255, 255, 255});
            } 
            break;
        case 2:
            textos[7]->setString(to_string(PRECIO_GANC[siguienteMejora-1]));
            if(PRECIO_GANC[siguienteMejora-1]>esencias){
                textos[7]->setColorTexto({255, 0, 0, 255});
            }else{
                textos[7]->setColorTexto({255, 255, 255, 255});
            } 
            break;
        case 3:
            textos[7]->setString(to_string(PRECIO_PVEL[siguienteMejora-1]));
            if(PRECIO_PVEL[siguienteMejora-1]>esencias){
                textos[7]->setColorTexto({255, 0, 0, 255});
            }else{
                textos[7]->setColorTexto({255, 255, 255, 255});
            } 
            break;
        
        default:
            break;
    }
    textos[7]->setPosicion(despVistaX+1250, despVistaY+250+10*60);
    textos[4]->setString("Mejorar");
    textos[5]->setPosicion(despVistaX+250, despVistaY+250+8*60);
    textos[4]->setPosicion(despVistaX+1450, despVistaY+250+10*60);
    textos[4]->draw();
    textos[5]->draw();
    textos[7]->draw();


    //Marco boton de mejora
    marco[8]->setScale(2,2);
    marco[9]->setScale(2,2);
    marco[10]->setScale(2,2);
    marco[11]->setScale(2,2);

    marco[8]->setPosition(despVistaX+1450, despVistaY+250+10*60);//Esq superior izq
    marco[9]->setPosition(despVistaX+1450+textos[4]->getAncho(), despVistaY+250+10*60);//Esq superior dere
    marco[10]->setPosition(despVistaX+1450+textos[4]->getAncho(), despVistaY+260+textos[4]->getAlto()+10*60);//Esq abajo dere
    marco[11]->setPosition(despVistaX+1450, despVistaY+260+textos[4]->getAlto()+10*60);//Esq abajo izq
    marco[8]->draw();
    marco[9]->draw();
    marco[10]->draw();
    marco[11]->draw();
}


void EstadoTienda::update(Tiempo tNU, bool input[TAM_INPUT]){
    
    VentanaRender* ventana = VentanaRender::Instancia();
    if(!ventana->getFade()) {
        vector<float> pos = ventana->mapPixelToCoords(ventana->getPosicionRaton());
        
        bool botonMejorar = false;
        //Si se pasa el raton por un texto
        if(hoverRaton(pos[0], pos[1], &botonMejorar)){
            
            //Selecciona una casilla
            if(input[16] && tiempoEspera <= 0){
                if(!botonMejorar){
                    if(seleccionado == -1){
                        if(hover != -1){
                            seleccionado = hover;
                            siguienteMejora = mejoras[seleccionado]+1;
                        }else{
                            seleccionado = -1;
                        }
                    }else{
                        seleccionado = -1;
                    }
                }else{
                    mejorar(pos[0], pos[1]);
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
bool EstadoTienda::hoverRaton(float x, float y, bool* boton){
    bool pasa = false;
    for(unsigned int i = 0; i < textos.size(); i++){

        //Se comprueba si las coordenadas del raton pasan por algun sprite, si es asi se guarda en hover
        if(textos[i]->getContains(x, y) && i <=NUM_MEJORAS){
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

//Reinicia el tiempo de cierre del inventario
void EstadoTienda::resetTiempoEspera() {
    tiempoEspera = TIEMPO_ESPERA;
}

bool EstadoTienda::mejorar(float x, float y){
    Guardado *guar = Guardado::Instancia();

    int esencias = guar->getEsencias();
    bool mejorado = false;
    //Se pulsa mejorar
    if(textos[4]->getContains(x, y)){
        switch (seleccionado)
        {
            case 0:
                if(guar->getMejoraSalud()+1 <= CANT_MEJORAS_VIDA){
                    if(PRECIO_VIDA[siguienteMejora-1] <= esencias){
                        guar->setMejoraSalud(guar->getMejoraSalud()+1);
                        guar->setEsencias(esencias-PRECIO_VIDA[siguienteMejora-1]);
                        guar->guardarPartida();
                    }
                }
                break;
            case 1:
                if(guar->getMejoraPoderMax()+1 <= CANT_MEJORAS_PMAX){
                    if(PRECIO_PMAX[siguienteMejora-1] <= esencias){
                        guar->setMejoraPoderMax(guar->getMejoraPoderMax()+1);
                        guar->setEsencias(esencias-PRECIO_PMAX[siguienteMejora-1]);
                        guar->guardarPartida();
                    }
                }
                break;
            
            case 2:
                if(guar->getGancho() && guar->getMejoraDistGancho()+1 <= CANT_MEJORAS_GANC){
                    if(PRECIO_GANC[siguienteMejora-1] <= esencias){
                        guar->setMejoraDistGancho(guar->getMejoraDistGancho()+1);
                        guar->setEsencias(esencias-PRECIO_GANC[siguienteMejora-1]);
                        guar->guardarPartida();
                    }
                }
                break;
            case 3:
                if(guar->getMejoraPoderVel()+1 <= CANT_MEJORAS_PVEL){
                    if(PRECIO_PVEL[siguienteMejora-1] <= esencias){
                        guar->setMejoraPoderVel(guar->getMejoraPoderVel()+1);
                        guar->setEsencias(esencias-PRECIO_PVEL[siguienteMejora-1]);
                        guar->guardarPartida();
                    }
                }
                break;
            
            default:
                break;
        }
    }
    cargarMejoras();
    return mejorado;
}