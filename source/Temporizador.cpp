#include "Temporizador.h"

Temporizador::Temporizador(float segundos) {

    tiempoInicial = segundos;
    tiempoRestante = segundos;

}

Temporizador::~Temporizador() {}

void Temporizador::Actualizar() {

    // Solo resto si todavía queda tiempo
    if (tiempoRestante > 0) {
        tiempoRestante -= GetFrameTime();
    }

    else {
        tiempoRestante = 0; // Evito números negativos
    }

}

void Temporizador::Reiniciar() {

    tiempoRestante = tiempoInicial;

}

bool Temporizador::SeAcaboElTiempo() {

    return tiempoRestante <= 0;

}