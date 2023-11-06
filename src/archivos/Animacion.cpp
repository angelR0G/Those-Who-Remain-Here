#include "Animacion.h"
#include <SFML/Graphics.hpp>



Animacion::Animacion(Sprite* sprite)
	:sprite(sprite), lastAnimation(NULL)
{
	
}

Animacion::~Animacion()
{
	for (auto &i : this->animaciones)
	{
		delete i.second;
	}
/*	for(int i=0;i<animaciones.size();i++){
		delete animaciones[i];
	}*/
}

void Animacion::addAnimation(
	const std::string estado,
	float tiempoAnim,
	int primerFrame_x, int primerFrame_y, int endFrames_x, int endFrames_y, int width, int height
)
{
	this->animaciones[estado] = new Animar(
		this->sprite,
		tiempoAnim,
		primerFrame_x, primerFrame_y, endFrames_x, endFrames_y, width, height
	);
}

void Animacion::play(const std::string estado, const float & dt)
{	 

	if(pause==false){
		if (this->lastAnimation != this->animaciones[estado])
		{
		if(this->lastAnimation == NULL)
			this->lastAnimation = this->animaciones[estado];
		else
		{
			this->lastAnimation->reset();
			this->lastAnimation = this->animaciones[estado];
		}	
		}
		this->animaciones[estado]->play(dt);
	}
	
}

void Animacion::playOnce(const std::string estado, const float & dt)
{	 

	if(pause==false){
		if (this->lastAnimation != this->animaciones[estado])
		{
		if(this->lastAnimation == NULL)
			this->lastAnimation = this->animaciones[estado];
		else
		{
			this->lastAnimation->reset();
			this->lastAnimation = this->animaciones[estado];
		}	
		}
		this->animaciones[estado]->playOnce(dt);
	}
	
}

bool Animacion::getFinished() {
	return lastAnimation->getFinished();
}

void Animacion::setPause()
{
	if(this->pause){
		pause=false;
	}else{
		pause=true;
	}
}

//Reinicia el estado de la animacion
void Animacion::reset() {
	lastAnimation->reset();
}