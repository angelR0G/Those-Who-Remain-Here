#include "ControladorSonido.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../fachada/Buffer.h"
#include "../fachada/Sonido.h"


std::map<std::string, Musica*> ControladorSonido::musica = {};

ControladorSonido::ControladorSonido(){
	pause=false;
}

ControladorSonido::~ControladorSonido()
{
	for(auto i=sonidos.begin(); i != sonidos.end(); ++i) {
        delete sonidos.find(i->first)->second;
    }
	sonidos.clear();
}

void ControladorSonido::addSonido(std::string estado, Buffer* buffer, float volume)
{
	this->sonidos[estado] = new Sonido(buffer, volume);
	
}

void ControladorSonido::play(const std::string estado)
{	
	
		if((sonidos[estado]->getStatus(1)) ){
			this->sonidos[estado]->play();
		}		
}
void ControladorSonido::stop(const std::string estado)
{	
	
		this->sonidos[estado]->stop();		
}



void ControladorSonido::setPause()
{
	if(this->pause){
		pause=false;
	}else{
		pause=true;
	}
}


//Musica


void ControladorSonido::addMusica(std::string estado, string ruta, float volume)
{
	musica[estado] = new Musica(ruta, volume);
	
}

void ControladorSonido::removeMusica(std::string estado)
{
	musica.erase(estado);
	
}

void ControladorSonido::playMusica(const std::string estado)
{	
	
		
	musica[estado]->play();
	musica[estado]->setLoop(true);

				
}

void ControladorSonido::stopMusica(const std::string estado)
{            
    musica[estado]->stop();
            
}

//Metodo que para toda la musica que este en el vector
void ControladorSonido::pararTodaMusica(){
	for(auto i = musica.begin(); i!=musica.end(); ++i){
		if(i->second != nullptr){
			(i->second)->stop();
		}
	}
}

//Comprueba si la musica que corresponde a la clave pasada existe
bool ControladorSonido::existeMusica(string clave){
	bool existe = false;
	for(auto i = musica.begin(); i!=musica.end() && !existe; ++i){
		if(i->first == clave){
			existe = true;
		}
	}
	return existe;
}