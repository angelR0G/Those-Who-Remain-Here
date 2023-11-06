#include "EstadoMenuJuego.h"

const int TAM_INPUT = 19;
const int CAN_RETRASO = 3;

using namespace std;
//Se inicializa el puntero de la instancia
EstadoMenuJuego *EstadoMenuJuego::pEstadoMenuJuego = 0;

/*Se crea la instancia de EstadoMenu
* 
*/
EstadoMenuJuego* EstadoMenuJuego::Instancia(){
    if(pEstadoMenuJuego == 0){   //Primera llamada
        pEstadoMenuJuego = new EstadoMenuJuego(); //Se crea la instancia de EstadoMenuJuego
    }
    return pEstadoMenuJuego;
}


//Cambiar estado
void EstadoMenuJuego::cambiarEstado(int valor){
    //if(valor == 1)
    if(valor == 2){
        EstadoPartida* estado = EstadoPartida::Instancia();
        Jugador* j = estado->getJugador();
        j->setMuerto(true);
        Particula::eliminarParticulas();
        for(int i=0; i<8; i++)
            new ParticulaSimpleCuadrada(j->getPosNueva(), {255, 0, 0, 255}, 40.0, 1000);
    }
    
    PilaEstados* pilaEstados = PilaEstados::Instancia();
    
    //Cambia el estado actual al estado partida
    pilaEstados->eliminarUltimoEstado();
    

    VentanaRender* ventana = VentanaRender::Instancia();
    ventana->setMira(false);
}

//Inicialzar estado menu
void EstadoMenuJuego::inicializar(){
    VentanaRender* ventana = VentanaRender::Instancia();

    cambiarA = 0;
    //Centrar menu
    vector<float> centroVista = ventana->getCentroVista();
    despVistaX          = centroVista[0] - (ventana->getSize()[0]/2);
    despVistaY          = centroVista[1] - (ventana->getSize()[1]/2);
}

//Creador de menu
EstadoMenuJuego::EstadoMenuJuego() {
    VentanaRender* ventana = VentanaRender::Instancia();
    
    ancho = ventana->getSize()[0];
    alto=ventana->getSize()[1];
    tiempoRetraso = 0;

    //Crear fondos
    fondotx = new Textura("resources/sprites/menu/fondo.jpg");
    fondo = new Sprite(fondotx, 0, 0, 1920, 1080);
    
    fondo->setScale(ancho/1920,alto/1080);

    //Colores, tamano y fuente de las letras del menu
    col_sel =      {255,255,  0,255};     //Seleccionado: Amarillo
    col_normal =   {255,255,255,255};     //Normal: Blanco
    col_aura =     {179,144, 20,255};     //Contorno

    int tam_letra=90;

    fuente=new Fuente("resources/sprites/generales/Chonkly.otf");

    //Crear titulo y cada opcion del menu

    titulo = new Texto("Those Who Remain Here");

    titulo->setFuente(fuente);
    titulo->setTamanyo(tam_letra);
    titulo->setEscala(1.2,1.2);
    titulo->setColorTexto(col_sel);
    titulo->setPosicion(ancho/2 - titulo->getAncho()/2, alto / (MAX_ITEMS_SUB + 3) * 1 - 30);

    //Se parte seleccionada la primera opcion
    seleccionItemIndex = 0;

    vector<string> nomOpciones;

    //Menu principal

    nomOpciones.push_back("Continuar");     //Opcion 1
    nomOpciones.push_back("Ir al pueblo");  //Opcion 2
    nomOpciones.push_back("Salir");     //Opcion 3

    for(int opc = 0;opc<MAX_ITEMS_SUB;opc++){
        menu[opc] = new Texto(nomOpciones[opc]);    //Elegir nombre correspondiente de la opcion
        //printf("%s\n",menu[opc]->getTexto()->getString().toAnsiString().c_str());
        menu[opc]->setFuente(fuente);
        menu[opc]->setTamanyo(tam_letra);
        menu[opc]->setColorContorno(col_aura);
        //Posicion relativa al maximo de opciones y opcion concreta
        menu[opc]->setPosicion(ancho / 2.5, alto / (MAX_ITEMS_SUB + 3) * (2+opc));

        //Elegir color de la opcion, preseleccionada o no
        if(seleccionItemIndex == opc)
            menu[opc]->setColorTexto(col_sel);
        else
            menu[opc]->setColorTexto(col_normal);
    }

    cambiarA = 0;
}

//Destructor de menu
EstadoMenuJuego::~EstadoMenuJuego() {
    /*
    delete titulo;
    delete fuente;

    for(int i = 0;i<MAX_ITEMS_SUB;i++) {
        delete menu[i];
    }

    delete fondo;
    delete fondotx;


    col_sel.clear();
    col_normal.clear();
    col_aura.clear();
    */
}

//Dibuja el menu
void EstadoMenuJuego::dibujar(float fNU, int iNU){
    VentanaRender* ventana = VentanaRender::Instancia();

    //Fondo de menu
    ventana->dibujar(fondo);

    //Titulo siempre mostrado
    ventana->dibujar(titulo);

    //Opciones del menu activo
    for(int i = 0; i < MAX_ITEMS_SUB; i++){
        ventana->dibujar(menu[i]);
    }
    
}

//Actualizar menu
void EstadoMenuJuego::update(Tiempo tNU, bool input[TAM_INPUT]){
    VentanaRender* ventana = VentanaRender::Instancia();
    if(tiempoRetraso>0){
        tiempoRetraso--;
    }
   
    
    //Centrar menu
    vector<float> centroVista = ventana->getCentroVista();
    despVistaX          = centroVista[0] - (ventana->getSize()[0]/2);
    despVistaY          = centroVista[1] - (ventana->getSize()[1]/2);
    

    ancho = ventana->getSize()[0];
    alto=ventana->getSize()[1];
    fondo->setScale(ancho/1920,alto/1080);
    fondo->setPosition(despVistaX,despVistaY);
    titulo->setPosicion(despVistaX+ ancho/2 - titulo->getAncho()/2, despVistaY+ alto / (MAX_ITEMS_SUB + 3) * 1 - 30);
    for(int opc = 0;opc<MAX_ITEMS_SUB;opc++){
        menu[opc]->setPosicion(despVistaX+ ancho / 2.5, despVistaY+ alto / (MAX_ITEMS_SUB + 3) * (2+opc));
    }

    //Cambiar opcion
    //Si no esta el raton en una opcion, se puede mover con teclado
    if(!ventana->getFade()) {
        //Comprueba si ha pulsado en jugar
        if(cambiarA != 0) {
            cambiarEstado(cambiarA);
        }
        else {
            vector<float> pos = ventana->mapPixelToCoords(ventana->getPosicionRaton());
            if(!raton(pos[0],pos[1])){
                if(input[7] || input[3]){ //W o flechita arriba
                    if(tiempoRetraso==0){
                        arriba();
                        tiempoRetraso=CAN_RETRASO;
                    }
                }
                if(input[5] || input[1]){ //S o flechita abajo
                    if(tiempoRetraso==0){
                        abajo();
                        tiempoRetraso=CAN_RETRASO;
                    }
                }
                if(input[15]){ //Enter
                    if(tiempoRetraso==0){
                        aceptar();
                        tiempoRetraso=CAN_RETRASO;
                    }
                }
            }
            else{
                if(input[16]){ //Click iz
                    if(tiempoRetraso==0){
                        aceptar();
                        tiempoRetraso=CAN_RETRASO;
                    }
                }
            }
        }
    }
}

//Activar opcion
void EstadoMenuJuego::aceptar(){
    VentanaRender* ventana = VentanaRender::Instancia();
    switch (seleccionItemIndex)
    {
    case 0:     //Jugar
        cambiarA = 1;
        reset(0);
        hover(0);
        break;
    case 1:     //Creditos
        cambiarA = 2;
        reset(0);
        hover(0);
        break;
    case 2:     //Salir
        ventana->cerrar();
        break; 
    }
}

//Moverse arriba en el menu con el teclado
void EstadoMenuJuego::arriba(){
    int opa = seleccionItemIndex - 1;

    //Subir si hay opcion superior
    if(opa >= 0){
        reset(opa);
        hover(opa);
    }else{//Bajar del todo
        seleccionItemIndex = MAX_ITEMS_SUB-1;
        opa = seleccionItemIndex;
        reset(opa);
        hover(opa);
    }
}

//Moverse abajo en el menu con el teclado
void EstadoMenuJuego::abajo(){
    int opb = seleccionItemIndex + 1;

    //Bajar si hay opcion inferior
    if(opb < MAX_ITEMS_SUB){
        reset(opb);
        hover(opb);
    }else{//Subir del todo
        seleccionItemIndex = 0;
        opb = seleccionItemIndex;
        reset(opb);
        hover(opb);
    }
}

//Moverse en el menu con el raton
bool EstadoMenuJuego::raton(float x, float y){
    int num=-1;

    //Revisar cada opcion y si coincide la posicion del raton con alguno, se actualiza
    for(int i=0;i<MAX_ITEMS_SUB;i++){
        if(menu[i]->getContains(x,y)){
            reset(i);
            num=i;
        }
    }
    hover(num);

    return num!=-1;
}

//Resetear colores en base al elegido
void EstadoMenuJuego::reset(int num){

    //Resetear a color normal todas las opciones
    for(int i=0;i<MAX_ITEMS_SUB;i++){
        menu[i]->setColorTexto(col_normal);
    }

    //Seleccionar la opcion pasada por parametro
    if(num<MAX_ITEMS_SUB && num>=0){
        menu[num]->setColorTexto(col_sel);
        seleccionItemIndex = num;
    }else{
        seleccionItemIndex = -1;
    }
}

//Resetear hovers en base al seleccionado
void EstadoMenuJuego::hover(int num){

    //Resetear contorno
    for(int i=0;i<MAX_ITEMS_SUB;i++){
        menu[i]->setGrosorContorno(0);
    }
    //Poner contorno a la opcion que coincide
    if(num<MAX_ITEMS_SUB && num>=0){
        menu[num]->setGrosorContorno(5);
    }
}