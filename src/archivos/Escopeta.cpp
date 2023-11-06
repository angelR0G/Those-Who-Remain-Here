#include "Escopeta.h"

const int VEL_BALA         = 500;
const float DISPERSION_PEQ = 0.2;
const float DISPERSION_GRA = 0.4;

const float EXPLOSION_PEQ = 0.6;
const float EXPLOSION_GRA = 0.8;
const int TAM_EXPLOSION   = 80;

const vector<int> MEJORAS_DANYO = {5, 5, 5, 5, 5, 5};
const vector<int> MEJORAS_ALCANCE = {75, 75, 75};
const vector<int> MEJORAS_CADENCIA = {40, 40, 40};
const vector<vector<int>> PRECIO_MEJORAS_DANYO = {{0, 2, 1, 4}, {0, 3, 2, 3}, {3, 4, 4, 6}, {5, 8, 3, 2}, {6, 10, 7, 4}, {8, 7, 7, 7}};
const vector<vector<int>> PRECIO_MEJORAS_ALCANCE= {{0, 6, 1, 2}, {3, 9, 5, 5}, {6, 10, 7, 8}};
const vector<vector<int>> PRECIO_MEJORAS_CADENCIA = {{1, 3, 2, 5}, {3, 4, 4, 10}, {7, 5, 8, 13}};

//Constructor de la clase Escopeta. Recibe:
//Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG
Escopeta::Escopeta(int id, string ruta, string nombre, int dmg, int alc, float cad, float rec, int mun, int pre, int runaP, int runaG, vector<int> recorte, float escala) : Arma(id, ruta, nombre, dmg, alc, cad, rec, mun, pre, runaP, runaG, recorte, escala){
    setMunicion(mun);
    sonidos->addSonido("recargar",new Buffer("resources/sonidos/recargar.wav"),60);
    sonidos->addSonido("disparo",new Buffer("resources/sonidos/Escopeta.ogg"),60);
    primera = true;
}

Escopeta::~Escopeta(){
}

//Cuando se pulsa el boton izq del raton se llama a esta funciion
//Si no pasa un tiempo estre bala y bala no se anade la bala al vector por lo que no dispara
//Si se queda sin balas llama al contador de la recarga
void Escopeta::disparar(int elapsedTime){
    if(cooldownAtaque >= cadencia){
        
        float dispersionFinalP = DISPERSION_PEQ;
        float dispersionFinalG = DISPERSION_GRA;
        int alcanceFinal       = alcance;
        if(primera){
            if(puedeDisparar()){
                //Se reproduce el sonido del disparo de la escopeta
                sonidos->play("disparo");

                //Si tiene equipado la mejora de francotirador se ajusta la dispersion y el alcance
                if(pequenyas[5]){
                    dispersionFinalP -= dispersionFinalP*0.6;
                    dispersionFinalG -= dispersionFinalG*0.6;
                    alcanceFinal     += alcanceFinal * 0.2;
                }else if(grandes[5]){
                    dispersionFinalP -= dispersionFinalP*0.9;
                    dispersionFinalG -= dispersionFinalG*0.9;
                    alcanceFinal     += alcanceFinal * 0.4;
                }
                
                disparos(dispersionFinalP, dispersionFinalG, alcanceFinal, primera, 0);
                balasDisparadas--;
                if(pequenyas[4] || grandes[4]){
                    primera = false;
                }
            }else{
                //primera = true;
                coolDownRecarga(elapsedTime);    //No tiene mas balas
            }
        }else{
            //Si tiene equipado la mejora de francotirador se ajusta la dispersion y el alcance
            if(pequenyas[5]){
                dispersionFinalP -= dispersionFinalP*0.6;
                dispersionFinalG -= dispersionFinalG*0.6;
                alcanceFinal     += alcanceFinal * 0.2;
            }else if(grandes[5]){
                dispersionFinalP -= dispersionFinalP*0.9;
                dispersionFinalG -= dispersionFinalG*0.9;
                alcanceFinal     += alcanceFinal * 0.4;
            }
            //Dispara dos cartuchos seguidos
           if(pequenyas[4]){
                disparos(dispersionFinalP, dispersionFinalG, alcanceFinal, primera, 0.3);
                
            }else if(grandes[4]){
                disparos(dispersionFinalP, dispersionFinalG, alcanceFinal, primera, 0);
            }
            primera = true; 
        }
        
        cooldownAtaque = 0.f;        //Una vez disparada una bala se reinicia el tiempo entre bala y bala
    }
}

void Escopeta::disparos(float dispersionFinalP, float dispersionFinalG, int alcanceFinal, bool dispPrimera, float retroceso ){
    EstadoPartida *estado = EstadoPartida::Instancia();
    VentanaRender *ventana = VentanaRender::Instancia();
    //Se crean los sprites para las cinco balas
    Sprite* sprite = nullptr;
    Sprite* sprite1 = nullptr;
    Sprite* sprite2 = nullptr;
    Sprite* sprite3 = nullptr;
    Sprite* sprite4 = nullptr;

    Sprite* spriteExp = nullptr;
    Sprite* spriteExp1 = nullptr;
    Sprite* spriteExp2 = nullptr;
    Sprite* spriteExp3 = nullptr;
    Sprite* spriteExp4 = nullptr;

    //Si es la primera vez que se dispara se generan balas naranjas
    if(dispPrimera){
        sprite  = new Sprite(estado->getTexturas(2), 0, 2*16, 16, 16);
        sprite1 = new Sprite(estado->getTexturas(2), 0, 2*16, 16, 16);
        sprite2 = new Sprite(estado->getTexturas(2), 0, 2*16, 16, 16);
        sprite3 = new Sprite(estado->getTexturas(2), 0, 2*16, 16, 16);
        sprite4 = new Sprite(estado->getTexturas(2), 0, 2*16, 16, 16);
    }else{  //Si el disparo se produce por la runa las balas son azules
        sprite  = new Sprite(estado->getTexturas(2), 9*16, 2*16, 16, 16);
        sprite1 = new Sprite(estado->getTexturas(2), 9*16, 2*16, 16, 16);
        sprite2 = new Sprite(estado->getTexturas(2), 9*16, 2*16, 16, 16);
        sprite3 = new Sprite(estado->getTexturas(2), 9*16, 2*16, 16, 16);
        sprite4 = new Sprite(estado->getTexturas(2), 9*16, 2*16, 16, 16);
    }
    
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

    vector<float> posRaton = ventana->mapPixelToCoords(ventana->getPosicionRaton());
    vector<float> posIni = spriteArma->getPosition();
    //Se calcula la direccion de la bala
    vector<float> direccion;
    vector<float> direccionN={-1, -1};
    direccion.push_back(posRaton[0] - posIni[0]);
    direccion.push_back(posRaton[1] - posIni[1]);
    float distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
    float angulo = atan2(direccion[1],direccion[0]);
    float anguloNuevo = 0;
    if(!dispPrimera){
        if(angulo<=M_PI && angulo>=-M_PI/2){
            anguloNuevo = angulo+retroceso;
        }else{
            anguloNuevo = angulo+retroceso;
        }
        direccion[0] = cos(anguloNuevo);
        direccion[1] = sin(anguloNuevo);
        distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
        direccionN[0] = direccion[0]/distNorm;
        direccionN[1] = direccion[1]/distNorm;
    }else{
        direccionN[0] = direccion[0]/distNorm;
        direccionN[1] = direccion[1]/distNorm;
    }
    
    //Se comprueba si tiene equipada la mejora de disparos explosivos
    if(!pequenyas[3] && !grandes[3]){
        Proyectil *p = new ProyectilRecto(0, getDanyoTotal(), sprite, posIni, direccionN, VEL_BALA, alcanceFinal, runasElementales);
        estado->anyadirProyectil(p);
    }else{
        if(pequenyas[3]){
            //Runa disparos explosivos pequenya
            spriteExp = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_PEQ, sprite, spriteExp, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p);
        }else if(grandes[3]){
            //Runa disparos explosivos grande
            spriteExp = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_GRA, sprite, spriteExp, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p);
        }
    }

    if(angulo<=M_PI && angulo>=-M_PI/2){
        anguloNuevo = angulo-dispersionFinalP+retroceso;
    }else{
        anguloNuevo = angulo+dispersionFinalP+retroceso;
    }
    direccion[0] = cos(anguloNuevo);
    direccion[1] = sin(anguloNuevo);
    distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
    direccionN[0] = direccion[0]/distNorm;
    direccionN[1] = direccion[1]/distNorm;

    //Se comprueba si tiene equipada la mejora de disparos explosivos
    if(!pequenyas[3] && !grandes[3]){
        Proyectil *p1 = new ProyectilRecto(0, getDanyoTotal(), sprite1, posIni, direccionN, VEL_BALA, alcanceFinal, runasElementales);
        estado->anyadirProyectil(p1);
    }else{
        if(pequenyas[3]){
            //Runa disparos explosivos pequenya
            spriteExp1 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p1 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_PEQ, sprite1, spriteExp1, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p1);
        }else if(grandes[3]){
            //Runa disparos explosivos grande
            spriteExp1 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p1 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_GRA, sprite1, spriteExp1, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p1);
        }
    }


    if(angulo<=M_PI && angulo>=-M_PI/2){
        anguloNuevo = angulo-dispersionFinalG+retroceso;
    }else{
        anguloNuevo = angulo+dispersionFinalG+retroceso;
    }
    direccion[0] = cos(anguloNuevo);
    direccion[1] = sin(anguloNuevo);
    distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
    direccionN[0] = direccion[0]/distNorm;
    direccionN[1] = direccion[1]/distNorm;

    //Se comprueba si tiene equipada la mejora de disparos explosivos
    if(!pequenyas[3] && !grandes[3]){
        Proyectil *p2 = new ProyectilRecto(0, getDanyoTotal(), sprite2, posIni, direccionN, VEL_BALA, alcanceFinal, runasElementales);
        estado->anyadirProyectil(p2);
    }else{
        if(pequenyas[3]){
            //Runa disparos explosivos pequenya
            spriteExp2 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p2 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_PEQ, sprite2, spriteExp2, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p2);
        }else if(grandes[3]){
            //Runa disparos explosivos grande
            spriteExp2 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p2 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_GRA, sprite2, spriteExp2, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p2);
        }
    }


    if(angulo<=M_PI && angulo>=-M_PI/2){
        anguloNuevo = angulo+dispersionFinalP+retroceso;
    }else{
        anguloNuevo = angulo-dispersionFinalP+retroceso;
    }
    direccion[0] = cos(anguloNuevo);
    direccion[1] = sin(anguloNuevo);
    distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
    direccionN[0] = direccion[0]/distNorm;
    direccionN[1] = direccion[1]/distNorm;
    
    //Se comprueba si tiene equipada la mejora de disparos explosivos
    if(!pequenyas[3] && !grandes[3]){
        Proyectil *p3 = new ProyectilRecto(0, getDanyoTotal(), sprite3, posIni, direccionN, VEL_BALA, alcanceFinal, runasElementales);
        estado->anyadirProyectil(p3);
    }else{
        if(pequenyas[3]){
            //Runa disparos explosivos pequenya
            spriteExp3 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p3 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_PEQ, sprite3, spriteExp3, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p3);
        }else if(grandes[3]){
            //Runa disparos explosivos grande
            spriteExp3 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p3 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_GRA, sprite3, spriteExp3, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p3);
        }
    }


    if(angulo<=M_PI && angulo>=-M_PI/2){
        anguloNuevo = angulo+dispersionFinalG+retroceso;
    }else{
        anguloNuevo = angulo-dispersionFinalG+retroceso;
    }
    direccion[0] = cos(anguloNuevo);
    direccion[1] = sin(anguloNuevo);
    distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
    direccionN[0] = direccion[0]/distNorm;
    direccionN[1] = direccion[1]/distNorm;

    //Se comprueba si tiene equipada la mejora de disparos explosivos
    if(!pequenyas[3] && !grandes[3]){
        Proyectil *p4 = new ProyectilRecto(0, getDanyoTotal(), sprite4, posIni, direccionN, VEL_BALA, alcanceFinal, runasElementales);
        estado->anyadirProyectil(p4);
    }else{
        if(pequenyas[3]){
            //Runa disparos explosivos pequenya
            spriteExp4 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p4 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_PEQ, sprite4, spriteExp4, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p4);
        }else if(grandes[3]){
            //Runa disparos explosivos grande
            spriteExp4 = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
            Proyectil *p4 = new ProyectilExplosivo(0, getDanyoTotal()*EXPLOSION_GRA, sprite4, spriteExp4, nullptr, posIni, direccionN, VEL_BALA, TAM_EXPLOSION, false, false, runasElementales, alcanceFinal);
            estado->anyadirProyectil(p4);
        }
    }
    if(!dispPrimera){
        primera = true;
    }
}


//Resetea la municion del arma para recargarla
void Escopeta::recargar(){
    balasDisparadas = municion;
    cooldownAtaque = cadencia;
    cooldownRecarga = 0.f;
    recargando = false;
}

//Devuelve la municion maxima del arma
int Escopeta::getMunicion(){
    return municion;
}

//Pone la municion y las balas restantes segun lo que se pase
void Escopeta::setMunicion(int mun){
    municion = mun;
    balasDisparadas = mun;
}

/*
  INFORMACION RUNAS
*/
vector<string> Escopeta::getNombreRunas(){
    vector<string> nombres;
    nombres.push_back("Balas de impacto");
    nombres.push_back("Gatillo agil");
    nombres.push_back("Francopeta");
    return nombres;
}

vector<string> Escopeta::getDescRunasNormales(){
    vector<string> desc;
    desc.push_back("Balas que generan explosiones");
    desc.push_back("Disparas dos cartuchos rapidamente con una desviacion");
    desc.push_back("Reduce la dispersion y aumenta el alcance");
    return desc;
}

vector<string> Escopeta::getDescRunasGrandes(){
    vector<string> desc;
    desc.push_back("Balas que generan explosiones mas potentes");
    desc.push_back("Disparas dos cartuchos rapidamente");
    desc.push_back("Reduce mas la dispersion y aumenta mas el alcance");
    return desc;
}

vector<int> Escopeta::getMaterialesRuna(int runa){
    vector<int> materiales;
    switch (runa)
    {
        //explosion
        case 0:
            materiales = {12, 11};
            break;
        //dos cartuchos
        case 1:
            materiales = {10, 11};
            break;
        //francopeta
        case 2:
            materiales = {10, 12};
            break;
        default:
            break;
    }
    return materiales;
}

//Devuelve el area de recorte de las runas del arma
vector<int> Escopeta::getRecorteSpriteRuna(int runa){
    vector<int> recorte = {0, 0};
    switch (runa)
    {
        //explosion
        case 0:
            recorte = {3*64, 1*64};
            break;
        //dos cartuchos
        case 1:
            recorte = {1*64, 3*64};
            break;
        //francopeta
        case 2:
            recorte = {0, 2*64};
            break;
        default:
            break;
    }
    return recorte;
}

//Metodo que devuelve los strings de las posibles mejoras
vector<string> Escopeta::getTextoMejoras(){
    vector<string> tex;
    tex.push_back("Danyo");
    tex.push_back("Alcance");
    tex.push_back("Cadencia");
    return tex;
}

//Metodo que devuelve la cantidad de mejoras que tiene de cada tipo
vector<int> Escopeta::getMejorasArma(){
    return {getMejorasDanyo(), getMejorasAlcance(), getMejorasCadencia()};
}

//Se le pasa el indice de la mejora que quieres aumentar
void Escopeta::setMejorasArma(int indice, int indMejora){
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
                alcance+=MEJORAS_ALCANCE[indMejora];
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

vector<int> Escopeta::getMaterialesMejora(int indice, int indMejora){
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