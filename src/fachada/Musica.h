#pragma once
#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;

class Musica {
    public:
        Musica(string, float);
        

        ~Musica();


        void play();
        void pause();
        void stop();
        void setLoop(bool);
        bool getLoop();
        bool getStatus(int);
        void setVolume(float);

        

    private:       
        sf::Music* musica;  
                      
};