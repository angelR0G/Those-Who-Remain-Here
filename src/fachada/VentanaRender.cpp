#include "VentanaRender.h"

using namespace std;
//Se inicializa el puntero de la instancia
VentanaRender* VentanaRender::pventanaRender = 0;

/*Se crea la instancia de VentanaRender
* Se le pasa ancho, alto y el nombre de la ventana
*/
VentanaRender* VentanaRender::Instancia(int ancho, int alto, string nombre){
    if(pventanaRender == 0){   //Primera llamada
        pventanaRender = new VentanaRender(ancho, alto, nombre); //Se crea la instancia de ventanaRender
    }
    return pventanaRender;
}

//Devuelve la instancia de VentanaRender
VentanaRender* VentanaRender::Instancia(){
    return pventanaRender;
}

//Constructor
VentanaRender::VentanaRender(int ancho, int alto, string nombre) {
    this->ancho = ancho;
    this->alto = alto;
    ventana.create(sf::VideoMode(ancho, alto), nombre);

    texFundido      = new Textura("resources/sprites/generales/negro.jpg");
    spriteFundido   = new Sprite(texFundido, 0, 0, 1920, 1080);
    spriteFundido->setPosition(-1920, -1080);
    spriteFundido->setScale(10, 10);

    pantallaNegro   = false;
    fade            = false;
}

//Destructor
VentanaRender::~VentanaRender() {
    delete spriteFundido;
    delete texFundido;
}

//Pone una vista pasada por parametro a la ventana
void VentanaRender::setVista(Vista* vista){
    ventana.setView(vista->getVista());
}

//Limpia la ventana con el color negro
void VentanaRender::limpiar(){
    ventana.clear();
}

//Muestra en la ventana lo que se haya renderizado
void VentanaRender::mostrar(){
    ventana.display();
}

//Dibuja un sprite
void VentanaRender::dibujar(Sprite* s){
    ventana.draw(*s->getSprite());
}

//Dibuja un texto
void VentanaRender::dibujar(Texto* t){
    ventana.draw(*t->getTexto());
}

//Dibuja un array de vertices
void VentanaRender::dibujar(ArrayVertices* a){
    ventana.draw(a->getArrayVertices());
}

//Comprueba si la ventana del juego esta abierta. True si la ventana esta abierta
bool VentanaRender::isOpen(){
    return ventana.isOpen();
}

//Cierra la ventana del juego
void VentanaRender::cerrar(){
    ventana.close();
}

//Devuelve el pollevent de la ventana
bool VentanaRender::pollEvento(Evento* evento){
    sf::Event *aux = evento->getEvento();
    return ventana.pollEvent(*aux);
}

//Convirte las coordenadas del punto pasado por parametro a las coordenadas del mundo
vector<float> VentanaRender::mapPixelToCoords(std::vector<int> pos){
    vector<float> coordsNuevas;

    sf::Vector2f coords = ventana.mapPixelToCoords(sf::Vector2i(pos[0], pos[1]), ventana.getView());
    coordsNuevas.push_back(coords.x);
    coordsNuevas.push_back(coords.y);
    return coordsNuevas;
}

//Limita el framerate del juego a los frames por segundo pasados
void VentanaRender::setFramerate(unsigned int fps){
    ventana.setFramerateLimit(fps);
}

vector<unsigned int> VentanaRender::getSize(){
    vector<unsigned int> size;
    size.push_back(ancho);
    size.push_back(alto);
    return size;
}

vector<unsigned int> VentanaRender::getSizeReal(){
    vector<unsigned int> size;
    size.push_back(ventana.getSize().x);
    size.push_back(ventana.getSize().y);
    return size;
}

vector<float> VentanaRender::getSizeVista() {
    sf::Vector2f size = ventana.getView().getSize();

    return {size.x, size.y};
}

//Devuelve la posicion del raton en la ventana
vector<int> VentanaRender::getPosicionRaton(){
    vector<int> posRaton;
    sf::Vector2i pos = sf::Mouse::getPosition(ventana);
    posRaton.push_back(pos.x);
    posRaton.push_back(pos.y);
    return posRaton;
}

//Recupera el boolean que indica si se esta haciendo fade in o fade out
bool VentanaRender::getFade() {
    return fade;
}

void VentanaRender::fadeIn(int velocidad) {
    fade = true;
    velocidad = max(velocidad, 1);
    velFundido = velocidad;
}

void VentanaRender::fadeOut(int velocidad) {
    fade = true;
    velocidad = max(velocidad, 1);
    velFundido = -velocidad;
}

//Actualiza el valor del fade
void VentanaRender::actualizarFade() {
    //Comprueba que se esta realizando fade
    if(fade) {
        //Comprueba si se hace un fade in o un fade out
        if(velFundido > 0) {
            //Suma la velocidad de fundido
            estadoFundido += velFundido;
            if(estadoFundido >= 255) {
                //Si ha alcanzado la maxima opacidad, termina el fade in
                estadoFundido = 255;
                fade = false;
                pantallaNegro = true;
                tiempoFade = 0;
            }
        }
        else {
            //Suma la velocidad de fundido
            estadoFundido += velFundido;
            pantallaNegro = false;
            if(estadoFundido <= 0) {
                //Si ha alcanzado la minima opacidad, termina el fade out
                estadoFundido = 0;
                fade = false;
                tiempoFade = 0;
            }
        }

        //Actualiza la opacidad del sprite
        spriteFundido->setColor({255, 255, 255, estadoFundido});
    }
}

//Dibuja la imagen de fundido
void VentanaRender::dibujarFade() {
    if(estadoFundido > 0) {
        ventana.draw(*spriteFundido->getSprite());
    }
}

//Devuelve si la pantalla esta en negro
bool VentanaRender::getPantallaNegro() {
    return pantallaNegro;
}

//Coloca un sprite en el puntero del raton
void VentanaRender::setMira(bool mostrar){
    ventana.setMouseCursorVisible(mostrar);
}

//Actualiza el tiempo en el que se llama a fade in
void VentanaRender::setTiempoFade(int t) {
    tiempoFade = t;
}

//Devuelve el tiempo en el que comenzo el fade
int VentanaRender::getTiempoFade() {
    return tiempoFade;
}

//Devuelve el centro de la vista actual de la ventana
vector<float> VentanaRender::getCentroVista() {
    sf::View view = ventana.getView();
    sf::Vector2f centro = view.getCenter();

    return {centro.x, centro.y};
}