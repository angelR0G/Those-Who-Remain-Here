#include "LanzaCohetes.h"

const int VEL_BALA  = 500;
const int AUTODANYO = 2;

const vector<int> MEJORAS_DANYO = {40, 40, 40, 40, 40, 40};
const vector<float> MEJORAS_RADIO = {0.5*32, 0.5*32, 0.5*32};
const vector<int> MEJORAS_RECARGA = {300, 300, 300};
const vector<vector<int>> PRECIO_MEJORAS_DANYO = {{0, 2, 1, 4}, {0, 3, 2, 3}, {3, 4, 4, 6}, {5, 8, 3, 2}, {6, 10, 7, 4}, {8, 7, 7, 7}};
const vector<vector<int>> PRECIO_MEJORAS_RADIO= {{0, 6, 1, 2}, {3, 9, 5, 5}, {6, 10, 7, 8}};
const vector<vector<int>> PRECIO_MEJORAS_RECARGA = {{1, 3, 2, 5}, {3, 4, 4, 10}, {7, 5, 8, 13}};

//Constructor de la clase pistola. Recibe:
//Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG
LanzaCohetes::LanzaCohetes(int id, string ruta, string nombre, int dmg, int alc, float cad, float rec, int mun, int pre, int runaP, int runaG, vector<int> recorte, float escala, float r) : Arma(id, ruta, nombre, dmg, alc, cad, rec, mun, pre, runaP, runaG, recorte, escala){
    setMunicion(mun);
    radio = r;
    sonidos->addSonido("recargar",new Buffer("resources/sonidos/recargar.wav"),60);
    sonidos->addSonido("disparo",new Buffer("resources/sonidos/Lanzacohetes.ogg"),60);
}

LanzaCohetes::~LanzaCohetes(){
}

//Cuando se pulsa el boton izq del raton se llama a esta funciion
//Si no pasa un tiempo estre bala y bala no se anade la bala al vector por lo que no dispara
//Si se queda sin balas llama al contador de la recarga
void LanzaCohetes::disparar(int elapsedTime){
    EstadoPartida *estado = EstadoPartida::Instancia();
    VentanaRender *ventana = VentanaRender::Instancia();
    Sprite* sprite    = new Sprite(estado->getTexturas(2), 0, 16*17, 16, 16);
    Sprite* spriteExp = new Sprite(estado->getTexturas(4), 0, 0, 32, 32);
    Sprite* veneno    = nullptr;
    if(cooldownAtaque >= cadencia){
        if(puedeDisparar()){
            sonidos->play("disparo");
            float danyoFinal = getDanyoTotal();
            int autodanyo    = AUTODANYO;
            int gas          = 0;
            int dobleExplo   = 0;
            vector<float> posRaton = ventana->mapPixelToCoords(ventana->getPosicionRaton());
            vector<float> posIni = spriteArma->getPosition();


            vector<float> direccion;
            direccion.push_back(posRaton[0] - posIni[0]);
            direccion.push_back(posRaton[1] - posIni[1]);
            float distNorm = (static_cast<float>(sqrt(pow(direccion[0], 2) + pow(direccion[1], 2))));
            direccion[0] = direccion[0]/distNorm;
            direccion[1] = direccion[1]/distNorm;
            float angulo = atan2(direccion[1],direccion[0]);
            angulo = angulo*180/M_PI;
            sprite->setRotation(angulo);
            sprite->setScale(2.0, 2.0);
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

            //Se aplican los efectos de la runa de autodanyo, peq(-50%), gran (-100%)
            if(pequenyas[5]){
                autodanyo = autodanyo/2;
            }else if(grandes[5]){
                autodanyo = 0;
            }


            if(pequenyas[4]){
                //Crea una nube de gas
                gas = 1;
                veneno = new Sprite(estado->getTexturas(5), 0, 0, 64, 64);
            }else if(grandes[4]){
                //Crea una nube de gas (grande)
                gas = 2;
                veneno = new Sprite(estado->getTexturas(5), 0, 0, 64, 64);
            }


            if(pequenyas[3]){
                //el misil explota dos veces
                dobleExplo = 1;
            }else if(grandes[3]){
                //el misil explota dos veces (grande)
                dobleExplo = 2;
            }

            p = new ProyectilExplosivo(autodanyo, danyoFinal, sprite, spriteExp, veneno, posIni, direccion, VEL_BALA, radio, gas, dobleExplo, runasElementales, -1);
            estado->anyadirProyectil(p);
            balasDisparadas--;
            
        }else{
            coolDownRecarga(elapsedTime);    //No tiene mas balas
        }
        cooldownAtaque = 0.f;        //Una vez disparada una bala se reinicia el tiempo entre bala y bala
    }
}

//Resetea la municion del arma para recargarla
void LanzaCohetes::recargar(){
    balasDisparadas = municion;
    cooldownAtaque = cadencia;
    cooldownRecarga = 0.f;
    recargando = false;
}

//Devuelve la municion maxima del arma
int LanzaCohetes::getMunicion(){
    return municion;
}

//Pone la municion y las balas restantes segun lo que se pase
void LanzaCohetes::setMunicion(int mun){
    municion = mun;
    balasDisparadas = mun;
}


/*
  INFORMACION RUNAS
*/
vector<string> LanzaCohetes::getNombreRunas(){
    vector<string> nombres;
    nombres.push_back("Misil de fragmentacion");
    nombres.push_back("Nube mortal");
    nombres.push_back("Recubrimiento de kevlar");
    return nombres;
}

vector<string> LanzaCohetes::getDescRunasNormales(){
    vector<string> desc;
    desc.push_back("Produce una segunda explosion leve");
    desc.push_back("Crea una nube de gas (5 dps)");
    desc.push_back("Reduce el danyo autoinflingido");
    return desc;
}

vector<string> LanzaCohetes::getDescRunasGrandes(){
    vector<string> desc;
    desc.push_back("Produce una segunda explosion moderada");
    desc.push_back("Crea una nube de gas (10 dps)");
    desc.push_back("Inmune al danyo autoinflingido");
    return desc;
}

vector<int> LanzaCohetes::getMaterialesRuna(int runa){
    vector<int> materiales;
    switch (runa)
    {
        //Doble explosion
        case 0:
            materiales = {10, 11};
            break;
        //Nube de gas
        case 1:
            materiales = {10, 12};
            break;
        //Autodanyo
        case 2:
            materiales = {11, 12};
            break;
        default:
            break;
    }
    return materiales;
}

//Devuelve el area de recorte de las runas del arma
vector<int> LanzaCohetes::getRecorteSpriteRuna(int runa){
    vector<int> recorte = {0, 0};
    switch (runa)
    {
        //doble explosion
        case 0:
            recorte = {2*64, 2*64};
            break;
        //gas
        case 1:
            recorte = {3*64, 2*64};
            break;
        //auto danyo
        case 2:
            recorte = {2*64, 3*64};
            break;
        default:
            break;
    }
    return recorte;
}

//Metodo que devuelve los strings de las posibles mejoras
vector<string> LanzaCohetes::getTextoMejoras(){
    vector<string> tex;
    tex.push_back("Danyo");
    tex.push_back("Radio");
    tex.push_back("Recarga");
    return tex;
}

//Metodo que devuelve la cantidad de mejoras que tiene de cada tipo
vector<int> LanzaCohetes::getMejorasArma(){
    return {getMejorasDanyo(), getMejorasRadio(), getMejorasRecarga()};
}

//Se le pasa el indice de la mejora que quieres aumentar
void LanzaCohetes::setMejorasArma(int indice, int indMejora){
    switch (indice)
    {
        case 0:
            //Danyo
            if(indMejora<6){
                mejorasArma[0]++;
                danyo += MEJORAS_DANYO[indMejora];
            }
            break;
        case 1:
            //Radio
            if(indMejora<3){
                mejorasArma[4]++;
                radio += MEJORAS_RADIO[indMejora];
            }
            break;
        case 2:
            //Recarga
            if(indMejora<3){
                mejorasArma[5]++;
                recarga -= MEJORAS_RECARGA[indMejora];
            }
            break;
        default:
            break;
    }

}

vector<int> LanzaCohetes::getMaterialesMejora(int indice, int indMejora){
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
                mat = PRECIO_MEJORAS_RADIO[indMejora];    
            }
            
            break;
        case 2:
            if(indMejora<3){
                mat = PRECIO_MEJORAS_RECARGA[indMejora];
            }
            
            break;
        
        default:
            break;
    }
    return mat;
}