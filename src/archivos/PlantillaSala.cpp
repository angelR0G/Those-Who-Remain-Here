#include "PlantillaSala.h"

//Constructor. Recibe el nombre del archivo de la sala y un int por cada lado indicando si hay una puerta o no
PlantillaSala::PlantillaSala(string nom, int arr, int der, int abj, int izq) {
    nombre      = nom;
    arriba      = arr;
    derecha     = der;
    abajo       = abj;
    izquierda   = izq;
}

//Destructor.
PlantillaSala::~PlantillaSala() {
    nombre = "";
}

/* Dada una direccion en forma de int, indica si hay una puerta en la sala en esa direccion
*  que puede ser accedida en cualquier momento. Los posibles valores son:
*0 - Arriba
*1 - Derecha
*2 - Abajo
*3 - Izquierda
*/
bool PlantillaSala::puertaNormal(int dir) {
  bool puerta = false;
  int valorPuerta = 0;

  //Guarda el valor de la puerta que se encuentra en la direccion indicada
  switch(dir) {
    case 0:
      valorPuerta = arriba;
      break;
    case 1:
      valorPuerta = derecha;
      break;
    case 2:
      valorPuerta = abajo;
      break;
    case 3:
      valorPuerta = izquierda;
      break;
  }

  //Comprueba si hay una puerta accesible en cualquier momento
  if(valorPuerta == 1) {
    puerta = true;
  }

  return puerta;
}

/* Dada una direccion en forma de int, indica si hay una puerta en la sala en esa direccion
*  independientemente de si se necesita alguna habilidad para acceder o no. 
*Los posibles valores son:
*0 - Arriba
*1 - Derecha
*2 - Abajo
*3 - Izquierda
*/
bool PlantillaSala::puertaCualquiera(int dir) {
  bool puerta = false;
  int valorPuerta = 0;

  //Guarda el valor de la puerta que se encuentra en la direccion indicada
  switch(dir) {
    case 0:
      valorPuerta = arriba;
      break;
    case 1:
      valorPuerta = derecha;
      break;
    case 2:
      valorPuerta = abajo;
      break;
    case 3:
      valorPuerta = izquierda;
      break;
  }

  //Comprueba si hay una puerta accesible en cualquier momento
  if(valorPuerta > 0) {
    puerta = true;
  }

  return puerta;
}

//Devuelve la cantidad de puertas de la sala
int PlantillaSala::getNumPuertas() {
  int cantidad = 0;

  if(arriba > 0) 
    cantidad++;
  if(derecha > 0) 
    cantidad++;
  if(abajo > 0) 
    cantidad++;
  if(izquierda > 0) 
    cantidad++;

  return cantidad;
}

//Devuelve el nombre del archivo de la sala
string PlantillaSala::getNombre() {
  return nombre;
}