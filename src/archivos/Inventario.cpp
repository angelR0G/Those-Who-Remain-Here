#include "Inventario.h"

using namespace std;

const int tiempo    =    600;

const int MEJORA_danyo = 6;
const int MEJORA_resto = 3;

//constructor al cual se le pasa el puntero a arma que tiene el jugador
Inventario::Inventario(Arma** arma){

    //Inicializamos el las variables para saber si hay hover o click a -1 por defecto, tambien el nivel de materiales a 1, igualamos el temporizador a la constante tiempo y la variable esMayor a false
    hover                       =   -1;
    elegido                     =   -1;
    nivelMateriales             =   1;
    temporizador                =   tiempo;
    esMayor                     =   false;
    mejoraElementalElegida      =   -1;
    runaHover                   =   -1;
    mejoraRunicaElegida         =   -1;
    runaEspecificaHover         =   -1;
    mejoraHover                 =   -1;
    elegiHover                  =   -1;

    //Inicializo los vectores de colores
    blanco      = {255,255,255,255};
    amarillo    = {255,255,0,255};

    //Por defecto por primera vez abre en la pestanya objetos
    modo    =    0;

    //Se comprueba que los vectores de objetos y materiales esten vacios (se resetean tras cada run)
    if(objetos.size()>0){
        objetos.clear();
    }
    if(materiales.size()>0){
        materiales.clear();
    }

    //Para preparar las barras de mejoras de armas
    texBarra        =   new Textura("resources/sprites/personaje/icons.png");
    barraExterior   =   new Sprite(texBarra, 16*4, 0, 48, 16);
    relleno         =   new Sprite(texBarra, 16*7.4, 16*10, 48, 12);

    //inicializo vectores 
    interfaz       =   {nullptr, nullptr, nullptr, nullptr, nullptr};
    sptInterfaz    =   {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    //Creamos texturas
    interfaz[0]    =   new Textura("resources/sprites/inventario/inventario-objetos.png");
    interfaz[1]    =   new Textura("resources/sprites/inventario/inventario-materiales.png");
    interfaz[2]    =   new Textura("resources/sprites/inventario/inventario-arma.png");
    interfaz[3]    =   new Textura("resources/sprites/inventario/cuadro.png");
    interfaz[4]    =   new Textura("resources/sprites/inventario/cuadro2.png");

    //Ahora los sprites correspondientes
    for(unsigned int i=0;   i<interfaz.size(); i++){
        if(i<3){
            sptInterfaz[i] =   new Sprite(interfaz[i], 0, 0, 1680, 1050);
            sptInterfaz[i]->setOrigin(1680/2, 1050/2);
            sptInterfaz[i]->setScale(3/4.f, 3/4.f);
        }
        else{
            sptInterfaz[i] =   new Sprite(interfaz[i], 0, 0, 16, 16);
            sptInterfaz[i]->setOrigin(16/2, 16/2);
            sptInterfaz[i]->setScale(6.f, 6.f);
        }
    }

    //Boton para fabricar objetos
    texFabricar         =   new Textura("resources/sprites/inventario/botonFabricar.png");
    sptFabricar         =   new Sprite(texFabricar, 0, 0, 32, 16);
    sptFabricar->setOrigin(32/2, 16/2);
    sptFabricar->setScale(4.f, 4.f);
    //Boton mejorar arma
    sptMejorar        =   new Sprite(texFabricar, 0, 16, 32, 16);
    sptMejorar->setOrigin(32/2, 16/2);
    sptMejorar->setScale(4.f, 4.f);
    //Boton eliminar runa
    sptBorrar         =   new Sprite(texFabricar, 0, 32, 32, 16);
    sptBorrar ->setOrigin(32/2, 16/2);
    sptBorrar ->setScale(4.f, 4.f);

    //Creamos la fuente
    fuente              =   new Fuente("resources/sprites/generales/Chonkly.otf");

    //Texto de la pestanya objetos
    textoObjeto         =   new Texto("Objetos");
    textoObjeto->setFuente(fuente);
    textoObjeto->setTamanyo(36);
    textoObjeto->setEscala(1, 1);
    textoObjeto->setColorTexto(blanco);

    //Texto para las cantidades a la hora de mejorar el arma
    
    cantidadPrimero         =   new Texto("");
    cantidadPrimero->setFuente(fuente);
    cantidadPrimero->setTamanyo(36);
    cantidadPrimero->setEscala(1, 1);
    cantidadPrimero->setColorTexto(blanco);

    cantidadSegundo         =   new Texto("");
    cantidadSegundo->setFuente(fuente);
    cantidadSegundo->setTamanyo(36);
    cantidadSegundo->setEscala(1, 1);
    cantidadSegundo->setColorTexto(blanco);

    maximo         =   new Texto("MEJORA AL MAXIMO");
    maximo ->setFuente(fuente);
    maximo ->setTamanyo(44);
    maximo ->setEscala(1, 1);
    maximo ->setColorTexto(blanco);

    
    

    //Texto de la pestanya materiales
    textoMateriales     =   new Texto("Materiales");
    textoMateriales->setFuente(fuente);
    textoMateriales->setTamanyo(36);
    textoMateriales->setEscala(1, 1);
    textoMateriales->setColorTexto(blanco);

    //Texto de la pestanya Arma
    textoArma               =   new Texto("Arma");
    textoArma->setFuente(fuente);
    textoArma->setTamanyo(36);
    textoArma->setEscala(1,1);
    textoArma->setColorTexto(blanco);


    //Iniciamos textos para cuando debamos mostrar el nombre y descripcion de algun item
    textoNombre              =   new Texto("Sample text");
    textoNombre->setFuente(fuente);
    textoNombre->setTamanyo(36);
    textoNombre->setEscala(1,1);
    textoNombre->setColorTexto(blanco);

    textoDescripcion         =   new Texto("Sample text");
    textoDescripcion->setFuente(fuente);
    textoDescripcion->setTamanyo(36);
    textoDescripcion->setEscala(1,1);
    textoDescripcion->setColorTexto(blanco);

    //Creamos instancia de fabrica de materiales
    FabricaMateriales* fabricaMateriales    =   FabricaMateriales::Instancia();

    //Llamamos al metodo para crear el vector materiales
    materiales  =   fabricaMateriales->creaMaterial();

    //Texto de la pestanya objetos
    textoFabricar         = new Texto("");
    textoFabricar->setFuente(fuente);
    textoFabricar->setTamanyo(36);
    textoFabricar->setEscala(1, 1);
    textoFabricar->setColorTexto(blanco);

    //Texto para las mejoras runicas
    for(int i = 0; i < 4; i++){
        Texto* textoRunas = new Texto("");
        textoRunas->setFuente(fuente);
        textoRunas->setTamanyo(36);
        textoRunas->setEscala(1, 1);
        textoRunas->setColorTexto(blanco);
        if(i == 1){
           textoRunas->setTamanyo(30); 
        }
        if(i > 1){
            textoRunas->setString("x 1");
        }
        textoRunico.push_back(textoRunas);
    }

    //Se recoge el puntero del arma
    puntArma        = *arma;

    //recogemos las mejoras que tiene actualemente el arma
    mejorasArma = puntArma->getMejorasArma();

    //Se crea el sprite usando el constructor de copia del Sprite que contiene el arma, como vamos a escalarla es necesaria ya que si no seria el mismo
    sptArma         =     new Sprite(*puntArma->getSprite());
    sptArma->setOrigin(sptArma->getRect()[2]/2, sptArma->getRect()[3]/2);
    sptArma->setScale(6.0f, 6.0f);

    //Creamos el nombre del arma
    nombreArma        =   new Texto(puntArma->getNombre());
    nombreArma ->setFuente(fuente);
    nombreArma ->setTamanyo(36);
    nombreArma ->setEscala(1,1);
    nombreArma ->setColorTexto(blanco);

    //Textura que contiene todo lo necesario para las runas, creamos el sprite de seleccion y hover y lo guardamos en el vector
    texRuna           =   new Textura("resources/sprites/inventario/runaContenedor.png");
    sptInterfaz[5] = new Sprite(texRuna, 64*3, 64*3, 64, 64);
    sptInterfaz[5]->setOrigin(64/2, 64/2);

    //Preparamos las posibles mejoras tanto las generales como las inherentes al arma
    for(unsigned int i = 0; i < 3; i++){ 
        if(i < 2){
            Sprite* auxiliar = new Sprite(texRuna, (2+i)*64, 0, 64, 64);
            auxiliar->setOrigin(64/2, 64/2);
            sptElementales.push_back(auxiliar); 
        }
        else{
            Sprite* auxiliar = new Sprite(texRuna, 0, 3*64, 64, 64);
            auxiliar->setOrigin(64/2, 64/2);
            sptElementales.push_back(auxiliar);
        }
        vector<int> recorte = puntArma->getRecorteSpriteRuna(i);
        Sprite* auxiliar = new Sprite(texRuna, recorte[0], recorte[1], 64, 64);
        auxiliar->setOrigin(64/2, 64/2);
        sptEspecificas.push_back(auxiliar);
    }

    for(int i = 0; i < 3; i++){
        Texto* t       =   new Texto("");
        t->setFuente(fuente);
        t->setTamanyo(36);
        t->setEscala(1, 1);
        t->setColorTexto(blanco);
        textosMejorables.push_back(t);
    }

    //Marcos necesarios para la interfaz
    marcos = Marco::Instancia()->getMarco();
}

Inventario::~Inventario(){

    //Borramos vector de punteros a objetos
    for(unsigned int i=0; i<objetos.size(); i++){
        delete objetos[i];
    }
    objetos.clear();

    //Borramos vector de punteros a materiales
    for(unsigned int i=0; i<materiales.size(); i++){
        delete materiales[i];
    }
    materiales.clear();

    //Borramos vectores de punteros a texturas y sprites
    for(unsigned int i=0;    i<interfaz.size(); i++){
        delete interfaz[i];
    }
    for(unsigned int i=0;    i<sptInterfaz.size(); i++){
        delete sptInterfaz[i];
    }
    for(unsigned int i = 0; i<textoRunico.size(); i++){
        if(textoRunico[i]!=nullptr){
            delete textoRunico[i];
        }
    }
    for(unsigned int i = 0; i<sptElementales.size(); i++){
        if(sptElementales[i]!=nullptr){
            delete sptElementales[i];
        }
    }
    for(unsigned int i = 0; i<sptEspecificas.size(); i++){
        if(sptEspecificas[i]!=nullptr){
            delete sptEspecificas[i];
        }
    }
    for(unsigned int i = 0; i<textosMejorables.size(); i++){
        if(textosMejorables[i]!=nullptr){
            delete textosMejorables[i];
        }
    }

    //Vaciamos los vectores
    textosMejorables.clear();
    sptEspecificas.clear();
    sptElementales.clear();
    marcos.clear();
    textoRunico.clear();
    interfaz.clear();
    sptInterfaz.clear();
    mejorasArma.clear();

    //Borramos punteros de fuente y textos
    puntArma = nullptr;
    delete fuente;
    delete texFabricar;
    delete sptFabricar;
    delete textoNombre;
    delete textoDescripcion;
    delete textoArma;
    delete textoMateriales;
    delete textoObjeto;
    delete textoFabricar;
    delete sptArma;
    delete texRuna;
    delete nombreArma;
    delete cantidadPrimero;
    delete cantidadSegundo;
    delete maximo;
    delete barraExterior;
    delete relleno;
    delete texBarra;

    //Vaciamos los vectores usados para el color
    blanco.clear();
    amarillo.clear();
}

//Metodo que disminuye la cantidad de materiales
void Inventario::incrementaMaterial(int indice, int cantidad){
    materiales[indice]->aumentarCantidad(cantidad);
}

//Metodo que disminuye la cantidad de materiales
void Inventario::reduceMaterial(int indice, int cantidad){
    materiales[indice]->reducirCantidad(cantidad);
}

//Metodo que devuelve la cantidad de materiales correspondiente a la id pasada
int Inventario::cantidadMateriales(int id){
    return materiales[id]->getCantidad();
}

//Comprobar a traves del id si existe un objeto
bool Inventario::poseeObjeto(int id){
    bool dev    =   false;
    for(unsigned int i = 0;   i < objetos.size() && !dev;   i++){
        if(id == objetos[i]->getId()){
            dev =   true;
        }
    }
    return dev;
}

//Metodo para mostrar el inventario en pantalla, tiene modos 0, 1 y 2 que corresponden a objetos, materiales y arma respectivamente, el modo decide que pestanya se muestra
void Inventario::dibujar(){
    
    //Recogemos la ventana 
    VentanaRender* window   =   VentanaRender::Instancia();

    //para que el inventario vaya conforme el jugador se mueva de sala
    vector<float> centroVista = window->getCentroVista();
    float despVistaX          = centroVista[0] - (window->getSize()[0]/2);
    float despVistaY          = centroVista[1] - (window->getSize()[1]/2);

    //Imprimimos primero la ventana principal, usando el modo
    window->dibujar(sptInterfaz[modo]);

    //Para que la ventana del inventario salga acorder a la camara
    for(unsigned int i = 0; i < 3; i++){
        sptInterfaz[i]->setPosition(despVistaX +950, despVistaY + 500);
    }
    textoObjeto->setPosicion(despVistaX + 375, despVistaY + 110);
    textoMateriales->setPosicion(despVistaX + 590, despVistaY + 110);
    textoArma->setPosicion(despVistaX + 875, despVistaY + 110);

    
    //Ahora con una condiciones vemos que necesitamos dibujar dentro de cada una de estas ventanas
    //Variable para indicar si hace falta pasar a otra fila las declaro aqui ya que se usan en 2 de los 3 modos
    int fila, columna, especial, centrar;
    vector<float> posicion;

    //Estamos en la pestanya objetos
    if(modo==0){

        //Recorremos la cantidad de objetos que tenemos
        for(unsigned int i = 0;   i < objetos.size(); i++){
            fila    =   75*(i/4);
            columna =   i%4;

            //Por cada objeto dibujamos un cuadrado negro alrededor
            sptInterfaz[3]->setPosition(despVistaX + 450 + columna*150, fila + despVistaY + 250 + fila);
            window->dibujar(sptInterfaz[3]);

            //Dibujarmos el objeto
            objetos[i]->getSprite()->setPosition(despVistaX + 450 + columna*150, fila + despVistaY + 250 +fila);
            window->dibujar(objetos[i]->getSprite());
            
        }

        //Si hemos elegido mediante click algun objeto se destacará su cuadro, y se mostrara a la derecha, el nombre y la descripcion.
        if(elegido!=-1){

            //Con esto le ponemos las mismas coordenadas al rectangulo azul de seleccion y lo dibujamos
            sptInterfaz[4]->setPosition(objetos[elegido]->getSprite()->getPosition()[0], objetos[elegido]->getSprite()->getPosition()[1]);
            window->dibujar(sptInterfaz[4]);

            //Y de esta manera mostramos el texto del objeto:
            //Primero asignamos la posicion a cada uno
            textoNombre->setPosicion(despVistaX + 1075, despVistaY + 175);
            textoDescripcion->setPosicion(despVistaX + 1075, despVistaY + 255);

            //Luego ponemos el nuevo string a cada uno
            textoNombre->setString(objetos[elegido]->getNombre());
            textoDescripcion->setString(objetos[elegido]->getDescripcion());

            //Finalmente lo dibujamos
            window->dibujar(textoDescripcion);
            window->dibujar(textoNombre);
        }

        //Con esto comprobamos si el raton esta haciendo hover en algun item
        if(hover!=-1){

            //Dibujamos el cuadrado azul donde el raton este haciendo hover
            sptInterfaz[4]->setPosition(objetos[hover]->getSprite()->getPosition()[0], objetos[hover]->getSprite()->getPosition()[1]);
            window->dibujar(sptInterfaz[4]);
        }
    }

    //Pestanya materiales
    if(modo==1){

        //Recorremos la cantidad de materiales que tenemos
        for(unsigned int i = 0;   i < materiales.size(); i++){

            //Con estas condiciones controlamos el numero materiales por fila, ya que la ultima fila muestra 4 debemos usar una condicion
            if(i >= materiales.size()-4){
                columna     =   i%4;
                fila        =   120*4;
                especial    =   50;
                centrar     =   0;
            }else{
                fila        =   120*(i/3);
                columna     =   i%3;
                especial    =   0;
                centrar     =   0;
                if(columna ==1 || columna == 2){
                    centrar =   25;
                }
            }

            //Por cada material dibujamos un cuadrado negro alrededor
            sptInterfaz[3]->setPosition(despVistaX + 450 + columna*(200 - especial + centrar), despVistaY + 250 + fila);
            window->dibujar(sptInterfaz[3]);

            //Cantidad del material
            string cant     = to_string(materiales[i]->getCantidad());
            Texto* cantidad = new Texto(cant);
            cantidad->setFuente(fuente);
            cantidad->setTamanyo(36);
            cantidad->setEscala(1,1);
            cantidad->setColorTexto(blanco);

            //Dibujamos el material y su cantidad
            materiales[i]->getSprite()->setPosition(despVistaX + 450 + columna*(200 - especial + centrar), despVistaY + 250 + fila);
            window->dibujar(materiales[i]->getSprite());
            posicion    =   materiales[i]->getSprite()->getPosition();
            cantidad->setPosicion(posicion[0] + 50, posicion[1] - 25);
            window->dibujar(cantidad);

            //Con esta condicion salta a los materiales especiales dependiendo del nivel del juego en el que estes
            if((nivelMateriales == 1 && i ==2 ) || (nivelMateriales == 2 && i == 5)){
                i   =   8;
            }
        }

        //Si hemos elegido mediante click algun objeto se destacará su cuadro, y se mostrara a la derecha, el nombre y la descripcion.
        if(elegido!=-1){

            //Con esto le ponemos las mismas coordenadas al rectangulo azul de seleccion y lo dibujamos
            sptInterfaz[4]->setPosition(materiales[elegido]->getSprite()->getPosition()[0], materiales[elegido]->getSprite()->getPosition()[1]);
            window->dibujar(sptInterfaz[4]);

            //Y de esta manera mostramos el texto del objeto:
            //Primero asignamos la posicion a cada uno
            textoNombre->setPosicion(despVistaX + 1075, despVistaY + 175);
            textoDescripcion->setPosicion(despVistaX + 1075, despVistaY + 255);

            //Luego ponemos el nuevo string a cada uno
            textoNombre->setString(materiales[elegido]->getNombre());
            textoDescripcion->setString(materiales[elegido]->getDescripcion());

            //Finalmente lo dibujamos
            window->dibujar(textoDescripcion);
            window->dibujar(textoNombre);

            //Dibujamos tambien lo necesario para fabricarlo y un boton en caso de ser posible su fabricacion
            if(elegido > 2 && elegido < 9){
                muestraNecesarios(elegido);
            }
        }
        //Con esto comprobamos si el raton esta haciendo hover en algun item
        if(hover != -1){

            //Dibujamos el cuadrado azul donde el raton este haciendo hover
            sptInterfaz[4]->setPosition(materiales[hover]->getSprite()->getPosition()[0], materiales[hover]->getSprite()->getPosition()[1]);
            window->dibujar(sptInterfaz[4]);
        }
    }
    //Pestanya para las armas
    if(modo == 2){

        //Se usa este int ya que hay 4 posiciones distintas
        int runa    =   0;

        //Imprimimos el nombre del arma y su sprite teniendo en cuenta la posicion actual de la vista
        nombreArma->setPosicion(100 + despVistaX + (1680/2) - nombreArma->getAncho()/2, despVistaY + 200);
        window->dibujar(nombreArma);
        sptArma->setPosition(despVistaX + 550, despVistaY + 350);
        window->dibujar(sptArma);

        //Este vector se usa para tener una distancia de referencia de las runas respecto al arma
        vector<int> t = sptArma->getGlobalPixelSize();

        //Para mostrar las mejoras del arma
        muestraMejoras(despVistaX, despVistaY);

        //Recorremos las runas que tiene el arma
        for(unsigned int j = 0; j < puntArma->getRunas().size(); j++){

                //Cuando se meta a cualquiera de las condiciones imprimira el hueco de la runa y si luego se mete en la condicion de dentro imprimira la runa que está activa actualmente en esa posicion.
                if(runa==0){
                    puntArma->getRunas()[j]->getSpriteGeneral()->setPosition(sptArma->getPosition()[0] - 48 - t[0]/2, sptArma->getPosition()[1]);
                    window->dibujar(puntArma->getRunas()[j]->getSpriteGeneral());
                    runa++;
                    if(puntArma->getRunas()[j]->activada()){
                        puntArma->getRunas()[j]->getSpriteRuna()->setPosition(sptArma->getPosition()[0] - 48 - t[0]/2, sptArma->getPosition()[1]);
                        window->dibujar(puntArma->getRunas()[j]->getSpriteRuna());
                    }
                }
                else if(runa==1){
                    puntArma->getRunas()[j]->getSpriteGeneral()->setPosition(sptArma->getPosition()[0], sptArma->getPosition()[1] - 48 - t[1]/2);
                    window->dibujar(puntArma->getRunas()[j]->getSpriteGeneral());
                    runa++;
                    if(puntArma->getRunas()[j]->activada()){
                        puntArma->getRunas()[j]->getSpriteRuna()->setPosition(sptArma->getPosition()[0], sptArma->getPosition()[1] - 48 - t[1]/2);
                        window->dibujar(puntArma->getRunas()[j]->getSpriteRuna());
                    }
                }
                else if(runa==2){
                    puntArma->getRunas()[j]->getSpriteGeneral()->setPosition(sptArma->getPosition()[0] + 48 + t[0]/2, sptArma->getPosition()[1]);
                    window->dibujar(puntArma->getRunas()[j]->getSpriteGeneral());
                    runa++;
                    if(puntArma->getRunas()[j]->activada()){
                        puntArma->getRunas()[j]->getSpriteRuna()->setPosition(sptArma->getPosition()[0] + 48 + t[0]/2, sptArma->getPosition()[1]);
                        window->dibujar(puntArma->getRunas()[j]->getSpriteRuna());
                    }
                }
                else if(runa==3){
                    puntArma->getRunas()[j]->getSpriteGeneral()->setPosition(sptArma->getPosition()[0], sptArma->getPosition()[1] + 48 + t[1]/2);
                    window->dibujar(puntArma->getRunas()[j]->getSpriteGeneral());
                    runa++;
                    if(puntArma->getRunas()[j]->activada()){
                        puntArma->getRunas()[j]->getSpriteRuna()->setPosition(sptArma->getPosition()[0], sptArma->getPosition()[1] + 48 + t[1]/2);
                        window->dibujar(puntArma->getRunas()[j]->getSpriteRuna());
                    }
                }
        }

        if(elegiHover != -1){
            interfazMejoras(despVistaX, despVistaY);
        }
        //Si se ha hecho hover en algun hueco de runa se imprime un sprite con la misma forma pero de color amarillo para indicarlo
        if(hover != -1){
            sptInterfaz[5]->setPosition(puntArma->getRunas()[hover]->getSpriteGeneral()->getPosition()[0], puntArma->getRunas()[hover]->getSpriteGeneral()->getPosition()[1]);
            window->dibujar(sptInterfaz[5]);
        }


        //Si se ha hecho click y elegido un hueco de runa se imprimel sprite amarillo de seleccion y la interfaz de mejoras
        if(elegido != -1){
            sptInterfaz[5]->setPosition(puntArma->getRunas()[elegido]->getSpriteGeneral()->getPosition()[0], puntArma->getRunas()[elegido]->getSpriteGeneral()->getPosition()[1]);
            interfazRunas(despVistaX, despVistaY);
            window->dibujar(sptInterfaz[5]);

            //Se comprueba si estas haciendo hover en una runa elemental de ser asi tiene el mismo comportamiento que los demas hover
            if(runaHover != -1){
                sptInterfaz[5]->setPosition(sptElementales[runaHover]->getPosition()[0], sptElementales[runaHover]->getPosition()[1]);
                sptInterfaz[5]->draw();
            }

            //Comprueba si has seleccionado una runa elemental la marca e imprime su nombre, descripcion, materiales necesario, y boton para fabricar
            if(mejoraElementalElegida != -1){
                imprimeNecesarios(mejoraElementalElegida, elegidaMayor);
                sptInterfaz[5]->setPosition(sptElementales[mejoraElementalElegida]->getPosition()[0], sptElementales[mejoraElementalElegida]->getPosition()[1]);
                sptInterfaz[5]->draw();
            }

            //Tanto el hover como eleccion que vienen a continuacion hacen lo mismo que los anteriores pero con las runas especificas de cada arma
            if(runaEspecificaHover !=- 1){
                sptInterfaz[5]->setPosition(sptEspecificas[runaEspecificaHover]->getPosition()[0], sptEspecificas[runaEspecificaHover]->getPosition()[1]);
                sptInterfaz[5]->draw();
            }
            if(mejoraRunicaElegida !=- 1){
                //Si es especifica se le suma 3 al indice ya que estas van del 3 al 5 en el case que hay dentro del metodo
                imprimeNecesarios(mejoraRunicaElegida+3, elegidaMayor);
                sptInterfaz[5]->setPosition(sptEspecificas[mejoraRunicaElegida]->getPosition()[0], sptEspecificas[mejoraRunicaElegida]->getPosition()[1]);
                sptInterfaz[5]->draw();
            }
        }
    }
    
    //Dibujamos los textos de las pestanyas
    window->dibujar(textoObjeto);
    window->dibujar(textoMateriales);
    window->dibujar(textoArma);
}

//Metodo para detectar si el raton esta haciendo hover por alguno de los items del inventario
void Inventario::posicionRaton(float x, float y){
    
    //Para comprobar si se cumple la siguiente condicion
    bool dev    =   false;

    //Para los objetos:
    if(modo ==   0){
        for(unsigned int i = 0;   i < objetos.size();   i++){

            //Comprobamos si las coordenadas x e y de lraton coinciden con algun sprite
            if(objetos[i]->getSprite()->contains(x, y)){
                //De ser asi nos guardamos el indice y ponemos el boolean a true
                hover   =   i;
                dev =   true;
            }
            //Si el raton no esta encima de nada borramos el indice guardado anteriormente
            if(!dev){
                hover   =   -1;
            }
        }
    }
    //Para los materiales:
    if(modo ==  1){
        for(unsigned int i = 0;   i < materiales.size();   i++){

            //Comprobamos si las coordenadas x e y del raton coinciden con algun sprite y que solo sean de los materiales del cuadro izquierdo
            if(materiales[i]->getSprite()->contains(x, y)   &&  x<materiales[2]->getSprite()->getPosition()[0] + 50){
                //De ser asi nos guardamos el indice y ponemos el boolean a true
                hover   =   i;
                dev =   true;
            }
            //Si el ratón no esta encima de nada borramos el indice guardado anteriormente
            if(!dev){
                hover   =   -1;
            }
        }
    }

    //Para las runas:
    if(modo ==  2){
        //Se recorre el array de runas que tiene el arma
        for(unsigned int i=0;   i<puntArma->getRunas().size(); i++){
            //Si el Sprite de hueco de runa conincide con las coordenadas del raton se guarda el indice y si es mayor, la variable dev se pone a true ya que en caso contrario al ser false reiniciaria el hover
            if(puntArma->getRunas()[i]->getSpriteGeneral()->contains(x, y)){
                hover   =   i;
                dev     = true;
                esMayor = puntArma->getRunas()[i]->esMayor();
            }
            if(!dev){
                hover = -1;
            }
        }

        for(unsigned int i = 0; i<textosMejorables.size(); i++){
            if(textosMejorables[i]->getContains(x, y)){
                dev = true;
                textosMejorables[i]->setColorTexto(amarillo);
            }
            if(!dev && (unsigned int)elegiHover != i){
                textosMejorables[i]->setColorTexto(blanco);
            }
        }

        //Mismo comportamiento que el anterior hover pero con las runas elementales y especificas, en este caso ya no hace falta guardar si es mayor, es lo proporciona el hueco
        for(unsigned int i=0; i<sptElementales.size(); i++){ 
            if(sptElementales[i]->contains(x, y)){
                runaHover   =   i;
                dev     = true;
            }
            if(!dev){
                runaHover = -1;
            }
        }
        for(unsigned int i=0; i<sptEspecificas.size(); i++){ 
            if(sptEspecificas[i]->contains(x, y)){
                runaEspecificaHover   =   i;
                dev     = true;
            }
            if(!dev){
                runaEspecificaHover  = -1;
            }
        }
    }

    //Para los textos de las pestanyas comprueba de que color debe ser el texto en funcio de si hay hover en ellos
    if(textoObjeto->getContains(x, y) && modo!=0){
        textoObjeto->setColorTexto(amarillo);
    }
    else{
        textoObjeto->setColorTexto(blanco);
    }
    if(textoMateriales->getContains(x, y) && modo!=1){
        textoMateriales->setColorTexto(amarillo);
    }
    else{
        textoMateriales->setColorTexto(blanco);
    }
    if(textoArma->getContains(x, y) && modo!=2){
        textoArma->setColorTexto(amarillo);
    }
    else{
        textoArma->setColorTexto(blanco);
    }
}

//Metodo para controlar el comportamiento de los clicks en el inventario
void Inventario::clickRaton(float x, float y){
    bool dev = false;
    //Para los objetos
    if(modo ==  0){
        for(unsigned int i=0;   i<objetos.size();   i++){
            //Si coinciden con las coordenadas del click guardamos el indice
            if(objetos[i]->getSprite()->contains(x, y) ){
                elegido =   i;
                dev     =   true;
            }
        }
        if(!dev){
            elegido =   -1;
        }
    }
    //Para los materiales
    if(modo ==  1){
        //Recorremos el array de los materiales
        for(unsigned int i=0;   i<materiales.size();   i++){
            //Si coinciden con las coordenadas del click guardamos el indice, tenemos en cuenta que solo sean del cuadrado izquierdo
            if(materiales[i]->getSprite()->contains(x, y) &&  x<materiales[2]->getSprite()->getPosition()[0]+50){
                elegido =   i;
                dev     =   true;
            }
        }
        //Controla la deseleccion de materiales
        if(!dev && !sptFabricar->contains(x, y)){
            elegido =   -1;
        }
        //Comrpueba si haces click en el boton de fabricar y llama al metodo correspondiente
        if(sptFabricar->contains(x, y)){           
            fabricar(elegido);
        }       
    }
    //Para las runas
    if(modo == 2){

        //Recorremos el array de runas del arma
        for(unsigned int i=0;   i < puntArma->getRunas().size();   i++){

            //Si coinciden con las coordenadas del click guardamos el indice, si es mayor y controlamos la deseleccion de mejoras
            if(puntArma->getRunas()[i]->getSpriteGeneral()->contains(x, y)){
                elegido =   i;
                dev     =   true;
                elegidaMayor = puntArma->getRunas()[i]->esMayor();
                mejoraElementalElegida = -1;
                mejoraRunicaElegida = -1;
                elegiHover = -1;
            }
        }
        for(unsigned int i = 0; i<textosMejorables.size(); i++){
            if(textosMejorables[i]->getContains(x, y)){
                dev = true;
                elegiHover = i;
                textosMejorables[i]->setColorTexto(amarillo);
                for(unsigned int j = 0; j<textosMejorables.size(); j++){
                    if(j!=i){
                        textosMejorables[j]->setColorTexto(blanco);
                    }
                }
                mejoraElementalElegida = -1;
                mejoraRunicaElegida = -1;
                elegido = -1;
            }
            if(!dev && (x<marcos[0]->getPosition()[0] || x>marcos[2]->getPosition()[0] || y<marcos[0]->getPosition()[1] || y>marcos[6]->getPosition()[1])){
                elegiHover = -1;
                textosMejorables[i]->setColorTexto(blanco);
            }
        }
        if(sptMejorar->contains(x, y)){
            mejorarArma();
        }
        //La deseleccion no funciona dentro del marco de mejoras, asi damos mejor experiencia la usuario
        if(!dev && (x<marcos[0]->getPosition()[0] || x>marcos[2]->getPosition()[0] || y<marcos[0]->getPosition()[1] || y>marcos[6]->getPosition()[1])){           
            elegido =   -1;
            mejoraElementalElegida = -1;
            mejoraRunicaElegida = -1;
        }

        //Se recorren las mejoras elementales y especificas, si encuentra una de ellas guarda el indice y pone a -1 la otra
        for(unsigned int i = 0; i<sptElementales.size(); i++){ 
            if(sptElementales[i]->contains(x, y)){   
                mejoraElementalElegida = i;
                mejoraRunicaElegida = -1;
            } 
        }

        for(unsigned int i = 0; i<sptEspecificas.size(); i++){     
            if(sptEspecificas[i]->contains(x, y)){            
                mejoraRunicaElegida = i;
                mejoraElementalElegida = -1;
            } 
        }

        //Boton de fabricar de la runa, discrimina si es elemental o especifica para pasar true o false en el tercer parametro
        if(sptFabricar->contains(x, y) && elegido!=-1){       
            if(mejoraRunicaElegida != -1){
                fabricaRuna(elegido, mejoraRunicaElegida, false, elegidaMayor );
            }
            else{
                
                fabricaRuna(elegido, mejoraElementalElegida, true, elegidaMayor );
            }
        }

        //Boton para borrar las runas
        if(sptBorrar->contains(x, y) && elegido!=-1){       
                borraRuna();
        }
    }

    //Para los textos de las pestanyas
    if(textoObjeto->getContains(x, y) && modo!=0){
        modo=0;
        elegido=-1;
    }
    if(textoMateriales->getContains(x, y) && modo!=1){
        modo=1;
        elegido=-1;
    }
    if(textoArma->getContains(x, y) && modo!=2){
        modo=2;
        elegido=-1;
    }
}

//Metodo que recibe un objeto y lo anyade al vector de objetos
void Inventario::anyadeObjeto(Objeto* obj){

    if(obj != nullptr) {
        objetos.push_back(obj);
    }
}

//Metodo que recibe la id de un objeto y si se encientra en el vector lo elimina

void Inventario::eliminaObjeto(int id) {

    bool buscando = true;

    //Recorre el vector de objetos, se usa un boolean para parar una vez lo encuentre y bajar coste computacional
    for(unsigned int i=0; i < objetos.size() && buscando; i++) {
        if(objetos[i] != nullptr && objetos[i]->getId() == id) {
            
            //Si la id coincide, lo borra
            delete objetos[i];
            objetos.erase(objetos.begin() + i);
            buscando = false;
        }
    }
}

//Metodo para fabricar materiales
void Inventario::fabricar(int indice){

    //Se le pasa el indice  del material al fabricar, y dentro del switch se decide que hacer dependiendo del caso
    //El temporizador controla que no hagas materiales de más sin querer y ayuda a la animacion del boton
    if(temporizador>=tiempo){
        switch (indice){
        /**
         * Todos los case funcionan igual, comprueban que tengas la cantidad necesaria de materiales necesario, si es asi los reduce y aumenta en 1
         * el material deseado, cambia el sprite del boton y activa el temporizador para que el boton luego vuelva a la normalidad
         */
        case 3:
            if(materiales[0]->getCantidad()>=3 && materiales[1]->getCantidad()>=2){
                materiales[0]->reducirCantidad(3);
                materiales[1]->reducirCantidad(2);
                materiales[3]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        case 4:
            if(materiales[0]->getCantidad()>=2 && materiales[2]->getCantidad()>=2){
                materiales[0]->reducirCantidad(2);
                materiales[2]->reducirCantidad(2);
                materiales[4]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        case 5:
            if(materiales[0]->getCantidad()>=1 && materiales[1]->getCantidad()>=3 && materiales[2]->getCantidad()>=1){
                materiales[0]->reducirCantidad(1);
                materiales[1]->reducirCantidad(3);
                materiales[2]->reducirCantidad(1);
                materiales[5]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        case 6:
            if(materiales[3]->getCantidad()>=3 && materiales[4]->getCantidad()>=2){
                materiales[3]->reducirCantidad(3);
                materiales[4]->reducirCantidad(2);
                materiales[6]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        case 7:
            if(materiales[4]->getCantidad()>=4 && materiales[5]->getCantidad()>=2){
                materiales[3]->reducirCantidad(4);
                materiales[5]->reducirCantidad(2);
                materiales[7]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        case 8:
            if(materiales[3]->getCantidad()>=2 && materiales[5]->getCantidad()>=3){
                materiales[3]->reducirCantidad(2);
                materiales[5]->reducirCantidad(3);
                materiales[8]->aumentarCantidad(1);
                sptFabricar->setRect(32, 0, 32, 16);
                temporizador = 0;
            }
            break;
        
        default:
            break;
        }
    }


    
}

//Metodo al cual se le pasa el indice del material y muestra lo que se necesita para fabricarlo
void Inventario::muestraNecesarios(int indice){
    
    //Recogemos la ventana 
    VentanaRender* window     =   VentanaRender::Instancia();

    //para poner los sprites y texto donde se debe
    vector<float> centroVista = window->getCentroVista();
    float despVistaX          = centroVista[0] - (window->getSize()[0]/2);
    float despVistaY          = centroVista[1] - (window->getSize()[1]/2);

    switch (indice)
    {
    /**
     * En los cases se imprime el texto con las cantidades necesarias y los sprites los materiales necesarios se can construyendo e imprimiendo los strings
     * si no tienes suficiente cantidad de materiales, la cantidad se imprime en rojo, en caso contrario en blanco
     */
    case 3:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[0]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[1]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        window->dibujar(materiales[0]->getSprite());
        window->dibujar(materiales[1]->getSprite()); 
        if(materiales[0]->getCantidad()<3){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 3");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[1]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    case 4:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[0]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[2]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        window->dibujar(materiales[0]->getSprite());
        window->dibujar(materiales[2]->getSprite()); 
        if(materiales[0]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[1]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    case 5:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[0]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[1]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        materiales[2]->getSprite()->setPosition(despVistaX+1100, despVistaY+700);
        window->dibujar(materiales[0]->getSprite());
        window->dibujar(materiales[1]->getSprite());
        window->dibujar(materiales[2]->getSprite());  
        if(materiales[0]->getCantidad()<1){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 1");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[1]->getCantidad()<3){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 3");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        if(materiales[2]->getCantidad()<1){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 1");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+675);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    case 6:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[3]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[4]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        window->dibujar(materiales[3]->getSprite());
        window->dibujar(materiales[4]->getSprite()); 
        if(materiales[3]->getCantidad()<3){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 3");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[4]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    case 7:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[4]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[5]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        window->dibujar(materiales[4]->getSprite());
        window->dibujar(materiales[5]->getSprite()); 
        if(materiales[4]->getCantidad()<4){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 4");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[5]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    case 8:
        textoFabricar->setColorTexto(blanco);
        textoFabricar->setString("Necesitas:");
        textoFabricar->setPosicion(despVistaX+1075, despVistaY+400);
        window->dibujar(textoFabricar);
        materiales[3]->getSprite()->setPosition(despVistaX+1100, despVistaY+500);
        materiales[5]->getSprite()->setPosition(despVistaX+1100, despVistaY+600);
        window->dibujar(materiales[3]->getSprite());
        window->dibujar(materiales[5]->getSprite()); 
        if(materiales[3]->getCantidad()<2){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        }  
        textoFabricar->setString("x 2");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+475);
        window->dibujar(textoFabricar);
        if(materiales[5]->getCantidad()<3){
            textoFabricar->setColorTexto({255,0,0,255});
        }
        else{
            textoFabricar->setColorTexto(blanco);
        } 
        textoFabricar->setString("x 3");
        textoFabricar->setPosicion(despVistaX+1150, despVistaY+575);
        window->dibujar(textoFabricar);
        sptFabricar->setPosition(despVistaX+1300, despVistaY+825);
        window->dibujar(sptFabricar);
        break;
    }
}

//Modifica el nivel de los materiales que se pueden imprimir
void Inventario::setNivelMateriales(int nivel) {
    if(nivel >= 1) {
        nivelMateriales = nivel;
    }
}

//Temporizador para animar boton y evitar crafteos de más
void Inventario::actualizarTemporizador(int time){
    if(temporizador<tiempo){
        temporizador+=time;
        if(temporizador>=tiempo){
            sptFabricar->setRect(0, 0, 32, 16);
            sptMejorar->setRect(0, 16, 32, 16);
            sptBorrar->setRect(0, 32, 32, 16);
        }
    }
}

//Metodo que recoge un arma nueva y asigna las variables de clase acorde a las de la nueva arma
void Inventario::nuevaArma(Arma** arma){
    //Para evitar problemas con la seleccion de runas se ponen la variables de eleccion a -1
    elegido=-1;
    mejoraElementalElegida=-1;
    mejoraRunicaElegida=-1;
    hover =-1;
    elegiHover = -1;
    //Limpiamos el vector de mejoras
    mejorasArma.clear();
    //Recogemos el puntero
    puntArma = *arma;
    //Borramos el sprite que teniamos guardado anteriormente y creamos uno nuevo asignando 
    delete sptArma;
    sptArma = new Sprite(*puntArma->getSprite());
    sptArma->setOrigin(sptArma->getRect()[2]/2, sptArma->getRect()[3]/2);
    sptArma->setScale(6.0f, 6.0f);
    nombreArma->setString(puntArma->getNombre());
    
    //Cambiamos las runas epecificas
    for(int i = 0; i<3; i++){
        vector<int> recorte = puntArma->getRecorteSpriteRuna(i);
        sptEspecificas[i]->setRect(recorte[0], recorte[1], 64, 64);
    }
    mejorasArma = puntArma->getMejorasArma();
}

//Imprime la interfaz de mejoras runicas se le pasa el desplazamiento de la vista
void Inventario::interfazRunas(int despX, int despY){
    //Se escalan los marcos
    for(unsigned int i = 0; i<marcos.size(); i++){
        marcos[i]->setScale(2.f, 2.f);
    }
    marcos[0]->setPosition(despX+400, despY+600);//esquina sup izq
    marcos[1]->setPosition(despX+950, despY+600);//raya arriba
    marcos[2]->setPosition(despX+1500, despY+600);//esq sup dere
    marcos[3]->setPosition(despX+1500, despY+725);//raya dere
    marcos[4]->setPosition(despX+1500, despY+850);//esq dere abajo
    marcos[5]->setPosition(despX+950, despY+850);//raya abajo
    marcos[6]->setPosition(despX+400, despY+850);//esq inf izq
    marcos[7]->setPosition(despX+400, despY+725);//raya izq
    //Tras ponerle la posicion a cada uno se dibujan
    for(unsigned int i = 0; i<marcos.size(); i++){
        marcos[i]->draw();
    }
    //Dibujamos los Sprites de las mejoras disponibles para el arma
    for(int i = 0; i<3; i++){
        sptEspecificas[i]->setPosition(despX+450+(64*i), despY+750);
        sptEspecificas[i]->draw();
        sptElementales[i]->setPosition(despX+450+(64*i), despY+650);
        sptElementales[i]->draw();
    }
    if(puntArma->getRunas()[elegido]->activada()){
        sptBorrar->setPosition(despX+1400, despY+800);
        sptBorrar->draw();
    }
}
//Metodo que imprime lo que necesitas para fabricar cada runa, recibe el indice y si es mayor o menor
void Inventario::imprimeNecesarios(int runa, bool tipo){
    //Recogemos la ventana 
    VentanaRender* window        =   VentanaRender::Instancia();
    //para que el hud vaya conforme el jugador se mueva de sala
    vector<float> centroVista   =    window->getCentroVista();
    float despX                 =    centroVista[0] - (window->getSize()[0]/2);
    float despY                 =    centroVista[1] - (window->getSize()[1]/2);
    //Se pone la posicion de cada texto
    textoRunico[0]->setPosicion(despX+650, despY+600);
    textoRunico[1]->setPosicion(despX+650, despY+650);
    textoRunico[2]->setPosicion(despX+750, despY+725);
    textoRunico[3]->setPosicion(despX+750, despY+775);
    //Inicializo vectores necesarios en las siguientes condiciones
    vector<int> matNecesario;
    vector<string> nombre;
    vector<string> descMenor;
    vector<string> descMayor;
    /**
     * Si el indice es mayor que 2 nos encontramos ante una mejora especifica por lo que recogemos todos los datos necesarios
     * Los materiales necesarios para fabricarla, el nombre la descripcion si es menor y la descripcion si es mayor
     * Tambien compruebo si tengo materiales necesarios y de ser asi los imprimire en blanco y si no en rojo
     */
    if(runa>2){
        matNecesario = puntArma->getMaterialesRuna(runa-3);
        nombre = puntArma->getNombreRunas();
        descMenor = puntArma->getDescRunasNormales();
        descMayor = puntArma->getDescRunasGrandes();
        if(materiales[matNecesario[0]]->getCantidad()<1){
            textoRunico[2]->setColorTexto({255,0,0,255});
        }
        else{
            textoRunico[2]->setColorTexto(blanco);
        }
        if(materiales[matNecesario[1]]->getCantidad()<1){
            textoRunico[3]->setColorTexto({255,0,0,255});
        }
        else{
            textoRunico[3]->setColorTexto(blanco);
        }
        materiales[matNecesario[0]]->getSprite()->setPosition(despX+700, despY+750);
        materiales[matNecesario[0]]->getSprite()->draw();
        materiales[matNecesario[1]]->getSprite()->setPosition(despX+700, despY+800);
        materiales[matNecesario[1]]->getSprite()->draw();
    }
    else{
        if(materiales[9]->getCantidad()<1){
            textoRunico[2]->setColorTexto({255,0,0,255});
        }
        else{
            textoRunico[2]->setColorTexto(blanco);
        }
        textoRunico[3]->setColorTexto(blanco);
        materiales[9]->getSprite()->setPosition(despX+700, despY+750);
    }
    /**
     * Dependiendo del caso mostramos una runa u otra, los 3 primeros al ser elementales la informacion es la misma para cualquier arma
     * En los 3 ultimos casos al ser especificas usamos los datos que hemos recopilado si el indice de la runa era mayor que 2
     */
    switch (runa)
    {
    case 0:
        if(tipo){
            textoRunico[0]->setString("Runa de fuego mayor");
            textoRunico[1]->setString("Quema al enemigo +6dps");
            materiales[10]->getSprite()->setPosition(despX+700, despY+800);
            materiales[10]->getSprite()->draw();
            if(materiales[10]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }     
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        else{
            textoRunico[0]->setString("Runa de fuego menor");
            textoRunico[1]->setString("Quema al enemigo +3dps");
            materiales[10]->getSprite()->setPosition(despX+700, despY+800);
            materiales[10]->getSprite()->draw();
            if(materiales[10]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        materiales[9]->getSprite()->draw();
        break;
    case 1:
        if(tipo){
            textoRunico[0]->setString("Runa de hielo mayor");
            textoRunico[1]->setString("Ralentiza mucho al enemigo");
            materiales[11]->getSprite()->setPosition(despX+700, despY+800);
            materiales[11]->getSprite()->draw();
            if(materiales[11]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        else{
            textoRunico[0]->setString("Runa de hielo menor");
            textoRunico[1]->setString("Ralentiza un poco al enemigo");
            materiales[11]->getSprite()->setPosition(despX+700, despY+800);
            materiales[11]->getSprite()->draw();
            if(materiales[11]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        materiales[9]->getSprite()->draw();
        break;
    case 2:
        if(tipo){
            textoRunico[0]->setString("Runa de electricidad mayor");
            textoRunico[1]->setString("Aumenta mucho el danyo hacia el enemigo");
            materiales[12]->getSprite()->setPosition(despX+700, despY+800);
            materiales[12]->getSprite()->draw();
            if(materiales[12]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        else{
            textoRunico[0]->setString("Runa de electricidad menor");
            textoRunico[1]->setString("Aumenta un poco el danyo hacia el enemigo");
            materiales[12]->getSprite()->setPosition(despX+700, despY+800);
            materiales[12]->getSprite()->draw();
            if(materiales[12]->getCantidad()<1){
                textoRunico[3]->setColorTexto({255,0,0,255});
            }
            for(unsigned int i=0; i<textoRunico.size(); i++){
                textoRunico[i]->draw();
            }
        }
        materiales[9]->getSprite()->draw();
        break;
    case 3:
        
        if(tipo){
            textoRunico[0]->setString(nombre[0]+" mayor");
            textoRunico[1]->setString(descMayor[0]);
        }
        else{
            textoRunico[0]->setString(nombre[0]+" menor");
            textoRunico[1]->setString(descMenor[0]);
        }
        for(unsigned int i=0; i<textoRunico.size(); i++){
            textoRunico[i]->draw();
        }
        break;
    case 4:
        
        if(tipo){
            textoRunico[0]->setString(nombre[1]+" mayor");
            textoRunico[1]->setString(descMayor[1]);
        }
        else{
            textoRunico[0]->setString(nombre[1]+" menor");
            textoRunico[1]->setString(descMenor[1]);
        }
        for(unsigned int i=0; i<textoRunico.size(); i++){
            textoRunico[i]->draw();
        }
        break;
    case 5:
      
        if(tipo){
            textoRunico[0]->setString(nombre[2]+" mayor");
            textoRunico[1]->setString(descMayor[2]);
        }
        else{
            textoRunico[0]->setString(nombre[2]+" menor");
            textoRunico[1]->setString(descMenor[2]);
        }
        for(unsigned int i=0; i<textoRunico.size(); i++){
            textoRunico[i]->draw();
        }
        break;
    default:
        break;
    }
    //Por ultimo mostramos el boton para fabricarla
    sptFabricar->setPosition(despX+1250, despY+800);
    sptFabricar->draw();
}

//Metodo para fabricar la runa, se le pasa indice del hueco, el indice de mejora, un boolean para saber si es elemental y otro para saber si es mayor
void Inventario::fabricaRuna(int indice, int indiceMejora, bool esElemental, bool esMayor){
    //Estos dos vectores guardan las runas activas grandes o pequeñas del arma
    vector<bool> fabricadasG = puntArma->getRunasGrandes();
    vector<bool> fabricadasP = puntArma->getRunasPequenyas();
    //Vector m = materiales necesarios para la fabricacion. Vector r = area de recorte de la runa en cuestion
    vector<int> m;
    vector<int> r;
    //Si es elemental sumamos 3 al indice de mejora, recordamos que 0 a 2 son elementales y 3 a 5 son especificas
    if(!esElemental){
        indiceMejora+=3;
    }
    //Si es especifica recogemos los materiales necesarios y las areas de recorte
    if(indiceMejora>2){
        m = puntArma->getMaterialesRuna(indiceMejora-3);
        r = puntArma->getRecorteSpriteRuna(indiceMejora-3);
    }
    //Si hay ya una runa en ese hueco no se puede fabricar una nueva
    if(puntArma->getRunas()[indice]->activada()){
        indiceMejora=99;
    }
    //Temporizador para animar el boton
    if(temporizador>=tiempo){
        /**
         * El switch depende de si vamos a fabricar las mejoras de la 0 a la 5 en cualquiera de los casos se comprueba que el arma ya no *tenga
         * ninguna mejora igual de la que se quiere aplicar, se comrpueba que tenemos suficientes materiales y de ser asi los reduce, e *invoca
         * un metodo de arma para activar el efecto y con el area de recorte (predefinida si es elemental y recogida si es especifica) 
         * crea el sprite que se imprime encima del hueco en el array de runas del arma, luego cambia el boton a pulsado y el temporizador *se activa
         */
        switch (indiceMejora)
        {
        case 0:
            if(!fabricadasG[0] && !fabricadasP[0] ){
                if(materiales[9]->getCantidad()>=1 && materiales[10]->getCantidad()>=1){
                    materiales[9]->reducirCantidad(1);
                    materiales[10]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna({2*64, 0}, 0);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        case 1:
            if(!fabricadasG[1] && !fabricadasP[1] ){
                if(materiales[9]->getCantidad()>=1 && materiales[11]->getCantidad()>=1){
                    materiales[9]->reducirCantidad(1);
                    materiales[11]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna({3*64, 0}, 1);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        case 2:
            if(!fabricadasG[2] && !fabricadasP[2] ){
                if(materiales[9]->getCantidad()>=1 && materiales[12]->getCantidad()>=1){
                    materiales[9]->reducirCantidad(1);
                    materiales[12]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna({0, 3*64}, 2);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        case 3:
            if(!fabricadasG[3] && !fabricadasP[3] ){
                if(materiales[m[0]]->getCantidad()>=1 && materiales[m[1]]->getCantidad()>=1){
                    materiales[m[0]]->reducirCantidad(1);
                    materiales[m[1]]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna(r, 3);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        case 4:
            if(!fabricadasG[4] && !fabricadasP[4] ){
                if(materiales[m[0]]->getCantidad()>=1 && materiales[m[1]]->getCantidad()>=1){
                    materiales[m[0]]->reducirCantidad(1);
                    materiales[m[1]]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna(r, 4);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        case 5:
            if(!fabricadasG[5] && !fabricadasP[5] ){
                if(materiales[m[0]]->getCantidad()>=1 && materiales[m[1]]->getCantidad()>=1){
                    materiales[m[0]]->reducirCantidad(1);
                    materiales[m[1]]->reducirCantidad(1);
                    puntArma->setRunas(indiceMejora, esMayor, true );
                    puntArma->getRunas()[indice]->setRuna(r, 5);
                    sptFabricar->setRect(32, 0, 32, 16);
                    temporizador = 0;
                }
            }
            break;
        default:
            break;
        }
    }
}

//Borra la runa elegida actualmente
void Inventario::borraRuna(){
    if(temporizador>=tiempo){
        if(elegido!=-1){
            puntArma->setRunas(puntArma->getRunas()[elegido]->getIndice(), puntArma->getRunas()[elegido]->esMayor(), false);
            sptBorrar->setRect(32, 32, 32, 16);
            puntArma->getRunas()[elegido]->desactivar();
            temporizador = 0;
        }
    }    
}

//Dibuja las barras de las mejoras tanto las exteriores como su relleno, tambien los nombres de cada una de elllas
void Inventario::muestraMejoras(int despX, int despY){
    textosMejorables[0]->setString(puntArma->getTextoMejoras()[0]);
    textosMejorables[1]->setString(puntArma->getTextoMejoras()[1]);
    textosMejorables[2]->setString(puntArma->getTextoMejoras()[2]);
    for(unsigned int i = 0; i<textosMejorables.size(); i++){
        textosMejorables[i]->setPosicion(despX+800, despY+300+(i*60));
        textosMejorables[i]->draw();
    }

    //Para el danyo
    for(int i = 0; i<MEJORA_danyo; i++){
        barraExterior->setPosition(despX+1000+(i*60), despY+295);
        barraExterior->setScale(1.f, 4.f);
        barraExterior->draw();
        if(i<mejorasArma[0]){
            relleno->setPosition(despX+998+(i*60), despY+287);
            relleno->setScale(1.f, 4.f);
            relleno->draw();
        }
    }
    //Para el resto
    for(int i = 0; i<MEJORA_resto; i++){
        barraExterior->setScale(2.f, 4.f);
        barraExterior->setPosition(despX+1000+(i*120), despY+355);
        barraExterior->draw();
        if(i<mejorasArma[1]){
            relleno->setPosition(despX+998+(i*120), despY+347);
            relleno->setScale(2.f, 4.f);
            relleno->draw();
        }
        barraExterior->setPosition(despX+1000+(i*120), despY+415);
        barraExterior->draw();
        if(i<mejorasArma[2]){
            relleno->setPosition(despX+998+(i*120), despY+407);
            relleno->setScale(2.f, 4.f);
            relleno->draw();
        }
    }
}

void Inventario::interfazMejoras(int despX, int despY){
    //Se escalan los marcos
    for(unsigned int i = 0; i<marcos.size(); i++){
        marcos[i]->setScale(2.f, 2.f);
    }
    marcos[0]->setPosition(despX+700, despY+600);//esquina sup izq
    marcos[1]->setPosition(despX+950, despY+600);//raya arriba
    marcos[2]->setPosition(despX+1200, despY+600);//esq sup dere
    marcos[3]->setPosition(despX+1200, despY+725);//raya dere
    marcos[4]->setPosition(despX+1200, despY+850);//esq dere abajo
    marcos[5]->setPosition(despX+950, despY+850);//raya abajo
    marcos[6]->setPosition(despX+700, despY+850);//esq inf izq
    marcos[7]->setPosition(despX+700, despY+725);//raya izq
    //Tras ponerle la posicion a cada uno se dibujan
    for(unsigned int i = 0; i<marcos.size(); i++){
        marcos[i]->draw();
    }
    //Inicializamos vector con los materiales para la mejora
    vector<int> matsMejora;
    if(elegiHover==0){
        //Si esta mejorada al maximo imprimira un mensaje y no se metera en esta condicion
        if(mejorasArma[0]<6){
            //Dependiendo de el tipo de arma y su nivel de mejora obtenemos unos requisitos u otros
            matsMejora = puntArma->getMaterialesMejora(elegiHover, mejorasArma[0]);
            materiales[matsMejora[2]]->getSprite()->setPosition(despX+800, despY+750);
            materiales[matsMejora[2]]->getSprite()->draw();
            materiales[matsMejora[0]]->getSprite()->setPosition(despX+800, despY+675);
            materiales[matsMejora[0]]->getSprite()->draw();
            //Si no tenemos los suficientes se dibujara la cantidad en rojo
            if(materiales[matsMejora[0]]->getCantidad()<matsMejora[1]){
                cantidadPrimero->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadPrimero->setColorTexto(blanco);
            }
            if(materiales[matsMejora[2]]->getCantidad()<matsMejora[3]){
                cantidadSegundo->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadSegundo->setColorTexto(blanco);
            }
            cantidadPrimero->setString("x "+to_string(matsMejora[1]));
            cantidadPrimero->setPosicion(despX+850, despY+650);
            cantidadPrimero->draw();
            cantidadSegundo->setString("x "+to_string(matsMejora[3]));
            cantidadSegundo->setPosicion(despX+850, despY+725);
            cantidadSegundo->draw();
            sptMejorar->setPosition(despX+1050, despY+710);
            sptMejorar->draw();
        }
        else{
            maximo ->setPosicion(despX+712, despY+698);
            maximo ->draw();
        }
    }
    else if(elegiHover==1){
        if(mejorasArma[1]<3){
            matsMejora = puntArma->getMaterialesMejora(elegiHover, mejorasArma[1]);
            materiales[matsMejora[2]]->getSprite()->setPosition(despX+800, despY+750);
            materiales[matsMejora[2]]->getSprite()->draw();
            materiales[matsMejora[0]]->getSprite()->setPosition(despX+800, despY+675);
            materiales[matsMejora[0]]->getSprite()->draw();
            if(materiales[matsMejora[0]]->getCantidad()<matsMejora[1]){
                cantidadPrimero->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadPrimero->setColorTexto(blanco);
            }
            if(materiales[matsMejora[2]]->getCantidad()<matsMejora[3]){
                cantidadSegundo->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadSegundo->setColorTexto(blanco);
            }
            cantidadPrimero->setString("x "+to_string(matsMejora[1]));
            cantidadPrimero->setPosicion(despX+850, despY+650);
            cantidadPrimero->draw();
            cantidadSegundo->setString("x "+to_string(matsMejora[3]));
            cantidadSegundo->setPosicion(despX+850, despY+725);
            cantidadSegundo->draw();
            sptMejorar->setPosition(despX+1050, despY+710);
            sptMejorar->draw();
        }
        else{
            maximo ->setPosicion(despX+712, despY+698);
            maximo ->draw();
        }
    }
    else if(elegiHover==2){
        if(mejorasArma[2]<3){

            matsMejora = puntArma->getMaterialesMejora(elegiHover, mejorasArma[2]);
            materiales[matsMejora[2]]->getSprite()->setPosition(despX+800, despY+750);
            materiales[matsMejora[2]]->getSprite()->draw();
            materiales[matsMejora[0]]->getSprite()->setPosition(despX+800, despY+675);
            materiales[matsMejora[0]]->getSprite()->draw();
            if(materiales[matsMejora[0]]->getCantidad()<matsMejora[1]){
                cantidadPrimero->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadPrimero->setColorTexto(blanco);
            }
            if(materiales[matsMejora[2]]->getCantidad()<matsMejora[3]){
                cantidadSegundo->setColorTexto({255, 0, 0, 255});
            }
            else{
                cantidadSegundo->setColorTexto(blanco);
            }
            cantidadPrimero->setString("x "+to_string(matsMejora[1]));
            cantidadPrimero->setPosicion(despX+850, despY+650);
            cantidadPrimero->draw();
            cantidadSegundo->setString("x "+to_string(matsMejora[3]));
            cantidadSegundo->setPosicion(despX+850, despY+725);
            cantidadSegundo->draw();
            sptMejorar->setPosition(despX+1050, despY+710);
            sptMejorar->draw();
        }
        else{
            maximo ->setPosicion(despX+712, despY+698);
            maximo ->draw();
        }

    }
}

void Inventario::mejorarArma(){
    /**
     * Se comprueba que la mejora no esté al maximo (aunque no deberia poder darle al boton ya que desaparece al estar al  maximo)
     * Usamos el temporizador para establecer un tiempo entre mejora y mejora para evitar mejorar un arma de mas sin intencion.
     * Si se tienen lo materiales estos se reducen en cantidad necesaria se llama un metodo que las activa en el arma y actualizamos
     * las mejoras actuales del arma
     */
    bool check = true;
    vector<int> mats = puntArma->getMaterialesMejora(elegiHover, mejorasArma[elegiHover]);
    if(elegiHover==0){
        if(mejorasArma[elegiHover]==6){
            check=false;
        }
    }
    else{
        if(mejorasArma[elegiHover]==3){
            check=false;
        }
    }
    if(check){
        if(temporizador>=tiempo){   
            if(mats.size()>=4){
                if(materiales[mats[0]] != nullptr && materiales[mats[2]] != nullptr){
                    if(materiales[mats[0]]->getCantidad()>=mats[1] && materiales[mats[2]]->getCantidad()>=mats[3]){
                        sptMejorar->setRect(32, 16, 32, 16);
                        materiales[mats[0]]->reducirCantidad(mats[1]);
                        materiales[mats[2]]->reducirCantidad(mats[3]);
                        puntArma->setMejorasArma(elegiHover, mejorasArma[elegiHover]);
                        mejorasArma = puntArma->getMejorasArma(); 
                        temporizador = 0;
                    }
                }
            }     
        }
    }

}


