#pragma once

#include "ObjetoFisico.h"
#include "Aro.h"
#include "Tirador.h"
#include "Pelota.h"
#include "Borde.h"
#include "Temporizador.h"

#include <box2d.h>
#include <vector>
#include <memory>

// Escuchador de colisiones
class EscuchadorColisiones : public b2ContactListener {

private:

    Sound* sonidoPelota = nullptr; // Puntero para acceder al sonido desde acá

public:

    EscuchadorColisiones();
    void SetSonidoPelota(Sound* sonido); // Método para ponerle el sonido
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
    Music musicaFondo;
    Texture2D texturaFondo;
    Sound sonidoAro;
    Sound sonidoPelota;
    Sound sonidoFin;

    // Listener
    std::unique_ptr<EscuchadorColisiones> escuchador;

    // Objetos
    std::unique_ptr<Tirador> tirador;
    std::unique_ptr<Aro> aro;
    Pelota* pelotaPrincipal; // Puntero para rastrear la pelota principal
    std::unique_ptr<Temporizador> cronometro; // Reloj

    // Para mostrar la info en pantalla
    bool modoDebug = false;
    int puntaje;

    bool puedeRecargar;

public:

    Juego();
    ~Juego();

    void Iniciar();
    void Actualizar();
    void Renderizar();
    void Reiniciar();

};