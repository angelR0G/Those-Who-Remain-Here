#include "ParticulaDireccional.h"

const int TIEMPO_RESTANTE_FADE  = 300;

/*Constructor. Recibe la posicion inicial, el color de la particula, la velocidad media de desplazamiento, 
*el tiempo de vida en milisegundos, la direccion de desplazamiento, su variacion y la variacion de la velocidad.
*/
ParticulaDireccional::ParticulaDireccional(vector<float> pos, vector<int> col, float vel, int tiempo, vector<float> dir, float variacion, float variacionVel) {
    //Normaliza la variacion por si acaso
    variacion       = max(min(1.0f, variacion), 0.0f);
    variacionVel    = max(min(1.0f, variacionVel), 0.0f);

    //Aleatoriamente, modifica la direccion en base a la variacion
    if(variacion > 0.0001) {
        int v           = rand() % (int)(variacion*2000) - (int)(variacion*1000);
        float angulo    = atan2(dir[1], dir[0]);
        angulo += (float)(v/1000.0)*M_PI;
        dir[0] = cos(angulo);
        dir[1] = sin(angulo);
    }

    direccion = dir;

    //Ahora calcula la velocidad del mismo modo
    if(variacionVel > 0.0001) {
        int v   = rand() % (int)(variacionVel*2000) - (int)(variacionVel*1000);
        vel     = vel + (float)(v/1000.0)*vel;
    }

    velocidad = vel;

    //Inicializa las variables basicas
    if(pos.size() == 2) {
        posNueva        = pos;
    }
    else {
        posNueva        = {0.0, 0.0};
    }
    posAnterior         = posNueva;
    tiempoVida          = tiempo;

    //Calcula un tamanyo aleatorio
    tamanyo = rand() % 3 + 2;       //Se utiliza al poner la posicion de los vertices

    //Crea la particula y le asigna sus propiedades
    particula           = new ArrayVertices(1, 3);

    if(col.size() == 4) {
        color = col;
    }
    else {
        color = {255, 255, 255, 255};
    }

    //Coloca los vertices
    vector<float> dirPerp = {direccion[1], direccion[0]*(-1.0f)};
    particula->setArrayColor(color[0], color[1], color[2], color[3]);
    particula->setVertexPosition(0, (int)pos[0] + direccion[0]*tamanyo, (int)pos[1] + direccion[1]*tamanyo);
    particula->setVertexPosition(1, (int)pos[0] + dirPerp[0]*tamanyo, (int)pos[1] + dirPerp[1]*tamanyo);
    particula->setVertexPosition(2, (int)pos[0] - dirPerp[0]*tamanyo, (int)pos[1] - dirPerp[1]*tamanyo);
}

//Destructor
ParticulaDireccional::~ParticulaDireccional() {
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
void ParticulaDireccional::update(Tiempo t) {
    if(!destruir) {
        //Actualiza las posiciones
        posAnterior = posNueva;

        posNueva[0] += direccion[0]*velocidad*t.segundos();
        posNueva[1] += direccion[1]*velocidad*t.segundos();

        //Actualiza el tiempo de vida de la particula
        tiempoVida -= t.milisegundos();

        //Modifica la opacidad segun el tiempo de vida restante
        if(tiempoVida < TIEMPO_RESTANTE_FADE && tiempoVida > 0) {
            particula->setArrayColor(color[0], color[1], color[2], (tiempoVida/(float)TIEMPO_RESTANTE_FADE)*255);
        }

        //Comprueba si se debe destruir
        if(tiempoVida <= 0) {
            destruir = true;
        }
    }
}

//Dibuja la particula
void ParticulaDireccional::dibujar(float percent) {
    if(particula != nullptr && !destruir) {
        for(unsigned int i=0; i<3; i++) {
            float xInterpolada = posAnterior[0]*(1-percent) + posNueva[0]*percent;
            float yInterpolada = posAnterior[1]*(1-percent) + posNueva[1]*percent;
            vector<float> dirPerp;
            
            switch(i) {
                case 0:
                    particula->setVertexPosition(i, (int)xInterpolada + direccion[0]*tamanyo, (int)yInterpolada + direccion[1]*tamanyo);
                    break;
                case 1:
                    dirPerp = {direccion[1], direccion[0]*(-1.0f)};
                    particula->setVertexPosition(i, (int)xInterpolada + dirPerp[0]*tamanyo, (int)yInterpolada +dirPerp[1]*tamanyo);
                    break;
                case 2:
                    dirPerp = {direccion[1]*(-1.0f), direccion[0]};
                    particula->setVertexPosition(i, (int)xInterpolada + dirPerp[0]*tamanyo, (int)yInterpolada +dirPerp[1]*tamanyo);
                    break;
            }
        }

        particula->draw();
    }
}