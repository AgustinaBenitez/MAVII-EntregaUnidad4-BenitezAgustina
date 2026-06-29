#pragma once

#include "ObjetoFisico.h"
#include "Aro.h"
#include "Tirador.h"

#include <box2d.h>
#include <vector>
#include <memory>

// Escuchador de colisiones
class EscuchadorColisiones : public b2ContactListener {

public:

    EscuchadorColisiones();
    void BeginContact(b2Contact* contacto) override;

};

// Estados del juego
enum EstadoJuego { INICIO, JUGANDO, TERMINADO };

// Estados para controlar la animación del tirador
enum EstadoTirador { LISTO, SALTANDO, REPOSO };

class Juego {

private:

    EstadoJuego estadoActual;

    EstadoTirador estadoTirador;

    // Mundo físico (contenedor) + objetos
    std::unique_ptr<b2World> mundo;
    std::vector<std::unique_ptr<ObjetoFisico>> objetos;

    // Recursos
    //Music musicaFondo;
    Texture2D texturaFondo;

    // Listener
    std::unique_ptr<EscuchadorColisiones> escuchador;

    // Objetos
    std::unique_ptr<Tirador> tirador;
    std::unique_ptr<Aro> aro;
    //ObjetoFisico* pelotaPrincipal; // Puntero para rastrear la pelota principal

    // Para mostrar la info solicitada en pantalla sin ensuciar la pantalla de juego
    bool modoDebug = false;

public:

    Juego();
    ~Juego();

    void Iniciar();
    void Actualizar();
    void Renderizar();
    void Reiniciar();

};