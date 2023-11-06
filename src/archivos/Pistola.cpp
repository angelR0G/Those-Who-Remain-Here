#include "Pistola.h"

const int VEL_BALA = 500;

const vector<int> MEJORAS_DANYO = {10, 10, 10, 10, 10, 10};
const vector<int> MEJORAS_ALCANCE = {75, 75, 75};
const vector<int> MEJORAS_CADENCIA = {50, 50, 50};
const vector<vector<int>> PRECIO_MEJORAS_DANYO = {{0, 2, 1, 4}, {0, 3, 2, 3}, {3, 4, 4, 6}, {5, 8, 3, 2}, {6, 10, 7, 4}, {8, 7, 7, 7}};
const vector<vector<int>> PRECIO_MEJORAS_ALCANCE= {{0, 6, 1, 2}, {3, 9, 5, 5}, {6, 10, 7, 8}};
const vector<vector<int>> PRECIO_MEJORAS_CADENCIA = {{1, 3, 2, 5}, {3, 4, 4, 10}, {7, 5, 8, 13}};

//Constructor de la clase pistola. Recibe:
//Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG
Pistola::Pistola(int id, string ruta, string nombre, int dmg, int alc, float cad, float rec, int mun, int pre, int runaP, int runaG, vector<int> recorte, float escala) : Arma(id, ruta, nombre, dmg, alc, cad, rec, mun, pre, runaP, runaG, recorte, escala){
    setMunicion(mun);
    sonidos->addSonido("recargar",new Buffer("resources/sonidos/recargar.wav"),60);
    sonidos->addSonido("disparo",new Buffer("resources/sonidos/Pistola.ogg"),60);
}

Pistola::~Pistola(){
}

//Cuando se pulsa el boton izq del raton se llama a esta funciion
//Si no pasa un tiempo estre bala y bala no se anade la bala al vector por lo que no dispara
//Si se queda sin balas llama al contador de la recarga
void Pistola::disparar(int elapsedTime){
    EstadoPartida *estado = EstadoPartida::Instancia();
    VentanaRender *ventana = VentanaRender::Instancia();
    Sprite* sprite = nullptr;
    if(cooldownAtaque >= cadencia){
        if(puedeDisparar()){
            //Se reproduce el sonido de disparo
            sonidos->play("disparo");

            float danyo             = getDanyoTotal();
            float danyoFinal        = danyo;
            vector<float> posRaton  = ventana->mapPixelToCoords(ventana->getPosicionRaton());
            vector<float> posIni    = spriteArma->getPosition();

            //float distPosDisparo = (static_cast<float>(sqrt(pow(posRaton[0], 2) + pow(posRaton[1], 2))));
            //Se calcula la direccion de la bala
            vector<float> direccion;
            direccion.push_back(posRaton[0] - posIni[0]);
            direccion.push_back(posRaton[1] - posIni[1]);
            float distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
            direccion[0] = direccion[0]/distNorm;
            direccion[1] = direccion[1]/distNorm;

            Proyectil *p = nullptr;
            
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

            if(pequenyas[4]){
                //Tiene la mejora de que los dos ultimos diparos hacen mas danyo (normal)
                if(balasDisparadas<=2){
                    danyoFinal+=danyo*0.50;
                }
            }else if(grandes[4]){
                //Tiene la mejora de que los dos ultimos diparos hacen mas danyo (grande)
                if(balasDisparadas<=3){
                    danyoFinal+=danyo*0.75;
                }
            }
            if(!pequenyas[3] && !grandes[3]){
                sprite = new Sprite(estado->getTexturas(2), 0, 17, 16, 16);
                p = new ProyectilRecto(0, danyoFinal, sprite, posIni, direccion, VEL_BALA, alcance, runasElementales);
            }else{
                if(pequenyas[3]){
                    sprite = new Sprite(estado->getTexturas(2), 8*16 + 2, 12*16 + 6, 10, 5);
                    float angulo = atan2(direccion[1],direccion[0]);
                    angulo = angulo*180/M_PI;
                    sprite->setRotation(angulo);
                    sprite->setScale(2, 2);
                    //Perfora solo un enemigo
                    p = new ProyectilPerforante(0, danyoFinal, sprite, posIni, direccion, VEL_BALA, alcance, runasElementales, 1);
                }else if(grandes[3]){
                    sprite = new Sprite(estado->getTexturas(2), 14*16, 12*16, 16, 16);
                    float angulo = atan2(direccion[1],direccion[0]);
                    angulo = angulo*180/M_PI;
                    sprite->setRotation(angulo);
                    sprite->setScale(2, 2);
                    //Perfora a todos los enemigos
                    p = new ProyectilPerforante(0, danyoFinal, sprite, posIni, direccion, VEL_BALA, alcance, runasElementales, -1);
                }
            }
            estado->anyadirProyectil(p);
            balasDisparadas--;
            
        }else{
            coolDownRecarga(elapsedTime);    //No tiene mas balas
        }
        cooldownAtaque = 0.f;        //Una vez disparada una bala se reinicia el tiempo entre bala y bala
    }
}

//Resetea la municion del arma para recargarla
void Pistola::recargar(){
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
int Pistola::getMunicion(){
    float municionFinal = municionOriginal;
    if(pequenyas[5]){
        municionFinal = municionOriginal + municionOriginal*0.4;
    }else if(grandes[5]){
        municionFinal = municionOriginal + municionOriginal*0.8;
    }
    return municionFinal;
}

//Pone la municion y las balas restantes segun lo que se pase
void Pistola::setMunicion(int mun){
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
vector<string> Pistola::getNombreRunas(){
    vector<string> nombres;
    nombres.push_back("Balas perforantes");
    nombres.push_back("Recamara trampa");
    nombres.push_back("Cargador ampliado");
    return nombres;
}

vector<string> Pistola::getDescRunasNormales(){
    vector<string> desc;
    desc.push_back("Balas que perforan a 1 objetivo");
    desc.push_back("Los dos ultimos disparos hacen mas danyo");
    desc.push_back("Aumenta el tamanyo del cargador un 40%");
    return desc;
}

vector<string> Pistola::getDescRunasGrandes(){
    vector<string> desc;
    desc.push_back("Balas que perforan a todos los objetivos");
    desc.push_back("Los tres ultimos disparos hacen aun mas danyo");
    desc.push_back("Aumenta el tamanyo del cargador un 80%");
    return desc;
}

vector<int> Pistola::getMaterialesRuna(int runa){
    vector<int> materiales;
    switch (runa)
    {
        //Balas perforantes
        case 0:
            materiales = {10,11};
            break;
        //ultimas balas
        case 1:
            materiales = {11, 12};
            break;
        //aumento cargador
        case 2:
            materiales = {10, 12};
            break;
        default:
            break;
    }
    return materiales;
}

//Devuelve el area de recorte de las runas del arma
vector<int> Pistola::getRecorteSpriteRuna(int runa){
    vector<int> recorte;
    switch (runa)
    {
        //Balas perforantes
        case 0:
            recorte = {0, 1*64};
            break;
        //ultimas balas
        case 1:
            recorte = {1*64, 1*64};
            break;
        //aumento cargador
        case 2:
            recorte = {2*64, 1*64};
            break;
        default:
            break;
    }
    return recorte;
}

//Metodo que devuelve los strings de las posibles mejoras
vector<string> Pistola::getTextoMejoras(){
    vector<string> tex;
    tex.push_back("Danyo");
    tex.push_back("Alcance");
    tex.push_back("Cadencia");
    return tex;
}

//Metodo que devuelve la cantidad de mejoras que tiene de cada tipo
vector<int> Pistola::getMejorasArma(){
    return {getMejorasDanyo(), getMejorasAlcance(), getMejorasCadencia()};
}

//Se le pasa el indice de la mejora que quieres aumentar
void Pistola::setMejorasArma(int indice, int indMejora){
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
            //Alcance
            if(indMejora<3){
                mejorasArma[1]++;
                alcance += MEJORAS_ALCANCE[indMejora];
            }
            break;
        case 2:
            //Cadencia
            if(indMejora<3){
                mejorasArma[2]++;
                cadencia -= MEJORAS_CADENCIA[indMejora];
            }
            break;
        default:
            break;
    }

}

vector<int> Pistola::getMaterialesMejora(int indice, int indMejora){
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
                mat = PRECIO_MEJORAS_ALCANCE[indMejora];    
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