#include "Arma.h"
#include <math.h>

const float ESCALA_RECARGA = 0.5;

using namespace std;


//Id, Ruta imagen, damage, alcance, cadencia, recarga, municion, precision, runaP, runaG
Arma::Arma(int id, string ruta, string nombre, int dmg, int alc, float cad, float rec, int mun, float pre, int runaP, int runaG, vector<int> recorte, float escala){
    //Textura y sprite del arma
    texArma      = new Textura("resources/sprites/armas/" + ruta);
    spriteArma   = new Sprite(texArma, 0, 0, recorte[0], recorte[1]);
    texRecarga   = new Textura("resources/sprites/armas/icono_recarga.png");
    recargaIcono = new Sprite(texRecarga, 0, 0, 64, 64);

    sonidos = new ControladorSonido();

    spriteArma->setScale(escala, escala);
    spriteArma->setRotation(0.0f);
    spriteArma->setOrigin(spriteArma->getRect()[2]/2, spriteArma->getRect()[3]/2);
    recargaIcono->setScale(ESCALA_RECARGA, ESCALA_RECARGA);
    recargaIcono->setOrigin(recargaIcono->getRect()[2]/2, recargaIcono->getRect()[3]/2);
    pequenyas = {false, false, false, false, false, false};
    grandes   = {false, false, false, false, false, false};
    //Inicializar variables
    this->id        = id;
    this->nombre    = nombre;
    this->escala    = escala;
    danyo           = dmg;
    alcance         = alc;
    cadencia        = cad;
    recarga         = rec;
    municion        = mun;
    municionOriginal= mun;
    cooldownAtaque  = cad;
    cooldownRecarga = 0.0f;
    precision       = pre;
    this->runaP     = runaP;
    this->runaG     = runaG;
    recargando      = false;
    increRot        = 0;
    posAntiguasArma = {0, 0};
    posArma         = {0, 0};
    mejorasArma     = {0, 0, 0, 0, 0, 0};
    danyoExtra      = 0;
    for(int i = 0; i <runaP; i++){
        runasArma.push_back(new Runa(false));
    }
    for(int i = 0; i <runaG; i++){
        runasArma.push_back(new Runa(true));
    }
}

//Destructor
Arma::~Arma(){
    delete texArma;
    delete spriteArma;
    delete texRecarga;
    delete recargaIcono;
}

/*void Arma::setMunicion(int mun){
    municion = mun;
    balasDisparadas = mun;
}*/

//Dibuja el sprite del arma y cuando recarga el icono de recarga
void Arma::dibujar(float percentTick, int update_tick_time){
    VentanaRender *ventana = VentanaRender::Instancia();
    //Calcula la posicion del sprite interpolando la posicion anterior y la nueva
    float x = posAntiguasArma[0]*(1-percentTick) + posArma[0]*percentTick;
    float y = posAntiguasArma[1]*(1-percentTick) + posArma[1]*percentTick;

    //Actualiza la posicion del sprite y lo dibuja
    spriteArma->setPosition(x, y);
    ventana->dibujar(spriteArma);

    if(recargando && cooldownRecarga < recarga){
        recargaIcono->setPosition(spriteArma->getPosition()[0], spriteArma->getPosition()[1] - 25); //Coloca el sprite de recarga encima del arma
        recargaIcono->setRotation(5+increRot);       //Rota el sprite de la recarga mientras se termina el cooldown de la recarga
        ventana->dibujar(recargaIcono);    //Se renderiza el sprite en la pantalla
    }
    increRot+=10;
}

//Update del arma
void Arma::update(vector<float> posJugador, int elapsedTime){
    posAntiguasArma = posArma;
    //Se calcula la rotacion del arma segun la posicion del raton
    VentanaRender*ventana = VentanaRender::Instancia();

    vector<float> posRaton = ventana->mapPixelToCoords(ventana->getPosicionRaton());
    vector<float> pos = {posRaton[0]-posArma[0], posRaton[1]-posArma[1]};
    float rotArma = (atan2(pos[0], pos[1])*180/M_PI);
    
    if(posJugador[0]<=posRaton[0]){
        spriteArma->setScale(escala, escala);
        spriteArma->setRotation(-rotArma+100);
        posArma[0] = posJugador[0]+40;
        posArma[1] = posJugador[1];
    }else{
        spriteArma->setScale(-escala, escala);
        spriteArma->setRotation(-rotArma-100);
        posArma[0] = posJugador[0]-40;
        posArma[1] = posJugador[1];
    }

    if(!puedeDisparar()){
      coolDownRecarga(elapsedTime);
    }
}

//Evita que se pueda disparar hasta pasado un tiempo
void Arma::updateCoolDown(int elapsedTime){
    if(cooldownAtaque < cadencia){
        cooldownAtaque += elapsedTime;
    }
}

/*Coloca la posicion del arma con respecto al jugador y se apunta hacia donde mira. Se le pasa la posicion del jugador y la direccion hacia donde mira.
*Tambien se indica como tercer parametro si debe actualizar las coordenadas antiguas
*/
void Arma::setPosArma(vector<float> posJugador, int direc, bool actAntiguas){
    if(direc == 0){
        //spriteArma->setPosition(posJugador[0]+20, posJugador[1]+2);
        posArma[0] = posJugador[0]+25;
        posArma[1] = posJugador[1];
        spriteArma->setScale(escala, escala);
    }else{
        //spriteArma->setPosition(posJugador[0]-20, posJugador[1]+2);
        posArma[0] = posJugador[0]-25;
        posArma[1] = posJugador[1];
        spriteArma->setScale(-escala, escala);
    }

    if(actAntiguas) {
        //Actualiza la posicion antigua
        posAntiguasArma = posArma;
    }
}


//Reinicia las balas disparadas y el tiempo de espera de la recarga
void Arma::recargar(){
    cooldownAtaque = cadencia;
    balasDisparadas = municion;
    cooldownRecarga = 0.f;
    recargando = false;
}

//Si el cargador se ha quedado sin balas no se puede disparar hasta pasado un tiempo mientras recarga
void Arma::coolDownRecarga(int elapsedTime){
    sonidos->play("recargar");
    VentanaRender *ventana = VentanaRender::Instancia();
    if(cooldownRecarga < recarga){
        cooldownRecarga +=elapsedTime;
        recargaIcono->setPosition(spriteArma->getPosition()[0], spriteArma->getPosition()[1] - 25); //Coloca el sprite de recarga encima del arma
        recargando = true;
        recargaIcono->setRotation(5);       //Rota el sprite de la recarga mientras se termina el cooldown de la recarga
        ventana->dibujar(recargaIcono);    //Se renderiza el sprite en la pantalla
    }else{
        sonidos->stop("recargar");
        recargar();
    }
}

//Si la cantidad de balas que se han disparado es 0 o inferior se pasa true para que recargue el arma
bool Arma::puedeDisparar(){
    if(balasDisparadas <= 0){
        return false;
    }
    return true;
}

//Cuando el jugador recarga se vacia el cargador
void Arma::vaciarCargador(){
    if(balasDisparadas != municion) {
        balasDisparadas = 0;
    }
}

//Devuelve la municion maxima del arma
int Arma::getMunicion(){
    return municion;
}

//Devuelve la cantidad de balas que quedan en el cargador
int Arma::getMunicionRestante(){
    return balasDisparadas;
}

//Devuelve el nombre del arma
string Arma::getNombre(){
    return nombre;
}

vector<bool> Arma::getRunasPequenyas(){
    return pequenyas;
}

vector<bool> Arma::getRunasGrandes(){
    return grandes;
}

//Permite cambiar el booleano de una poscion del array de runas
//Primero posicion del array->0:Fuego, 1:Trueno, 2:Hielo, 3-5:Especifico de cada arma
//Pistola->3:Perforacion, 4:danyo adicional en las ultimas balas, 5:aumento municion
//Escopeta->3:, 4:Dos cartuchos seguidos, 5:Francotirador
//Metralleta->3:Perforacion, 4:Disparar dos balas, 5:aumento municion
//Segundo si es una runa grande->true o pequenya->false
//Tercero si se quiere activar o desactivar la runa
void Arma::setRunas(int pos, bool esGrande, bool act){
    if(esGrande){
        grandes[pos] = act;
    }else{
        pequenyas[pos] = act;
    }
}

//Devuelve el cooldown restante para poder volver a disparar
float Arma::getCooldown(){
    return cooldownAtaque;
}

//Devuelve la cadencia del arma
float Arma::getCadencia(){
    return cadencia;
}

//Devuelve la id del arma
int Arma::getId(){
    return id;
}

//Si el parametro es true pone el cooldown al maximo para que se pueda dispara, si es false se resetea el cooldown
void Arma::setCooldown(bool max){
    if(max){
        cooldownAtaque = cadencia;
    }else{
        cooldownAtaque = 0;
    }
}

/*
    Mejoras arma
*/
int Arma::getMejorasDanyo(){
    return mejorasArma[0];
}

int Arma::getMejorasAlcance(){
    return mejorasArma[1];
}

int Arma::getMejorasCadencia(){
    return mejorasArma[2];
}

int Arma::getMejorasPrecision(){
    return mejorasArma[3];
}

int Arma::getMejorasRadio(){
    return mejorasArma[4];
}

int Arma::getMejorasRecarga(){
    return mejorasArma[5];
}

int Arma::getDanyoExtra() {
    return danyoExtra;
}

void Arma::setDanyoExtra(int d) {
    if(d >= 0) {
        danyoExtra = d;
    }
}

float Arma::getDanyoTotal() {
    float d = (float)danyo;

    if(danyoExtra > 0) {
        d += danyo*(danyoExtra/100.0);
    }
    
    return d;
}

Sprite* Arma::getSprite(){
    return spriteArma;
}

int Arma::getCantidadPequenyas(){
    return runaP;
}

int Arma::getCantidadGrandes(){
    return runaG;
}

vector<Runa*> Arma::getRunas(){
    return runasArma;
}