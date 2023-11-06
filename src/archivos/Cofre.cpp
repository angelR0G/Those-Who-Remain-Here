#include "Cofre.h"

const int PROB_MAXIMA       = 100;
const int PROB_OBJETO       = 35;   //35%
const int PROB_ARMA         = 25;   //60%
const int PROB_ESCUDO       = 20;   //80%
const int PROB_MATERIAL     = 20;   //100%

const string ANIM_ABRIR     = "CofreAbrir";
const string ANIM_APARECER  = "CofreAparecer";

//Rutas de texturas
const string RUTA_ESCUDO   = "resources/sprites/personaje/icons.png";

//Constructor
Cofre::Cofre(Textura* tex, Sprite* s):ElementoEntorno(tex, s, true) {
    abierto             = false;
    reproduciendoAnim   = true;

    animacion = new Animacion(sprite);
    animacion->addAnimation(ANIM_ABRIR, 50.0, 0, 0, 4, 1, 48, 32);
    animacion->addAnimation(ANIM_APARECER, 50.0, 0, 2, 4, 3, 48, 32);
}

//Destructor
Cofre::~Cofre() {
    delete animacion;
}

void Cofre::update(Tiempo t) {
    if(reproduciendoAnim) {
        if(abierto) {
            animacion->playOnce(ANIM_ABRIR, t.milisegundos());
        }
        else {
            animacion->playOnce(ANIM_APARECER, t.milisegundos());
        }

        //Comprueba que ya ha terminado
        if(animacion->getFinished()) {
            reproduciendoAnim = false;
        }
    }
}

//Abre el cofre y crea un objeto aleatorio
void Cofre::interactuar() {
    //Comprueba que no se ha abierto
    if(!abierto && !reproduciendoAnim) {
        bool recompensaSeleccionada = false;
        //Modifica el estado del cofre
        abierto             = true;
        interactuable       = false;
        reproduciendoAnim   = true;

        //Genera el numero aleatorio que decidira la recompensa
        int random = rand() % PROB_MAXIMA;

        //El cofre contiene un objeto
        if(random < PROB_OBJETO) {
            recompensaSeleccionada = true;

            int id = FabricaObjetos::Instancia()->getIDDisponible();
            if(id != -1) {
                //Reserva la id para que no cree mas objetos iguales
                Objeto::idObjetoUsada(id);

                //Crea el objeto que aparecera en la sala
                Textura* tex = new Textura(FabricaObjetos::Instancia()->getRuta(id));
                Sprite* s = new Sprite(tex, 0, 0, 32, 32);
                ObjetoSala* obj = new ObjetoSuelto(posicion, s, tex, id);

                //Anyade el objeto a la sala
                EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(obj);
            }
        }
        random -= PROB_OBJETO;

        //El cofre contiene un arma
        if(!recompensaSeleccionada && random < PROB_ARMA) {
            recompensaSeleccionada = true;
            
            FabricaArmas* fabrica = FabricaArmas::Instancia();
            int maxArmas    = fabrica->getTotalArmas();
            int random      = rand() % maxArmas;
            vector<int> rect = fabrica->getRecorte(random);

            //Crea el arma que aparecera en el cofre
            Textura* tex = new Textura("resources/sprites/armas/" + fabrica->getRuta(random));
            Sprite* s = new Sprite(tex, 0, 0, rect[0], rect[1]);
            ObjetoSala* arma = new ArmaSuelta(posicion, s, tex, random);

            //Anyade el arma a la sala
            EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(arma);
        }
        random -= PROB_ARMA;

        //El cofre contiene un escudo
        if(!recompensaSeleccionada && random < PROB_ESCUDO) {
            recompensaSeleccionada = true;

            //Crea el escudo que aparecera en el cofre
            Textura* tex = new Textura(RUTA_ESCUDO);
            Sprite* s = new Sprite(tex, 0, 16, 16, 16);
            s->setScale(2.0, 2.0);
            ObjetoSala* esc = new EscudoSuelto(posicion, s, tex);

            //Anyade el escudo a la sala
            EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(esc);
        }
        random -= PROB_ESCUDO;

        //El cofre contiene un material
        if(!recompensaSeleccionada && random < PROB_MATERIAL) {
            recompensaSeleccionada = true;

            //Selecciona aleatoriamente un material especial
            int idMat = rand() % 4 + 9;

            //Crea el material que aparecera en el cofre
            Textura* tex = new Textura(FabricaMateriales::Instancia()->getRutaPorId(idMat));
            Sprite* s = new Sprite(tex, 0, 0, 32, 32);
            ObjetoSala* mat = new MaterialSuelto(posicion, s, tex, idMat, 1);

            //Anyade el material a la sala
            EstadoPartida::Instancia()->getSalaActual()->anyadirObjetoSala(mat);
        }

        //Actualiza el sprite
        //sprite->setRect(192, 32, 48, 32);
        
    }
}

//Devuelve si el cofre ha sido abierto
bool Cofre::getAbierto() {
    return abierto;
}