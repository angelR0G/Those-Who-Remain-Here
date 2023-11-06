#include "Musica.h"


using namespace std;

//Constructor
Musica::Musica(string ruta, float volume) {
    musica = new sf::Music();
    if (!musica->openFromFile(ruta)) {
        exit(0);
    }
    musica->setVolume(volume);
    
}

//Destructor
Musica::~Musica() {
    delete musica;
}


void Musica::play() {
    musica->play();
}

void Musica::pause() {
    musica->pause();
}

void Musica::stop() {
    musica->stop();
}

bool Musica::getLoop() {
    return musica->getLoop();
}
void Musica::setLoop(bool loop) {
    musica->setLoop(loop);
}


void Musica::setVolume(float volume) {
    musica->setVolume(volume);
}


bool Musica::getStatus(int status){

    
    switch(status){
        case 0: 
            if(musica->getStatus()==sf::Music::Status::Playing){
                return true;
            }
        break;
        case 1:
            if(musica->getStatus()==sf::Music::Status::Stopped){
                return true;
            } 
        break;
        case 2:
            if(musica->getStatus()==sf::Music::Status::Paused){
                return true;
            }  
        break;
    
        default: return false;

    }
    return false;

}

/*
void Musica::setTime(Tiempo time) {
    musica->setPlayingOffset(time);
}
*/