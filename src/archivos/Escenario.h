#pragma once

#include <map>
#include <vector>
#include <math.h>

#include "../tinyxml2/LectorXML.h"
#include "../fachada/ArrayVertices.h"
#include "../fachada/Textura.h"
#include "../fachada/VentanaRender.h"
#include "../fachada/Sprite.h"

using namespace std;

class Escenario {
    public:
        Escenario(string, Textura*, vector<int>);

        ~Escenario();

        vector<int> getTamanyoMapa();
        vector<int> getTamanyoTile();
        vector<float> centrarCamara(float, float);
        void dibujar();
        void cargarTilesheet(Textura*);
        void almacenaSpriteMapa(vector<int>, unsigned long);
        map<vector<int>, int>* getMapaColisiones();
        int getIdColision(int);
        int getConstTamTiles();

        void anyadirColision(map<vector<int>, int>*);
        void eliminarColision(vector<vector<int>>*);
    protected:
        map<vector<int>, Sprite*> mapa;         //Mapa con los tiles del escenario
        map<vector<int>, int> mapaColisiones;   //Mapa con las id de las colisiones del escenario
        map<int, vector<int>> idTiles;          //Posicion de cada tile en el tilesheet segun su id
        string nombre;                          //Nombre del escenario
        vector<int> tamanyoMapa;                //Tamanyo del mapa en tiles
        vector<int> tamanyoTile;                //Tamanyo de los tiles en pixeles
        ArrayVertices* fondo;                   //Fondo del mapa
        Textura* tilesheet;                     //Textura con el tilsesheet del mapa
};