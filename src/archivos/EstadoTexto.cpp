#include "EstadoTexto.h"

const int TAM_INPUT         = 19;
const int FREC_CARACTER     = 200;
const float TAM_CARACTER    = 16.0;
const float TAM_LINEA       = 20.0;

const float DESP_X_TECLA    = -80.0;
const float DESP_Y_TECLA    = -80.0;

//Rutas
const string RUTA_TECLA     = "resources/sprites/menu/controles.png";
const string RUTA_FUENTE    = "resources/sprites/generales/Chonkly.otf";

using namespace std;

EstadoTexto* EstadoTexto::pEstadoTexto = 0;

EstadoTexto* EstadoTexto::Instancia(){
    if(pEstadoTexto == 0){   //Primera llamada
        pEstadoTexto = new EstadoTexto(); //Se crea la instancia de EstadoTexto
    }
    return pEstadoTexto;
}

//Constructor
EstadoTexto::EstadoTexto(){
    EstadoPartida* ePartida     = EstadoPartida::Instancia(); 
    VentanaRender* ventana      = VentanaRender::Instancia();
    vector<float> tamVentana    = ventana->getSizeVista();
    vector<float> centroVista   = ePartida->getVista()->getCentro();
                    
    //Carga la textura de las teclas y crea su sprite
    texTecla    = new Textura(RUTA_TECLA);
    spriteTecla = new Sprite(texTecla, 36, 22, 9, 10);
    spriteTecla->setOrigin(4.0, 5.0);
    spriteTecla->setScale(6.0, 6.0);

    //Crea el array para el fondo
    fondo       = new ArrayVertices(4, 4);

    //Modifica la posicion de los puntos del fondo
    fondo->setVertexPosition(0, centroVista[0] - tamVentana[0]/2, centroVista[1] - tamVentana[1]/2);
    fondo->setVertexPosition(1, centroVista[0] + tamVentana[0]/2, centroVista[1] - tamVentana[1]/2);
    fondo->setVertexPosition(2, centroVista[0] + tamVentana[0]/2, centroVista[1] + tamVentana[1]/2);
    fondo->setVertexPosition(3, centroVista[0] - tamVentana[0]/2, centroVista[1] + tamVentana[1]/2);
    fondo->setArrayColor(0, 0, 0, 255);

    //Carga la fuente y el texto
    fuente      = new Fuente(RUTA_FUENTE);
    texto       = new Texto("");
    texto->setFuente(fuente);
    texto->setColorTexto({255, 255, 255, 255});
    texto->setTamanyo(24);

    //Inicializa el resto de variables
    vStrings.clear();
    stringActual        = 0;
    stringTotales       = 0;
    caracteresMostrados = 0;
    stringMostrado      = false;
    teclaMantenida      = false;
}

//Destructor
EstadoTexto::~EstadoTexto(){
    //Vacia el vector de strings
    vStrings.clear();

    //Elimina los punteros
    if(texto != nullptr) {
        delete texto;
        texto = nullptr;
    }
    if(fuente != nullptr) {
        delete fuente;
        fuente = nullptr;
    }
    if(spriteTecla != nullptr) {
        delete spriteTecla;
        spriteTecla = nullptr;
    }
    if(texTecla != nullptr) {
        delete texTecla;
        texTecla = nullptr;
    }
    if(fondo != nullptr) {
        delete fondo;
        fondo = nullptr;
    }
}

void EstadoTexto::cambiarEstado(int valor){
    if(valor == 1) {
        PilaEstados* pilaEstados = PilaEstados::Instancia();
        
        //Elimina el EstadoTexto de la pila
        pilaEstados->eliminarUltimoEstado();
        
        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->fadeOut(10);
    }
}

void EstadoTexto::dibujar(float fNU, int iNU){
    //Dibuja el fondo
    if(fondo != nullptr) {
        fondo->draw();
    }

    //Dibuja el texto
    if(texto != nullptr) {
        texto->draw();
    }

    //Si el texto se muestra completamente, dibuja la tecla de avance
    if(stringMostrado && spriteTecla != nullptr) {
        spriteTecla->draw();
    }
}

void EstadoTexto::update(Tiempo tNU, bool input[TAM_INPUT]){
    EstadoPartida *ePartido = EstadoPartida::Instancia();
    VentanaRender* ventana = VentanaRender::Instancia();
    if(!ventana->getFade()) {
        if(ventana->getPantallaNegro()) {
            if(stringActual >= stringTotales - 1) {
                //Cambia de estado cuando ha mostrado todos los textos
                cambiarEstado(1);
            }
            else {
                ventana->fadeOut(50);
            }
        }
        else {
            //Update normal, recibe los inputs y actualiza el texto
            if(input[13] && !teclaMantenida) {
                //Se ha pulsado el boton de avanzar
                teclaMantenida = true;

                //Si ha terminado de mostrar el string, comprueba que hacer
                if(stringMostrado) {
                    //Comprueba si queda algun string por mostrar
                    if(stringActual < stringTotales - 1) {
                        //Resetea el texto actual
                        texto->setString("");
                        stringMostrado = false;
                        caracteresMostrados = 0;

                        //Actualiza el valor del string actual
                        stringActual++;

                        //Cuenta cuantas lineas hay en el nuevo string
                        int lineas = 1;
                        int pos = 0;
                        while(pos < (int)vStrings[stringActual].length() && vStrings[stringActual].find("\n", pos) != string::npos) {
                            lineas ++;
                            pos = vStrings[stringActual].find("\n", pos) + 1;
                        }

                        //Calcula la nueva posicion del texto aproximando el tamanyo
                        vector<float> centroVista   = ePartido->getVista()->getCentro();
                        int tamX                    = vStrings[stringActual].length() / (lineas*2);
                        texto->setPosicion(centroVista[0] - tamX*TAM_CARACTER, centroVista[1] - (lineas/2)*TAM_LINEA);
                    }
                    else {
                        //Hace fade para cambiar de estado
                        ventana->fadeIn(25);
                    }
                }
                else {
                    //Si todavia no se habia mostrado todo el string, lo muestra completo
                    if(stringActual < stringTotales) {
                        caracteresMostrados = vStrings[stringActual].length();
                        texto->setString(vStrings[stringActual].substr(0, caracteresMostrados));
                        stringMostrado = true;
                    }
                }
            }
            else {
                if(!input[13]) {
                    //Se indica que no se esta pulsando ninguna tecla
                    teclaMantenida = false;
                }
                //Cuenta cuantas lineas hay en el nuevo string
                int lineas = 1;
                int pos = 0;
                while(pos < (int)vStrings[stringActual].length() && vStrings[stringActual].find("\n", pos) != string::npos) {
                    lineas ++;
                    pos = vStrings[stringActual].find("\n", pos) + 1;
                }
                //Calcula la nueva posicion del texto aproximando el tamanyo
                VentanaRender* ventana      = VentanaRender::Instancia();
                vector<float> centroVista   = ePartido->getVista()->getCentro();
                int tamX                    = vStrings[stringActual].length() / (lineas*2);
                texto->setPosicion(centroVista[0] - tamX*TAM_CARACTER, centroVista[1] - (lineas/2)*TAM_LINEA);

                //Si no se ha terminado de mostrar el string se actualiza
                if(!stringMostrado) {
                    vector<float> tamVentana    = ventana->getSizeVista();
                    vector<float> centroVista   = ePartido->getVista()->getCentro();

                    //Modifica la posicion de los puntos del fondo
                    fondo->setVertexPosition(0, centroVista[0] - tamVentana[0], centroVista[1] - tamVentana[1]);
                    fondo->setVertexPosition(1, centroVista[0] + tamVentana[0], centroVista[1] - tamVentana[1]);
                    fondo->setVertexPosition(2, centroVista[0] + tamVentana[0], centroVista[1] + tamVentana[1]);
                    fondo->setVertexPosition(3, centroVista[0] - tamVentana[0], centroVista[1] + tamVentana[1]);

                    //Actualiza los caracteres a mostrar
                    caracteresMostrados += max(1, tNU.milisegundos()/FREC_CARACTER);
                    caracteresMostrados = min(caracteresMostrados, (int)vStrings[stringActual].length());

                    //Actualiza el texto
                    texto->setString(vStrings[stringActual].substr(0, caracteresMostrados));

                    //Comprueba si se ha mostrado todo el texto
                    if(caracteresMostrados == (int)vStrings[stringActual].length()) {
                        stringMostrado = true;
                    }

                    //Actualiza el sprite de la tecla de avance
                    spriteTecla->setPosition(centroVista[0] + tamVentana[0]/2.0 + DESP_X_TECLA, centroVista[1] + tamVentana[1]/2.0 + DESP_Y_TECLA);
                }
            }
        }
    }
}

void EstadoTexto::inicializar(int valor) {
    //Reinicia las variables
    texto->setString("");
    stringActual        = 0;
    stringTotales       = 0;
    caracteresMostrados = 0;
    stringMostrado      = false;
    teclaMantenida      = false;
    vStrings.clear();

    //Inicia los nuevos valores a mostrar
    switch (valor) {
        case 1:
            vStrings    =   {"Eres Shiv",
                            "Un muchacho que ha muerto y ha aparecido en 'El Otro Lado'\n el lugar donde los muertos pasan la eternidad.",
                            "Tu objetivo es salir de aqui, y cuentas con la ayuda de un dios del tiempo.",
                            "Kairos, que tambien esta atrapado, te prestara sus poderes\npara llegar al final del 'Pasaje de los guardianes' y volver a la vida."};

            stringTotales = 4;
            break;
        case 2:
            vStrings    =   {"Has desbloqueado el gancho.",
                            "Si pulsas click derecho se lanza en la direccion en la que estas apuntando.\nSi choca contra una pared te acercas a ella."};

            stringTotales = 2;
            break;
        case 3:
            vStrings    =   {"Has desbloqueado el martillo.",
                            "Pulsa la F para asestar un golpe en la direccion en la que estas apuntando.\nPuede romper muros."};

            stringTotales = 2;
            break;
        case 4:
            vStrings    =   {"Has desbloqueado la linterna.",
                            "Esta te permite atravesar los muros oscuros y los espiritus ya no te ralentizan."};

            stringTotales = 2;
            break;
        case 5:
            vStrings    =   {"Shiv y Kairos llegan a la Frontera de las Almas y ...",
                            "para saber que le depara a nuestros heroes,\ntendras que esperar a la siguiente actualizacion",
                            "Gracias por jugar a Those Who Remain Here.",
                            "Mientras tanto puedes seguir jugando y explorando el Pasaje de los Guardianes.",
                            "Hasta pronto."};

            stringTotales = 5;
            break;
    }

    if(stringTotales > 0) {
        //Cuenta cuantas lineas hay en el nuevo string
        int lineas = 1;
        int pos = 0;
        while(pos < (int)vStrings[stringActual].length() && vStrings[stringActual].find("\n", pos) != string::npos) {
            lineas ++;
            pos = vStrings[stringActual].find("\n", pos) + 1;
        }

        //Calcula la nueva posicion del texto aproximando el tamanyo
        VentanaRender* ventana      = VentanaRender::Instancia();
        EstadoPartida* ePartida     = EstadoPartida::Instancia();
        vector<float> centroVista   = ePartida->getVista()->getCentro();
        int tamX                    = vStrings[stringActual].length() / (lineas*2);
        texto->setPosicion(centroVista[0] - tamX*TAM_CARACTER, centroVista[1] - (lineas/2)*TAM_LINEA);


        vector<float> tamVentana    = ventana->getSizeVista();
        //Modifica la posicion de los puntos del fondo
        fondo->setVertexPosition(0, centroVista[0] - tamVentana[0], centroVista[1] - tamVentana[1]);
        fondo->setVertexPosition(1, centroVista[0] + tamVentana[0], centroVista[1] - tamVentana[1]);
        fondo->setVertexPosition(2, centroVista[0] + tamVentana[0], centroVista[1] + tamVentana[1]);
        fondo->setVertexPosition(3, centroVista[0] - tamVentana[0], centroVista[1] + tamVentana[1]);
    }
}