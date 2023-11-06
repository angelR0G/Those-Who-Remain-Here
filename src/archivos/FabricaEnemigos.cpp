#include "FabricaEnemigos.h"

FabricaEnemigos* FabricaEnemigos::pFabricaEnemigos = 0;

// VALORES PARA LA FABRICA

//Salud
const int SALUD_TORRETA             = 300;
const int SALUD_GOBLIN              = 360;
const int SALUD_PAJARO              = 400;
const int SALUD_DUENDE              = 400;
const int SALUD_CHAMAN              = 340;
const int SALUD_KAMIKAZE            = 170;
const int SALUD_AGUILA              = 280;
const int SALUD_MANIQUI             = 500;
const int SALUD_ESQUELETO           = 700;
const int SALUD_HECHICERO           = 1000;
const int SALUD_BUITRE              = 600;
const int SALUD_YETI                = 2400;
const int SALUD_MAGO                = 1300;
const int SALUD_GEODUDE             = 1600;
const int SALUD_TARANTULA           = 600;


//Danyo
const int DANYO_1                   = 1;
const int DANYO_2                   = 2;

//Velocidad
const int VELOCIDAD_TORRETA         = 30;
const int VELOCIDAD_GOBLIN          = 60;
const int VELOCIDAD_PAJARO          = 70;
const int VELOCIDAD_DUENDE          = 140;
const int VELOCIDAD_CHAMAN          = 0;
const int VELOCIDAD_KAMIKAZE        = 200;
const int VELOCIDAD_AGUILA          = 10;
const int VELOCIDAD_MANIQUI         = 0;
const int VELOCIDAD_ESQUELETO       = 60;
const int VELOCIDAD_HECHICERO       = 0;
const int VELOCIDAD_BUITRE          = 2;
const int VELOCIDAD_YETI            = 14;
const int VELOCIDAD_MAGO            = 0;
const int VELOCIDAD_GEODUDE         = 0;
const int VELOCIDAD_TARANTULA       = 110;


//Tiempo de recuperacion de los ataques (ms)
const int TIEMPO_REC_TORRETA        = 2500;
const int TIEMPO_REC_GOBLIN         = 3000;
const int TIEMPO_REC_PAJARO         = 5000;
const int TIEMPO_REC_DUENDE         = 1500;
const int TIEMPO_REC_CHAMAN         = 5500;
const int TIEMPO_REC_KAMIKAZE       = 200;
const int TIEMPO_REC_AGUILA         = 1200;
const int TIEMPO_REC_MANIQUI        = 0;
const int TIEMPO_REC_ESQUELETO      = 3000;
const int TIEMPO_REC_HECHICERO      = 4000;
const int TIEMPO_REC_BUITRE         = 2000;
const int TIEMPO_REC_YETI           = 6000;
const int TIEMPO_REC_MAGO           = 3000;
const int TIEMPO_REC_GEODUDE        = 3000;
const int TIEMPO_REC_TARANTULA      = 3000;


//Esencias soltadas al morir
const int ESENCIAS_TORRETA          = 100;
const int ESENCIAS_GOBLIN           = 100;
const int ESENCIAS_PAJARO           = 150;
const int ESENCIAS_DUENDE           = 250;
const int ESENCIAS_CHAMAN           = 300;
const int ESENCIAS_KAMIKAZE         = 0;
const int ESENCIAS_AGUILA           = 150;
const int ESENCIAS_MANIQUI          = 0;
const int ESENCIAS_ESQUELETO        = 100;
const int ESENCIAS_HECHICERO        = 300;
const int ESENCIAS_BUITRE           = 180;
const int ESENCIAS_YETI             = 700;
const int ESENCIAS_MAGO             = 300;
const int ESENCIAS_GEODUDE          = 250;
const int ESENCIAS_TARANTULA        = 150;



//Sprites (ruta)
const string SPRITE_TORRETA         = "resources/sprites/personaje/mHero.png";
const string SPRITE_GOBLIN          = "resources/sprites/pnjs/goblin_.png";
const string SPRITE_PAJARO          = "resources/sprites/pnjs/murcielago.png";
const string SPRITE_DUENDE          = "resources/sprites/pnjs/goblin_.png";
const string SPRITE_CHAMAN          = "resources/sprites/pnjs/Chaman.png";
const string SPRITE_KAMIKAZE        = "resources/sprites/pnjs/Kamikaze.png";
const string SPRITE_AGUILA          = "resources/sprites/pnjs/murcielago.png";
const string SPRITE_MANIQUI         = "resources/sprites/pnjs/maniqui.png";
const string SPRITE_ESQUELETO       = "resources/sprites/pnjs/skeleton_.png";
const string SPRITE_HECHICERO       = "resources/sprites/pnjs/hechicero.png";
const string SPRITE_BUITRE          = "resources/sprites/pnjs/buitre.png";
const string SPRITE_YETI            = "resources/sprites/pnjs/Yeti.png";
const string SPRITE_MAGO            = "resources/sprites/pnjs/mago.png";
const string SPRITE_GEODUDE         = "resources/sprites/pnjs/geodude.png";
const string SPRITE_TARANTULA       = "resources/sprites/pnjs/spider.png";

//ID de los materiales que suelta al ser derrotado
const vector<int> MAT_TORRETA       =   {0, 1};
const vector<int> MAT_GOBLIN        =   {1, 2};
const vector<int> MAT_PAJARO        =   {3, 3, 5, 5, 2};
const vector<int> MAT_DUENDE        =   {3, 4, 5};
const vector<int> MAT_CHAMAN        =   {0, 1};
const vector<int> MAT_KAMIKAZE      =   {};
const vector<int> MAT_AGUILA        =   {0, 2};
const vector<int> MAT_MANIQUI       =   {};
const vector<int> MAT_ESQUELETO     =   {3, 3, 4, 4, 1};
const vector<int> MAT_HECHICERO     =   {7, 7, 8, 8, 4};
const vector<int> MAT_BUITRE        =   {6, 6, 8, 8, 2};
const vector<int> MAT_YETI          =   {6, 7, 7, 8};
const vector<int> MAT_MAGO          =   {6, 6, 7, 7, 8, 3};
const vector<int> MAT_GEODUDE       =   {7, 8, 3, 4, 5};
const vector<int> MAT_TARANTULA     =   {4, 4, 5, 5, 0};

//Devuelve una instancia de fabrica de enemigos, creandola si no existe
FabricaEnemigos* FabricaEnemigos::Instancia(){
    if(pFabricaEnemigos == 0){                      //Primera llamada
        pFabricaEnemigos = new FabricaEnemigos();   //Se crea la instancia de FabricaEnemigos
    }
    return pFabricaEnemigos;
}

//Constructor
FabricaEnemigos::FabricaEnemigos() {}

//Destructor
FabricaEnemigos::~FabricaEnemigos() {}

//Dada una id, fabrica el enemigo correspondiente
Enemigo* FabricaEnemigos::fabricarEnemigo(int id) {
    Enemigo* enemigo = nullptr;

    switch(id) {
        case 0:{     //Torreta basica
            Textura* t = new Textura(SPRITE_TORRETA);
            Sprite* s = new Sprite(t, 0, 0, 14, 10);
            s->setOrigin(8, 8);
            s->setScale(3, 3);
            s->setColor({255, 255, 255, 200});
            enemigo = new Torreta(SALUD_TORRETA, DANYO_1, VELOCIDAD_TORRETA, TIEMPO_REC_TORRETA, ESENCIAS_TORRETA, MAT_TORRETA, t, s);
            break;
        } 
        case 1:{     //Goblin
            Textura* t2 = new Textura(SPRITE_GOBLIN);
            Sprite* s2 = new Sprite(t2, 0, 0, 24, 24);
            s2->setOrigin(24/2, 24/2);
            s2->setScale(3, 3);
            enemigo = new Goblin(SALUD_GOBLIN, DANYO_1, VELOCIDAD_GOBLIN, TIEMPO_REC_GOBLIN, ESENCIAS_GOBLIN, MAT_GOBLIN, t2, s2);
            break;
        }
        case 2:{    //Pajaro a melee
            Textura* t3 = new Textura(SPRITE_PAJARO);
            Sprite* s3 = new Sprite(t3, 0, 0, 16, 16);
            s3->setOrigin(16/2, 16/2);
            s3->setScale(3, 3);
            enemigo = new Pajaro(SALUD_PAJARO, DANYO_1, VELOCIDAD_PAJARO, TIEMPO_REC_PAJARO, ESENCIAS_PAJARO, MAT_PAJARO, t3, s3);
            break;
        }
        case 3:{    //Duende
            Textura* t4 = new Textura(SPRITE_DUENDE);
            Sprite* s4 = new Sprite(t4, 0, 0, 16, 16);
            s4->setOrigin(24/2, 24/2);
            s4->setColor({120, 120, 255, 255});
            s4->setScale(3, 3);
            enemigo = new Duende(SALUD_DUENDE, DANYO_1, VELOCIDAD_DUENDE, TIEMPO_REC_DUENDE, ESENCIAS_DUENDE, MAT_DUENDE, t4, s4);
            break;
        }
        case 4:{    //Chaman
            Textura* t5 = new Textura(SPRITE_CHAMAN);
            Sprite* s5 = new Sprite(t5, 0, 0, 14, 18);
            s5->setOrigin(14/2, 18/2 - 2);
            s5->setScale(3, 3);
            enemigo = new Chaman(SALUD_CHAMAN, DANYO_1, VELOCIDAD_CHAMAN, TIEMPO_REC_CHAMAN, ESENCIAS_CHAMAN, MAT_CHAMAN, t5, s5);
            break;
        }
        case 5:{    //Kamikaze
            Textura* t6 = new Textura(SPRITE_KAMIKAZE);
            Sprite* s6 = new Sprite(t6, 0, 0, 14, 14);
            s6->setOrigin(14/2, 14/2);
            s6->setScale(2, 2);
            enemigo = new Kamikaze(SALUD_KAMIKAZE, DANYO_1, VELOCIDAD_KAMIKAZE, TIEMPO_REC_KAMIKAZE, ESENCIAS_KAMIKAZE, MAT_KAMIKAZE, t6, s6);
            break;
        }
        case 6:{    //Aguila que dispara
            Textura* t7 = new Textura(SPRITE_AGUILA);
            Sprite* s7 = new Sprite(t7, 0, 0, 16, 16);
            s7->setColor({160, 160, 255, 255});
            s7->setOrigin(16/2, 16/2);
            s7->setScale(3, 3);
            enemigo = new Aguila(SALUD_AGUILA, DANYO_1, VELOCIDAD_AGUILA, TIEMPO_REC_AGUILA, ESENCIAS_AGUILA, MAT_AGUILA, t7, s7);
            break;
        }
        case 7:{     //Esqueleto parabólico
            Textura* t8 = new Textura(SPRITE_ESQUELETO);
            Sprite* s8 = new Sprite(t8, 0, 0, 24, 24);
            s8->setOrigin(24/2, 24/2);
            s8->setScale(3, 3);
            enemigo = new Esqueleto(SALUD_ESQUELETO, DANYO_2, VELOCIDAD_ESQUELETO, TIEMPO_REC_ESQUELETO, ESENCIAS_ESQUELETO, MAT_ESQUELETO, t8, s8);
            break;
        }
        case 8:{    //Hechicero
            Textura* t9 = new Textura(SPRITE_HECHICERO);
            Sprite* s9 = new Sprite(t9, 0, 0, 24, 24);
            s9->setOrigin(24/2, 24/2);
            s9->setScale(3, 3);
            enemigo = new Hechicero(SALUD_HECHICERO, DANYO_1, VELOCIDAD_HECHICERO, TIEMPO_REC_HECHICERO, ESENCIAS_HECHICERO, MAT_HECHICERO, t9, s9);
            break;
        }
        case 9:{    //Buitre
            Textura* t10 = new Textura(SPRITE_BUITRE);
            Sprite* s10 = new Sprite(t10, 0, 0, 16, 16);
            s10->setOrigin(8, 8);
            s10->setScale(3, 3);
            enemigo = new Buitre(SALUD_BUITRE, DANYO_1, VELOCIDAD_BUITRE, TIEMPO_REC_BUITRE, ESENCIAS_BUITRE, MAT_BUITRE, t10, s10);
            break;
        }
        case 10:{    //Yeti
            Textura* t11 = new Textura(SPRITE_YETI);
            Sprite* s11 = new Sprite(t11, 0, 0, 24, 28);
            s11->setOrigin(12, 14);
            s11->setScale(2, 2);
            enemigo = new Yeti(SALUD_YETI, DANYO_2, VELOCIDAD_YETI, TIEMPO_REC_YETI, ESENCIAS_YETI, MAT_YETI, t11, s11);
            break;
        }
        case 11:{     //Geodude
            Textura* t12 = new Textura(SPRITE_GEODUDE);
            Sprite* s12 = new Sprite(t12, 0, 0, 16, 20);
            s12->setOrigin(8, 8);
            s12->setScale(3, 3);
            enemigo = new Geodude(SALUD_GEODUDE, DANYO_2, VELOCIDAD_GEODUDE, TIEMPO_REC_GEODUDE, ESENCIAS_GEODUDE, MAT_GEODUDE, t12, s12);
            break;
        } 
        case 12:{     //Mago
            Textura* t13 = new Textura(SPRITE_MAGO);
            Sprite* s13 = new Sprite(t13, 0, 0, 18, 18);
            //s13->setOrigin(8, 8);
            //s13->setScale(3, 3);
            enemigo = new Mago(SALUD_MAGO, DANYO_2, VELOCIDAD_MAGO, TIEMPO_REC_MAGO, ESENCIAS_MAGO, MAT_MAGO, t13, s13);
            break;
        }
        case 13:{     //Tarantula
            Textura* t14 = new Textura(SPRITE_TARANTULA);
            Sprite* s14 = new Sprite(t14, 0, 0, 16, 12);
            s14->setOrigin(8, 6);
            s14->setScale(3.5, 3.5);
            enemigo = new Tarantula(SALUD_TARANTULA, DANYO_1, VELOCIDAD_TARANTULA, TIEMPO_REC_TARANTULA, ESENCIAS_TARANTULA, MAT_TARANTULA, t14, s14);
            break;
        }  
    }

    return enemigo;
}

//Dada una id, fabrica un jefe y lo devuelve
Enemigo* FabricaEnemigos::fabricarJefe(int id) {
    Enemigo* enemigo = nullptr;

    switch(id) {
        case 1:
            enemigo = new JefeRana();
            break;
        case 2:
            enemigo = new JefeOjo();
            break;
        case 3:
            enemigo = new JefeCaballero();
            break;
    }

    return enemigo;
}

//Crea un maniqui, enemigo especial de pruebas
Enemigo* FabricaEnemigos::fabricarManiqui() {
    Textura* tM = new Textura(SPRITE_MANIQUI);
    Sprite* sM = new Sprite(tM, 0, 0, 14, 18);
    sM->setOrigin(14/2, 18/2 - 2);
    sM->setScale(-3, 3);
    Enemigo* maniqui = new Maniqui(SALUD_MANIQUI, DANYO_1, VELOCIDAD_MANIQUI, TIEMPO_REC_MANIQUI, ESENCIAS_MANIQUI, {}, tM, sM);

    return maniqui;
}