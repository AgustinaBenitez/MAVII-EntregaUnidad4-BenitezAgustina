#include "Aro.h"

Aro::Aro(b2World* mundo, b2Vec2 posicion, float escala, bool empiezaSubiendo)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_dynamicBody, WHITE) {

    // Cargo la textura
    texturaAro = LoadTexture("assets/img/texturaAro.png");

    // Defino ancho y alto en base a la imagen original por la escala
    ancho = texturaAro.width * escala;
    alto = texturaAro.height * escala;

    // Asigno la velocidad inicial
    if (empiezaSubiendo) {
        velocidad = -150.0f;
    }
    else {
        velocidad = 80.0f;
    }
    
    // Bloqueo la rotación para que el cuerpo se mantenga siempre derecho
    cuerpo->SetFixedRotation(true);

    // Sugerencia de Gemini: Anulo la gravedad para que le motor no tenga que luchar contra ella
    cuerpo->SetGravityScale(0.0f);

    // Defino el alto del aro (aprox el 20% de la imagen)
    float altoAro = alto * 0.2f;

    // Calculo el centro del aro empujándolo hacia el techo de mi rectángulo
    b2Vec2 centroAro(0.0f, -alto / 2.0f + altoAro / 2.0f);

    // Creo la forma geométrica pasándole el centro desplazado
    b2PolygonShape formaAro;
    formaAro.SetAsBox(ancho / 2.0f, altoAro / 2.0f, centroAro, 0.0f);

    // Le doy poca densidad para que la pelota frene al chocar
    b2FixtureDef fixAro;
    fixAro.shape = &formaAro;
    fixAro.density = 1.0f;
    fixAro.friction = 0.5f;
    fixAro.restitution = 0.2f;
    cuerpo->CreateFixture(&fixAro);

    // Creo un ancla estática invisible en la misma posición de origen
    b2BodyDef defAncla;
    defAncla.type = b2_staticBody;
    defAncla.position = posicion;
    b2Body* ancla = mundo->CreateBody(&defAncla);

    // Configuro el joint prismático para que se deslice solo en el eje Y (0 en X, 1 en Y)
    b2PrismaticJointDef defJoint;
    b2Vec2 ejeMovimiento(0.0f, 1.0f);
    defJoint.Initialize(ancla, cuerpo, posicion, ejeMovimiento);

    // Le habilito los topes. Le digo que suba hasta 80 píxeles por encima del ancla
    defJoint.enableLimit = true;
    defJoint.lowerTranslation = -80.0f;
    defJoint.upperTranslation = 40.0f;

    // Le enciendo el motor y le doy fuerza
    defJoint.enableMotor = true;
    defJoint.maxMotorForce = 500000.0f;
    defJoint.motorSpeed = velocidad;

    joint = (b2PrismaticJoint*)mundo->CreateJoint(&defJoint);

}

Aro::~Aro() {

    UnloadTexture(texturaAro);

}

void Aro::Actualizar() {

    // Me fijo cuánto se desplazó el joint desde su punto de origen
    float recorrido = joint->GetJointTranslation();

    // Si chocó contra el límite de arriba, invierto la velocidad para que baje
    if (recorrido <= joint->GetLowerLimit()) {

        velocidad = 80.0f; // Positivo baja, lo hago bajar un poco más lento
        joint->SetMotorSpeed(velocidad);

    }

    // Si volvió al piso, invierto la velocidad para que suba rápido de nuevo
    else if (recorrido >= joint->GetUpperLimit()) {

        velocidad = -150.0f; // Negativo sube rápido
        joint->SetMotorSpeed(velocidad);

    }

}

void Aro::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    Rectangle origen = { 0.0f, 0.0f, (float)texturaAro.width, (float)texturaAro.height };
    Rectangle destino = { pos.x, pos.y, ancho, alto };
    Vector2 ejeRotacion = { ancho / 2.0f, alto / 2.0f };

    DrawTexturePro(texturaAro, origen, destino, ejeRotacion, 0.0f, WHITE);

}

void Aro::DibujarDebug() {

    b2Vec2 ancla = joint->GetAnchorA();
    float inferior = joint->GetLowerLimit();
    float superior = joint->GetUpperLimit();

    // Como viaja en el eje Y, sumo los límites a la posición original del ancla
    Vector2 inicio = { ancla.x, ancla.y + inferior };
    Vector2 fin = { ancla.x, ancla.y + superior };

    // Línea de riel de movimiento con punto de anclaje
    DrawLineEx(inicio, fin, 4.0f, DARKGREEN);
    DrawCircle(ancla.x, ancla.y, 4.0f, BLUE);

    // Escribo la info
    DrawText("Prismatic Joint", ancla.x - 120, ancla.y - 65, 15, BLUE);
    DrawText("Restringe el movimiento a una unica direccion", ancla.x - 120, ancla.y - 45, 10, BLACK);
    DrawText("Cuerpo Dinamico", ancla.x - 120, ancla.y - 30, 12, BLUE);

}