#include "FabricaArmas.h"

using namespace std;
//Se inicializa el puntero de la instancia
FabricaArmas *FabricaArmas::pFabricaArmas = 0;

/*
**  STATS DE LAS ARMAS
*/
const int TOTAL_ARMAS   = 12;
/*
*   PISTOLAS
*/
//Constantes con las stats de la pistola base
const string        PISTOLA_BASE_ruta       = "arma_1.png";
const string        PISTOLA_BASE_nombre     = "Pistola oxidada";
const int           PISTOLA_BASE_danyo      = 60;
const int           PISTOLA_BASE_alcance    = 400;
const float         PISTOLA_BASE_cadencia   = 500;
const float         PISTOLA_BASE_recarga    = 3000;
const int           PISTOLA_BASE_municion   = 12;
const int           PISTOLA_BASE_precision  = 0;
const int           PISTOLA_BASE_runanorm   = 1;
const int           PISTOLA_BASE_runagran   = 0;
const vector<int>   PISTOLA_BASE_recorte    = {11, 11};
const float         PISTOLA_BASE_escala     = 2.5;

//Constantes con las stats de la pistola Epica
const string        PISTOLA_EPICA_ruta       = "arma_4.png";
const string        PISTOLA_EPICA_nombre     = "Aliento del dragon";
const int           PISTOLA_EPICA_danyo      = 90;
const int           PISTOLA_EPICA_alcance    = 450;
const float         PISTOLA_EPICA_cadencia   = 600;
const float         PISTOLA_EPICA_recarga    = 3500;
const int           PISTOLA_EPICA_municion   = 10;
const int           PISTOLA_EPICA_precision  = 0;
const int           PISTOLA_EPICA_runanorm   = 2;
const int           PISTOLA_EPICA_runagran   = 1;
const vector<int>   PISTOLA_EPICA_recorte    = {24, 20};
const float         PISTOLA_EPICA_escala     = 1.0;

//Constantes con las stats de la pistola Legendaria
const string        PISTOLA_LEGE_ruta       = "arma_5.png";
const string        PISTOLA_LEGE_nombre     = "Cortadora de plasma";
const int           PISTOLA_LEGE_danyo      = 160;
const int           PISTOLA_LEGE_alcance    = 750;
const float         PISTOLA_LEGE_cadencia   = 700;
const float         PISTOLA_LEGE_recarga    = 5000;
const int           PISTOLA_LEGE_municion   = 8;
const int           PISTOLA_LEGE_precision  = 0;
const int           PISTOLA_LEGE_runanorm   = 1;
const int           PISTOLA_LEGE_runagran   = 3;
const vector<int>   PISTOLA_LEGE_recorte    = {21, 16};
const float         PISTOLA_LEGE_escala     = 1.5;


/*
*   ESCOPETAS
*/
//Constantes con las stats de la escopeta base
const string        ESCOPETA_BASE_ruta       = "arma_2.png";
const string        ESCOPETA_BASE_nombre     = "Escopeta normal";
const int           ESCOPETA_BASE_danyo      = 20;
const int           ESCOPETA_BASE_alcance    = 280;
const float         ESCOPETA_BASE_cadencia   = 500;
const float         ESCOPETA_BASE_recarga    = 3000;
const int           ESCOPETA_BASE_municion   = 4;
const int           ESCOPETA_BASE_precision  = 0;
const int           ESCOPETA_BASE_runanorm   = 2;
const int           ESCOPETA_BASE_runagran   = 0;
const vector<int>   ESCOPETA_BASE_recorte    = {28, 11};
const float         ESCOPETA_BASE_escala     = 2.5;

//Constantes con las stats de la escopeta epica
const string        ESCOPETA_EPICA_ruta       = "arma_6.png";
const string        ESCOPETA_EPICA_nombre     = "Escopeta recortada";
const int           ESCOPETA_EPICA_danyo      = 22;
const int           ESCOPETA_EPICA_alcance    = 220;
const float         ESCOPETA_EPICA_cadencia   = 250;
const float         ESCOPETA_EPICA_recarga    = 1000;
const int           ESCOPETA_EPICA_municion   = 2;
const int           ESCOPETA_EPICA_precision  = 0;
const int           ESCOPETA_EPICA_runanorm   = 1;
const int           ESCOPETA_EPICA_runagran   = 1;
const vector<int>   ESCOPETA_EPICA_recorte    = {24, 13};
const float         ESCOPETA_EPICA_escala     = 2.0;

//Constantes con las stats de la escopeta legendaria
const string        ESCOPETA_LEGE_ruta       = "arma_7.png";
const string        ESCOPETA_LEGE_nombre     = "Escopeta antigua";
const int           ESCOPETA_LEGE_danyo      = 36;
const int           ESCOPETA_LEGE_alcance    = 280;
const float         ESCOPETA_LEGE_cadencia   = 800;
const float         ESCOPETA_LEGE_recarga    = 2000;
const int           ESCOPETA_LEGE_municion   = 5;
const int           ESCOPETA_LEGE_precision  = 0;
const int           ESCOPETA_LEGE_runanorm   = 2;
const int           ESCOPETA_LEGE_runagran   = 2;
const vector<int>   ESCOPETA_LEGE_recorte    = {36, 19};
const float         ESCOPETA_LEGE_escala     = 1.5;

/*
*   METRALLETAS
*/
//Constantes con las stats de la metralleta base
const string        METRALLETA_BASE_ruta       = "arma_3.png";
const string        METRALLETA_BASE_nombre     = "HK-48";
const int           METRALLETA_BASE_danyo      = 20;
const int           METRALLETA_BASE_alcance    = 700;
const float         METRALLETA_BASE_cadencia   = 220;
const float         METRALLETA_BASE_recarga    = 4000;
const int           METRALLETA_BASE_municion   = 26;
const float         METRALLETA_BASE_precision  = 0.2;
const int           METRALLETA_BASE_runanorm   = 2;
const int           METRALLETA_BASE_runagran   = 0;
const vector<int>   METRALLETA_BASE_recorte    = {28, 11};
const float         METRALLETA_BASE_escala     = 2.5;

//Constantes con las stats de la metralleta epica
const string        METRALLETA_EPICA_ruta       = "arma_8.png";
const string        METRALLETA_EPICA_nombre     = "Carabina";
const int           METRALLETA_EPICA_danyo      = 50;
const int           METRALLETA_EPICA_alcance    = 1000;
const float         METRALLETA_EPICA_cadencia   = 300;
const float         METRALLETA_EPICA_recarga    = 4000;
const int           METRALLETA_EPICA_municion   = 20;
const float         METRALLETA_EPICA_precision  = 0.12;
const int           METRALLETA_EPICA_runanorm   = 2;
const int           METRALLETA_EPICA_runagran   = 1;
const vector<int>   METRALLETA_EPICA_recorte    = {35, 17};
const float         METRALLETA_EPICA_escala     = 1.5;

//Constantes con las stats de la metralleta legendaria
const string        METRALLETA_LEGE_ruta       = "arma_9.png";
const string        METRALLETA_LEGE_nombre     = "Minigun";
const int           METRALLETA_LEGE_danyo      = 10;
const int           METRALLETA_LEGE_alcance    = 1000;
const float         METRALLETA_LEGE_cadencia   = 100;
const float         METRALLETA_LEGE_recarga    = 6000;
const int           METRALLETA_LEGE_municion   = 40;
const float         METRALLETA_LEGE_precision  = 0.4;
const int           METRALLETA_LEGE_runanorm   = 2;
const int           METRALLETA_LEGE_runagran   = 2;
const vector<int>   METRALLETA_LEGE_recorte    = {32, 19};
const float         METRALLETA_LEGE_escala     = 1.5;

/*
*   LANZACOHETES
*/
//Constantes con las stats del lanzacohetes base
const string        LANZACOHETES_BASE_ruta       = "arma_10.png";
const string        LANZACOHETES_BASE_nombre     = "Motor alienigena";
const int           LANZACOHETES_BASE_danyo      = 150;
const int           LANZACOHETES_BASE_alcance    = 700;
const float         LANZACOHETES_BASE_cadencia   = 2500;
const float         LANZACOHETES_BASE_recarga    = 8400;
const int           LANZACOHETES_BASE_municion   = 2;
const float         LANZACOHETES_BASE_precision  = 0;
const int           LANZACOHETES_BASE_runanorm   = 3;
const int           LANZACOHETES_BASE_runagran   = 0;
const vector<int>   LANZACOHETES_BASE_recorte    = {31, 18};
const float         LANZACOHETES_BASE_escala     = 2.0;
const float         LANZACOHETES_BASE_radio      = 2.0*32;

//Constantes con las stats del lanzacohetes epica
const string        LANZACOHETES_EPICA_ruta       = "arma_11.png";
const string        LANZACOHETES_EPICA_nombre     = "Canyon venenoso";
const int           LANZACOHETES_EPICA_danyo      = 190;
const int           LANZACOHETES_EPICA_alcance    = 1000;
const float         LANZACOHETES_EPICA_cadencia   = 100;
const float         LANZACOHETES_EPICA_recarga    = 6500;
const int           LANZACOHETES_EPICA_municion   = 1;
const float         LANZACOHETES_EPICA_precision  = 0;
const int           LANZACOHETES_EPICA_runanorm   = 2;
const int           LANZACOHETES_EPICA_runagran   = 1;
const vector<int>   LANZACOHETES_EPICA_recorte    = {35, 19};
const float         LANZACOHETES_EPICA_escala     = 1.5;
const float         LANZACOHETES_EPICA_radio      = 3.5*32;

//Constantes con las stats del lanzacohetes legendaria
const string        LANZACOHETES_LEGE_ruta       = "arma_12.png";
const string        LANZACOHETES_LEGE_nombre     = "Desintegrador";
const int           LANZACOHETES_LEGE_danyo      = 240;
const int           LANZACOHETES_LEGE_alcance    = 1000;
const float         LANZACOHETES_LEGE_cadencia   = 100;
const float         LANZACOHETES_LEGE_recarga    = 7500;
const int           LANZACOHETES_LEGE_municion   = 1;
const float         LANZACOHETES_LEGE_precision  = 0.4;
const int           LANZACOHETES_LEGE_runanorm   = 1;
const int           LANZACOHETES_LEGE_runagran   = 3;
const vector<int>   LANZACOHETES_LEGE_recorte    = {39, 25};
const float         LANZACOHETES_LEGE_escala     = 2.0;
const float         LANZACOHETES_LEGE_radio      = 5.0*32;
/*Se crea la instancia de Fabrica de armas
* 
*/
FabricaArmas* FabricaArmas::Instancia(){
    if(pFabricaArmas == 0){   //Primera llamada
        pFabricaArmas = new FabricaArmas(); //Se crea la instancia de FabricaArmas
    }
    return pFabricaArmas;
}

//Constructor de la clase
FabricaArmas::FabricaArmas() {}

//Destructor
FabricaArmas::~FabricaArmas() {}

//Se fabrica un arma segun la id pasada 
Arma* FabricaArmas::fabricarArma(int id){
    Arma *arma  = nullptr;
    switch (id)
    {
        //Pistolas
        case 0:
            arma = new Pistola(id, PISTOLA_BASE_ruta, PISTOLA_BASE_nombre, PISTOLA_BASE_danyo, PISTOLA_BASE_alcance, PISTOLA_BASE_cadencia, PISTOLA_BASE_recarga, PISTOLA_BASE_municion, PISTOLA_BASE_precision,PISTOLA_BASE_runanorm, PISTOLA_BASE_runagran, PISTOLA_BASE_recorte, PISTOLA_BASE_escala);
            break;

        case 1:
            arma = new Pistola(id, PISTOLA_EPICA_ruta, PISTOLA_EPICA_nombre, PISTOLA_EPICA_danyo, PISTOLA_EPICA_alcance, PISTOLA_EPICA_cadencia, PISTOLA_EPICA_recarga, PISTOLA_EPICA_municion, PISTOLA_EPICA_precision,PISTOLA_EPICA_runanorm, PISTOLA_EPICA_runagran, PISTOLA_EPICA_recorte, PISTOLA_EPICA_escala);
            break;
        case 2:
            arma = new Pistola(id, PISTOLA_LEGE_ruta, PISTOLA_LEGE_nombre, PISTOLA_LEGE_danyo, PISTOLA_LEGE_alcance, PISTOLA_LEGE_cadencia, PISTOLA_LEGE_recarga, PISTOLA_LEGE_municion, PISTOLA_LEGE_precision,PISTOLA_LEGE_runanorm, PISTOLA_LEGE_runagran, PISTOLA_LEGE_recorte, PISTOLA_LEGE_escala);
            break;

        //Escopetas
        case 3:
            arma = new Escopeta(id, ESCOPETA_BASE_ruta, ESCOPETA_BASE_nombre, ESCOPETA_BASE_danyo, ESCOPETA_BASE_alcance, ESCOPETA_BASE_cadencia, ESCOPETA_BASE_recarga, ESCOPETA_BASE_municion, ESCOPETA_BASE_precision,ESCOPETA_BASE_runanorm, ESCOPETA_BASE_runagran, ESCOPETA_BASE_recorte, ESCOPETA_BASE_escala);
            break;
        case 4:
            arma = new Escopeta(id, ESCOPETA_EPICA_ruta, ESCOPETA_EPICA_nombre, ESCOPETA_EPICA_danyo, ESCOPETA_EPICA_alcance, ESCOPETA_EPICA_cadencia, ESCOPETA_EPICA_recarga, ESCOPETA_EPICA_municion, ESCOPETA_EPICA_precision,ESCOPETA_EPICA_runanorm, ESCOPETA_EPICA_runagran, ESCOPETA_EPICA_recorte, ESCOPETA_EPICA_escala);
            break;
        case 5:
            arma = new Escopeta(id, ESCOPETA_LEGE_ruta, ESCOPETA_LEGE_nombre, ESCOPETA_LEGE_danyo, ESCOPETA_LEGE_alcance, ESCOPETA_LEGE_cadencia, ESCOPETA_LEGE_recarga, ESCOPETA_LEGE_municion, ESCOPETA_LEGE_precision,ESCOPETA_LEGE_runanorm, ESCOPETA_LEGE_runagran, ESCOPETA_LEGE_recorte, ESCOPETA_LEGE_escala);
            break;

        //Metralletas
        case 6:
            arma = new Metralleta(id, METRALLETA_BASE_ruta, METRALLETA_BASE_nombre, METRALLETA_BASE_danyo, METRALLETA_BASE_alcance, METRALLETA_BASE_cadencia, METRALLETA_BASE_recarga, METRALLETA_BASE_municion, METRALLETA_BASE_precision,METRALLETA_BASE_runanorm, METRALLETA_BASE_runagran, METRALLETA_BASE_recorte, METRALLETA_BASE_escala);
            break;
        case 7:
            arma = new Metralleta(id, METRALLETA_EPICA_ruta, METRALLETA_EPICA_nombre, METRALLETA_EPICA_danyo, METRALLETA_EPICA_alcance, METRALLETA_EPICA_cadencia, METRALLETA_EPICA_recarga, METRALLETA_EPICA_municion, METRALLETA_EPICA_precision,METRALLETA_EPICA_runanorm, METRALLETA_EPICA_runagran, METRALLETA_EPICA_recorte, METRALLETA_EPICA_escala);
            break;
        case 8:
            arma = new Metralleta(id, METRALLETA_LEGE_ruta, METRALLETA_LEGE_nombre, METRALLETA_LEGE_danyo, METRALLETA_LEGE_alcance, METRALLETA_LEGE_cadencia, METRALLETA_LEGE_recarga, METRALLETA_LEGE_municion, METRALLETA_LEGE_precision,METRALLETA_LEGE_runanorm, METRALLETA_LEGE_runagran, METRALLETA_LEGE_recorte, METRALLETA_LEGE_escala);
            break;
        //Lanzacohetes
        case 9:
            arma = new LanzaCohetes(id, LANZACOHETES_BASE_ruta, LANZACOHETES_BASE_nombre, LANZACOHETES_BASE_danyo, LANZACOHETES_BASE_alcance, LANZACOHETES_BASE_cadencia, LANZACOHETES_BASE_recarga, LANZACOHETES_BASE_municion, LANZACOHETES_BASE_precision,LANZACOHETES_BASE_runanorm, LANZACOHETES_BASE_runagran, LANZACOHETES_BASE_recorte, LANZACOHETES_BASE_escala, LANZACOHETES_BASE_radio);
            break;
        case 10:
            arma = new LanzaCohetes(id, LANZACOHETES_EPICA_ruta, LANZACOHETES_EPICA_nombre, LANZACOHETES_EPICA_danyo, LANZACOHETES_EPICA_alcance, LANZACOHETES_EPICA_cadencia, LANZACOHETES_EPICA_recarga, LANZACOHETES_EPICA_municion, LANZACOHETES_EPICA_precision,LANZACOHETES_EPICA_runanorm, LANZACOHETES_EPICA_runagran, LANZACOHETES_EPICA_recorte, LANZACOHETES_EPICA_escala, LANZACOHETES_EPICA_radio);
            break;
        case 11:
            arma = new LanzaCohetes(id, LANZACOHETES_LEGE_ruta, LANZACOHETES_LEGE_nombre, LANZACOHETES_LEGE_danyo, LANZACOHETES_LEGE_alcance, LANZACOHETES_LEGE_cadencia, LANZACOHETES_LEGE_recarga, LANZACOHETES_LEGE_municion, LANZACOHETES_LEGE_precision,LANZACOHETES_LEGE_runanorm, LANZACOHETES_LEGE_runagran, LANZACOHETES_LEGE_recorte, LANZACOHETES_LEGE_escala, LANZACOHETES_LEGE_radio);
            break;
        default:
            break;
    }
    return arma;
}



string FabricaArmas::getRuta(int id){
    string ruta;
    switch (id)
    {
        case 0:
            ruta = PISTOLA_BASE_ruta;
            break;
        case 1:
            ruta = PISTOLA_EPICA_ruta;
            break;
        case 2:
            ruta = PISTOLA_LEGE_ruta;
            break;
        case 3:
            ruta = ESCOPETA_BASE_ruta;
            break;
        case 4:
            ruta = ESCOPETA_EPICA_ruta;
            break;
        case 5:
            ruta = ESCOPETA_LEGE_ruta;
            break;
        case 6:
            ruta = METRALLETA_BASE_ruta;
            break;
        case 7:
            ruta = METRALLETA_EPICA_ruta;
            break;
        case 8:
            ruta = METRALLETA_LEGE_ruta;
            break;
        case 9:
            ruta = LANZACOHETES_BASE_ruta;
            break;
        case 10:
            ruta = LANZACOHETES_EPICA_ruta;
            break;
        case 11:
            ruta = LANZACOHETES_LEGE_ruta;
            break;
        default:
            break;
    }
    return ruta;
}
string FabricaArmas::getNombre(int id){
    string nombre;
    switch (id)
    {
        case 0:
            nombre = PISTOLA_BASE_nombre;
            break;
        case 1:
            nombre = PISTOLA_EPICA_nombre;
            break;
        case 2:
            nombre = PISTOLA_LEGE_nombre;
            break;
        case 3:
            nombre = ESCOPETA_BASE_nombre;
            break;
        case 4:
            nombre = ESCOPETA_EPICA_nombre;
            break;
        case 5:
            nombre = ESCOPETA_LEGE_nombre;
            break;
        case 6:
            nombre = METRALLETA_BASE_nombre;
            break;
        case 7:
            nombre = METRALLETA_EPICA_nombre;
            break;
        case 8:
            nombre = METRALLETA_LEGE_nombre;
            break;
        case 9:
            nombre = LANZACOHETES_BASE_nombre;
            break;
        case 10:
            nombre = LANZACOHETES_EPICA_nombre;
            break;
        case 11:
            nombre = LANZACOHETES_LEGE_nombre;
            break;
        default:
            break;
    }
    return nombre;
}
int FabricaArmas::getDanyo(int id){
    int danyo = 0;
    switch (id)
    {
        case 0:
            danyo = PISTOLA_BASE_danyo;
            break;
        case 1:
            danyo = PISTOLA_EPICA_danyo;
            break;
        case 2:
            danyo = PISTOLA_LEGE_danyo;
            break;
        case 3:
            danyo = ESCOPETA_BASE_danyo;
            break;
        case 4:
            danyo = ESCOPETA_EPICA_danyo;
            break;
        case 5:
            danyo = ESCOPETA_LEGE_danyo;
            break;
        case 6:
            danyo = METRALLETA_BASE_danyo;
            break;
        case 7:
            danyo = METRALLETA_EPICA_danyo;
            break;
        case 8:
            danyo = METRALLETA_LEGE_danyo;
            break;
        case 9:
            danyo = LANZACOHETES_BASE_danyo;
            break;
        case 10:
            danyo = LANZACOHETES_EPICA_danyo;
            break;
        case 11:
            danyo = LANZACOHETES_LEGE_danyo;
            break;
        default:
            break;
    }
    return danyo;
}
int FabricaArmas::getAlcance(int id){
    int alcance = 0;
    switch (id)
    {
        case 0:
            alcance = PISTOLA_BASE_alcance;
            break;
        case 1:
            alcance = PISTOLA_EPICA_alcance;
            break;
        case 2:
            alcance = PISTOLA_LEGE_alcance;
            break;
        case 3:
            alcance = ESCOPETA_BASE_alcance;
            break;
        case 4:
            alcance = ESCOPETA_EPICA_alcance;
            break;
        case 5:
            alcance = ESCOPETA_LEGE_alcance;
            break;
        case 6:
            alcance = METRALLETA_BASE_alcance;
            break;
        case 7:
            alcance = METRALLETA_EPICA_alcance;
            break;
        case 8:
            alcance = METRALLETA_LEGE_alcance;
            break;
        case 9:
            alcance = LANZACOHETES_BASE_alcance;
            break;
        case 10:
            alcance = LANZACOHETES_EPICA_alcance;
            break;
        case 11:
            alcance = LANZACOHETES_LEGE_alcance;
            break;
        default:
            break;
    }
    return alcance;
}
float FabricaArmas::getCadencia(int id){
    float cadencia = 0;
    switch (id)
    {
        case 0:
            cadencia = PISTOLA_BASE_cadencia;
            break;
        case 1:
            cadencia = PISTOLA_EPICA_cadencia;
            break;
        case 2:
            cadencia = PISTOLA_LEGE_cadencia;
            break;
        case 3:
            cadencia = ESCOPETA_BASE_cadencia;
            break;
        case 4:
            cadencia = ESCOPETA_EPICA_cadencia;
            break;
        case 5:
            cadencia = ESCOPETA_LEGE_cadencia;
            break;
        case 6:
            cadencia = METRALLETA_BASE_cadencia;
            break;
        case 7:
            cadencia = METRALLETA_EPICA_cadencia;
            break;
        case 8:
            cadencia = METRALLETA_LEGE_cadencia;
            break;
        case 9:
            cadencia = LANZACOHETES_BASE_cadencia;
            break;
        case 10:
            cadencia = LANZACOHETES_EPICA_cadencia;
            break;
        case 11:
            cadencia = LANZACOHETES_LEGE_cadencia;
            break;
        default:
            break;
    }
    return cadencia;
}
float FabricaArmas::getRecarga(int id){
    float recarga = 0;
    switch (id)
    {
        case 0:
            recarga = PISTOLA_BASE_recarga;
            break;
        case 1:
            recarga = PISTOLA_EPICA_recarga;
            break;
        case 2:
            recarga = PISTOLA_LEGE_recarga;
            break;
        case 3:
            recarga = ESCOPETA_BASE_recarga;
            break;
        case 4:
            recarga = ESCOPETA_EPICA_recarga;
            break;
        case 5:
            recarga = ESCOPETA_LEGE_recarga;
            break;
        case 6:
            recarga = METRALLETA_BASE_recarga;
            break;
        case 7:
            recarga = METRALLETA_EPICA_recarga;
            break;
        case 8:
            recarga = METRALLETA_LEGE_recarga;
            break;
        case 9:
            recarga = LANZACOHETES_BASE_recarga;
            break;
        case 10:
            recarga = LANZACOHETES_EPICA_recarga;
            break;
        case 11:
            recarga = LANZACOHETES_LEGE_recarga;
            break;
        default:
            break;
    }
    return recarga;
}

int FabricaArmas::getMunicion(int id){
    int municion = 0;
    switch (id)
    {
        case 0:
            municion = PISTOLA_BASE_municion;
            break;
        case 1:
            municion = PISTOLA_EPICA_municion;
            break;
        case 2:
            municion = PISTOLA_LEGE_municion;
            break;
        case 3:
            municion = ESCOPETA_BASE_municion;
            break;
        case 4:
            municion = ESCOPETA_EPICA_municion;
            break;
        case 5:
            municion = ESCOPETA_LEGE_municion;
            break;
        case 6:
            municion = METRALLETA_BASE_municion;
            break;
        case 7:
            municion = METRALLETA_EPICA_municion;
            break;
        case 8:
            municion = METRALLETA_LEGE_municion;
            break;
        case 9:
            municion = LANZACOHETES_BASE_municion;
            break;
        case 10:
            municion = LANZACOHETES_EPICA_municion;
            break;
        case 11:
            municion = LANZACOHETES_LEGE_municion;
            break;
        default:
            break;
    }
    return municion;
}

int FabricaArmas::getPrecicion(int id){
    int precision = 0;
    switch (id)
    {
        case 0:
            precision = PISTOLA_BASE_precision;
            break;
        case 1:
            precision = PISTOLA_EPICA_precision;
            break;
        case 2:
            precision = PISTOLA_LEGE_precision;
            break;
        case 3:
            precision = ESCOPETA_BASE_precision;
            break;
        case 4:
            precision = ESCOPETA_EPICA_precision;
            break;
        case 5:
            precision = ESCOPETA_LEGE_precision;
            break;
        case 6:
            precision = METRALLETA_BASE_precision;
            break;
        case 7:
            precision = METRALLETA_EPICA_precision;
            break;
        case 8:
            precision = METRALLETA_LEGE_precision;
            break;
        case 9:
            precision = LANZACOHETES_BASE_precision;
            break;
        case 10:
            precision = LANZACOHETES_EPICA_precision;
            break;
        case 11:
            precision = LANZACOHETES_LEGE_precision;
            break;
        default:
            break;
    }
    return precision;
}
int FabricaArmas::getRunasNorm(int id){
    int runanorm = 0;
    switch (id)
    {
        case 0:
            runanorm = PISTOLA_BASE_runanorm;
            break;
        case 1:
            runanorm = PISTOLA_EPICA_runanorm;
            break;
        case 2:
            runanorm = PISTOLA_LEGE_runanorm;
            break;
        case 3:
            runanorm = ESCOPETA_BASE_runanorm;
            break;
        case 4:
            runanorm = ESCOPETA_EPICA_runanorm;
            break;
        case 5:
            runanorm = ESCOPETA_LEGE_runanorm;
            break;
        case 6:
            runanorm = METRALLETA_BASE_runanorm;
            break;
        case 7:
            runanorm = METRALLETA_EPICA_runanorm;
            break;
        case 8:
            runanorm = METRALLETA_LEGE_runanorm;
            break;
        case 9:
            runanorm = LANZACOHETES_BASE_runanorm;
            break;
        case 10:
            runanorm = LANZACOHETES_EPICA_runanorm;
            break;
        case 11:
            runanorm = LANZACOHETES_LEGE_runanorm;
            break;
        default:
            break;
    }
    return runanorm;
}
int FabricaArmas::getRunasGran(int id){
    int runagran = 0;
    switch (id)
    {
        case 0:
            runagran = PISTOLA_BASE_runagran;
            break;
        case 1:
            runagran = PISTOLA_EPICA_runagran;
            break;
        case 2:
            runagran = PISTOLA_LEGE_runagran;
            break;
        case 3:
            runagran = ESCOPETA_BASE_runagran;
            break;
        case 4:
            runagran = ESCOPETA_EPICA_runagran;
            break;
        case 5:
            runagran = ESCOPETA_LEGE_runagran;
            break;
        case 6:
            runagran = METRALLETA_BASE_runagran;
            break;
        case 7:
            runagran = METRALLETA_EPICA_runagran;
            break;
        case 8:
            runagran = METRALLETA_LEGE_runagran;
            break;
        case 9:
            runagran = LANZACOHETES_BASE_runagran;
            break;
        case 10:
            runagran = LANZACOHETES_EPICA_runagran;
            break;
        case 11:
            runagran = LANZACOHETES_LEGE_runagran;
            break;
        default:
            break;
    }
    return runagran;
}
vector<int> FabricaArmas::getRecorte(int id){
    vector<int> recorte;
    switch (id)
    {
        case 0:
            recorte = PISTOLA_BASE_recorte;
            break;
        case 1:
            recorte = PISTOLA_EPICA_recorte;
            break;
        case 2:
            recorte = PISTOLA_LEGE_recorte;
            break;
        case 3:
            recorte = ESCOPETA_BASE_recorte;
            break;
        case 4:
            recorte = ESCOPETA_EPICA_recorte;
            break;
        case 5:
            recorte = ESCOPETA_LEGE_recorte;
            break;
        case 6:
            recorte = METRALLETA_BASE_recorte;
            break;
        case 7:
            recorte = METRALLETA_EPICA_recorte;
            break;
        case 8:
            recorte = METRALLETA_LEGE_recorte;
            break;
        case 9:
            recorte = LANZACOHETES_BASE_recorte;
            break;
        case 10:
            recorte = LANZACOHETES_EPICA_recorte;
            break;
        case 11:
            recorte = LANZACOHETES_LEGE_recorte;
            break;
        default:
            break;
    }
    return recorte;
}
float FabricaArmas::getEscala(int id){
    float escala = 0;
    switch (id)
    {
        case 0:
            escala = PISTOLA_BASE_escala;
            break;
        case 1:
            escala = PISTOLA_EPICA_escala;
            break;
        case 2:
            escala = PISTOLA_LEGE_escala;
            break;
        case 3:
            escala = ESCOPETA_BASE_escala;
            break;
        case 4:
            escala = ESCOPETA_EPICA_escala;
            break;
        case 5:
            escala = ESCOPETA_LEGE_escala;
            break;
        case 6:
            escala = METRALLETA_BASE_escala;
            break;
        case 7:
            escala = METRALLETA_EPICA_escala;
            break;
        case 8:
            escala = METRALLETA_LEGE_escala;
            break;
        case 9:
            escala = LANZACOHETES_BASE_escala;
            break;
        case 10:
            escala = LANZACOHETES_EPICA_escala;
            break;
        case 11:
            escala = LANZACOHETES_LEGE_escala;
            break;
        default:
            break;
    }
    return escala;
}

//Devuelve la cantidad de armas existente (constante)
int FabricaArmas::getTotalArmas() {
    return TOTAL_ARMAS;
}