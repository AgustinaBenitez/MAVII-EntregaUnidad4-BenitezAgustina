#pragma once
#include "ObjetoFisico.h"

class Aro : public ObjetoFisico {

private:

    float ancho;
    Texture2D texturaAro;
    b2PrismaticJoint* joint;
    float velocidad;

public:

    Aro(b2World* mundo, b2Vec2 posicion, float escala, bool empiezaSubiendo);
    ~Aro();

    float alto;

    void Dibujar() override;
    void Actualizar();

};