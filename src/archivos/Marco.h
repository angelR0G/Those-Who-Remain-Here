#pragma once

#include "../fachada/Sprite.h"
#include "../fachada/Textura.h"

using namespace std;

class Marco{
    public:
        static Marco* Instancia();
        ~Marco();
        Textura* getTextura();
        vector<Sprite*> getMarco();
    protected:
        Marco();
    private:
        static Marco* pMarco;
        Textura* texMarco;
        vector<Sprite*> marco;
};