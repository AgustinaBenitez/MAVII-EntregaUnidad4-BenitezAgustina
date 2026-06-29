#include "Tirador.h"

Tirador::Tirador(b2World* mundo, b2Vec2 posicion, float escala, const char* ruta01, const char* ruta02, const char* ruta03)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_staticBody, WHITE) {

    fuerzaCargada = 0.0f;
    yaDisparo = false;

    // Cargo las texturas
    texturaTirador01 = LoadTexture("assets/img/texturaTirador01.png");
    texturaTirador02 = LoadTexture("assets/img/texturaTirador02.png");
    texturaTirador03 = LoadTexture("assets/img/texturaTirador03.png");

    estadoAnimacion = 1;
    timerSalto = 0.0f;

    // Calculo el ancho y alto en base a la imagen original por la escala
    ancho = texturaTirador01.width * escala;
    alto = texturaTirador01.height * escala;

    // Creo una caja estática para que la pelota se apoye arriba sin caerse
    b2PolygonShape formaCaja;

    // Defino cuántos píxeles hacia abajo quiero desplazar la caja
    float bajarCaja = 35.0f;
    b2Vec2 centroDesplazado(0.0f, bajarCaja);

    // Le paso el ancho, alto, el centro desplazado, y 0 rotación
    formaCaja.SetAsBox(ancho / 2.0f, alto / 2.0f, centroDesplazado, 0.0f);


    b2FixtureDef fixTirador;
    fixTirador.shape = &formaCaja;
    fixTirador.friction = 0.0f; // Fricción nula para que la pelota pueda girar al salir
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
    if (!yaDisparo && fuerzaCargada < 4500.0f) {
        fuerzaCargada += 75.0f;
    }

}

float Tirador::GetPorcentajeFuerza() {
    // Devuelve un valor entre 0.0 y 1.0
    return fuerzaCargada / 4500.0f;
}

void Tirador::Disparar(b2Body* cuerpoPelota) {

    // Solo dispara si todavía no se usó
    if (!yaDisparo) {

        if (cuerpoPelota != nullptr) {

            // Obtengo el centro real de la pelota en el mundo
            b2Vec2 centro = cuerpoPelota->GetWorldCenter();

            // Sugerido por Gemini: Defino el punto de impacto un poco más abajo del centro
            // Esto funciona como palanca para generar el backspin automáticamente
            b2Vec2 puntoImpacto = b2Vec2(centro.x, centro.y + 15.0f);

            // El vector de fuerza
            b2Vec2 impulso = b2Vec2(fuerzaCargada * 0.7f, -fuerzaCargada);

            // Aplico el impulso lineal pasándole el punto desplazado en lugar del centro
            cuerpoPelota->ApplyLinearImpulse(impulso, puntoImpacto, true);

        }

        fuerzaCargada = 0.0f;
        yaDisparo = true;

        // Inicio el salto
        estadoAnimacion = 2;
        timerSalto = 0.5f; // Medio segundo en el aire

    }

}

void Tirador::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    Texture2D texturaDibujar;

    float offsetSalto = 0.0f; // Para moverlo en el eje Y

    if (estadoAnimacion == 1) {
        texturaDibujar = texturaTirador01;
    }
    else if (estadoAnimacion == 2) {
        texturaDibujar = texturaTirador02;
        offsetSalto = -30.0f; // Lo dubijo 30 píxeles más arriba
    }
    else {
        texturaDibujar = texturaTirador03;
    }

    Rectangle origen = { 0.0f, 0.0f, (float)texturaDibujar.width, (float)texturaDibujar.height };
    Rectangle destino = { pos.x, pos.y + offsetSalto, ancho, alto };
    Vector2 ejeRotacion = { ancho / 2.0f, alto / 2.0f };

    DrawTexturePro(texturaDibujar, origen, destino, ejeRotacion, 0.0f, WHITE);

}

void Tirador::Actualizar() {

    // Si está saltando, le descuento tiempo
    if (estadoAnimacion == 2) {
        timerSalto -= GetFrameTime();
        if (timerSalto <= 0.0f) {
            estadoAnimacion = 3; // Terminó el salto, pasa a reposo
        }
    }
}

void Tirador::ReiniciarTiro() {

    yaDisparo = false;
    fuerzaCargada = 0.0f;
    estadoAnimacion = 1;

}

void Tirador::DibujarDebug() {

    b2Vec2 pos = cuerpo->GetPosition();

    // Le dibujo un rectángulo fucsia alrededor y el texto explicativo
    DrawRectangleLines(pos.x - ancho / 2.0f, pos.y - alto / 2.0f, ancho, alto, PURPLE);
    DrawText("Impulso Lineal", pos.x + 35, pos.y - 10, 15, PURPLE);
    DrawText("Aplica fuerza en Y", pos.x + 35, pos.y + 5, 10, BLACK);

}