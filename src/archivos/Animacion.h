#ifndef ANIMACION_H
#define ANIMACION_H

#include<iostream>
#include<string>
#include<map>
//Librerias facade

#include "../fachada/Textura.h"
#include "../fachada/Sprite.h"

class Animacion
{
private:
	class Animar
	{
	public:
		
		Sprite* sprite;
		
		float tiempoAnim;
		float timer;
		int width;
		int height;
		bool finished;
		bool startOnce;
		vector<int> primerImg;
		vector<int> currentRect;
		vector<int> ultimaImg;

		Animar(Sprite* sprite,
			float tiempoAnim, 
			int primerFrame_x, int primerFrame_y, int endFrames_x, int endFrames_y, int width, int height)
			: sprite(sprite),  
			tiempoAnim(tiempoAnim), width(width), height(height)
		{
			this->timer = 0.f;

			//this->primerImg = sf::IntRect(primerFrame_x * width, primerFrame_y * height, width, height);
			this->primerImg.push_back(primerFrame_x * width);
			this->primerImg.push_back( primerFrame_y * height);
			this->primerImg.push_back(width);
			this->primerImg.push_back(height);

			this->currentRect = this->primerImg;
			//this->ultimaImg = sf::IntRect(endFrames_x * width,  * height, width, height);
			this->ultimaImg.push_back(endFrames_x * width);
			this->ultimaImg.push_back( endFrames_y * height);
			this->ultimaImg.push_back(width);
			this->ultimaImg.push_back(height);

			finished = false;
			startOnce = true;	
		}

		
		void play(const float& dt)
		{
			this->timer +=  dt;
			if (this->timer >= this->tiempoAnim)
			{
				this->timer = 0.f;

				if (this->currentRect[0] != this->ultimaImg[0])
				{
					this->currentRect[0] += this->width;
				}
				else if(currentRect[1] < ultimaImg[1]) {
					currentRect[0] = primerImg[0];
					currentRect[1] += height;
				}
				else
				{
					this->currentRect[0] = this->primerImg[0];
					currentRect[1] = primerImg[1];
				}

				this->sprite->setRect(this->currentRect[0],this->currentRect[1],this->currentRect[2],this->currentRect[3]);
			}
		}

		//Reproduce una animacion una unica vez
		void playOnce(const float& dt)
		{
			if(startOnce) {
				//Si es la primera vez que se llama, se indica el estado como no acabada
				startOnce = false;
				finished = false;
			}

			if(!finished) {
				timer +=  dt;
				if (timer >= tiempoAnim){
					timer = 0.f;

					if (currentRect[0] != ultimaImg[0]){
						currentRect[0] += width;
					}
					else if(currentRect[1] < ultimaImg[1]) {
						currentRect[0] = primerImg[0];
						currentRect[1] += height;
					}
					else{
						//Cuando se llega al final se indica que ha acabado
						finished = true;
					}

					//Actualiza el sprite
					sprite->setRect(currentRect[0],currentRect[1],currentRect[2],currentRect[3]);
				}
			}
			
		}

		//Devuelve un bool que indica si ha terminado la animacion
		bool getFinished() {
			return finished;
		}

		void reset()
		{
			this->timer = 0.f;
			this->currentRect = this->primerImg;
			startOnce = true;
			finished = false;
		}
	};

	Sprite* sprite;
	
	std::map<std::string, Animar*> animaciones;
	Animar* lastAnimation;
	bool pause=false;
public:
	Animacion(Sprite* sprite);
	virtual ~Animacion();

	void addAnimation(const std::string estado,
	float tiempoAnim,
	int primerFrame_x, int primerFrame_y, int endFrames_x, int endFrames_y, int width, int height);

	void play(const std::string estado, const float& dt);
	void playOnce(const std::string estado, const float& dt);
	bool getFinished();
	void setPause();
	void reset();
};

#endif