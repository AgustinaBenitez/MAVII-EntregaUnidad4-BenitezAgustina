#pragma once
#include "raylib.h"

class Temporizador {

    friend class Juego;

private:

    float tiempoRestante;
    float tiempoInicial;

public:

    Temporizador(float segundos);
    ~Temporizador();

    void Actualizar();
    void Reiniciar();
    bool SeAcaboElTiempo();

};
