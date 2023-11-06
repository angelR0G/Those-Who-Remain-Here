#pragma once

/*
** EscritorXML es una clase fachada que utiliza la libreria tinyxml2 para funcionar.
** Su objetivo es crear documentos XML.
*/

#include <string>
#include <sstream>
#include <vector>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class EscritorXML {
    public:
        EscritorXML(string, string);

        ~EscritorXML();

        void nuevoElemento(string);
        void anyadirAtributo(string, string);
        void anyadirElementoADocumento();
        bool guardarArchivo();

    private:
        string ruta;                        //Ruta donde se guardara el archivo
        XMLDocument archivo;                //Documento XML creado
        XMLError error;                     //Gestor de errores de tinyxml2
        XMLNode* raiz;                      //Elemento raiz del documento
        XMLElement* elementoActual;         //Elemento que esta siendo creado
};