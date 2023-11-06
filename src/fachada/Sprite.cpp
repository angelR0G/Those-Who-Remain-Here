#include "Sprite.h"
#include "VentanaRender.h"

using namespace std;

//Constructor
Sprite::Sprite(Textura* tex, int x0, int y0, int width, int height) {
    //Le asigna la textura pasada por parametro
    sprite = new sf::Sprite();
    sprite->setTexture(*tex->getTextura());

    //Crea el rectangulo de recorte a partir de los int
    sprite->setTextureRect(sf::IntRect(x0, y0, width, height));
}

//Constructor de copia
Sprite::Sprite(Sprite& s){
    sprite = new sf::Sprite();
    sprite->setTexture(*s.getSprite()->getTexture());
    sprite->setTextureRect(s.getSprite()->getTextureRect());
}

//Destructor
Sprite::~Sprite() {
    delete sprite;
}

//Metodo que dibuja el sprite
void Sprite::draw() {
    VentanaRender* ventana = VentanaRender::Instancia();

    ventana->dibujar(this);
}

//Especifica la escala del sprite a partir de 2 floats
void Sprite::setScale(float scaleX, float scaleY) {
    sprite->setScale(scaleX, scaleY);
}

//Especifica la rotacion del sprite en angulos (valor negativo = sentido antihorario)
void Sprite::setRotation(float rotacion) {
    sprite->setRotation(rotacion);
}

//Especifica el punto de origen del sprite, a partir del cual se realizan las transformaciones
void Sprite::setOrigin(float x0, float y0) {
    sprite->setOrigin(x0, y0);
}

//Especifica la posicion del sprite en pantalla (en pixeles)
void Sprite::setPosition(float x, float y) {
    sprite->setPosition(x, y);
}

//Cambia la textura del sprite por la que recibe por parametro
void Sprite::setTextura(Textura* tex) {
    sprite->setTexture(*tex->getTextura());
}

/*A partir de 4 ints, especifica el rectangulo de recorte de la textura.
**Los dos primeros ints indican la posicion, el tercero el ancho y el ultimo el alto
*/
void Sprite::setRect(int x0, int y0, int width, int height) {
    sprite->setTextureRect(sf::IntRect(x0, y0, width, height));
}

//Devuelve un vector con los 4 int que definen el rectangulo de recorte
vector<int> Sprite::getRect() {
    sf::IntRect rect = sprite->getTextureRect();

    vector<int> vRect = {rect.left, rect.top, rect.width, rect.height};
    return vRect;
}

//Devuelve la posicion del sprite
vector<float> Sprite::getPosition() {
    sf::Vector2f aux = sprite->getPosition();
    vector<float> posicion = {aux.x, aux.y};

    return posicion;
}

//Devuelve la escala del sprite
vector<float> Sprite::getScale() {
    sf::Vector2f aux = sprite->getScale();
    vector<float> escala = {aux.x, aux.y};

    return escala;
}

//Devuelve el origen del sprite
vector<float> Sprite::getOrigin() {
    sf::Vector2f aux = sprite->getOrigin();
    vector<float> origen = {aux.x, aux.y};

    return origen;
}

//Devuelve la rotacion del sprite
float Sprite::getRotation() {
    return sprite->getRotation();
}

//Devuelve el sprite de SFML
sf::Sprite* Sprite::getSprite() {
    return sprite;
}

//Aplica un desplazamiento al sprite (lo mueve)
void Sprite::mover(int x, int y) {
    sprite->move(x, y);
}

//Comprueba si el sprite pasado por referencia intersecta con este.
bool Sprite::intersect(Sprite* s) {
    return sprite->getGlobalBounds().intersects(s->sprite->getGlobalBounds());
}

//Devuelve el tamanyo en pixeles del sprite teniendo en cuenta las transformaciones
vector<int> Sprite::getGlobalPixelSize() {
    vector<int> pixelSize;

    //Recupera el tamanyo del sprite y su escala
    sf::IntRect rect = sprite->getTextureRect();
    sf::Vector2f scale = sprite->getScale();

    //Guarda el tamanyo final
    pixelSize.push_back(abs(rect.width*scale.x));
    pixelSize.push_back(abs(rect.height*scale.y));

    return pixelSize;
}

//Devuelve si las coordenadas pasadas por parametro se encuentran dentro del sprite
bool Sprite::contains(float x, float y) {
    return sprite->getGlobalBounds().contains(x, y);
}

//Cambia el color del sprite
void Sprite::setColor(vector<int> col){
    sprite->setColor(sf::Color(col[0], col[1], col[2], col[3]));
}

//Recoge el color del sprite
vector<int> Sprite::getColor() {
    sf::Color color = sprite->getColor();

    return {color.r, color.g, color.b, color.a};
}