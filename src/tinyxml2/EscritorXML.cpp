#include "EscritorXML.h"

//Constructor. Recibe la ruta donde sera almacenado y el nombre del primer elemento.
EscritorXML::EscritorXML(string r, string primE) {
    ruta = r;

    //Convierte el string en char*
    char primC[primE.length() + 1];
    strcpy(primC, primE.c_str());
    
    raiz = archivo.NewElement(primC);
    archivo.InsertFirstChild(raiz);
}

//Destructor.
EscritorXML::~EscritorXML() {}

//Crea un nuevo elemento con el valor pasado por parametro
void EscritorXML::nuevoElemento(string nombre) {
    //Convierte el string en char*
    char nombreC[nombre.length() + 1];
    strcpy(nombreC, nombre.c_str());

    elementoActual = archivo.NewElement(nombreC);
}

//Anyade un atributo con su valor al elemento actual
void EscritorXML::anyadirAtributo(string nombre, string valor) {
    //Comprueba que existe un elemento actual
    if(elementoActual != nullptr) {
        //Convierte los string en char*
        char nombreC[nombre.length() + 1];
        strcpy(nombreC, nombre.c_str());
        char valorC[valor.length() + 1];
        strcpy(valorC, valor.c_str());

        elementoActual->SetAttribute(nombreC, valorC);
    }
}

//Anyade el elemento actual al documento. Tras hacerlo, se pierde la referencia al elemento.
void EscritorXML::anyadirElementoADocumento() {
    //Comprueba que existe un elemento actual
    if(elementoActual != nullptr) {
        raiz->InsertEndChild(elementoActual);
        elementoActual = nullptr;
    }
}

//Guarda el archivo creado en la ruta que tiene guardada.
bool EscritorXML::guardarArchivo() {
    bool guardado = false;

    //Convierte el string en char*
    char rutaC[ruta.length() + 1];
    strcpy(rutaC, ruta.c_str());

    //Guarda el archivo
    error = archivo.SaveFile(rutaC);

    //Comprueba si se ha guardado correctamente
    if(error == XML_SUCCESS) {
        guardado = true;
    }

    return guardado;
}