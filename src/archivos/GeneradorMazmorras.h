#pragma once

#include <stack>
#include <vector>
#include <iostream>

#include "PlantillaSala.h"
#include "Sala.h"
#include "../fachada/Textura.h"
#include "../tinyxml2/LectorXML.h"

using namespace std;

class GeneradorMazmorras {
    public:
        GeneradorMazmorras(string);

        ~GeneradorMazmorras();

        bool generarMazmorra();
        void dibujarMazmorra();
        vector<vector<Sala*>> getMazmorra();
        vector<int> getSalaInicial();
        vector<int> getSalaFinal();
        void setTexturas(Textura*, Textura*);

        PlantillaSala* buscarPlantilla(string);
        void plantillaUsada(string);
        PlantillaSala* getPlantillaPorEntrada(int, bool);
        bool comprobarMazmorra();
        void resetearMazmorra();
        bool desplazarMazmorra(int, stack<vector<int>>*);
        int getNivelMateriales();
        int getSigArea();

    private:
        vector<PlantillaSala*> listaPlantillas;             //Lista de todas las plantillas del area
        vector<PlantillaSala*> plantillasDisponibles;       //Lista con las plantillas que aun no han sido usadas
        vector<PlantillaSala*> plantillasUsadas;            //Lista con las plantillas que ya han sido usadas
        vector<string> plantillasBasicas;                   //Lista con los nombres de las salas basicas
        vector<vector<PlantillaSala*>> mazmorra;            //Matriz con la combinacion de plantillas que forma la mazmorra

        int nivMaterial;                                    //Nivel maximo de los materiales de esta area
        int sigArea;                                        //Indice del area posterior a la de la plantilla
        int numSalas;                                       //Cantidad de salas unicas de la mazmorra
        int inicioX;                                        //Posicion de la sala de inicio
        int inicioY;

        int finalX;                                         //Posicion de la sala del jefe
        int finalY;

        Textura* tilesheet;                                 //Referencias a las texturas que necesitan las salas
        Textura* puertaTapiada;
};