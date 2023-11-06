#include "Hud.h"

using namespace std;
const float escala = 6.0;
//Constructor de Hud
Hud::Hud(){

    EstadoPartida* estado = EstadoPartida::Instancia();//recuperamos el estado partida para acceder posteriormente a jugador y Sala
    Jugador* jugador = estado->getJugador();//Puntero a jugador

    //Inicializamos variables con las estadisticas del jugador
    vida        =   jugador->getVida();
    escudo      =   jugador->getEscudo();
    roboVida    =   jugador->getRoboVida();
    vidaMax     =   jugador->getVidaMax();
    visible     =   true;
    //Inicializo texturas y Sprites de todo menos minimapa
    tex                 =   new Textura("resources/sprites/personaje/icons.png");
    sptVida             =   new Sprite(tex, 0, 0, 16, 16);
    sptMediaVida        =   new Sprite(tex, 16, 0, 16, 16);
    sptVidaCero         =   new Sprite(tex, 16, 16, 16, 16);
    sptEscudo           =   new Sprite(tex, 0, 16, 16, 16);
    sptContornoBarra    =   new Sprite(tex, 16*4, 16, 48, 16);
    sptBarraRobo        =   new Sprite(tex, 16*10, 9, 1, 8);
    sptTiempoListo      =   new Sprite(tex, 0, 32, 16 ,16);
    sptTiempoCero       =   new Sprite(tex, 16, 32, 16, 16);
    //Se selecciona el origen de los sprites
    sptVida->setOrigin(16/2,16/2);
    sptMediaVida->setOrigin(16/2,16/2);
    sptVidaCero->setOrigin(16/2,16/2);
    sptEscudo->setOrigin(16/2,16/2);
    sptContornoBarra->setOrigin(16/2,16/2);
    sptBarraRobo->setOrigin(8/2,8/2);
    sptTiempoCero->setOrigin(16/2,16/2);
    //Se ajustan escalas
    sptVida->setScale(escala, escala);
    sptMediaVida->setScale(escala, escala);
    sptVidaCero->setScale(escala, escala);
    sptEscudo->setScale(escala, escala);
    sptContornoBarra->setScale(escala, escala);
    sptBarraRobo->setScale(2.f, escala);
    sptTiempoListo->setScale(escala, escala);
    sptTiempoCero->setScale(escala, escala);

    //Para crear el minimapa
    salas               = estado->getSalas();//Puntero a Sala
    //Cargamos texturas
    texMap              =   new Textura("resources/sprites/personaje/minimap.png");
    //Hacemos los sprites
    sptMapa             =   new Sprite(texMap, 0, 0, 16, 16);
    sptMapaRecompensa   =   new Sprite(texMap, 0, 16, 16, 16);
    sptMapaConJugador   =   new Sprite(texMap, 16, 0, 16, 16);
    sptMapaConBullet    =   new Sprite(texMap, 16, 16, 16, 16);
    sptMapaConBoss      =   new Sprite(texMap, 32, 0, 16, 16);
    sptMapa->setOrigin(16/2, 16/2);
    sptMapa->setScale(2.f, 2.f);
    sptMapaRecompensa->setOrigin(16/2, 16/2);
    sptMapaRecompensa->setScale(2.f, 2.f);
    sptMapaConJugador->setOrigin(16/2, 16/2);
    sptMapaConJugador->setScale(2.f, 2.f);
    sptMapaConBoss->setOrigin(16/2, 16/2);
    sptMapaConBoss->setScale(2.f, 2.f);
    sptMapaConBullet->setOrigin(16/2, 16/2);
    sptMapaConBullet->setScale(2.f, 2.f);

    //Para mostrar la municion
    //Creamos la fuente
    fuente                      =   new Fuente("resources/sprites/generales/Chonkly.otf");
    textoMunicion               =   new Texto("");
    textoMunicion->setFuente(fuente);
    textoMunicion->setTamanyo(36);
    textoMunicion->setEscala(1, 1);
    textoMunicion->setColorTexto({255,255,255,255});
    
    //Para mostrar las esencias
    /*
    texEsencia                  =   new Textura("resources/sprites/generales/esencia.png");
    sptEsencia                  =   new Sprite(texEsencia, 0, 0, 16, 16);
    sptEsencia->setOrigin(16/2, 16/2);
    cantidadEsencias            =   new Texto("");
    cantidadEsencias->setFuente(fuente);
    cantidadEsencias->setTamanyo(30);
    cantidadEsencias->setEscala(1, 1);
    cantidadEsencias->setColorTexto({255,255,255,255});
    */

}
Hud::~Hud(){
    delete tex;
    delete sptVida;
    delete sptMediaVida;
    delete sptVidaCero;
    delete sptEscudo;
    delete sptContornoBarra;
    delete sptBarraRobo;
    delete sptTiempoListo;
    delete sptTiempoCero;
    delete texMap;
    delete sptMapa;
    delete sptMapaRecompensa;
    delete sptMapaConJugador;
    delete sptMapaConBullet;
    delete sptMapaConBoss;
    delete textoMunicion;
    delete fuente;
    /*
    delete sptEsencia;
    delete texEsencia;
    delete cantidadEsencias;
    */
    salas = nullptr;
}

void Hud::dibujar(){
    if(visible){
        //Recupero la ventana
        VentanaRender* window   = VentanaRender::Instancia();
        //Recupero el estado para acceder al jugador
        EstadoPartida* estado   = EstadoPartida::Instancia();
        //Creo un objeto jugador y cojo el tiempo para la habilidad
        Jugador* jugador        = estado->getJugador();
        float temporizador      = jugador->getCooldownHabilidad();
        //para que el hud vaya conforme el jugador se mueva de sala
        vector<float> centroVista = estado->getVista()->getCentro();
        float despVistaX          =centroVista[0] - (window->getSize()[0]/2);
        float despVistaY          =centroVista[1] - (window->getSize()[1]/2);
        //con este bucle se comprueba si se debe dibujar un corazon lleno, medio o vacio y cuantos de estos.
        for(int i = 0; i<vidaMax; i++){
            if(vida>=2*(i+1)){
                sptVida->setPosition(despVistaX + 150 + 80*i, despVistaY+40);
                window->dibujar(sptVida);

            }else if(vida==(2*(i+1))-1){
                sptMediaVida->setPosition(despVistaX + 150 + 80*i, despVistaY+40);
                window->dibujar(sptMediaVida); 
            }
            else{

                sptVidaCero->setPosition(despVistaX + 150 + 80*i, despVistaY+45);
                window->dibujar(sptVidaCero); 
            }
            //Aqui comprobamos que una vez dibujados los corazones de vida, si es necesario se ponen los de escudo
            if(i+1==vidaMax){
                for(int j=0; j<escudo; j++){
                    i++;
                    sptEscudo->setPosition(despVistaX + 150 + 80*i, despVistaY+50);
                    window->dibujar(sptEscudo);
                }
            }
        }
        //Dibujamos tanto la parte exterior como interior del indicador de recuperacion de vida
        for(int k=0; k<roboVida; k++){
            sptBarraRobo->setPosition(despVistaX + 120+(k*2),despVistaY + 90);
            window->dibujar(sptBarraRobo);
        }
        sptContornoBarra->setPosition(despVistaX + 150, despVistaY + 100);
        sptContornoBarra->draw();
        //Indicador de carga de la habilidad de parar el tiempo
        sptTiempoCero->setPosition(despVistaX + 50, despVistaY + 65);
        window->dibujar(sptTiempoCero);
        int pixels = 16*temporizador;
        
        sptTiempoListo->setRect(0, 32+(16-pixels), 16, pixels);
        sptTiempoListo->setOrigin(0, pixels);
        sptTiempoListo->setPosition(despVistaX + 2, despVistaY + 65 + 48);
        window->dibujar(sptTiempoListo);


        //Dibujamos minimapa
        if(visible){
            for(unsigned int i=0; i<salas->size(); i++){
                for(unsigned int j=0; j<(*salas)[i].size(); j++){
                    if((*salas)[j][i] != nullptr){
                        if((*salas)[j][i]->getDescubierta()){
                            sptMapa->setPosition(despVistaX+1600+(32*j), despVistaY + 50+(32*(salas->size()-i)));
                            window->dibujar(sptMapa);
                        }
                        if((*salas)[j][i]->getSuperada()&&(*salas)[j][i]->getRecompensa()==1&&(*salas)[j][i]->cofresPorAbrir()){
                            sptMapaRecompensa->setPosition(despVistaX + 1600+(32*j), despVistaY + 50+(32*(salas->size()-i)));
                            window->dibujar(sptMapaRecompensa);
                        }
                        if((*salas)[j][i]->getSuperada()&&(*salas)[j][i]->getRecompensa()==2){
                            sptMapaConBullet->setPosition(despVistaX + 1600+(32*j), despVistaY + 50+(32*(salas->size()-i)));
                            window->dibujar(sptMapaConBullet);
                        }
                        if((*salas)[j][i]->getSuperada()&&(*salas)[j][i]->getRecompensa()==3){
                            sptMapaConBoss->setPosition(despVistaX + 1600+(32*j), despVistaY + 50+(32*(salas->size()-i)));
                            window->dibujar(sptMapaConBoss);
                        }
                        if((*salas)[j][i]==estado->getSalaActual()){
                            sptMapaConJugador->setPosition(despVistaX + 1600+(32*j), despVistaY + 50+(32*(salas->size()-i)));
                            window->dibujar(sptMapaConJugador);
                        }
                    }
                }
            }
        }
        string total, restante;
        total       = to_string(jugador->getArma()->getMunicion());
        restante    = to_string(jugador->getArma()->getMunicionRestante());
        textoMunicion->setString(restante+"/"+total);
        textoMunicion->setPosicion(despVistaX+1720, despVistaY+900);
        window->dibujar(textoMunicion);
    }
}

void Hud::setVisible(bool visibilidad){
    visible=visibilidad;
}
void Hud::setVida(int v){
    vida=v;
}
void Hud::setEscudo(int e){
    escudo=e;
}
void Hud::setRoboVida(int rv){
    roboVida=rv;
}
void Hud::setVidaMax(int vm){
    vidaMax=vm;
}

/*
void Hud::muestraEsencias(){
    //Recogemos la ventana 
    VentanaRender* window   =   VentanaRender::Instancia();
    Guardado* guardado = Guardado::Instancia();
    //para que el inventario vaya conforme el jugador se mueva de sala
    vector<float> centroVista = window->getCentroVista();
    float despVistaX          = centroVista[0] - (window->getSize()[0]/2);
    float despVistaY          = centroVista[1] - (window->getSize()[1]/2);
    cantidadEsencias->setPosicion(despVistaX+250, despVistaY+1000);
    sptEsencia->setPosition(despVistaX+200, despVistaY+1000);
    cantidadEsencias->setString(to_string(guardado->getEsencias()));
    cantidadEsencias->draw();
    sptEsencia->draw();
}
*/

