#include "FabricaObjetos.h"

using namespace std;

//Inicializamos puntero
FabricaObjetos* FabricaObjetos::pFabricaObjetos =   0;

//Constante para el numero max de objetos
const int MAX_OBJETOS           =   13;

//Constantes con los nombres de los diferentes objetos
const string OBJETO_0_nombre    =   "Balas de plata";
const string OBJETO_1_nombre    =   "Comida";
const string OBJETO_2_nombre    =   "Regenerador de aura";
const string OBJETO_3_nombre    =   "Pluma explosiva";
const string OBJETO_4_nombre    =   "Trilobites anciano";
const string OBJETO_5_nombre    =   "Higado infectado";
const string OBJETO_6_nombre    =   "Ampolla gaseosa";
const string OBJETO_7_nombre    =   "Esencia maldita";
const string OBJETO_8_nombre    =   "Latido perpetuo";
const string OBJETO_9_nombre    =   "Collar de mimico";
const string OBJETO_10_nombre   =   "Plumaje aureo";
const string OBJETO_11_nombre   =   "Colmillo drenaje";
const string OBJETO_12_nombre   =   "Pluma de fenix";

//Constantes con las descripciones de los objetos
const string OBJETO_0_desc      =   "Aumenta la vida\nmaxima en 1";
const string OBJETO_1_desc      =   "Aumenta la vida\nmaxima en 2";
const string OBJETO_2_desc      =   "Explosion alrededor\nal sufrir danyo";
const string OBJETO_3_desc      =   "Te vuelves inmune al\nprimer impacto al entrar\nen una sala nueva";
const string OBJETO_4_desc      =   "Aumenta la velocidad\nde carga del poder de\ndetener el tiempo";
const string OBJETO_5_desc      =   "La probabilidad de\nque los enemigos suelten\nmateriales al morir\naumenta";
const string OBJETO_6_desc      =   "Los enemigos sueltan\nuna nube de gas al morir";
const string OBJETO_7_desc      =   "Existe la posibilidad\nde que un enemigo\nexplote al derrotar a\notro";
const string OBJETO_8_desc      =   "Aumenta el danyo\ncuando la salud es baja";
const string OBJETO_9_desc      =   "Al para el tiempo\ngeneras una copia que\nexplota al reanudarlo";
const string OBJETO_10_desc     =   "Aumenta el tiempo de\ninvulnerabilidad al\nsufrir danyos";
const string OBJETO_11_desc     =   "Existe la posibilidad\nde recuperar mas vida\nal rellenar la barra";
const string OBJETO_12_desc     =   "Al morir revives a\ncosta de la mitad de\ntu vida maxima";

//Constantes con la ruta a las texturas de los objetos
const string OBJETO_0_ruta      =   "resources/sprites/objetos/plata.png";
const string OBJETO_1_ruta      =   "resources/sprites/objetos/comida.png";
const string OBJETO_2_ruta      =   "resources/sprites/objetos/auraextendida.png";
const string OBJETO_3_ruta      =   "resources/sprites/objetos/explotido.png";
const string OBJETO_4_ruta      =   "resources/sprites/objetos/trilobites.png";
const string OBJETO_5_ruta      =   "resources/sprites/objetos/higado.png";
const string OBJETO_6_ruta      =   "resources/sprites/objetos/ampolla.png";
const string OBJETO_7_ruta      =   "resources/sprites/objetos/esencia.png";
const string OBJETO_8_ruta      =   "resources/sprites/objetos/latido.png";
const string OBJETO_9_ruta      =   "resources/sprites/objetos/mimico.png";
const string OBJETO_10_ruta     =   "resources/sprites/objetos/escudo.png";
const string OBJETO_11_ruta     =   "resources/sprites/objetos/drenaje.png";
const string OBJETO_12_ruta     =   "resources/sprites/objetos/fenix.png";

//Creamos instancia de fabrica de objetos
FabricaObjetos* FabricaObjetos::Instancia(){

    //Si es la primera vez, llama al constructor y crea la fabrica
    if(pFabricaObjetos  ==  0){
        pFabricaObjetos =   new FabricaObjetos();
    }

    //Devuelve ele puntero a la fabrica
    return pFabricaObjetos;
}

//Constructor de la clase
FabricaObjetos::FabricaObjetos(){}

//Destructor
FabricaObjetos::~FabricaObjetos(){}

//Se fabrica objeto segun la id
Objeto* FabricaObjetos::fabricarObjetoPorID(int id){

    //Inicializamos puntero a null 
    Objeto* objeto      =   nullptr;

    //Recuperamos el vector con los objetos creados
    vector<int> creados =   Objeto::getCreados();

    //Dependiendo de la id creamos un objeto u otro
    switch (id){
        case 0:
            objeto  =   new Objeto(id, OBJETO_0_nombre, OBJETO_0_desc, OBJETO_0_ruta );
            break;
        case 1:
            objeto  =   new Objeto(id, OBJETO_1_nombre, OBJETO_1_desc, OBJETO_1_ruta );
            break;
        case 2:
            objeto  =   new Objeto(id, OBJETO_2_nombre, OBJETO_2_desc, OBJETO_2_ruta );
            break;
        case 3:
            objeto  =   new Objeto(id, OBJETO_3_nombre, OBJETO_3_desc, OBJETO_3_ruta );
            break;
        case 4:
            objeto  =   new Objeto(id, OBJETO_4_nombre, OBJETO_4_desc, OBJETO_4_ruta );
            break;
        case 5:
            objeto  =   new Objeto(id, OBJETO_5_nombre, OBJETO_5_desc, OBJETO_5_ruta );
            break;
        case 6:
            objeto  =   new Objeto(id, OBJETO_6_nombre, OBJETO_6_desc, OBJETO_6_ruta );
            break;
        case 7:
            objeto  =   new Objeto(id, OBJETO_7_nombre, OBJETO_7_desc, OBJETO_7_ruta );
            break;
        case 8:
            objeto  =   new Objeto(id, OBJETO_8_nombre, OBJETO_8_desc, OBJETO_8_ruta );
            break;
        case 9:
            objeto  =   new Objeto(id, OBJETO_9_nombre, OBJETO_9_desc, OBJETO_9_ruta );
            break;
        case 10:
            objeto  =   new Objeto(id, OBJETO_10_nombre, OBJETO_10_desc, OBJETO_10_ruta );
            break;
        case 11:
            objeto  =   new Objeto(id, OBJETO_11_nombre, OBJETO_11_desc, OBJETO_11_ruta );
            break;
        case 12:
            objeto  =   new Objeto(id, OBJETO_12_nombre, OBJETO_12_desc, OBJETO_12_ruta );
            break;
    }
    
    return objeto;
}

string FabricaObjetos::getNombre(int id) {
    string nom = "";

    //Devuelve el nombre de la id correspondiente
    switch (id){
        case 0:
            nom = OBJETO_0_nombre;
            break;
        case 1:
            nom = OBJETO_1_nombre;
            break;
        case 2:
            nom = OBJETO_2_nombre;
            break;
        case 3:
            nom = OBJETO_3_nombre;
            break;
        case 4:
            nom = OBJETO_4_nombre;
            break;
        case 5:
            nom = OBJETO_5_nombre;
            break;
        case 6:
            nom = OBJETO_6_nombre;
            break;
        case 7:
            nom = OBJETO_7_nombre;
            break;
        case 8:
            nom = OBJETO_8_nombre;
            break;
        case 9:
            nom = OBJETO_9_nombre;
            break;
        case 10:
            nom = OBJETO_10_nombre;
            break;
        case 11:
            nom = OBJETO_11_nombre;
            break;
        case 12:
            nom = OBJETO_12_nombre;
            break;
    }

    return nom;
}

string FabricaObjetos::getDescripcion(int id) {
    string desc = "";

    //Devuelve la descripcion de la id correspondiente
    switch (id){
        case 0:
            desc = OBJETO_0_desc;
            break;
        case 1:
            desc = OBJETO_1_desc;
            break;
        case 2:
            desc = OBJETO_2_desc;
            break;
        case 3:
            desc = OBJETO_3_desc;
            break;
        case 4:
            desc = OBJETO_4_desc;
            break;
        case 5:
            desc = OBJETO_5_desc;
            break;
        case 6:
            desc = OBJETO_6_desc;
            break;
        case 7:
            desc = OBJETO_7_desc;
            break;
        case 8:
            desc = OBJETO_8_desc;
            break;
        case 9:
            desc = OBJETO_9_desc;
            break;
        case 10:
            desc = OBJETO_10_desc;
            break;
        case 11:
            desc = OBJETO_11_desc;
            break;
        case 12:
            desc = OBJETO_12_desc;
            break;
    }

    return desc;
}

string FabricaObjetos::getRuta(int id) {
    string ruta = "";

    //Devuelve la ruta de la id correspondiente
    switch (id){
        case 0:
            ruta = OBJETO_0_ruta;
            break;
        case 1:
            ruta = OBJETO_1_ruta;
            break;
        case 2:
            ruta = OBJETO_2_ruta;
            break;
        case 3:
            ruta = OBJETO_3_ruta;
            break;
        case 4:
            ruta = OBJETO_4_ruta;
            break;
        case 5:
            ruta = OBJETO_5_ruta;
            break;
        case 6:
            ruta = OBJETO_6_ruta;
            break;
        case 7:
            ruta = OBJETO_7_ruta;
            break;
        case 8:
            ruta = OBJETO_8_ruta;
            break;
        case 9:
            ruta = OBJETO_9_ruta;
            break;
        case 10:
            ruta = OBJETO_10_ruta;
            break;
        case 11:
            ruta = OBJETO_11_ruta;
            break;
        case 12:
            ruta = OBJETO_12_ruta;
            break;
    }

    return ruta;
}

int FabricaObjetos::getIDDisponible() {
    int id = -1;

    //Recuperamos el vector con los objetos creados
    vector<int> creados =   Objeto::getCreados();

    //Comprobamos que no esten creados ya todos los objetos
    if(creados.size() != MAX_OBJETOS){
        //Generamos una id aleatoria entre 0 y el numero de objetos totales
        srand(time(NULL));
        id = rand()%MAX_OBJETOS;

        for(unsigned int i=0; i<creados.size(); i++){
            //Si coincide la id pasamos a la siguiente id
            if(id==creados[i]){
                id = (id+1)%MAX_OBJETOS;
                i=-1;
            }
        }
    }

    return id;
}