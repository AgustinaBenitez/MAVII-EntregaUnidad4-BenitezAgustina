#pragma once
#include "raylib.h"
#include <box2d.h>

class ObjetoFisico {

protected:

    b2Body* cuerpo;
    Color color;

public:

    ObjetoFisico(b2World* mundo, b2Vec2 posicion, float angulo, b2BodyType tipo, Color col);
    virtual ~ObjetoFisico();

    // Para poder acceder al cuerpo físico desde afuera
    b2Body* GetCuerpo() { return cuerpo; }

    virtual void Dibujar() = 0;
    virtual void DibujarDebug() {} // Para mostrar interacciones e indicadores visuales

};