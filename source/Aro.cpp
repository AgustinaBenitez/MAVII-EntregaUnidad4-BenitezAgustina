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

    // Tablero (Colisión física vertical a la derecha)
    float anchoTablero = ancho * 0.15f;
    float altoTablero = alto * 0.5f;
    b2Vec2 centroTablero(ancho * -0.10f, -30.0f);

    b2PolygonShape formaTablero;
    formaTablero.SetAsBox(anchoTablero / 2.0f, altoTablero / 2.0f, centroTablero, 0.0f);

    b2FixtureDef fixTablero;
    fixTablero.shape = &formaTablero;
    fixTablero.density = 1.0f;
    fixTablero.friction = 0.5f;
    fixTablero.restitution = 0.2f;
    cuerpo->CreateFixture(&fixTablero);

    // Sugerido por Gemini: Punta del aro (Colisión física sólida para el borde delantero)
    float radioPunta = 5.0f;
    b2Vec2 centroPunta(-ancho * 0.48f, alto * 0.1f); // Justo en la punta visual

    b2CircleShape formaPunta;
    formaPunta.m_p = centroPunta;
    formaPunta.m_radius = radioPunta;

    b2FixtureDef fixPunta;
    fixPunta.shape = &formaPunta;
    fixPunta.density = 1.0f;
    fixPunta.restitution = 0.1f;
    cuerpo->CreateFixture(&fixPunta);

    // El sensor de la red
    float anchoSensor = ancho * 0.15f;
    float altoSensor = 20.0f;
    b2Vec2 centroSensor(-ancho * 0.32f, alto * 0.35f);

    b2PolygonShape formaSensor;
    formaSensor.SetAsBox(anchoSensor / 2.0f, altoSensor / 2.0f, centroSensor, 0.0f);

    b2FixtureDef fixSensor;
    fixSensor.shape = &formaSensor;
    fixSensor.isSensor = true;
    fixSensor.userData.pointer = 2;
    cuerpo->CreateFixture(&fixSensor);

    // Sensor de Entrada en la boca del aro
    float anchoGate = ancho * 0.15f;
    float altoGate = 15.0f;
    b2Vec2 centroGate(-ancho * 0.20f, alto * 0.05f);

    b2PolygonShape formaGate;
    formaGate.SetAsBox(anchoGate / 2.0f, altoGate / 2.0f, centroGate, 0.0f);

    b2FixtureDef fixGate;
    fixGate.shape = &formaGate;
    fixGate.isSensor = true;
    fixGate.userData.pointer = 1;
    cuerpo->CreateFixture(&fixGate);

    // Creo un ancla estática invisible en la misma posición de origen
    b2BodyDef defAncla;
    defAncla.type = b2_staticBody;
    defAncla.position = posicion;
    b2Body* ancla = mundo->CreateBody(&defAncla);

    // Configuro el joint prismático para que se deslice solo en el eje Y (0 en X, 1 en Y)
    b2PrismaticJointDef defJoint;
    b2Vec2 ejeMovimiento(0.0f, 1.0f);
    defJoint.Initialize(ancla, cuerpo, posicion, ejeMovimiento);

    // Le habilito los topes
    defJoint.enableLimit = true;
    defJoint.lowerTranslation = -90.0f; // para subir
    defJoint.upperTranslation = 50.0f;  // para bajar

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