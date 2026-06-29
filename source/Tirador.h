#pragma once
#include "ObjetoFisico.h"

class Tirador : public ObjetoFisico {

private:

    float ancho;
    float alto;
    Texture2D texturaTirador01;
    Texture2D texturaTirador02;
    Texture2D texturaTirador03;

    float fuerzaCargada;
    bool yaDisparo;

public:

    Tirador(b2World* mundo, b2Vec2 posicion, float escala, const char* ruta01, const char* ruta02, const char* ruta03);
    ~Tirador();

    void Dibujar() override;
    void Cargar();
    void Disparar(b2Body* cuerpoPelota);

    bool YaDisparo() { return yaDisparo; }

    void DibujarDebug() override;

};