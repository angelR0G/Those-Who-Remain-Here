#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include "Buffer.h"

using namespace std;

class Sonido {
    public:
        Sonido(Buffer*,float);
        

        ~Sonido();


        void setBuffer(Buffer*);
        void play();
        void pause();
        void stop();
        void setLoop(bool);
        bool getLoop();
        bool getStatus(int);
        void setVolume(float);
       // void setTime(Tiempo time);     

    private:  
        sf::Sound* sonido; 
      
                      
};