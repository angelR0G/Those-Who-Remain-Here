#include "MaterialSuelto.h"

//Constructor. Ademas de los parametros de ObjetoSala, recibe la id del material y la cantidad que contiene
MaterialSuelto::MaterialSuelto(vector<float> p, Sprite* s, Textura* t, int id, int cant):ObjetoSala(p, s, t) {
    idMaterial  = id;
    cantidad    = cant;

    FabricaMateriales* fabrica   = FabricaMateriales::Instancia();
    nombre = fabrica->getNombre(id);
    descripcion = fabrica->getDescripcion(id);

    //Crea el marco
    marcoN = new ArrayVertices(4, 4);

    marcoN->setArrayColor(0, 0, 0, 100);

    texto = new Texto(nombre);
    textod = new Texto(descripcion);

    Fuente* fuente=new Fuente("resources/sprites/generales/Chonkly.otf");
    vector<int> col_text =    {255,255,  0,255};

    texto->setFuente(fuente);
    texto->setTamanyo(20);
    texto->setEscala(1,1);
    texto->setColorTexto(col_text);

    textod->setFuente(fuente);
    textod->setTamanyo(20);
    textod->setEscala(1,1);
    textod->setColorTexto(col_text);

    an_f = s->getGlobalPixelSize()[0];
    al_f = s->getGlobalPixelSize()[1];
}

//Destructor
MaterialSuelto::~MaterialSuelto() {}

//Elimina el material del mapa y lo anyade al inventario del jugador
void MaterialSuelto::recoger(Inventario* inv) {
    if(!recogido) {
        recogido = true;

        inv->incrementaMaterial(idMaterial, cantidad);
    }
}

//Mostrar informacion de material
void MaterialSuelto::mostrarInformacion(){
    VentanaRender* ventana = VentanaRender::Instancia();

    int ancho = ventana->getSize()[0];
    int alto = ventana->getSize()[1];

    
    FabricaMateriales* fabrica   = FabricaMateriales::Instancia();
    nombre = fabrica->getNombre(idMaterial);
    //descripcion  = fabrica->getDescripcion(idMaterial);
    

    texto->setString("Material: "+nombre+" x"+to_string(cantidad));

    int anchotxt = texto->getAncho();
    int altotxt = texto->getAlto();

    int xtra = 1;

    texto->setEscala(ancho/1920,alto/1080);

    marcoN->setVertexPosition(0, (int)posicion[0] - anchotxt/2 - anchotxt/8, (int)posicion[1] - al_f*2          - texto->getAlto()/2);
    marcoN->setVertexPosition(1, (int)posicion[0] + anchotxt/2 + anchotxt/8, (int)posicion[1] - al_f*2          - texto->getAlto()/2);
    marcoN->setVertexPosition(2, (int)posicion[0] + anchotxt/2 + anchotxt/8, (int)posicion[1] - al_f/2 - al_f/8);
    marcoN->setVertexPosition(3, (int)posicion[0] - anchotxt/2 - anchotxt/8, (int)posicion[1] - al_f/2 - al_f/8);

    texto->setPosicion(posicion[0]-texto->getAncho()/2 ,posicion[1]-2*al_f);

    

    ventana->dibujar(marcoN);
    ventana->dibujar(texto);

    vector<int> tam = marcoN->getBounds();
    int marcoX = -(marcoN->getVertexPosition(1)[0]-marcoN->getVertexPosition(0)[0])/2 + posicion[0];
    int marcoY = -(marcoN->getVertexPosition(1)[1]-marcoN->getVertexPosition(0)[1])   + posicion[1] -2*al_f-texto->getAlto()/2;
    //Coloca el marco del mensaje modal
    marco[0]->setPosition(marcoX         , marcoY);//Esq superior izq
    marco[1]->setPosition(marcoX+tam[0]/2, marcoY);//Lado superior
    marco[2]->setPosition(marcoX+tam[0]  , marcoY);//Esq superior dere
    marco[3]->setPosition(marcoX+tam[0]  , marcoY+tam[1]/2);//Lado dere
    marco[4]->setPosition(marcoX+tam[0]  , marcoY+tam[1]);//Esq abajo dere
    marco[5]->setPosition(marcoX+tam[0]/2, marcoY+tam[1]);//Lado abajo
    marco[6]->setPosition(marcoX         , marcoY+tam[1]);//Esq abajo izq
    marco[7]->setPosition(marcoX         , marcoY+tam[1]/2);//Lado izq
    marco[1]->setScale((tam[0]*0.95)/16, 1.0);
    marco[3]->setScale(1.0, (tam[1]*0.9)/16);
    marco[5]->setScale((tam[0]*0.95)/16, 1.0);
    marco[7]->setScale(1.0, (tam[1]*0.9)/16);
    for(unsigned int i = 0; i < marco.size(); i++){
        marco[i]->draw();
    }
}