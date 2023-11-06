#pragma once

#include "ObjetoSala.h"
#include "EstadoPartida.h"

class EscudoSuelto : public ObjetoSala {
    public:
        EscudoSuelto(vector<float>, Sprite*, Textura*);

        ~EscudoSuelto();

        void recoger(Inventario*);

        void mostrarInformacion();
    private:
};