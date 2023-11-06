#include "LectorXML.h"

#include <iostream>
using namespace std;

LectorXML::LectorXML() {
    archivoAbierto = false;
}

LectorXML::~LectorXML() {
    elemento = nullptr;
}

//Abre el archivo de la ruta para leerlo. Devuelve false si no se ha podido abrir.
bool LectorXML::abrirArchivo(string ruta) {
    //Convierte el string en char*
    char rutaC[ruta.length() + 1];
    strcpy(rutaC, ruta.c_str());

    //Intenta cargar el archivo
    error = archivo.LoadFile(rutaC);

    //Comprueba que no ha habido ningun error
    if(error == XML_SUCCESS) {
        archivoAbierto = true;
        elemento = archivo.FirstChildElement()->ToElement();
    }
    else {
        archivoAbierto = false;
    }

    return archivoAbierto;
}

//Vuelve la referencia al elemento raiz
void LectorXML::volverARaiz() {
    elemento = archivo.FirstChildElement()->ToElement();
}

//Comprueba el siguiente hijo del elemento actual. Devuelve false si no lo encuentra.
bool LectorXML::siguienteHijo(string s) {
    bool encontrado = false;

    if(archivoAbierto && elemento != nullptr) {
        //Convierte el string en char
        char valor[s.length() + 1];
        strcpy(valor, s.c_str());

        //Crea un handle desde el elemento actual y consulta el elemento hermano
        XMLHandle handle(elemento);
        XMLElement* elem = handle.FirstChildElement(valor).ToElement();

        //Comprueba si ha encontrado el elemento
        if(elem) {
            encontrado = true;
            //Actualiza el puntero al ultimo elemento
            elemento = elem;
        }
        else {
            elemento = nullptr;
        }
    }

    return encontrado;
}

//Comprueba el siguiente hermano del elemento actual. Devuelve false si no lo encuentra.
bool LectorXML::siguienteHermano(string s) {
    bool encontrado = false;

    if(archivoAbierto && elemento != nullptr) {
        //Convierte el string en char
        char valor[s.length() + 1];
        strcpy(valor, s.c_str());

        //Crea un handle desde el elemento actual y consulta el elemento hermano
        XMLHandle handle(elemento);
        XMLElement* elem = handle.NextSiblingElement(valor).ToElement();

        //Comprueba si ha encontrado el elemento
        if(elem) {
            encontrado = true;
            //Actualiza el puntero al ultimo elemento
            elemento = elem;
        }
        else {
            elemento = nullptr;
        }
    }

    return encontrado;
}

//Actualiza el elemento actual al padre
void LectorXML::padre() {
    //Vuelve al elemento padre
    elemento = elemento->Parent()->ToElement();
}

//Devuelve el valor del contenido del ultimo elemento buscado. Devuelve cadena vacia por defecto.
string LectorXML::getElementText() {
    string texto = "";

    //Comprueba que existe el elemento
    if(elemento) {
        XMLHandle handle(elemento);

        //Recupera el texto con un handle
        texto = handle.FirstChild().ToText()->Value();
    }

    return texto;
}

//Devuelve el valor del atributo indicado del ultimo elemento buscado. Devuelve NULL por defecto.
string LectorXML::getAttribute(string att) {
    string texto = "";
    
    //Comprueba que existe el elemento
    if(elemento) {
        //Convierte el string en char 
        char valor[att.length() + 1];
        strcpy(valor, att.c_str());
        //Recupera el valor del atributo
        if(elemento->Attribute(valor)) {
            texto = elemento->Attribute(valor);
        }
    }

    return texto;
}