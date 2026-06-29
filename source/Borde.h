#pragma once
#include "ObjetoFisico.h"

class Borde : public ObjetoFisico {

private:

    float ancho;
    float alto;

public:

    Borde(b2World* mundo, b2Vec2 posicion, float w, float h, float angulo); // Le agrego el parámetro de ángulo en radianes
    ~Borde() override;

    void Dibujar() override;

};