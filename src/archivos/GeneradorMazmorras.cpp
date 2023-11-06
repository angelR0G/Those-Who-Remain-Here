#include "GeneradorMazmorras.h"

const int tamMatrizMaz  = 10;
const int tamMazmorra   = 4;    //Cambiar a 8
const int salasExtra    = 1;    //Cambiar a 2

//Crea un generador que utilizara las plantillas especificadas en el archivo cuya ruta recibe por parametro
GeneradorMazmorras::GeneradorMazmorras(string ruta) {
    //Crea un lector de XML para leer el archivo
    LectorXML lector;

    if(lector.abrirArchivo(ruta)) {
        if(!lector.siguienteHijo("plantilla")) {
            exit(0);
        }

        //Guarda el nombre de las plantillas basicas
        plantillasBasicas.push_back(lector.getAttribute("bbase"));
        plantillasBasicas.push_back(lector.getAttribute("barr"));
        plantillasBasicas.push_back(lector.getAttribute("bder"));
        plantillasBasicas.push_back(lector.getAttribute("babj"));
        plantillasBasicas.push_back(lector.getAttribute("bizq"));
        nivMaterial = stoi(lector.getAttribute("nivMat"));
        nivMaterial = max(nivMaterial, 1);
        sigArea     = stoi(lector.getAttribute("sigArea"));
        sigArea     = max(sigArea, 1);

        if(!lector.siguienteHermano("plantilla")) {
            exit(0);
        }

        //Recorre todas las plantillas del fichero XML
        do {
            string nombre;
            int arriba, der, abajo, izq;

            //Recoge los atributos de la plantilla
            nombre  = lector.getAttribute("nombre");
            arriba  = stoi(lector.getAttribute("arr"));
            der     = stoi(lector.getAttribute("der"));
            abajo   = stoi(lector.getAttribute("abj"));
            izq     = stoi(lector.getAttribute("izq"));

            //Crea la plantilla con los datos recogidos y la guarda en el vector
            listaPlantillas.push_back(new PlantillaSala(nombre, arriba, der, abajo, izq));
            plantillasDisponibles.push_back(listaPlantillas[listaPlantillas.size()-1]);

        } while(lector.siguienteHermano("plantilla"));

        //Inicializa el resto de parametros
        inicioX     = -1;
        inicioY     = -1;
        finalX      = -1;
        finalY      = -1;
        numSalas    =  0;

        tilesheet       = nullptr;
        puertaTapiada   = nullptr;

        //Crea la matriz de la mazmorra vacia (con nullptr)
        vector<PlantillaSala*> aux;
        for(unsigned int i=0; i<tamMatrizMaz; i++) {
            aux.push_back(nullptr);
        }
        for(unsigned int i=0; i<tamMatrizMaz; i++) {
            mazmorra.push_back(aux);
        }
    }
    else {
        //Si no puede abrir el archivo termina la ejecucion
        exit(0);
    }
}

//Destructor
GeneradorMazmorras::~GeneradorMazmorras() {
    for(unsigned int i=0; i<listaPlantillas.size(); i++) {
        delete listaPlantillas[i];
    }

    listaPlantillas.clear();
    plantillasDisponibles.clear();
    plantillasUsadas.clear();
    plantillasBasicas.clear();
    mazmorra.clear();

    tilesheet       = nullptr;
    puertaTapiada   = nullptr;
}

//Genera una mazmorra aleatoria a partir de las plantillas que tiene
bool GeneradorMazmorras::generarMazmorra() {
    int longitudActual = 0, nuevaDireccion = 0, vieneDe = -1;
    int xActual = tamMatrizMaz/2, yActual = tamMatrizMaz/2;
    stack<vector<int>> salasPorCompletar;
    srand(time(NULL));
    numSalas = 0;

    //Crea el inicio de la mazmorra en el centro
    mazmorra[xActual][yActual] = new PlantillaSala(plantillasBasicas[0], 1, 1, 1, 1);
    listaPlantillas.push_back(mazmorra[xActual][yActual]);
    inicioX = xActual;
    inicioY = yActual;

    //Anyade salas mientras no supere el tamanyo minimo
    while(longitudActual < tamMazmorra) {
        int dirRandom = rand() % 4;
        int auxDir = 0;
        bool posicionOcupada = true, direccionIncorrecta = true;

        //Comprueba que no esta en la primera sala
        if(vieneDe != -1) {
            //Coloca la sala y la envia al vector de salas usadas
            mazmorra[xActual][yActual] = getPlantillaPorEntrada(vieneDe, true);

            if(mazmorra[xActual][yActual] == nullptr) {
                //Esto no deberia ocurrir, asi que cierra el programa
                exit(0);
            }
            else {
                numSalas++;
                plantillaUsada(mazmorra[xActual][yActual]->getNombre());
            }
        }

        //Elige una puerta aleatoria como camino principal si es posible
        while(auxDir<=4 && direccionIncorrecta) {
            direccionIncorrecta = false;
            posicionOcupada     = true;
            auxDir++;
            dirRandom++;

            if(dirRandom > 3) {
                dirRandom = 0;
            }

            //Comprueba si viene por esa puerta y si hay puerta en esa direccion
            if(dirRandom == vieneDe || !mazmorra[xActual][yActual]->puertaNormal(dirRandom)) {
                direccionIncorrecta = true;
            }
            else {
                //Comprueba que no haya una sala y no se sale del mapa
                switch(dirRandom) {
                    case 0:     //Arriba
                        if(yActual < tamMatrizMaz-1 && mazmorra[xActual][yActual+1] == nullptr) {
                            posicionOcupada = false;
                        }
                        else if(yActual == tamMatrizMaz-1 && desplazarMazmorra(2, &salasPorCompletar)) {
                            yActual--;
                            posicionOcupada = false;
                        }
                        break;
                    case 1:     //Derecha
                        if(xActual < tamMatrizMaz-1 && mazmorra[xActual+1][yActual] == nullptr) {
                            posicionOcupada = false;
                        }
                        else if(xActual == tamMatrizMaz-1 && desplazarMazmorra(3, &salasPorCompletar)) {
                            xActual--;
                            posicionOcupada = false;
                        }
                        break;
                    case 2:     //Abajo
                        if(yActual > 0 && mazmorra[xActual][yActual-1] == nullptr) {
                            posicionOcupada = false;
                        }
                        else if(yActual == 0 && desplazarMazmorra(0, &salasPorCompletar)) {
                            yActual++;
                            posicionOcupada = false;
                        }
                        break;
                    case 3:     //Izquierda
                        if(xActual > 0 && mazmorra[xActual-1][yActual] == nullptr) {
                            posicionOcupada = false;
                        }
                        else if(xActual == 0 && desplazarMazmorra(1, &salasPorCompletar)) {
                            xActual++;
                            posicionOcupada = false;
                        }
                        break;
                }
                if(posicionOcupada) {
                    direccionIncorrecta = true;
                }
            }
        }

        //Flujo normal
        if(auxDir <= 4) {
            nuevaDireccion = dirRandom;

            //Comprueba que no esta en la primera sala
            if(vieneDe != -1) {
                //Si hay mas puertas anyade la sala a la pila
                if(mazmorra[xActual][yActual]->getNumPuertas()>2) {
                    salasPorCompletar.push({xActual, yActual});
                }
            }

            //Actualiza el valor de la proxima entrada y la posicion de la siguiente sala
            vieneDe = (nuevaDireccion +2) % 4;
            
            switch(nuevaDireccion) {
                case 0:     //Arriba
                    yActual++;
                    break;
                case 1:     //Derecha
                    xActual++;
                    break;
                case 2:     //Abajo
                    yActual--;
                    break;
                case 3:     //Izquierda
                    xActual--;
                    break;
            }

            longitudActual++;
        }
        else {
            //Si da la casualidad de que no se pueden colocar mas salas se termina la mazmorra
            longitudActual = tamMazmorra;
        }
    }

    //Coloca la sala del jefe al final del camino
    if(longitudActual >= tamMazmorra) {
        mazmorra[xActual][yActual] = new PlantillaSala(plantillasBasicas[0], 1, 1, 1, 1);
        listaPlantillas.push_back(mazmorra[xActual][yActual]);
        finalX = xActual;
        finalY = yActual;
    }

    //Una vez acabado el camino principal, revisa las salas con otros caminos posibles
    while(salasPorCompletar.size() > 0) {
        //Recupero las primera sala de la pila y la elimino
        vector<int> aux = salasPorCompletar.top();
        salasPorCompletar.pop();
        xActual = aux[0];
        yActual = aux[1];

        for(int i=0; i<4; i++) {
            int xObjetivo = -1;
            int yObjetivo = -1;
            vieneDe = (i + 2) % 4;

            //Comprueba que no haya una sala y no se sale del mapa donde haya puertas
            switch(i) {
                case 0:     //Arriba
                    if(mazmorra[xActual][yActual]->puertaCualquiera(i) && yActual < tamMatrizMaz-1 && mazmorra[xActual][yActual+1] == nullptr) {
                        xObjetivo = xActual;
                        yObjetivo = yActual+1;
                    }
                    else if(mazmorra[xActual][yActual]->puertaCualquiera(i) && yActual == tamMatrizMaz-1 && desplazarMazmorra(2, &salasPorCompletar)) {
                        yActual--;
                        xObjetivo = xActual;
                        yObjetivo = yActual+1;
                    }
                    break;
                case 1:     //Derecha
                    if(mazmorra[xActual][yActual]->puertaCualquiera(i) && xActual < tamMatrizMaz-1 && mazmorra[xActual+1][yActual] == nullptr) {
                        xObjetivo = xActual+1;
                        yObjetivo = yActual;
                    }
                    else if(mazmorra[xActual][yActual]->puertaCualquiera(i) && xActual == tamMatrizMaz-1 && desplazarMazmorra(3, &salasPorCompletar)) {
                        xActual--;
                        xObjetivo = xActual+1;
                        yObjetivo = yActual;
                    }
                    break;
                case 2:     //Abajo
                    if(mazmorra[xActual][yActual]->puertaCualquiera(i) && yActual > 0 && mazmorra[xActual][yActual-1] == nullptr) {
                        xObjetivo = xActual;
                        yObjetivo = yActual-1;
                    }
                    else if(mazmorra[xActual][yActual]->puertaCualquiera(i) && yActual == 0 && desplazarMazmorra(0, &salasPorCompletar)) {
                        yActual++;
                        xObjetivo = xActual;
                        yObjetivo = yActual-1;
                    }
                    break;
                case 3:     //Izquierda
                    if(mazmorra[xActual][yActual]->puertaCualquiera(i) && xActual > 0 && mazmorra[xActual-1][yActual] == nullptr) {
                        xObjetivo = xActual-1;
                        yObjetivo = yActual;
                    }
                    else if(mazmorra[xActual][yActual]->puertaCualquiera(i) && xActual == 0 && desplazarMazmorra(1, &salasPorCompletar)) {
                        xActual++;
                        xObjetivo = xActual-1;
                        yObjetivo = yActual;
                    }
                    break;
            }

            //Si ha encontrado donde colocar otra sala decide que hacer
            if(xObjetivo != -1) {
                PlantillaSala* nuevaP = nullptr;

                //Genera un numero aleatorio que decidira que hacer
                int decision = rand() % 10;

                if(numSalas <= tamMazmorra + salasExtra) {
                    decision += 3;
                }

                if(decision > 6) {
                    //Se anyade una nueva sala normal que no haya sido usada, si la hay
                    nuevaP = getPlantillaPorEntrada(vieneDe, false);
                    if(nuevaP != nullptr) {
                        //Crea una nueva sala y la anyade a la pila de salas por completar
                        mazmorra[xObjetivo][yObjetivo] = nuevaP;
                        salasPorCompletar.push({xObjetivo, yObjetivo});

                        plantillaUsada(nuevaP->getNombre());
                        numSalas++;
                    }
                }

                if((decision > 3 && decision <= 6) || (decision > 6 && nuevaP == nullptr)) {
                    //Se anyade una sala sin salida
                    switch(i) {
                        case 0:
                            mazmorra[xObjetivo][yObjetivo] = new PlantillaSala(plantillasBasicas[1], 0, 0, 1, 0);
                            listaPlantillas.push_back(mazmorra[xObjetivo][yObjetivo]);
                            break;
                        case 1:
                            mazmorra[xObjetivo][yObjetivo] = new PlantillaSala(plantillasBasicas[2], 0, 0, 0, 1);
                            listaPlantillas.push_back(mazmorra[xObjetivo][yObjetivo]);
                            break;
                        case 2:
                            mazmorra[xObjetivo][yObjetivo] = new PlantillaSala(plantillasBasicas[3], 1, 0, 0, 0);
                            listaPlantillas.push_back(mazmorra[xObjetivo][yObjetivo]);
                            break;
                        case 3:
                            mazmorra[xObjetivo][yObjetivo] = new PlantillaSala(plantillasBasicas[4], 0, 1, 0, 0);
                            listaPlantillas.push_back(mazmorra[xObjetivo][yObjetivo]);
                            break;
                    }
                }
            }
        }
    }

    //Comprueba que la mazmorra se ha generado correctamente
    bool generacionCorrecta = comprobarMazmorra();

    return generacionCorrecta;
}

/*Dada una direccion como int, intenta desplazar las salas de la matriz de la mazmorra en dicha direccion.
* Actualiza las posiciones de las salas inicial y final si estan colocadas y devuelve un bool indicando si
* ha sido posible desplazarlas
*/
bool GeneradorMazmorras::desplazarMazmorra(int dir, stack<vector<int>>* salasPorCompletar) {
    bool desplazado = false, hueco = true;
    stack<vector<int>> aux;

    //Desplaza las salas en la matriz
    switch(dir) {
        case 0:
            //Comprueba que no hay salas en la fila superior
            for(unsigned int i=0; i<tamMatrizMaz && hueco; i++) {
                if(mazmorra[i][tamMatrizMaz-1] != nullptr) {
                    hueco = false;
                }
            }

            if(hueco) {
                //Desplaza el mapa hacia arriba
                for(unsigned int i=tamMatrizMaz-1; i>0; i--) {
                    for(unsigned int j=0; j<tamMatrizMaz; j++) {
                        mazmorra[j][i] = mazmorra[j][i-1];
                    }
                }
                for(unsigned int i=0; i<tamMatrizMaz; i++) {
                    mazmorra[i][0] = nullptr;
                }
                desplazado = true;
            }
            break;
        case 1:
            //Comprueba que no hay salas en la columna derecha
            for(unsigned int i=0; i<tamMatrizMaz && hueco; i++) {
                if(mazmorra[tamMatrizMaz-1][i] != nullptr) {
                    hueco = false;
                }
            }

            if(hueco) {
                //Desplaza el mapa hacia la derecha
                for(unsigned int i=tamMatrizMaz-1; i>0; i--) {
                    for(unsigned int j=0; j<tamMatrizMaz; j++) {
                        mazmorra[i][j] = mazmorra[i-1][j];
                    }
                }
                for(unsigned int i=0; i<tamMatrizMaz; i++) {
                    mazmorra[0][i] = nullptr;
                }
                desplazado = true;
            }
            break;
        case 2:
            //Comprueba que no hay salas en la fila inferior
            for(unsigned int i=0; i<tamMatrizMaz && hueco; i++) {
                if(mazmorra[i][0] != nullptr) {
                    hueco = false;
                }
            }

            if(hueco) {
                //Desplaza el mapa hacia abajo
                for(unsigned int i=0; i<tamMatrizMaz-1; i++) {
                    for(unsigned int j=0; j<tamMatrizMaz; j++) {
                        mazmorra[j][i] = mazmorra[j][i+1];
                    }
                }
                for(unsigned int i=0; i<tamMatrizMaz; i++) {
                    mazmorra[i][tamMatrizMaz-1] = nullptr;
                }
                desplazado = true;
            }
            break;
        case 3:
            //Comprueba que no hay salas en la columna izquierda
            for(unsigned int i=0; i<tamMatrizMaz && hueco; i++) {
                if(mazmorra[0][i] != nullptr) {
                    hueco = false;
                }
            }

            if(hueco) {
                //Desplaza el mapa hacia la izquierda
                for(unsigned int i=0; i<tamMatrizMaz-1; i++) {
                    for(unsigned int j=0; j<tamMatrizMaz; j++) {
                        mazmorra[i][j] = mazmorra[i+1][j];
                    }
                }
                for(unsigned int i=0; i<tamMatrizMaz; i++) {
                    mazmorra[tamMatrizMaz-1][i] = nullptr;
                }
                desplazado = true;
            }
            break;
    }

    //Actualiza los valores de las salas principales
    if(hueco) {
        switch (dir){
            case 0:
                while(salasPorCompletar->size() > 0) {
                   vector<int> valor = salasPorCompletar->top();
                   salasPorCompletar->pop();
                   valor[1] += 1;
                   aux.push(valor);
                }
                while(aux.size() > 0) {
                    salasPorCompletar->push(aux.top());
                    aux.pop();
                }
                inicioY += 1;
                if(finalY != -1)
                    finalY += 1;
                break;
            case 1:
                while(salasPorCompletar->size() > 0) {
                    vector<int> valor = salasPorCompletar->top();
                    salasPorCompletar->pop();
                    valor[0] += 1;
                    aux.push(valor);
                }
                while(aux.size() > 0) {
                    salasPorCompletar->push(aux.top());
                    aux.pop();
                }
                inicioX += 1;
                if(finalX != -1)
                    finalX += 1;
                break;
            case 2:
                while(salasPorCompletar->size() > 0) {
                    vector<int> valor = salasPorCompletar->top();
                    salasPorCompletar->pop();
                    valor[1] -= 1;
                    aux.push(valor);
                }
                while(aux.size() > 0) {
                    salasPorCompletar->push(aux.top());
                    aux.pop();
                }
                inicioY -= 1;
                if(finalY != -1)
                    finalY -= 1;
                break;
            case 3:
                while(salasPorCompletar->size() > 0) {
                    vector<int> valor = salasPorCompletar->top();
                    salasPorCompletar->pop();
                    valor[0] -= 1;
                    aux.push(valor);
                }
                while(aux.size() > 0) {
                    salasPorCompletar->push(aux.top());
                    aux.pop();
                }
                inicioX -= 1;
                if(finalX != -1)
                    finalX -= 1;
                break;
        }
    }

    return desplazado;
}

//Revisa la mazmorra para ver si es suficientemente grande, devolviendo true si esta bien
bool GeneradorMazmorras::comprobarMazmorra() {
    bool correcto = true;
    
    if(numSalas < tamMazmorra-1) {
        //Comprueba que el tamanyo de la mazmorra es suficientemente grande
        correcto = false;
    }
    else if(inicioY == -1 || inicioX == -1 || finalY == -1 || finalX == -1) {
        //Comprueba que la primera y ultima sala se han situado
        correcto = false;
    }
    else if(abs(finalY-inicioY) + abs(finalX-inicioX) < 3) {
        //Comprueba que para llegar al final tienes que pasar por al menos 2 salas
        correcto = false;
    }

    return correcto;
}

//Resetea la mazmorra
void GeneradorMazmorras::resetearMazmorra() {
    //Si no es suficientemente grande se resetea
    for(unsigned int i=0; i<plantillasUsadas.size(); i++) {
        plantillasDisponibles.push_back(plantillasUsadas[i]);
    }
    plantillasUsadas.clear();

    //Reinicia los atributos
    inicioY = -1;
    inicioX = -1;
    finalY  = -1;
    finalX  = -1;

    //Vacia la mazmorra
    for(unsigned int i = 0; i<mazmorra.size(); i++) {
        mazmorra[i].clear();
    }
    mazmorra.clear();
    vector<PlantillaSala*> aux;
    for(unsigned int i=0; i<tamMatrizMaz; i++) {
        aux.push_back(nullptr);
    }
    for(unsigned int i=0; i<tamMatrizMaz; i++) {
        mazmorra.push_back(aux);
    }
}


//Imprime en la terminal el mapa de la mazmorra
void GeneradorMazmorras::dibujarMazmorra(){
    //Mapa simplificado indicando donde hay salas
    for(unsigned int i=0; i<mazmorra.size(); i++) {
        for(unsigned int j=0; j<mazmorra[0].size(); j++) {
            if(mazmorra[j][9-i] == nullptr) {
                cout << " . ";
            }
            else {
                cout << " O ";
            }
        }
        cout << endl;
    }
    cout << endl << endl;
    
    //Mapa donde se muestran las puertas y las salas base
    for(int i=mazmorra.size()*3-1; i>=0; i--) {
        for(unsigned long int j=0; j<mazmorra[0].size()*3; j++) {
            if(mazmorra[j/3][i/3] == nullptr) {
                cout << "-";
            }
            else {
                int pos = (i%3)*3 + (j%3);
                //Dibuja segun la posicion del cuadrado en la que este
                switch(pos) {
                    case 1:
                        if(mazmorra[j/3][i/3]->puertaCualquiera(2)) {
                            cout << " ";
                        }
                        else {
                            cout << "O";
                        }
                        break;
                    case 3:
                        if(mazmorra[j/3][i/3]->puertaCualquiera(3)) {
                            cout << " ";
                        }
                        else {
                            cout << "O";
                        }
                        break;
                    case 5:
                        if(mazmorra[j/3][i/3]->puertaCualquiera(1)) {
                            cout << " ";
                        }
                        else {
                            cout << "O";
                        }
                        break;
                    case 7:
                        if(mazmorra[j/3][i/3]->puertaCualquiera(0)) {
                            cout << " ";
                        }
                        else {
                            cout << "O";
                        }
                        break;
                    case 4:
                        if(mazmorra[j/3][i/3]->getNombre().compare(plantillasBasicas[0]) == 0) {
                            cout << "X";
                        }
                        else {
                            cout << "+";
                        }
                        break;
                    default:
                        cout << "O";
                        break;
                }
            }
        }
        cout << endl;
    }
}

/*Devuelve una plantilla aleatoria que tenga una puerta en la direccion enviada por parametro. Como segundo
* parametro se indica si se puede buscar entre las salas usadas. Devuelve nullptr si no encuentra ninguna.
*/
PlantillaSala* GeneradorMazmorras::getPlantillaPorEntrada(int dir, bool usadasPermitidas) {
    vector<PlantillaSala*> candidatas;
    PlantillaSala* seleccionada = nullptr;
    int posRandom = 0;

    //Busco las salas que cumplen la condicion
    for(unsigned int i=0; i<plantillasDisponibles.size(); i++) {
        if(plantillasDisponibles[i]->puertaNormal(dir)) {
            candidatas.push_back(plantillasDisponibles[i]);
        }
    }

    //Si no encuentro ninguna sin usar, miro en las usadas
    if(usadasPermitidas && candidatas.size() == 0) {
        for(unsigned int i=0; i<plantillasUsadas.size(); i++) {
            if(plantillasUsadas[i]->puertaNormal(dir)) {
                candidatas.push_back(plantillasUsadas[i]);
            }
        }
    }

    //Elijo una aleatoriamente
    if(candidatas.size() > 0) {
        posRandom = rand() % candidatas.size();
        seleccionada = candidatas[posRandom];
    }

    return seleccionada;
}

//Devuelve la referencia a una plantilla segun su nombre
PlantillaSala* GeneradorMazmorras::buscarPlantilla(string nombre) {
    PlantillaSala* p = nullptr;

    //Busca la plantilla en el array de plantillas
    for(unsigned int i=0; i<plantillasDisponibles.size() && p==nullptr; i++) {
        if(plantillasDisponibles[i]->getNombre().compare(nombre) == 0) {
            p = plantillasDisponibles[i];
        }
    }

    //Busca la plantilla en el array de plantillas usadas
    for(unsigned int i=0; i<plantillasUsadas.size() && p==nullptr; i++) {
        if(plantillasUsadas[i]->getNombre().compare(nombre) == 0) {
            p = plantillasUsadas[i];
        }
    }

    return p;
}

//Mueve la plantilla a plantillas usadas si existe
void GeneradorMazmorras::plantillaUsada(string nombre) {
    int posicion = -1;

    //Busca la plantilla en el array de plantillas
    for(unsigned int i=0; i<plantillasDisponibles.size() && posicion==-1; i++) {
        if(plantillasDisponibles[i]->getNombre().compare(nombre) == 0) {
            posicion = i;
        }
    }

    //Si la encuentra la mueve al vector de plantillas usadas
    if(posicion != -1) {
        plantillasUsadas.push_back(plantillasDisponibles[posicion]);
        plantillasDisponibles.erase(plantillasDisponibles.begin() + posicion);
    }
}

//Especifica las texturas que utilizaran las salas cuando se generen
void GeneradorMazmorras::setTexturas(Textura* texTile, Textura* texPuerta) {
    tilesheet       = texTile;
    puertaTapiada   = texPuerta;
}

//Devuelve un vector con las coordenadas de la sala de inicio
vector<int> GeneradorMazmorras::getSalaInicial() {
    return {inicioX, inicioY};
}

//Devuelve un vector con las coordenadas de la sala del jefe
vector<int> GeneradorMazmorras::getSalaFinal() {
    return {finalX, finalY};
}

//A partir de la matriz de PlantillasSala, genera la mazmorra de salas y la devuelve
vector<vector<Sala*>> GeneradorMazmorras::getMazmorra() {
    vector<vector<Sala*>> mazmorraSalas;

    //Comprueba que existe las referencias a las texturas que necesitan las salas
    if(tilesheet != nullptr && puertaTapiada != nullptr && plantillasUsadas.size() > 0) {

        int salaRecompensa = rand() % (int)plantillasUsadas.size();
        int salaRecompensaCont = 0;

        //Recorre la mazmorra para ir convirtiendo las plantillas en salas
        for(unsigned int i=0; i<tamMatrizMaz; i++) {
            //Anyade un vector vacio en el que se colocaran las salas
            mazmorraSalas.push_back({});

            for(unsigned int j=0; j<tamMatrizMaz; j++) {
                //Comprueba si en la posicion correspondiente hay una sala
                if(mazmorra[i][j] == nullptr) {
                    mazmorraSalas[i].push_back(nullptr);
                }
                else {
                    string archivo = mazmorra[i][j]->getNombre();
                    Sala* salaActual = new Sala("resources/niveles/"+archivo+".tmx", tilesheet, {8, 8}, puertaTapiada);
                    mazmorraSalas[i].push_back(salaActual);

                    if(((int)i != inicioX || (int)j != inicioY) && ((int)i != finalX || (int)j != finalY)) {
                        //Si no es la sala inicial ni la final, le asigna una recompensa
                        if(salaRecompensaCont == salaRecompensa) {
                            //Asigna el portal a la sala de desafio, que solo estara en una sala
                            salaActual->setRecompensa(2);
                            salaRecompensa = -1;
                        }
                        else {
                            //Asigna un cofre con objetos aleatorios
                            salaActual->setRecompensa(1);
                            salaRecompensaCont++;
                        }
                    }
                    else if((int)i == finalX && (int)j == finalY) {
                        //Genera el portal a la sala del jefe en la sala final
                        mazmorraSalas[i][j]->setRecompensa(3);
                    }
                }
            }
        }
        Sala::actualizarPuertasTapiadas(&mazmorraSalas);

        //Si por lo que sea no ha asignado el portal, recorre la matriz y lo anyade a la primera sala que encuentre
        if(salaRecompensa != -1) {
            for(unsigned int i=0; i<tamMatrizMaz && salaRecompensa != -1; i++) {
                for(unsigned int j=0; j<tamMatrizMaz && salaRecompensa != -1; j++) {
                    if(mazmorraSalas[i][j] == nullptr) {
                        //Comprueba que no es la sala final ni la inicial
                        if(((int)i != inicioX || (int)j != inicioY) && ((int)i != finalX || (int)j != finalY)) {
                            mazmorraSalas[i][j]->setRecompensa(2);
                            salaRecompensa = -1;
                        }
                    }
                }
            }
        }
    }
    
    return mazmorraSalas;
}

//Devuelve el nivel maximo de los materiales en el area generada
int GeneradorMazmorras::getNivelMateriales() {
    return nivMaterial;
}

//Devuelve el indice de la siguiente area
int GeneradorMazmorras::getSigArea() {
    return sigArea;
}