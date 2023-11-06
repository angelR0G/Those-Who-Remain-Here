#include "ParticulaSimpleCuadrada.h"

/*Constructor. Recibe la posicion inicial, el color de la particula, la velocidad media de desplazamiento 
*y el tiempo de vida en milisegundos.
*/
ParticulaSimpleCuadrada::ParticulaSimpleCuadrada(vector<float> pos, vector<int> col, float vel, int tiempo) {
    //Inicializa las variables basicas
    if(pos.size() == 2) {
        posNueva        = pos;
    }
    else {
        posNueva        = {0.0, 0.0};
    }
    posAnterior         = posNueva;
    tiempoVida          = tiempo;

    //Calcula una velocidad, direccion y un tamanyo aleatorios
    if(vel > 0) {
        int random = rand() % (int)vel - (int)(vel/2.0);
        velocidad  = vel + (float)random;
    }
    else {
        velocidad   = 0.0;
    }

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
    particula->setVertexPosition(0, (int)pos[0] - tamanyo, (int)pos[1] - tamanyo);
    particula->setVertexPosition(1, (int)pos[0] + tamanyo, (int)pos[1] - tamanyo);
    particula->setVertexPosition(2, (int)pos[0] + tamanyo, (int)pos[1] + tamanyo);
    particula->setVertexPosition(3, (int)pos[0] - tamanyo, (int)pos[1] + tamanyo);
}

//Destructor
ParticulaSimpleCuadrada::~ParticulaSimpleCuadrada() {
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
void ParticulaSimpleCuadrada::update(Tiempo t) {
    if(!destruir) {
        //Actualiza las posiciones
        posAnterior = posNueva;

        posNueva[0] += direccion[0]*velocidad*t.segundos();
        posNueva[1] += direccion[1]*velocidad*t.segundos();

        //Actualiza el tiempo de vida de la particula
        tiempoVida -= t.milisegundos();

        //Modifica la opacidad segun el tiempo de vida restante
        if(tiempoVida < 700 && tiempoVida > 0) {
            particula->setArrayColor(color[0], color[1], color[2], (tiempoVida/700.0)*255);
        }

        //Comprueba si se debe destruir
        if(tiempoVida <= 0) {
            destruir = true;
        }
    }
}

//Dibuja la particula
void ParticulaSimpleCuadrada::dibujar(float percent) {
    if(particula != nullptr && !destruir) {
        for(unsigned int i=0; i<4; i++) {
            float xInterpolada = posAnterior[0]*(1-percent) + posNueva[0]*percent;
            float yInterpolada = posAnterior[1]*(1-percent) + posNueva[1]*percent;
            
            switch(i) {
                case 0:
                    particula->setVertexPosition(i, (int)xInterpolada - tamanyo, (int)yInterpolada - tamanyo);
                    break;
                case 1:
                    particula->setVertexPosition(i, (int)xInterpolada + tamanyo, (int)yInterpolada - tamanyo);
                    break;
                case 2:
                    particula->setVertexPosition(i, (int)xInterpolada + tamanyo, (int)yInterpolada + tamanyo);
                    break;
                case 3:
                    particula->setVertexPosition(i, (int)xInterpolada - tamanyo, (int)yInterpolada + tamanyo);
                    break;
            }
        }

        particula->draw();
    }
}