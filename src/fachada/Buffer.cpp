#include "Buffer.h"

using namespace std;

//Constructor
Buffer::Buffer(string ruta) {
    buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(ruta)) {
        cout << "Error cargando el audio " << ruta;
        exit(0);
    }
}

//Destructor
Buffer::~Buffer() {
    delete buffer;
}


sf::SoundBuffer* Buffer::getBuffer() {
    return buffer;
}

