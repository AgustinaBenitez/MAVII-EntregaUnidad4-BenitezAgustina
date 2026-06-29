#include "ObjetoFisico.h"

ObjetoFisico::ObjetoFisico(b2World* mundo, b2Vec2 posicion, float angulo, b2BodyType tipo, Color col) {

    color = col;

    // Configuración inicial del cuerpo físico
    b2BodyDef defCuerpo;
    defCuerpo.type = tipo;
    defCuerpo.position = posicion;
    defCuerpo.angle = angulo;

    // Vinculo el cuerpo físico de Box2D con esta instancia
    defCuerpo.userData.pointer = reinterpret_cast<uintptr_t>(this);

    // Para que cree el cuerpo y me devuelva el puntero
    cuerpo = mundo->CreateBody(&defCuerpo);

}

ObjetoFisico::~ObjetoFisico() {

    if (cuerpo != nullptr) {

        // Obtengo el mundo al que pertenece el cuerpo y le pido que lo borre
        cuerpo->GetWorld()->DestroyBody(cuerpo);

    }

}