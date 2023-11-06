#include "ParticulaEsencia.h"

const int TIEMPO_ESPERA             = 600;          //Tiempo desde que se detienen las particulas hasta que viajan hacia el jugador
const float VEL_APARICION           = 200.0;        //Velocidad a la que saltan las particulas del personaje
const float VEL_RECOLECCION         = 400.0;        //Velocidad a la que se mueven hacia el jugador
const float DISTANCIA_FADE          = 100.0;        //Distancia hasta el objetivo en la que comienzan a desaparecer
const float DISTANCIA_DESAPARICION  = 20.0;         //Distancia a la que la particula es destruida

/*Constructor. Recibe la posicion inicial, el color de la particula 
*y el tiempo en milisegundos antes de ir hacia el jugador.
*/
ParticulaEsencia::ParticulaEsencia(vector<float> pos, vector<int> col, int tiempo) {
    //Inicializa las variables basicas
    if(pos.size() == 2) {
        posNueva        = pos;
    }
    else {
        posNueva        = {0.0, 0.0};
    }
    posAnterior         = posNueva;
    tiempoEstatico      = tiempo;
    tiempoActual        = tiempoEstatico;

    //Calcula una velocidad, direccion y un tamanyo aleatorios
    int random = rand() % (int)VEL_APARICION - (int)(VEL_APARICION/2.0);
    velocidad  = VEL_APARICION + (float)random;


    direccion.push_back((float)(rand() % 21 - 10));
    direccion.push_back((float)(rand() % 21 - 10));
    float norm = sqrt(pow(direccion[0], 2.0) + pow(direccion[1], 2.0));
    if(norm != 0) {
        direccion[0] /= norm;
        direccion[1] /= norm;
    }

    tamanyo = rand() % 3 + 1;       //Se utiliza al poner la posicion de los vertices

    //Crea la particula y le asigna sus propiedades
    particula           = new ArrayVertices(4, 4);

    if(col.size() == 4) {
        color = col;
    }
    else {
        color = {255, 255, 255, 255};
    }
    particula->setArrayColor(color[0], color[1], color[2], color[3]);
    particula->setVertexPosition(0, (int)pos[0], (int)pos[1] - tamanyo);
    particula->setVertexPosition(1, (int)pos[0] + tamanyo, (int)pos[1]);
    particula->setVertexPosition(2, (int)pos[0], (int)pos[1] + tamanyo);
    particula->setVertexPosition(3, (int)pos[0] - tamanyo, (int)pos[1]);
}

//Destructor
ParticulaEsencia::~ParticulaEsencia() {
    posNueva.clear();
    posAnterior.clear();
    direccion.clear();
    color.clear();

    if(particula != nullptr) {
        delete particula;
        particula = nullptr;
    }
}

//Actualiza la particula
void ParticulaEsencia::update(Tiempo t) {
    if(!destruir) {
        //Comprueba el estado de la particula
        if(tiempoActual > 0) {                      //Esta saliendo del enemigo
            //Calcula la velocidad
            float v = velocidad*(tiempoActual/(float)tiempoEstatico);

            //Actualiza las posiciones
            posAnterior = posNueva;

            posNueva[0] += direccion[0]*v*t.segundos();
            posNueva[1] += direccion[1]*v*t.segundos();

            //Actualiza el tiempo de vida de la particula
            tiempoActual -= t.milisegundos();
        }
        else if(tiempoActual > 0-TIEMPO_ESPERA) {
            //La particula se mantiene quita durante un momento
            tiempoActual -= t.milisegundos();
            posAnterior = posNueva;
        }
        else {
            //La particula viaja hacia el jugador
            float dist  = sqrt(pow(posJugador[0]-posNueva[0], 2) + pow(posJugador[1]-posNueva[1], 2));
            direccion   = {(posJugador[0]-posNueva[0])/dist, (posJugador[1]-posNueva[1])/dist};

            //Actualiza las posiciones
            posAnterior = posNueva;

            posNueva[0] += direccion[0]*VEL_RECOLECCION*t.segundos();
            posNueva[1] += direccion[1]*VEL_RECOLECCION*t.segundos();

            //Calcula la distancia de nuevo y si esta cerca del jugador desaparece
            dist  = sqrt(pow(posJugador[0]-posNueva[0], 2) + pow(posJugador[1]-posNueva[1], 2));

            if(dist < DISTANCIA_DESAPARICION) {
                destruir = true;
            }
            else if(dist < DISTANCIA_FADE){
                //Actualiza la opacidad conforme se acerca
                particula->setArrayColor(color[0], color[1], color[2], dist/DISTANCIA_FADE*255);
            }
        }
    }
}

//Dibuja la particula
void ParticulaEsencia::dibujar(float percent) {
    if(particula != nullptr && !destruir) {
        for(unsigned int i=0; i<4; i++) {
            float xInterpolada = posAnterior[0]*(1-percent) + posNueva[0]*percent;
            float yInterpolada = posAnterior[1]*(1-percent) + posNueva[1]*percent;
            
            switch(i) {
                case 0:
                    particula->setVertexPosition(i, (int)xInterpolada, (int)yInterpolada - tamanyo);
                    break;
                case 1:
                    particula->setVertexPosition(i, (int)xInterpolada + tamanyo, (int)yInterpolada);
                    break;
                case 2:
                    particula->setVertexPosition(i, (int)xInterpolada, (int)yInterpolada + tamanyo);
                    break;
                case 3:
                    particula->setVertexPosition(i, (int)xInterpolada - tamanyo, (int)yInterpolada);
                    break;
            }
        }

        particula->draw();
    }
}