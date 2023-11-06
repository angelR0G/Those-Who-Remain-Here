#include "FabricaElementosEntorno.h"

FabricaElementosEntorno* FabricaElementosEntorno::pFabrica = 0;

// VALORES PARA LA FABRICA

//Sprites (ruta)
const string SPRITE_TRAMPA              = "resources/sprites/entorno/Trampa.png";
const string SPRITE_COFRE               = "resources/sprites/generales/Chests.png";
const string SPRITE_PORTAL_DESAFIO      = "resources/sprites/generales/atemporal.png";
const string SPRITE_PORTAL_JEFE         = "resources/sprites/generales/Portal.png";
const string SPRITE_ESPIRITUS_OSCUROS   = "resources/sprites/entorno/EspiritusOscuros.png";
const string SPRITE_MURO_OSCURO         = "resources/sprites/entorno/MuroOscuridad.png";
const string SPRITE_MURO_DESTRUC        = "resources/sprites/entorno/MuroDestructible.png";
const string SPRITE_CUM_MATERIALES      = "resources/sprites/entorno/Huevos.png";

//Escala del sprite
const vector<float> ESCALA_TRAMPA               = {2.0, 2.0};
const vector<float> ESCALA_CUM_MATERIALES       = {2.0, 2.0};
const vector<float> ESCALA_PORTAL_JEFE          = {2.0, 2.0};
const vector<float> ESCALA_MURO_OSCURO          = {2.0, 3.0};

//Tamanyo sprite
const vector<int> TAM_TRAMPA            = {16, 16};
const vector<int> TAM_COFRE             = {48, 32};
const vector<int> TAM_PORTAL_DESAFIO    = {32, 32};
const vector<int> TAM_PORTAL_JEFE       = {100, 100};
const vector<int> TAM_ESPIRITUS_OSCUROS = {100, 80};
const vector<int> TAM_MURO_OSCURO       = {50, 100};
const vector<int> TAM_MURO_DESTRUC      = {32, 32};
const vector<int> TAM_CUM_MATERIALES    = {32, 32};

//Origen
const vector<float> ORIGEN_PORTAL_JEFE          = {50.0, 50.0};
const vector<float> ORIGEN_ESPIRITUS_OSCUROS    = {50.0, 40.0};
const vector<float> ORIGEN_MURO_OSCURO          = {20.0, 68.0};
const vector<float> ORIGEN_CUM_MATERIALES       = {16.0, 16.0};


//Devuelve una instancia de fabrica de elementos del entorno, creandola si no existe
FabricaElementosEntorno* FabricaElementosEntorno::Instancia(){
    if(pFabrica == 0){                                  //Primera llamada
        pFabrica = new FabricaElementosEntorno();     //Se crea la instancia de FabricaObjetosAtemporales
    }
    return pFabrica;
}

//Constructor
FabricaElementosEntorno::FabricaElementosEntorno() {}

//Destructor
FabricaElementosEntorno::~FabricaElementosEntorno() {}

//Dada una id, fabrica el objeto correspondiente
ElementoEntorno* FabricaElementosEntorno::fabricar(int id) {
    ElementoEntorno* elem = nullptr;
    Textura* t = nullptr;
    Sprite* s = nullptr;

    switch(id) {
        case 0:
        case 1:
        case 2:
        case 3:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_TRAMPA);
            s = new Sprite(t, 0, 0, TAM_TRAMPA[0], TAM_TRAMPA[1]);
            s->setScale(ESCALA_TRAMPA[0], ESCALA_TRAMPA[1]);

            //Orienta el sprite segun la direccion de disparo
            if(id == 0) {
                s->setOrigin(TAM_TRAMPA[0], TAM_TRAMPA[1]);
                s->setRotation(180.0);
            }
            else if(id == 1) {
                s->setOrigin(TAM_TRAMPA[0], 0);
                s->setRotation(-90.0);
            }
            else if(id == 3) {
                s->setOrigin(0, TAM_TRAMPA[1]);
                s->setRotation(90.0);
            }

            elem = new Trampa(t, s, id);
            break;
        case 4:
            //Crea un cofre
            t = new Textura(SPRITE_COFRE);
            s = new Sprite(t, 0, 0, TAM_COFRE[0], TAM_COFRE[1]);
            elem = new Cofre(t, s);
            break;
        case 5:
            //Portal a la sala de desafio
            t = new Textura(SPRITE_PORTAL_DESAFIO);
            s = new Sprite(t, 0, 0, TAM_PORTAL_DESAFIO[0], TAM_PORTAL_DESAFIO[1]);
            elem = new PortalSalaADesafio(t, s);
            break;
        case 6:
            //Portal a la sala del jefe
            t = new Textura(SPRITE_PORTAL_JEFE);
            s = new Sprite(t, 0, 0, TAM_PORTAL_JEFE[0], TAM_PORTAL_JEFE[1]);
            s->setOrigin(ORIGEN_PORTAL_JEFE[0], ORIGEN_PORTAL_JEFE[1]);
            s->setScale(ESCALA_PORTAL_JEFE[0], ESCALA_PORTAL_JEFE[1]);
            elem = new PortalJefe(t, s);
            break;
        case 7:
            //Espiritus oscuros que ralentizan
            t = new Textura(SPRITE_ESPIRITUS_OSCUROS);
            s = new Sprite(t, 0, 0, TAM_ESPIRITUS_OSCUROS[0], TAM_ESPIRITUS_OSCUROS[1]);
            s->setOrigin(ORIGEN_ESPIRITUS_OSCUROS[0], ORIGEN_ESPIRITUS_OSCUROS[1]);
            elem = new EspiritusOscuros(t, s);
            break;
        case 8:
            //Muro oscuro que impide avanzar
            t = new Textura(SPRITE_MURO_OSCURO);
            s = new Sprite(t, 0, 0, TAM_MURO_OSCURO[0], TAM_MURO_OSCURO[1]);
            s->setOrigin(ORIGEN_MURO_OSCURO[0], ORIGEN_MURO_OSCURO[1]);
            s->setScale(ESCALA_MURO_OSCURO[0], ESCALA_MURO_OSCURO[1]);
            elem = new MuroOscuro(t, s);
            break;
        case 9:
            //Muro destructible
            t = new Textura(SPRITE_MURO_DESTRUC);
            s = new Sprite(t, 0, 0, TAM_MURO_DESTRUC[0], TAM_MURO_DESTRUC[1]);
            elem = new MuroDestructible(t, s);
            break;
        case 10:
            //Cumulo de materiales
            t = new Textura(SPRITE_CUM_MATERIALES);
            s = new Sprite(t, 0, 0, TAM_CUM_MATERIALES[0], TAM_CUM_MATERIALES[1]);
            s->setOrigin(ORIGEN_CUM_MATERIALES[0], ORIGEN_CUM_MATERIALES[1]);
            s->setScale(ESCALA_CUM_MATERIALES[0], ESCALA_CUM_MATERIALES[1]);
            elem = new CumuloMateriales(t, s);
            break;
    }

    return elem;
}