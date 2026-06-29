#include "Tirador.h"

Tirador::Tirador(b2World* mundo, b2Vec2 posicion, float escala, const char* ruta01, const char* ruta02, const char* ruta03)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_staticBody, WHITE) {

    fuerzaCargada = 0.0f;
    yaDisparo = false;

    // Cargo las texturas
    texturaTirador01 = LoadTexture("assets/img/texturaTirador01.png");
    texturaTirador02 = LoadTexture("assets/img/texturaTirador02.png");
    texturaTirador02 = LoadTexture("assets/img/texturaTirador03.png");

    // Calculo el ancho y alto en base a la imagen original por la escala
    ancho = texturaTirador01.width * escala;
    alto = texturaTirador01.height * escala;

    // Creo una caja estática para que la pelota se apoye arriba sin caerse
    b2PolygonShape formaCaja;
    formaCaja.SetAsBox(ancho / 2.0f, alto / 2.0f);

    b2FixtureDef fixTirador;
    fixTirador.shape = &formaCaja;
    fixTirador.friction = 0.5f;
    fixTirador.restitution = 0.0f; // Cero rebote inicial

    cuerpo->CreateFixture(&fixTirador);

}

Tirador::~Tirador() {

    UnloadTexture(texturaTirador01);
    UnloadTexture(texturaTirador02);
    UnloadTexture(texturaTirador03);

}

void Tirador::Cargar() {

    // Aumenta la fuerza mientras se mantiene la barra espaciadora.
    if (!yaDisparo && fuerzaCargada < 3000.0f) {
        fuerzaCargada += 50.0f;
    }

}

void Tirador::Disparar(b2Body* cuerpoPelota) {

    // Solo dispara si todavía no se usó
    if (!yaDisparo) {
        if (cuerpoPelota != nullptr) {
            cuerpoPelota->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -fuerzaCargada), true);
        }

        fuerzaCargada = 0.0f;
        yaDisparo = true;

    }

}

void Tirador::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    Texture2D texturaDibujar;

    if (yaDisparo) {
        texturaDibujar = texturaTirador03; // Muestra al lanzador saltando
    }
    else {
        texturaDibujar = texturaTirador01; // Muestra al lanzador en reposo
    }

    Rectangle origen = { 0.0f, 0.0f, (float)texturaDibujar.width, (float)texturaDibujar.height };
    Rectangle destino = { pos.x, pos.y, ancho, alto };
    Vector2 ejeRotacion = { ancho / 2.0f, alto / 2.0f };

    DrawTexturePro(texturaDibujar, origen, destino, ejeRotacion, 0.0f, WHITE);

}

void Tirador::DibujarDebug() {

    b2Vec2 pos = cuerpo->GetPosition();

    // Le dibujo un rectángulo fucsia alrededor y el texto explicativo
    DrawRectangleLines(pos.x - ancho / 2.0f, pos.y - alto / 2.0f, ancho, alto, PURPLE);
    DrawText("Impulso Lineal", pos.x + 35, pos.y - 10, 15, PURPLE);
    DrawText("Aplica fuerza en Y", pos.x + 35, pos.y + 5, 10, BLACK);

}