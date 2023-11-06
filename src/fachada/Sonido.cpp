#include "Sonido.h"


using namespace std;

//Constructor
Sonido::Sonido(Buffer* buffer,float volume) {
    sonido = new sf::Sound();
    sonido->setBuffer(*buffer->getBuffer());
    sonido->setVolume(volume);
    
}

//Destructor
Sonido::~Sonido() {
    delete sonido;
}

void Sonido::setBuffer(Buffer* buffer) {
    sonido->setBuffer(*buffer->getBuffer());
    
}

void Sonido::play() {
    sonido->play();
}

void Sonido::pause() {
    sonido->pause();
}

void Sonido::stop() {
    sonido->stop();
}

bool Sonido::getLoop() {
    return sonido->getLoop();
}

void Sonido::setVolume(float volume) {
    sonido->setVolume(volume);
}


bool Sonido::getStatus(int status){

    
    switch(status){
        case 0: 
            if(sonido->getStatus()==sf::Sound::Status::Playing){
                return true;
            }
        break;
        case 1:
            if(sonido->getStatus()==sf::Sound::Status::Stopped){
                return true;
            } 
        break;
        case 2:
            if(sonido->getStatus()==sf::Sound::Status::Paused){
                return true;
            }  
        break;
    
        default: return false;

    }
    return false;

}

/*
void Sonido::setTime(Tiempo time) {
    sonido->setPlayingOffset(time);
}
*/