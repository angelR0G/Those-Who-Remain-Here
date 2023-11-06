#ifndef CONTROLADORSONIDO_H
#define CONTROLADORSONIDO_H


#include<iostream>
#include<string>
#include<map>

//Librerias facade

#include "../fachada/Buffer.h"
#include "../fachada/Sonido.h"
#include "../fachada/Musica.h"


class ControladorSonido
{
private:
	
	std::map<std::string, Sonido*> sonidos;
	static std::map<std::string, Musica*> musica;

	bool pause;
public:
	ControladorSonido();
	virtual ~ControladorSonido();
	//sonidos
	void addSonido(std::string estado,Buffer*,float);
	void play(const std::string estado);	
	void stop(const std::string estado);	
	void setPause();
	//musica
	static void addMusica(std::string estado,string ruta,float);
	static void removeMusica(std::string);
	static void playMusica(const std::string estado);
	static void stopMusica(const std::string estado);
	static void pararTodaMusica();
	static bool existeMusica(string);

};

#endif