#include "FabricaMateriales.h"

using namespace std;

//Se inicializa el puntero de la instancia
FabricaMateriales* FabricaMateriales::pFabricamateriales    =   0;

//Constantes con el numero de materiales existentes actualmente
const int  NMAX =   13;

//Constantes con los nombres de los materiales
const vector<string> NOMBRE     =   {"Sangre", "Hueso", "Pellejo",
                                    "Vertebra de sangre", "Visceras monstruosas", "Tuetano cristalizado",
                                    "Cartilago boreal", "Residuos corruptos", "Pneuma linfatica",
                                    "Triada elemental", "Panacea prismatica", "Colera embotellada", "Cortex de trocolo"};
//Constantes con las descripciones de los materiales
const vector<string> DESC       =   {"La sangre de este\ninsecto es flamable", 
                                    "Es tan afilado\nque se considera\nun arma", 
                                    "Duro y sesistente\nbuen material\nde construccion", 
                                    "Ideal para reforzar\narmas espirituales",
                                    "Aun irradian calor",
                                    "Tiene propiedades\nanimosas varias",
                                    "Elastico y versatil",
                                    "Sus efectos son muy\nimpredecibles",
                                    "En el inframundo son\nmuy cotizadas por\nsus capacidades para\nla defensa",
                                    "Contiene el poder de\nlos elementos ya\nque son restos de\nelementales de Trino",
                                    "Tiene la cura para\ncualquier enfermedad",
                                    "Una gota y cualquier\nser viviente sera\ntu enemigo",
                                    "Los trocolos son unos\npoderosos canalizadores\nmagicos"};
//Constantes con las rutas para las texturas
const vector<string> RUTA       =   {"resources/sprites/materiales/sangre.png", 
                                    "resources/sprites/materiales/hueso.png", 
                                    "resources/sprites/materiales/pellejo.png", 
                                    "resources/sprites/materiales/vertebraSangre.png",
                                    "resources/sprites/materiales/viscerasMonstruosas.png",
                                    "resources/sprites/materiales/tuetano.png",
                                    "resources/sprites/materiales/cartilago.png",
                                    "resources/sprites/materiales/residuosCorruptos.png",
                                    "resources/sprites/materiales/pneuma.png",
                                    "resources/sprites/materiales/triada.png",
                                    "resources/sprites/materiales/panacea.png",
                                    "resources/sprites/materiales/colera.png",
                                    "resources/sprites/materiales/trocolo.png"};

//Metodo para crear la instancia
FabricaMateriales* FabricaMateriales::Instancia(){

    //Si no estaba creado el objeto lo crea
    if(pFabricamateriales   ==  0){
        pFabricamateriales  =   new FabricaMateriales();
    }

    //Tanto si lo acaba de crear como si ya estaba creado devuelve el puntero
    return pFabricamateriales;
}

//Constructor
FabricaMateriales::FabricaMateriales(){}

//Destructor
FabricaMateriales::~FabricaMateriales(){}

vector<Material*> FabricaMateriales::creaMaterial(){
    
    //Inicializamos vector de puntero a materiales
    vector<Material*> materiales;

    //Creamos tantos materiales como el maximo que tenemos establecido
    for(int i=0; i<NMAX; i++){
        
        //Creamos un puntero a un material y lo almacenamos en el vector a devolver
        Material* material     =   nullptr;
        material    =   new Material(NOMBRE[i], DESC[i], RUTA[i]);
        materiales.push_back(material);
           
    }
    
    return materiales;
  

}

string FabricaMateriales::getNombre(int id) {
    string nom = "";

    if(id >= 0 && (unsigned int)id < NOMBRE.size()) {
        nom = NOMBRE[id];
    }

    return nom;
}

string FabricaMateriales::getDescripcion(int id) {
    string de = "";

    if(id >= 0 && (unsigned int)id < DESC.size()) {
        de = DESC[id];
    }

    return de;
}

string FabricaMateriales::getRutaPorId(int id) {
    string ruta = "";

    if(id >= 0 && (unsigned int)id < RUTA.size()) {
        ruta = RUTA[id];
    }

    return ruta;
}