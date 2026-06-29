#pragma once
#include "ObjetoFisico.h"

class Pelota : public ObjetoFisico {

private:

    float radio;
    Texture2D texturaPelota;

    bool tocoSensor = false;
    bool contabilizada = false;
    bool tocoSuelo = false;

public:

    Pelota(b2World* mundo, b2Vec2 posicion, float r, Color col);
    ~Pelota();

    void Dibujar() override;
    void DibujarDebug() override;

    void MarcarAnotacion() { tocoSensor = true; }
    bool Anoto() { return tocoSensor; }

    void SetContabilizada(bool estado) { contabilizada = estado; }
    bool FueContabilizada() { return contabilizada; }

    void MarcarEnSuelo() { tocoSuelo = true; }
    bool EnSuelo() { return tocoSuelo; }

    void ResetearEstados() {
        tocoSensor = false;
        contabilizada = false;
        tocoSuelo = false;
    }

};