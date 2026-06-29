#pragma once
#include "ObjetoFisico.h"

class Pelota : public ObjetoFisico {

private:

    float radio;
    Texture2D texturaPelota;

public:

    Pelota(b2World* mundo, b2Vec2 posicion, float r, Color col);
    ~Pelota();

    void Dibujar() override;
    void DibujarDebug() override;

};