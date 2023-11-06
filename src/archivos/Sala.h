#pragma once

#include "Escenario.h"
#include "EstadoPartida.h"
#include "Enemigo.h"
#include "FabricaEnemigos.h"
#include "FabricaObjetosAtemporales.h"
#include "FabricaElementosEntorno.h"
#include "ObjetoSala.h"
#include "ObjetoAtemporal.h"
#include "Trampa.h"
#include "../fachada/Tiempo.h"

using namespace std;

class Sala : public Escenario {
    public:
        Sala(string, Textura*, vector<int>, Textura*);

        ~Sala();

        void comprobarEstadoSala();
        void setPuertasTapiadas(vector<bool>);
        static void actualizarPuertasTapiadas(vector<vector<Sala*>>*);
        bool existePuerta(int);
        bool getSuperada();
        bool getDescubierta();
        int getRecompensa();
        vector<Enemigo*>* getEnemigos();
        vector<int> getCoordsAparicion(int);
        vector<int> getCoordsRecompensa();
        void setRecompensa(int);
        bool cofresPorAbrir();

        void dibujar(float, int);
        void dibujarPuertasTapiadas();
        void dibujarEnemigos(float, int);
        void dibujarObjetosSoltados(float);
        void dibujarElementosEntorno();
        void dibujarObjetosAtemporales();

        void update(Tiempo);
        void updateEnemigos(Tiempo);
        void updateObjetosSoltados(Tiempo);
        void updateElementosEntorno(Tiempo);
        void updateObjetosAtemporales(Tiempo);
        void detenerSala();
        void reanudarSala();

        bool eliminarObjetoSala(ObjetoSala*);
        void anyadirObjetoSala(ObjetoSala*);
        ObjetoSala* getObjetoSalaCercano(vector<float>, float*);
        void anyadirElementoEntorno(ElementoEntorno*);
        ElementoEntorno* getElementoInteractuableCercano(vector<float>, float*);
        void anyadirObjAtemporal(class ObjetoAtemporal*);
        bool eliminarObjAtemporal(class ObjetoAtemporal*);
        class ObjetoAtemporal* getObjetoAtemporalCercano(vector<float>, float*);
        void anyadirEnemigo(Enemigo*);
        void generarRecompensa();
        vector<class MuroDestructible*> getMurosDestructibles();

        void entrarPortal();
        static Sala* crearSalaJefe(int);
        
    private:
        vector<int> puertas;                        //Vector que indica donde hay puertas y si requieren de alguna habilidad
        vector<vector<int>> coordsPuertas;          //Vector con las coordenadas de cada puerta
        vector<vector<int>> coordsAparicion;        //Vector con las coordenadas de aparicion del jugador por cada puerta
        vector<ElementoEntorno*> elementosEntorno;  //Vector con los elementos que hay en la sala
        vector<class ObjetoAtemporal*> objAtemporales;    //Vector con objetos que pueden venir de otra dimension
        vector<ObjetoSala*> objetosSoltados;        //Vector de objetos soltados en la sala
        vector<Enemigo*> enemigos;                  //Vector con los enemigos restantes de la sala
        vector<int> coordsRecompensa;               //Vector con las coordenadas donde aparece la recompensa
        int recompensa;                             //Id de la recompensa
        bool superada;                              //Indica si se ha derrotado a todos los enemigos
        bool descubierta;                           //Indica si se ha entrado en la sala
        vector<bool> puertasTapiadas;               //Vector que indica que puertas han sido tapiadas
        vector<Sprite*> spritesTapiadas;            //Sprites de las puertas tapiadas
        Textura* texSpritesTapiadas;                //Textura de los sprites de las puertas tapiadas
};