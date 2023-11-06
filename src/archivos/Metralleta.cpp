#include "Metralleta.h"

const int VEL_BALA = 500;

const vector<int> MEJORAS_DANYO = {3, 3, 3, 3, 3, 3};
const vector<float> MEJORAS_PRECISION = {0.03, 0.03, 0.03};
const vector<int> MEJORAS_CADENCIA = {25, 25, 25};
const vector<vector<int>> PRECIO_MEJORAS_DANYO = {{0, 2, 1, 4}, {0, 3, 2, 3}, {3, 4, 4, 6}, {5, 8, 3, 2}, {6, 10, 7, 4}, {8, 7, 7, 7}};
const vector<vector<int>> PRECIO_MEJORAS_PRECISION= {{0, 6, 1, 2}, {3, 9, 5, 5}, {6, 10, 7, 8}};
const vector<vector<int>> PRECIO_MEJORAS_CADENCIA = {{1, 3, 2, 5}, {3, 4, 4, 10}, {7, 5, 8, 13}};

//Constructor de la clase pistola. Recibe:
//Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG
Metralleta::Metralleta(int id, string ruta, string nombre, int dmg, int alc, float cad, float rec, int mun, float pre, int runaP, int runaG, vector<int> recorte, float escala) : Arma(id, ruta, nombre, dmg, alc, cad, rec, mun, pre, runaP, runaG, recorte, escala){
    setMunicion(mun);
    sonidos->addSonido("recargar",new Buffer("resources/sonidos/recargar.wav"),60);
    sonidos->addSonido("disparo",new Buffer("resources/sonidos/Metralleta.ogg"),60);
}

Metralleta::~Metralleta(){
}

//Cuando se pulsa el boton izq del raton se llama a esta funciion
//Si no pasa un tiempo estre bala y bala no se anade la bala al vector por lo que no dispara
//Si se queda sin balas llama al contador de la recarga
void Metralleta::disparar(int elapsedTime){
    EstadoPartida *estado = EstadoPartida::Instancia();
    VentanaRender *ventana = VentanaRender::Instancia();
    long long t1 = chrono::high_resolution_clock::now().time_since_epoch().count();
    srand((unsigned int)t1);
    Sprite* sprite = new Sprite(estado->getTexturas(2), 0, 16*11, 16, 16);
    
    if(cooldownAtaque >= cadencia){
        if(puedeDisparar()){        
            sonidos->play("disparo");
            float danyo             = getDanyoTotal();
            float danyoFinal        = danyo;
            bool dispararDosVeces   = false;
            vector<float> posRaton  = ventana->mapPixelToCoords(ventana->getPosicionRaton());
            vector<float> posIni    = spriteArma->getPosition();

            //Se calcula la direccion de la bala
            vector<float> direccion;
            vector<float> direccionN={-1,-1};
            direccion.push_back(posRaton[0] - posIni[0]);
            direccion.push_back(posRaton[1] - posIni[1]);
            float distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));

            //Se calcula la dispersion de las balas
            float angulo = atan2(direccion[1],direccion[0]);
            float anguloNuevo = 0;
            float numRand = (-precision/2)+(float)rand() / (((float)RAND_MAX/precision));
            
            anguloNuevo = angulo+numRand;
            direccion[0] = cos(anguloNuevo);
            direccion[1] = sin(anguloNuevo);
            distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
            direccionN[0] = direccion[0]/distNorm;
            direccionN[1] = direccion[1]/distNorm;

            Proyectil *p = nullptr;
            Proyectil *p1 = nullptr;

            vector<int> runasElementales = {0, 0, 0};
            //Comprueba que runas elementales tiene equipadas
            if(pequenyas[0]){
                runasElementales[0] = 1;
            }else if(grandes[0]){
                runasElementales[0] = 2;
            }

            if(pequenyas[1]){
                runasElementales[1] = 1;
            }else if(grandes[1]){
                runasElementales[1] = 2;
            }

            if(pequenyas[2]){
                runasElementales[2] = 1;
            }else if(grandes[2]){
                runasElementales[2] = 2;
            }

            //Si se equipa disparar dos balas a la vez se calcula el nuevo danyo
            if(pequenyas[4]){
                danyoFinal-=danyo*0.40;
                dispararDosVeces = true;

            }else if(grandes[4]){
                danyoFinal-=danyo*0.20;
                dispararDosVeces = true;

            }
            //Si no se equipa balas perforantes dispara las normales
            if(!pequenyas[3] && !grandes[3]){
                float a = atan2(direccion[1],direccion[0]);
                a = a*180/M_PI;
                sprite->setRotation(a);
                p = new ProyectilRecto(0, danyo, sprite, posIni, direccionN, VEL_BALA, alcance, runasElementales);
                //Si tiene dos balas a la vez se calcula la direccion de esa nueva bala
                if(dispararDosVeces){
                    Sprite* sprite1 = new Sprite(estado->getTexturas(2), 0, 16*11, 16, 16);
                    float numRand = (-precision/2)+(float)rand() / (((float)RAND_MAX/precision));
                    anguloNuevo = angulo+numRand;
                    direccion[0] = cos(anguloNuevo);
                    direccion[1] = sin(anguloNuevo);
                    distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
                    direccionN[0] = direccion[0]/distNorm;
                    direccionN[1] = direccion[1]/distNorm;
                    p1 = new ProyectilRecto(0, danyoFinal, sprite1, posIni, direccionN, VEL_BALA, alcance, runasElementales);
                }
            }else{
                if(pequenyas[3]){
                    //Perfora solo un enemigo
                    //Se crea el sprite de la bala y se rota para que se dispare el sprite segun la direccion de disparo
                    sprite = new Sprite(estado->getTexturas(2), 8*16 + 2, 12*16 + 6, 10, 5);
                    float a = atan2(direccion[1],direccion[0]);
                    a = a*180/M_PI;
                    sprite->setRotation(a);
                    sprite->setScale(2, 2);
                    
                    p = new ProyectilPerforante(0, danyoFinal, sprite, posIni, direccion, VEL_BALA, alcance, runasElementales, 1);

                    //Si aparte tine la mejora de disparar dos veces se genera otra bala perforante
                    if(dispararDosVeces){
                        Sprite* sprite1 = new Sprite(estado->getTexturas(2), 8*16 + 2, 12*16 + 6, 10, 5);
                        float numRand = (-precision/2)+(float)rand() / (((float)RAND_MAX/precision));
                        anguloNuevo = angulo+numRand;
                        direccion[0] = cos(anguloNuevo);
                        direccion[1] = sin(anguloNuevo);
                        distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
                        direccionN[0] = direccion[0]/distNorm;
                        direccionN[1] = direccion[1]/distNorm;

                        //Se rota el sprite segun la direccion de disparo
                        float a = atan2(direccion[1],direccion[0]);
                        a = a*180/M_PI;
                        sprite1->setRotation(a);
                        sprite1->setScale(2, 2);

                        p1 = new ProyectilPerforante(0, danyoFinal, sprite1, posIni, direccion, VEL_BALA, alcance, runasElementales, 1);
                    }
                }else if(grandes[3]){
                    //Perfora a todos los enemigos
                    //Se crea el sprite de la bala y se rota para que se dispare el sprite segun la direccion de disparo
                    sprite = new Sprite(estado->getTexturas(2), 14*16, 12*16, 16, 16);
                    float angulo = atan2(direccion[1],direccion[0]);
                    float a = atan2(direccion[1],direccion[0]);
                    a = a*180/M_PI;
                    sprite->setRotation(a);
                    sprite->setScale(2, 2);

                    p = new ProyectilPerforante(0, danyoFinal, sprite, posIni, direccion, VEL_BALA, alcance, runasElementales, -1);

                    //Si aparte tine la mejora de disparar dos veces se genera otra bala perforante
                    if(dispararDosVeces){
                        Sprite* sprite1 = new Sprite(estado->getTexturas(2), 8*16 + 2, 12*16 + 6, 10, 5);
                        float numRand = (-precision/2)+(float)rand() / (((float)RAND_MAX/precision));
                        anguloNuevo = angulo+numRand;
                        direccion[0] = cos(anguloNuevo);
                        direccion[1] = sin(anguloNuevo);
                        distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
                        direccionN[0] = direccion[0]/distNorm;
                        direccionN[1] = direccion[1]/distNorm;

                        //Se rota el sprite segun la direccion de disparo
                        float a = atan2(direccion[1],direccion[0]);
                        a = a*180/M_PI;
                        sprite1->setRotation(a);
                        sprite1->setScale(2, 2);

                        p1 = new ProyectilPerforante(0, danyoFinal, sprite1, posIni, direccion, VEL_BALA, alcance, runasElementales, -1);
                    }
                }
            }
            estado->anyadirProyectil(p);
            if(p1 != nullptr){
                estado->anyadirProyectil(p1);
            }
            balasDisparadas--;
            
        }else{
            coolDownRecarga(elapsedTime);    //No tiene mas balas
        }
        cooldownAtaque = 0.f;        //Una vez disparada una bala se reinicia el tiempo entre bala y bala
    }
}

//Resetea la municion del arma para recargarla
void Metralleta::recargar(){
    float municionFinal = municionOriginal;
    if(pequenyas[5]){
        municionFinal = municionOriginal + municionOriginal*0.4;
    }else if(grandes[5]){
        municionFinal = municionOriginal + municionOriginal*0.8;
    }
    balasDisparadas = municionFinal;
    municion        = municionFinal;
    cooldownAtaque = cadencia;
    cooldownRecarga = 0.f;
    recargando = false;
}

//Devuelve la municion maxima del arma
int Metralleta::getMunicion(){
    float municionFinal = municionOriginal;
    if(pequenyas[5]){
        municionFinal = municionOriginal + municionOriginal*0.4;
    }else if(grandes[5]){
        municionFinal = municionOriginal + municionOriginal*0.8;
    }
    return municionFinal;
}


//Pone la municion y las balas restantes segun lo que se pase
void Metralleta::setMunicion(int mun){
    float municionFinal = mun;
    if(pequenyas[5]){
        municionFinal = mun + mun*0.4;
    }else if(grandes[5]){
        municionFinal = mun + mun*0.8;
    }
    municion = municionFinal;
    balasDisparadas = municionFinal;
}

/*
  INFORMACION RUNAS
*/
vector<string> Metralleta::getNombreRunas(){
    vector<string> nombres;
    nombres.push_back("Balas perforantes");
    nombres.push_back("Canyon doble");
    nombres.push_back("Cargador ampliado");
    return nombres;
}

vector<string> Metralleta::getDescRunasNormales(){
    vector<string> desc;
    desc.push_back("Balas que perforan a 1 objetivo");
    desc.push_back("Dispara una segunda bala con danyo de -40%");
    desc.push_back("Aumenta el tamanyo del cargador un 40%");
    return desc;
}

vector<string> Metralleta::getDescRunasGrandes(){
    vector<string> desc;
    desc.push_back("Balas que perforan a todos los objetivos");
    desc.push_back("Dispara una segunda bala con danyo de -20%");
    desc.push_back("Aumenta el tamanyo del cargador un 80%");
    return desc;
}

vector<int> Metralleta::getMaterialesRuna(int runa){
    vector<int> materiales;
    switch (runa)
    {
        //Balas perforantes
        case 0:
            materiales = {10, 12};
            break;
        //disparo doble
        case 1:
            materiales = {11, 12};
            break;
        //aumento cargador
        case 2:
            materiales = {10, 11};
            break;
        default:
            break;
    }
    return materiales;
}

//Devuelve el area de recorte de las runas del arma
vector<int> Metralleta::getRecorteSpriteRuna(int runa){
    vector<int> recorte = {0, 0};
    switch (runa)
    {
        //perforacion
        case 0:
            recorte = {0, 1*64};
            break;
        //dos balas
        case 1:
            recorte = {1*64, 2*64};
            break;
        //mayor cargador
        case 2:
            recorte = {2*64, 1*64};
            break;
        default:
            break;
    }
    return recorte;
}

//Metodo que devuelve los strings de las posibles mejoras
vector<string> Metralleta::getTextoMejoras(){
    vector<string> tex;
    tex.push_back("Danyo");
    tex.push_back("Precision");
    tex.push_back("Cadencia");
    return tex;
}

//Metodo que devuelve la cantidad de mejoras que tiene de cada tipo
vector<int> Metralleta::getMejorasArma(){
    return {getMejorasDanyo(), getMejorasPrecision(), getMejorasCadencia()};
}

//Se le pasa el indice de la mejora que quieres aumentar y el indice de la mejora a comprar
void Metralleta::setMejorasArma(int indice, int indMejora){
    switch (indice)
    {
        case 0:
            //Danyo
            if(indMejora<6){
                mejorasArma[0]++;
                danyo+=MEJORAS_DANYO[indMejora];
            }
            break;
        case 1:
            //precision
            if(indMejora<3){
                mejorasArma[3]++;
                precision-=MEJORAS_PRECISION[indMejora];
            }
            break;
        case 2:
            //cadencia
            if(indMejora<3){
                mejorasArma[2]++;
                cadencia -= MEJORAS_CADENCIA[indMejora];
            }
            break;
        default:
            break;
    }

}

vector<int> Metralleta::getMaterialesMejora(int indice, int indMejora){
    vector<int> mat = {};
    switch (indice)
    {
        case 0:
            if(indMejora<6){
                mat = PRECIO_MEJORAS_DANYO[indMejora];
            }
            break;
        case 1:
            if(indMejora<3){
                mat = PRECIO_MEJORAS_PRECISION[indMejora];
            }
            break;
        case 2:
            if(indMejora<3){
                mat = PRECIO_MEJORAS_CADENCIA[indMejora];
            }
            break;
        
        default:
            break;
    }
    return mat;
}