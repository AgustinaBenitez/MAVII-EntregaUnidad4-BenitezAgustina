#include "Pelota.h"


Pelota::Pelota(b2World* mundo, b2Vec2 posicion, float r, Color col)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_dynamicBody, col) {

    radio = r;

    texturaPelota = LoadTexture("assets/img/texturaPelota.png");

    b2CircleShape formaPelota;
    formaPelota.m_radius = radio;

    // Propiedades
    b2FixtureDef fixPelota;
    fixPelota.shape = &formaPelota;
    fixPelota.density = 0.01f;
    fixPelota.friction = 1.0f;
    fixPelota.restitution = 0.6f;

    cuerpo->CreateFixture(&fixPelota);

}

Pelota::~Pelota() {

    UnloadTexture(texturaPelota);

}

void Pelota::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    float angulo = cuerpo->GetAngle() * RAD2DEG;

    // Origen: mi imagen original
    Rectangle origen = { 0.0f, 0.0f, (float)texturaPelota.width, (float)texturaPelota.height };

    // Destino: dónde y de qué tamaño se dibuja (el diámetro es el doble del radio)
    Rectangle destino = { pos.x, pos.y, radio * 2.0f, radio * 2.0f };

    // Eje de rotación: el centro exacto de mi dibujo
    Vector2 ejeRotacion = { radio, radio };

    // Raylib dibuja la imagen rotada copiando la física
    DrawTexturePro(texturaPelota, origen, destino, ejeRotacion, angulo, WHITE);

}

void Pelota::DibujarDebug() {

    b2Vec2 pos = cuerpo->GetPosition();
    b2Vec2 vel = cuerpo->GetLinearVelocity();

    // Dibujo el vector de velocidad escalado para que no se salga de la pantalla
    DrawLineEx({ pos.x, pos.y }, { pos.x + vel.x * 0.1f, pos.y + vel.y * 0.1f }, 2.0f, BLUE);

    // Texto indicando el tipo de cuerpo de Box2D
    DrawText("Cuerpo Dinamico", pos.x + 20, pos.y - 15, 12, BLUE);
}