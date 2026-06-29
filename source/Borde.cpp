#include "Borde.h"

Borde::Borde(b2World* mundo, b2Vec2 posicion, float w, float h, float angulo)
    : ObjetoFisico(mundo, posicion, angulo, b2_staticBody, RED) {

    ancho = w;
    alto = h;

    b2PolygonShape formaBorde;
    formaBorde.SetAsBox(ancho / 2.0f, alto / 2.0f);

    b2FixtureDef fixBorde;
    fixBorde.shape = &formaBorde;
    fixBorde.friction = 0.1f;
    fixBorde.restitution = 1.0f;

    cuerpo->CreateFixture(&fixBorde);

}

Borde::~Borde() {}