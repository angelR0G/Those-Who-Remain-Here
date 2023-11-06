#include "CumuloMateriales.h"

const int PROB_OBJ_EXPECIAL = 50;
const float DIST_INTER      = 80.0;

const string ANIM_IDLE      = "HuevosIdle";
const string ANIM_ABRIR     = "HuevosAbrir";

//Constructor
CumuloMateriales::CumuloMateriales(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true) {
    abierto             = false;
    reproduciendoAnim   = true;

    animacion = new Animacion(sprite);
    animacion->addAnimation(ANIM_ABRIR, 50.0, 0, 0, 4, 1, 32, 32);
    animacion->addAnimation(ANIM_IDLE, 80.0, 0, 0, 3, 0, 32, 32);
    distInteraccion = DIST_INTER;
}

//Destructor
CumuloMateriales::~CumuloMateriales() {
    delete animacion;
}

//Actualiza las animaciones
void CumuloMateriales::update(Tiempo t) {
    //Comprueba que no se ha detenido el tiempo y no se ha roto
    if(!detenido && reproduciendoAnim) {
        if(abierto) {
            animacion->playOnce(ANIM_ABRIR, t.milisegundos());
        }
        else {
            animacion->play(ANIM_IDLE, t.milisegundos());
        }

        //Comprueba que ya ha terminado
        if(animacion->getFinished()) {
            reproduciendoAnim = false;
            sprite->setRect(32*3, 32*2, 32, 32);
        }
    }
}

//Abre los huevos y crea materiales aleatorios
void CumuloMateriales::interactuar() {
    //Comprueba que no se ha abierto
    if(!abierto) {
        //Modifica el estado del cofre
        abierto             = true;
        interactuable       = false;
        reproduciendoAnim   = true;

        //Genera el numero aleatorio que decidira el tipo de material
        int random = rand() % 100;

        if(random < PROB_OBJ_EXPECIAL) {
            //Genera un material especial
            //Selecciona aleatoriamente un material especial
            int idMat = rand() % 4 + 9;

            //Modifica la posicion de aparicion
            vector<float> posAux = posicion;
            posAux[0] += 16;

            //Crea el material que aparecera en el cofre
            Textura* tex = new Textura(FabricaMateriales::Instancia()->getRutaPorId(idMat));
            Sprite* s = new Sprite(tex, 0, 0, 32, 32);
            ObjetoSala* mat = new MaterialSuelto(posAux, s, tex, idMat, 1);

            //Anyade el material a la sala
            EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(mat);
        }
        else {
            //Genera tres materiales basicos
            for(int i=-1; i<2; i++) {
                //Selecciona aleatoriamente un material especial
                int idMat = rand() % 3;

                //Modifica la posicion de aparicion
                vector<float> posAux = posicion;
                posAux[0] += 32*i + 16;

                //Crea el material que aparecera en el cofre
                Textura* tex = new Textura(FabricaMateriales::Instancia()->getRutaPorId(idMat));
                Sprite* s = new Sprite(tex, 0, 0, 32, 32);
                ObjetoSala* mat = new MaterialSuelto(posAux, s, tex, idMat, 1);

                //Anyade el material a la sala
                EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(mat);
            }
        }
    }
}