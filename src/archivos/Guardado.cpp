#include "Guardado.h"

const string RUTA_GUARDADO      = "guardado/partida1.xml";
//Nombre de los campos en el archivo de guardado
const string NOM_SALUD          = "m_salud";
const string NOM_PODERMAX       = "m_poderMax";
const string NOM_PODERVEL       = "m_poderVel";
const string NOM_DISTGANCHO     = "m_distGancho";
const string NOM_ESENCIAS       = "esencias";
const string NOM_GANCHO         = "p_gancho";
const string NOM_MAZA           = "p_maza";
const string NOM_LINTERNA       = "p_linterna";
const string NOM_TUTORIAL       = "tutorial";

Guardado* Guardado::pGuardado = 0;

//Recupera la instancia de guardado
Guardado* Guardado::Instancia(){
    if(pGuardado == 0) {
        //Si aun no se ha creado, crea la instancia de guardado
        pGuardado = new Guardado();
    }

    return pGuardado;
}

//Constructor
Guardado::Guardado() {
    //Inicia todas las variables con sus valores por defecto
    esencias            = 0;
    mejoraSalud         = 0;
    mejoraPoderMax      = 0;
    mejoraPoderVel      = 0;
    mejoraDistGancho    = 0;
    gancho              = false;
    maza                = false;
    linterna            = false;
    tutorial            = false;
}

//Destructor
Guardado::~Guardado() {}

//Carga los datos guardados en un archivo
bool Guardado::cargarPartida() {
    bool partidaCargada = false;

    //Reinicia los valores por defecto por si no encuentra el archivo de guardado
    esencias            = 0;
    mejoraSalud         = 0;
    mejoraPoderMax      = 0;
    mejoraPoderVel      = 0;
    mejoraDistGancho    = 0;
    gancho              = false;
    maza                = false;
    linterna            = false;
    tutorial            = false;

    //Crea el lector para leer el archivo XML
    LectorXML lector;
    if(lector.abrirArchivo(RUTA_GUARDADO)) {
        partidaCargada = true;
        //Si encuentra el archivo recorre los diferentes valores que almacenar
        if(lector.siguienteHijo("dato")) {
            do {
                string campo = lector.getAttribute("nombre");

                if(campo.compare(NOM_SALUD) == 0) {
                    mejoraSalud = stoi(lector.getAttribute("valor"));
                }
                else if(campo.compare(NOM_ESENCIAS) == 0) {
                    esencias = stoi(lector.getAttribute("valor"));
                }
                else if(campo.compare(NOM_PODERMAX) == 0) {
                    mejoraPoderMax = stoi(lector.getAttribute("valor"));
                }
                else if(campo.compare(NOM_PODERVEL) == 0) {
                    mejoraPoderVel = stoi(lector.getAttribute("valor"));
                }
                else if(campo.compare(NOM_DISTGANCHO) == 0) {
                    mejoraDistGancho = stoi(lector.getAttribute("valor"));
                }
                else if(campo.compare(NOM_GANCHO) == 0) {
                    if(lector.getAttribute("valor").compare("1") == 0) {
                        gancho = true;
                    }
                }
                else if(campo.compare(NOM_LINTERNA) == 0) {
                    if(lector.getAttribute("valor").compare("1") == 0) {
                        linterna = true;
                    }
                }
                else if(campo.compare(NOM_MAZA) == 0) {
                    if(lector.getAttribute("valor").compare("1") == 0) {
                        maza = true;
                    }
                }
                else if(campo.compare(NOM_TUTORIAL) == 0) {
                    if(lector.getAttribute("valor").compare("1") == 0) {
                        tutorial = true;
                    }
                }

            }while(lector.siguienteHermano("dato"));
        }
    }

    return partidaCargada;
}

//Guarda los valores almacenados actualmente en un fichero de guardado
bool Guardado::guardarPartida() {
    bool partidaGuardada = false;

    //Crea un escritor de XML para generar el archivo de guardado
    EscritorXML escritor(RUTA_GUARDADO, "guardado");

    //Guarda las esencias
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_ESENCIAS);
    escritor.anyadirAtributo("valor", to_string(esencias));
    escritor.anyadirElementoADocumento();

    //Guarda las mejoras de salud
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_SALUD);
    escritor.anyadirAtributo("valor", to_string(mejoraSalud));
    escritor.anyadirElementoADocumento();

    //Guarda las mejoras de poder maximo
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_PODERMAX);
    escritor.anyadirAtributo("valor", to_string(mejoraPoderMax));
    escritor.anyadirElementoADocumento();

    //Guarda las mejoras de velocidad de recarga del poder
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_PODERVEL);
    escritor.anyadirAtributo("valor", to_string(mejoraPoderVel));
    escritor.anyadirElementoADocumento();

    //Guarda las mejoras de velocidad de recarga del poder
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_DISTGANCHO);
    escritor.anyadirAtributo("valor", to_string(mejoraDistGancho));
    escritor.anyadirElementoADocumento();

    //Guarda si tiene la habilidad del gancho
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_GANCHO);
    if(gancho)
        escritor.anyadirAtributo("valor", "1");
    else
        escritor.anyadirAtributo("valor", "0");
    escritor.anyadirElementoADocumento();

    //Guarda si tiene la habilidad de la maza
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_MAZA);
    if(maza)
        escritor.anyadirAtributo("valor", "1");
    else
        escritor.anyadirAtributo("valor", "0");
    escritor.anyadirElementoADocumento();

    //Guarda si tiene la habilidad de la linterna
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_LINTERNA);
    if(linterna)
        escritor.anyadirAtributo("valor", "1");
    else
        escritor.anyadirAtributo("valor", "0");
    escritor.anyadirElementoADocumento();

    //Guarda si se ha terminado el tutorial
    escritor.nuevoElemento("dato");
    escritor.anyadirAtributo("nombre", NOM_TUTORIAL);
    if(tutorial)
        escritor.anyadirAtributo("valor", "1");
    else
        escritor.anyadirAtributo("valor", "0");
    escritor.anyadirElementoADocumento();

    //Guarda el fichero
    partidaGuardada = escritor.guardarArchivo();

    return partidaGuardada;
}

//Recupera las esencias
int Guardado::getEsencias() {
    return esencias;
}

//Recupera la cantidad de mejoras de salud
int Guardado::getMejoraSalud() {
    return mejoraSalud;
}

//Recupera la cantidad de mejoras del poder
int Guardado::getMejoraPoderMax() {
    return mejoraPoderMax;
}

//Recupera la cantidad de mejoras de la velocidad de recuperacion del poder
int Guardado::getMejoraPoderVel() {
    return mejoraPoderVel;
}

//Recupera la cantidad de mejoras de distancia del gancho
int Guardado::getMejoraDistGancho() {
    return mejoraDistGancho;
}

//Recupera si ha desbloqueado el gancho
bool Guardado::getGancho() {
    return gancho;
}

//Recupera si ha desbloqueado la maza
bool Guardado::getMaza() {
    return maza;
}

//Recupera si ha desbloqueado la linterna
bool Guardado::getLinterna() {
    return linterna;
}

//Recupera si ha terminado el tutorial
bool Guardado::getTutorial() {
    return tutorial;
}


//Guarda las esencias
void Guardado::setEsencias(int e) {
    esencias = e;
}

//Guarda la cantidad de mejoras de salud
void Guardado::setMejoraSalud(int s) {
    mejoraSalud = s;
}

//Guarda la cantidad de mejoras del poder
void Guardado::setMejoraPoderMax(int p) {
    mejoraPoderMax = p;
}

//Guarda la cantidad de mejoras de la velocidad de recuperacion del poder
void Guardado::setMejoraPoderVel(int p) {
    mejoraPoderVel = p;
}

//Guarda la cantidad de mejoras de distancia del gancho
void Guardado::setMejoraDistGancho(int g) {
    mejoraDistGancho = g;
}

//Guarda si ha desbloqueado el gancho
void Guardado::setGancho(bool g) {
    gancho = g;
}

//Guarda si ha desbloqueado la maza
void Guardado::setMaza(bool m) {
    maza = m;
}

//Guarda si ha desbloqueado la linterna
void Guardado::setLinterna(bool l) {
    linterna = l;
}

//Guarda si ha terminado el tutorial
void Guardado::setTutorial(bool t) {
    tutorial = t;
}