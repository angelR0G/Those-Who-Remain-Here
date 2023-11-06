#include "FabricaObjetosAtemporales.h"

FabricaObjetosAtemporales* FabricaObjetosAtemporales::pFabrica = 0;

// VALORES PARA LA FABRICA

//Escala
const float ESCALA          = 4.0;

//Constantes de Escenario.cpp
const int COL               = 65;       //Id de las colisiones en el mapa
const int COL_PLAT          = 67;
const int TAM_TILES         = 32;       //Tamanyo final de los tiles

//Sprites (ruta)
const string SPRITE_0       = "resources/sprites/atemporal/atemporal_1.png";
const string SPRITE_1       = "resources/sprites/atemporal/atemporal_2.png";
const string SPRITE_2       = "resources/sprites/atemporal/atemporal_3.png";
const string SPRITE_3       = "resources/sprites/atemporal/atemporal_4.png";
const string SPRITE_4       = "resources/sprites/atemporal/atemporal_5.png";
const string SPRITE_5       = "resources/sprites/atemporal/atemporal_6.png";
const string SPRITE_6       = "resources/sprites/atemporal/atemporal_7.png";
const string SPRITE_7       = "resources/sprites/atemporal/atemporal_8.png";
const string SPRITE_8       = "resources/sprites/atemporal/atemporal_9.png";
const string SPRITE_9       = "resources/sprites/atemporal/atemporal_10.png";
const string SPRITE_10      = "resources/sprites/atemporal/atemporal_11.png";
const string SPRITE_11      = "resources/sprites/atemporal/atemporal_12.png";
const string SPRITE_12      = "resources/sprites/atemporal/atemporal_13.png";
const string SPRITE_13      = "resources/sprites/atemporal/atemporal_14.png";
const string SPRITE_14      = "resources/sprites/atemporal/atemporal_15.png";
const string SPRITE_15      = "resources/sprites/atemporal/atemporal_16.png";
const string SPRITE_16      = "resources/sprites/atemporal/atemporal_17.png";
const string SPRITE_17      = "resources/sprites/atemporal/atemporal_18.png";
const string SPRITE_18      = "resources/sprites/atemporal/atemporal_19.png";

//Tamanyo sprite
const vector<int> TAM_0     = {24, 16};
const vector<int> TAM_1     = {24, 40};
const vector<int> TAM_2     = {48, 32};
const vector<int> TAM_3     = {40, 48};
const vector<int> TAM_4     = {40, 8};
const vector<int> TAM_5     = {48, 40};
const vector<int> TAM_6     = {48, 40};

//Origen sprite
const vector<int> ORIGEN_0  = {8, 0};
const vector<int> ORIGEN_1  = {8, 16};
const vector<int> ORIGEN_2  = {16, 16};
const vector<int> ORIGEN_3  = {16, 16};
const vector<int> ORIGEN_4  = {16, 0};
const vector<int> ORIGEN_5  = {16, 16};
const vector<int> ORIGEN_6  = {16, 16};

//Mapa de colisiones
const map<vector<int>, int> MAPA_0  =   {{{0,0}, COL}, {{-1,0}, COL}, {{1,0}, COL}, {{0,1}, COL}, {{-1,1}, COL}, {{1,1}, COL}};
const map<vector<int>, int> MAPA_1  =   {{{-1,0}, COL}, {{-1,-1}, COL}, {{-1,-2}, COL}, {{0,-2}, COL}, {{1,-2}, COL}, {{1,-1}, COL},
                                        {{1,0}, COL}, {{1,1}, COL}, {{1,2}, COL}, {{0,2}, COL}, {{-1,2}, COL}, {{-1,1}, COL}};
const map<vector<int>, int> MAPA_2  =   {{{0,0}, COL}, {{-1,0}, COL}, {{-2,0}, COL}, {{-2,-1}, COL}, {{-2,-2}, COL}, {{-1,-2}, COL},
                                        {{-1,-1}, COL}, {{0,-1}, COL}, {{1,-1}, COL}, {{1,0}, COL}, {{2,0}, COL}, {{3,0}, COL},
                                        {{3,1}, COL}, {{2,1}, COL}, {{1,1}, COL}, {{0,1}, COL}};
const map<vector<int>, int> MAPA_3  =   {{{0,0}, COL}, {{0,-1}, COL}, {{0,-2}, COL}, {{1,-2}, COL}, {{2,-2}, COL}, {{2,-1}, COL},
                                        {{2,0}, COL}, {{2,1}, COL}, {{2,2}, COL}, {{2,3}, COL}, {{1,3}, COL}, {{0,3}, COL},
                                        {{-1,3}, COL}, {{-2,3}, COL}, {{-2,2}, COL}, {{-1,2}, COL}, {{0,2}, COL}, {{0,1}, COL}};
const map<vector<int>, int> MAPA_4  =   {{{0,0}, COL_PLAT}, {{-1,0}, COL_PLAT}, {{-2,0}, COL_PLAT}, {{1,0}, COL_PLAT}, {{2,0}, COL_PLAT}};
const map<vector<int>, int> MAPA_5  =   {{{0,0}, COL}, {{-1,0}, COL}, {{-1,1}, COL}, {{-1,2}, COL}, {{-2,2}, COL}, {{-2,1}, COL},
                                        {{-2,0}, COL}, {{-2,-1}, COL}, {{-1,-1}, COL}, {{0,-1}, COL}, {{0,-2}, COL}, {{1,-2}, COL},
                                        {{2,-2}, COL}, {{3,-2}, COL}, {{3,-1}, COL}, {{2,-1}, COL}, {{1,-1}, COL}, {{1,0}, COL}};
const map<vector<int>, int> MAPA_6  =   {{{-1,0}, COL}, {{-2,0}, COL}, {{-2,-1}, COL}, {{-2,-2}, COL}, {{-1,-2}, COL}, {{-1,-1}, COL},
                                        {{2,0}, COL}, {{3,0}, COL}, {{3,1}, COL}, {{3,2}, COL}, {{2,2}, COL}, {{2,1}, COL}};

//Devuelve una instancia de fabrica de objetos atemporales, creandola si no existe
FabricaObjetosAtemporales* FabricaObjetosAtemporales::Instancia(){
    if(pFabrica == 0){                                  //Primera llamada
        pFabrica = new FabricaObjetosAtemporales();     //Se crea la instancia de FabricaObjetosAtemporales
    }
    return pFabrica;
}

//Constructor
FabricaObjetosAtemporales::FabricaObjetosAtemporales() {}

//Destructor
FabricaObjetosAtemporales::~FabricaObjetosAtemporales() {}

//Dada una id, fabrica el objeto correspondiente
ObjetoAtemporal* FabricaObjetosAtemporales::fabricar(int id, vector<int> pos) {
    ObjetoAtemporal* objeto = nullptr;
    Textura* t = nullptr;
    Sprite* s = nullptr;
    map<vector<int>, int> mapaCol;
    vector<vector<int>> vectorCol;

    switch(id) {
        case 0:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_0);
            s = new Sprite(t, 0, 0, TAM_0[0], TAM_0[1]);
            s->setOrigin(ORIGEN_0[0], ORIGEN_0[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_0.begin(); i!=MAPA_0.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 1:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_1);
            s = new Sprite(t, 0, 0, TAM_1[0], TAM_1[1]);
            s->setOrigin(ORIGEN_1[0], ORIGEN_1[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_1.begin(); i!=MAPA_1.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 2:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_2);
            s = new Sprite(t, 0, 0, TAM_2[0], TAM_2[1]);
            s->setOrigin(ORIGEN_2[0], ORIGEN_2[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_2.begin(); i!=MAPA_2.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 3:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_3);
            s = new Sprite(t, 0, 0, TAM_3[0], TAM_3[1]);
            s->setOrigin(ORIGEN_3[0], ORIGEN_3[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_3.begin(); i!=MAPA_3.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 4:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_4);
            s = new Sprite(t, 0, 0, TAM_4[0], TAM_4[1]);
            s->setOrigin(ORIGEN_4[0], ORIGEN_4[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_4.begin(); i!=MAPA_4.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 5:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_5);
            s = new Sprite(t, 0, 0, TAM_5[0], TAM_5[1]);
            s->setOrigin(ORIGEN_5[0], ORIGEN_5[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_5.begin(); i!=MAPA_5.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 6:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_6);
            s = new Sprite(t, 0, 0, TAM_6[0], TAM_6[1]);
            s->setOrigin(ORIGEN_6[0], ORIGEN_6[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_6.begin(); i!=MAPA_6.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 7:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_7);
            s = new Sprite(t, 0, 0, TAM_0[0], TAM_0[1]);
            s->setOrigin(ORIGEN_0[0], ORIGEN_0[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_0.begin(); i!=MAPA_0.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 8:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_8);
            s = new Sprite(t, 0, 0, TAM_1[0], TAM_1[1]);
            s->setOrigin(ORIGEN_1[0], ORIGEN_1[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_1.begin(); i!=MAPA_1.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 9:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_9);
            s = new Sprite(t, 0, 0, TAM_2[0], TAM_2[1]);
            s->setOrigin(ORIGEN_2[0], ORIGEN_2[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_2.begin(); i!=MAPA_2.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 10:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_10);
            s = new Sprite(t, 0, 0, TAM_3[0], TAM_3[1]);
            s->setOrigin(ORIGEN_3[0], ORIGEN_3[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_3.begin(); i!=MAPA_3.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 11:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_11);
            s = new Sprite(t, 0, 0, TAM_5[0], TAM_5[1]);
            s->setOrigin(ORIGEN_5[0], ORIGEN_5[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_5.begin(); i!=MAPA_5.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 12:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_12);
            s = new Sprite(t, 0, 0, TAM_6[0], TAM_6[1]);
            s->setOrigin(ORIGEN_6[0], ORIGEN_6[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_6.begin(); i!=MAPA_6.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 13:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_13);
            s = new Sprite(t, 0, 0, TAM_0[0], TAM_0[1]);
            s->setOrigin(ORIGEN_0[0], ORIGEN_0[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_0.begin(); i!=MAPA_0.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 14:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_14);
            s = new Sprite(t, 0, 0, TAM_1[0], TAM_1[1]);
            s->setOrigin(ORIGEN_1[0], ORIGEN_1[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_1.begin(); i!=MAPA_1.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 15:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_15);
            s = new Sprite(t, 0, 0, TAM_2[0], TAM_2[1]);
            s->setOrigin(ORIGEN_2[0], ORIGEN_2[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_2.begin(); i!=MAPA_2.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 16:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_16);
            s = new Sprite(t, 0, 0, TAM_3[0], TAM_3[1]);
            s->setOrigin(ORIGEN_3[0], ORIGEN_3[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_3.begin(); i!=MAPA_3.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 17:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_17);
            s = new Sprite(t, 0, 0, TAM_5[0], TAM_5[1]);
            s->setOrigin(ORIGEN_5[0], ORIGEN_5[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_5.begin(); i!=MAPA_5.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
        case 18:
            //Crea la textura y el sprite
            t = new Textura(SPRITE_18);
            s = new Sprite(t, 0, 0, TAM_6[0], TAM_6[1]);
            s->setOrigin(ORIGEN_6[0], ORIGEN_6[1]);
            s->setScale(ESCALA, ESCALA);

            //Recorre el mapa de colisiones y genera el mapa con las coordenadas finales
            for(auto i=MAPA_6.begin(); i!=MAPA_6.end(); ++i) {
                vector<int> coords = i->first;
                coords[0] += pos[0];
                coords[1] += pos[1];
                vectorCol.push_back(coords);
                mapaCol[coords] = i->second;
            }
            objeto = new ObjetoAtemporal(t, s, vectorCol, mapaCol);
            objeto->setPosicion({(float)pos[0]*TAM_TILES, (float)pos[1]*TAM_TILES});
            break;
    }

    return objeto;
}