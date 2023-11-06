#pragma once

#include "../fachada/Sprite.h"
#include "../fachada/Texto.h"
#include "Objeto.h"
#include "../fachada/VentanaRender.h"
#include "FabricaMateriales.h"
#include "Arma.h"
#include <iostream>
#include "Marco.h"

using namespace std;

class Inventario {
    public:

        //Constructor del inventario, se le pasa un puntero de puntero a arma
        Inventario(Arma**);

        //Destructor de clase
        ~Inventario();

        //Método al cual se le pasa id de objeto y comprueba si se encuentra en el inventario
        bool poseeObjeto(int);

        //Metodo para interactual con el inventario, recibe las coordenadas primero x y despues y del raton
        void clickRaton(float, float);

        //Metodo para ver si el raton esta haciendo hover por algun item
        void posicionRaton(float, float);

        //Método para imprimir el inventario en la pantalla
        void dibujar();

        //Aumenta la cantidad de un material
        void incrementaMaterial(int, int);

        //Reduce la cantidad de un material
        void reduceMaterial(int, int);

        //Devuelve la cantidad de materiales segun la id pasada
        int cantidadMateriales(int);

        //Agrega objeto al inventario
        void anyadeObjeto(Objeto*);

        //Elimina el objeto con la id pasada del inventario
        void eliminaObjeto(int);

        //Fabrica un material a partir de otros
        void fabricar(int);

        //Dibuja los materiales necesarios para fabricar otro
        void muestraNecesarios(int);

        //Actualiza el nivel de los materiales
        void setNivelMateriales(int);

        //Metodo para activar el temporizador y animar los botones, ademas controla que no fabriques materiales de mas sin querer
        void actualizarTemporizador(int);

        //Recibe un puntero de puntero a arma, este metodo se encarga de actualizar los datos del inventario respecto a la nueva arma
        void nuevaArma(Arma**);

        //Imprime el marco inferior cuando seleccionas una runa
        void interfazRunas(int, int);
        void interfazMejoras(int, int);

        //Se le pasa el indice de la runa a imprimir y si es mayor, imprimira el nombre de la runa, descripcion y los materiales necesarios asi como el boton para fabricarla
        void imprimeNecesarios(int, bool);

        //Metodo que fabrica la runa, la activa en el arma y actualiza el sprite de esa runa en concreto.
        void fabricaRuna(int, int, bool, bool);

        //Borra la runa existente
        void borraRuna();

        //Este metodo muestra las interfaz de mejoras de stats de arma se le pasan dos enteros que son los desplazamientos en x e y de la vista
        void muestraMejoras(int, int);

        //Mejora en 1 el stat elegido usando los materiales necesarios
        void mejorarArma();

    private:

        //Variables para saber materiales desbloqueados
        int nivelMateriales;

        //hover es la id del objeto sobre el que está puesto el ratón, elegido es si has clickeado en un objeto y está seleccionado.
        int hover, elegido, mejoraRunicaElegida, runaHover, mejoraElementalElegida, runaEspecificaHover, mejoraHover, elegiHover;

        //modo guarda en que pestanya estaba el inventario cuando se cerró por ultima vez, tambien se usa para discriminar el comportamiento del metodo clickRaton()
        int modo;

        //Vector que contiene punteros a los objetos que tiene el jugador
        vector<Objeto*> objetos;

        //Vector que contiene punteros a los materiales que tiene el jugador
        vector<Material*> materiales;

        //vector de texturas que contiene las ventanas y marcos siendo 0:interfazObjetos, 1:interfazMateriales, 2:interfazArma, 3:marcoNegro, 4:marcoAzul
        vector<Textura*> interfaz;

        //vector de sprites que contiene las ventanas y marcos siendo 0:interfazObjetos, 1:interfazMateriales, 2:interfazArma, 3:marcoNegro, 4:marcoAzul, 5 rombo amarillo
        vector<Sprite*> sptInterfaz;

        //Textura para los botones
        Textura* texFabricar;

        //Sprites de los botones
        Sprite* sptFabricar;
        Sprite* sptMejorar;
        Sprite* sptBorrar;

        //Fuente de los textos
        Fuente* fuente;

        //textos para nombre y descripcion
        Texto* textoNombre;
        Texto* textoDescripcion;

        //Textos del menu
        Texto* textoObjeto;
        Texto* textoMateriales;
        Texto* textoArma;

        //Texto para mostrar lo necesario para fabricar ciertos items
        Texto* textoFabricar;

        //Texto para lo necesario para fabricar una runa
        vector<Texto*> textoRunico;

        //color RGB blanco
        vector<int> blanco;

        //color RGB amarillo;
        vector<int> amarillo;

        //Temporizador boton
        int temporizador;

        //Sprite auxiliar arma
        Sprite* sptArma;

        //Textura de la runa
        Textura* texRuna;

        //Puntero a arma
        Arma* puntArma;

        //Texto nombre del arma
        Texto* nombreArma;

        //booleano para saber si es una runa mayor o menor, el primero es para el hover y el segundo para el click
        bool esMayor;
        bool elegidaMayor;

        //vector de sprites para marquitos
        vector<Sprite*> marcos;

        //Vector de sprites de las runas para discriminar entre mejoras elementales (generales), o Especificas
        vector<Sprite*> sptElementales;
        vector<Sprite*> sptEspecificas;

        //Sprites y Textura para las mejoras de arma
        Sprite* barraExterior;
        Sprite* relleno;
        Textura* texBarra;

        //Texto para las mejoras de arma
        vector<Texto*> textosMejorables;
        vector<int> mejorasArma;
        Texto* cantidadPrimero;
        Texto* cantidadSegundo;
        Texto* maximo;


};