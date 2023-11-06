#pragma once

/*
** LectorXML es una clase fachada que utiliza la libreria tinyxml2 para funcionar.
** Su objetivo es leer documentos XML.
*/

#include <string>
#include <sstream>
#include <vector>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class LectorXML {
    public:
        LectorXML();

        ~LectorXML();

        bool abrirArchivo(string);
        bool siguienteHijo(string);
        void padre();
        void volverARaiz();
        bool siguienteHermano(string);
        string getElementText();
        string getAttribute(string);
    private:
        bool archivoAbierto;                //Indica si se ha abierto un archivo
        XMLDocument archivo;                //Documento XML abierto
        XMLError error;                     //Gestor de errores de tinyxml2
        XMLElement* elemento;               //Puntero al ultimo elemento buscado
};