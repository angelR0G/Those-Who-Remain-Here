#include "EstadoMenu.h"

const int TAM_INPUT = 19;
const int CAN_RETRASO = 3;

using namespace std;
//Se inicializa el puntero de la instancia
EstadoMenu *EstadoMenu::pEstadoMenu = 0;

/*Se crea la instancia de EstadoMenu
* 
*/
EstadoMenu* EstadoMenu::Instancia(){
    if(pEstadoMenu == 0){   //Primera llamada
        pEstadoMenu = new EstadoMenu(); //Se crea la instancia de estadoMenu
    }
    return pEstadoMenu;
}


//Cambiar estado
void EstadoMenu::cambiarEstado(int valor){
    if(valor == 1) {
        Guardado *guardado = Guardado::Instancia();
        PilaEstados* pilaEstados = PilaEstados::Instancia();
        if(guardado->cargarPartida() && guardado->getTutorial()){
            EstadoPueblo *ePueblo = EstadoPueblo::Instancia();
            pilaEstados->modificarUltimoEstado(ePueblo);
            ePueblo->inicializar();

        }else{
            EstadoPartida* ePartida = EstadoPartida::Instancia();
            pilaEstados->modificarUltimoEstado(ePartida);
            ePartida->inicializar(nullptr, 0);
            pilaEstados->anyadirEstado(EstadoTexto::Instancia());
            EstadoTexto::Instancia()->inicializar(1);
        }

        VentanaRender* ventana = VentanaRender::Instancia();
        ventana->fadeOut(10);
    }
}

//Creador de menu
EstadoMenu::EstadoMenu() {
    VentanaRender* ventana = VentanaRender::Instancia();
    
    float ancho = ventana->getSizeReal()[0],alto=ventana->getSizeReal()[1];
    tiempoRetraso = 0;

    //Crear fondos
    fondotx = new Textura("resources/sprites/menu/fondo.jpg");
    fondo = new Sprite(fondotx, 0, 0, 1920, 1080);
    
    fondo->setScale(ancho/1920,alto/1080);

    fondo2tx = new Textura("resources/sprites/menu/fondo2.jpg");
    fondo2 = new Sprite(fondo2tx, 0, 0, 1920, 1080);
    
    fondo2->setScale(ancho/1920,alto/1080);

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
    titulo->setPosicion(ancho/2 - titulo->getAncho()/2, alto / (MAX_ITEMS + 3) * 1 - 30);

    //Se parte seleccionada la primera opcion
    seleccionItemIndex = 0;
    //del menu principal
    menuActivo = 0;

    numItems = {5, 2, 1, 1};

    vector<vector<string>> nomOpciones;

    nomOpciones ={{},{},{},{}};

    //Menu principal

    nomOpciones[0].push_back("Jugar");     //Opcion 1
    nomOpciones[0].push_back("Opciones");  //Opcion 2
    nomOpciones[0].push_back("Controles"); //Opcion 3
    nomOpciones[0].push_back("Creditos");  //Opcion 4
    nomOpciones[0].push_back("Salir");     //Opcion 5

    for(int opc = 0;opc<numItems[0];opc++){
        menu[0][opc] = new Texto(nomOpciones[0][opc]);    //Elegir nombre correspondiente de la opcion
        //printf("%s\n",menu[opc]->getTexto()->getString().toAnsiString().c_str());
        menu[0][opc]->setFuente(fuente);
        menu[0][opc]->setTamanyo(tam_letra);
        menu[0][opc]->setColorContorno(col_aura);
        //Posicion relativa al maximo de opciones y opcion concreta
        menu[0][opc]->setPosicion(ancho / 2.5, alto / (MAX_ITEMS + 3) * (2+opc));

        //Elegir color de la opcion, preseleccionada o no
        if(seleccionItemIndex == opc)
            menu[0][opc]->setColorTexto(col_sel);
        else
            menu[0][opc]->setColorTexto(col_normal);
    }

    //Menu opciones

    nomOpciones[1].push_back("Silenciar");                //Opcion 1
    nomOpciones[1].push_back("Atras");                    //Opcion 2

    for(int opc = 0;opc<numItems[1];opc++){
        menu[1][opc] = new Texto(nomOpciones[1][opc]);    //Elegir nombre correspondiente de la opcion
        menu[1][opc]->setFuente(fuente);
        menu[1][opc]->setTamanyo(tam_letra);
        menu[1][opc]->setColorContorno(col_aura);
        //Posicion relativa al maximo de opciones y opcion concreta
        menu[1][opc]->setPosicion(ancho / 2.5, alto / (MAX_ITEMS + 3) * (2+opc));
        
        menu[1][opc]->setColorTexto(col_normal);
    }

    //Menus simples (controles y creditos)

    for(int menuSimple = 0;menuSimple<2;menuSimple++){
        menu[2+menuSimple][0] = new Texto("Atras");
        menu[2+menuSimple][0]->setFuente(fuente);
        menu[2+menuSimple][0]->setTamanyo(tam_letra);
        menu[2+menuSimple][0]->setColorContorno(col_aura);
        menu[2+menuSimple][0]->setPosicion(ancho / 2.5, alto / (MAX_ITEMS + 3) * (2+MAX_ITEMS));

        menu[2+menuSimple][0]->setColorTexto(col_sel);
    }

    //Datos de controles

    //Imagen controles
    controlesT = new Textura("resources/sprites/menu/controles.png");
    controlesS = new Sprite(controlesT, 0, 0, 200, 65); //200*1920/300, 65*1080/300

    controlesS->setScale(4*ancho/1920,4*alto/1080);

    controlesS->setPosition(ancho/2 - controlesS->getGlobalPixelSize()[0]/2, 7*alto/32); //alto/4

    //Controles
    vector<string> nomCon;
    nomCon.push_back("Movimiento: WASD, Espacio");
    nomCon.push_back("Disparo: Boton izquierdo del raton");
    nomCon.push_back("Gancho: Boton derecho del raton");
    nomCon.push_back("Control del tiempo: Q");
    nomCon.push_back("Martillo: F");
    nomCon.push_back("Interactuar: E");
    nomCon.push_back("Poder secundario: E, Rueda raton");
    nomCon.push_back("Recargar arma: R");
    nomCon.push_back("Menu del juego: Esc");
    nomCon.push_back("Inventario y fabricacion: Tab");
    for(int nom = 0;nom<10;nom++){
        controles[nom] = new Texto(nomCon[nom]);
        controles[nom]->setFuente(fuente);
        controles[nom]->setTamanyo(tam_letra/2.5);
        controles[nom]->setColorContorno(col_aura);
        controles[nom]->setColorTexto(col_normal);

        if(nom<5){
            controles[nom]->setPosicion(ancho/10, alto/2 + alto / 15 * (nom));
        }else{
            controles[nom]->setPosicion(ancho / 2, alto/2 + alto / 15 * (nom-5));
        }
    }

    
    //Datos de creditos

    //Nombres autores
    vector<string> nomCred;
    nomCred.push_back("Adrian Marco Bru");
    nomCred.push_back("Angel Robles Gonzalez");
    nomCred.push_back("Christian Jose Mira Castello");
    nomCred.push_back("Luis Angel Sanchez Tomas"); 
    nomCred.push_back("Miguel Angel Orta Julia");
    for(int nom = 0;nom<5;nom++){
        cred[nom] = new Texto(nomCred[nom]);
        cred[nom]->setFuente(fuente);
        cred[nom]->setTamanyo(tam_letra/3*2);
        cred[nom]->setColorContorno(col_aura);
        cred[nom]->setPosicion(ancho / 2 - cred[nom]->getAncho()/2, alto / (MAX_ITEMS + 3) * (2+nom));
        cred[nom]->setColorTexto(col_normal);
    }

    iniciandoPartida = false;
}

//Destructor de menu
EstadoMenu::~EstadoMenu() {
    delete titulo;
    delete fuente;
}

//Dibuja el menu
void EstadoMenu::dibujar(float fNU, int iNU){
    VentanaRender* ventana = VentanaRender::Instancia();

    //Fondo de menu principal o resto
    if(menuActivo == 0){
        ventana->dibujar(fondo);
    }else{
        ventana->dibujar(fondo2);
    }
    

    //Titulo siempre mostrado
    ventana->dibujar(titulo);

    //Opciones del menu activo
    if(menuActivo >= 0 && menuActivo <=4){
        for(int i = 0; i < numItems[menuActivo]; i++){
            ventana->dibujar(menu[menuActivo][i]);
        }
    }

    //Mostrar controles en su opcion
    if(menuActivo == 2){
        ventana->dibujar(controlesS);
        for(int i = 0; i < 10; i++){
            ventana->dibujar(controles[i]);
        }
    }

    //Mostrar creditos en su opcion
    if(menuActivo == 3){
        for(int i = 0; i < 5; i++){
            ventana->dibujar(cred[i]);
        }
    }
    
}

//Actualizar menu
void EstadoMenu::update(Tiempo tNU, bool input[TAM_INPUT]){
    VentanaRender* ventana = VentanaRender::Instancia();
    if(tiempoRetraso>0){
        tiempoRetraso--;
    }
    /*
                    -1
    Tecla_Defecto = 0,
    Left          = 1,
    Down          = 2,
    Right         = 3,
    Up            = 4,
    A             = 5,
    S             = 6,
    D             = 7,
    W             = 8,
    Space         = 9,
    Esc           = 10,
    Tab           = 11,
    Q             = 12,
    F             = 13,
    E             = 14,
    R             = 15,
    Enter         = 16

    Raton_Defecto = 0,
    Raton_Left    = 1,
    Raton_Right   = 2,
    Middle        = 3
    */
   
    //Cambiar opcion
    //Si no esta el raton en una opcion, se puede mover con teclado
    if(!ventana->getFade()) {
        //Comprueba si ha pulsado en jugar
        if(iniciandoPartida) {
            if(ventana->getPantallaNegro()) {
                cambiarEstado(1);
            }
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
void EstadoMenu::aceptar(){
    VentanaRender* ventana = VentanaRender::Instancia();
    if(menuActivo == 0){
        switch (seleccionItemIndex)
        {
        case 0:     //Jugar
            iniciandoPartida = true;
            ventana->fadeIn(5);
            break;
        case 1:     //Opciones
            menuActivo = 1;
            seleccionItemIndex = 0;
            reset(0);
            hover(0);
            break;
        case 2:     //Controles
            menuActivo = 2;
            seleccionItemIndex = 0;
            reset(0);
            hover(0);
            break;
        case 3:     //Creditos
            menuActivo = 3;
            seleccionItemIndex = 0;
            reset(0);
            hover(0);
            break;
        case 4:     //Salir
            ventana->cerrar();
            break;
        }
    }else if(menuActivo == 1){
        switch (seleccionItemIndex)
        {
        case 1:     //Atras
            menuActivo = 0;
            seleccionItemIndex = 0;
            reset(0);
            hover(0);
            break;
        }
    }else if(menuActivo > 1){
        //Atras
        menuActivo = 0;
        seleccionItemIndex = 0;
        reset(0);
        hover(0);
    }
    
}

//Moverse arriba en el menu con el teclado
void EstadoMenu::arriba(){
    int opa = seleccionItemIndex - 1;

    //Subir si hay opcion superior
    if(opa >= 0){
        reset(opa);
        hover(opa);
    }else{//Bajar del todo
        seleccionItemIndex = numItems[menuActivo]-1;
        opa = seleccionItemIndex;
        reset(opa);
        hover(opa);
    }
}

//Moverse abajo en el menu con el teclado
void EstadoMenu::abajo(){
    int opb = seleccionItemIndex + 1;

    //Bajar si hay opcion inferior
    if(opb < numItems[menuActivo]){
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
bool EstadoMenu::raton(float x, float y){
    int num=-1;

    //Revisar cada opcion y si coincide la posicion del raton con alguno, se actualiza
    for(int i=0;i<numItems[menuActivo];i++){
        if(menu[menuActivo][i]->getContains(x,y)){
            reset(i);
            num=i;
        }
    }
    hover(num);

    return num!=-1;
}

//Resetear colores en base al elegido
void EstadoMenu::reset(int num){

    //Resetear a color normal todas las opciones
    for(int i=0;i<numItems[menuActivo];i++){
        menu[menuActivo][i]->setColorTexto(col_normal);
    }

    //Seleccionar la opcion pasada por parametro
    if(num<numItems[menuActivo] && num>=0){
        menu[menuActivo][num]->setColorTexto(col_sel);
        seleccionItemIndex = num;
    }else{
        seleccionItemIndex = -1;
    }
}

//Resetear hovers en base al seleccionado
void EstadoMenu::hover(int num){

    //Resetear contorno
    for(int i=0;i<numItems[menuActivo];i++){
        menu[menuActivo][i]->setGrosorContorno(0);
    }
    //Poner contorno a la opcion que coincide
    if(num<numItems[menuActivo] && num>=0){
        menu[menuActivo][num]->setGrosorContorno(5);
    }
}