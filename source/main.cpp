#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

struct PhysicsBox
{
    b2Body* body;
    float width;
    float height;
    Color color;
};

struct PhysicsCircle
{
    b2Body* body;
    float radius;
    Color color;
};

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "MAVI II - Bienvenida");
    SetTargetFPS(60);

    Color fondo = { 110, 100, 215, 255 };
    Color textoPrincipal = RAYWHITE;
    Color textoSecundario = DARKPURPLE;
    Color sueloColor = Fade(DARKGREEN, 0.7f);

    // Mundo físico
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // -----------------------------
    // Suelo estático
    // -----------------------------
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    b2Body* groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    // -----------------------------
    // Crear algunas cajas dinámicas
    // -----------------------------
    for (int i = 0; i < 4; i++)
    {
        b2BodyDef boxDef;
        boxDef.type = b2_dynamicBody;
        boxDef.position.Set(300.0f + i * 80.0f, 80.0f + i * 20.0f);

        b2Body* boxBody = world.CreateBody(&boxDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(25.0f, 25.0f);

        b2FixtureDef boxFixture;
        boxFixture.shape = &boxShape;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.4f;
        boxFixture.restitution = 0.2f;

        boxBody->CreateFixture(&boxFixture);

        boxes.push_back({ boxBody, 50.0f, 50.0f, Fade(SKYBLUE, 0.95f) });
    }

    // -----------------------------
    // Crear algunos círculos dinámicos
    // -----------------------------
    for (int i = 0; i < 3; i++)
    {
        b2BodyDef circleDef;
        circleDef.type = b2_dynamicBody;
        circleDef.position.Set(650.0f + i * 60.0f, 60.0f + i * 30.0f);

        b2Body* circleBody = world.CreateBody(&circleDef);

        b2CircleShape circleShape;
        circleShape.m_radius = 20.0f;

        b2FixtureDef circleFixture;
        circleFixture.shape = &circleShape;
        circleFixture.density = 1.0f;
        circleFixture.friction = 0.3f;
        circleFixture.restitution = 0.6f;

        circleBody->CreateFixture(&circleFixture);

        circles.push_back({ circleBody, 20.0f, Fade(ORANGE, 0.95f) });
    }

    while (!WindowShouldClose())
    {
        // Avanzar simulación
        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

        // Dibujar cajas
        for (const auto& box : boxes)
        {
            b2Vec2 pos = box.body->GetPosition();
            float angle = box.body->GetAngle() * RAD2DEG;

            Rectangle rect = {
                pos.x - box.width / 2.0f,
                pos.y - box.height / 2.0f,
                box.width,
                box.height
            };

            Vector2 origin = { box.width / 2.0f, box.height / 2.0f };
            DrawRectanglePro(rect, origin, angle, box.color);
            DrawRectangleLinesEx(rect, 2, DARKBLUE);
        }

        // Dibujar círculos
        for (const auto& circle : circles)
        {
            b2Vec2 pos = circle.body->GetPosition();
            DrawCircleV({ pos.x, pos.y }, circle.radius, circle.color);
            DrawCircleLines((int)pos.x, (int)pos.y, circle.radius, BROWN);
        }

        // Panel superior
        DrawRectangle(90, 70, 820, 90, Fade(BLACK, 0.18f));
        DrawText("Bienvenidos a Modelos y Algoritmos para Videojuegos II", 120, 90, 28, textoPrincipal);
        DrawText("Raylib dibuja. Box2D simula.", 320, 125, 22, textoSecundario);

        // Pie
        DrawText("Primer contacto con simulacion fisica en 2D", 300, 540, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}