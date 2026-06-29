#include "Juego.h"

#include <raylib.h>

// Implementación del escuchador
EscuchadorColisiones::EscuchadorColisiones() {}

void EscuchadorColisiones::BeginContact(b2Contact* contacto) {

    // Obtengo los cuerpos que chocaron
    b2Body* cuerpoA = contacto->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contacto->GetFixtureB()->GetBody();

    // Recupero los punteros que le pegué al userData en ObjetoFisico.cpp
    ObjetoFisico* objA = reinterpret_cast<ObjetoFisico*>(cuerpoA->GetUserData().pointer);
    ObjetoFisico* objB = reinterpret_cast<ObjetoFisico*>(cuerpoB->GetUserData().pointer);

    // Si existen, les aviso que se tocaron
    //if (objA != nullptr) {
    //    objA->Golpeado();
    //}

    //if (objB != nullptr) {
    //    objB->Golpeado();
    //}

}

Juego::Juego() {

    // Inicializo gravedad
    b2Vec2 gravedad(0.0f, 150.0f);

    // Inicializo mundo físico
    mundo = std::make_unique<b2World>(gravedad);

}

void Juego::Iniciar() {

    InitWindow(1058, 992, "MAVII - Entrega Actividad 4 - Benitez Agustina");

    InitAudioDevice();

    SetTargetFPS(60);

    // Cargo imagen de fondo
    texturaFondo = LoadTexture("assets/img/texturaFondo.png");

    // Cargo música de fondo
    //musicaFondo = LoadMusicStream("assets/audio/musicaFondo.mp3");
    //musicaFondo.looping = true;     // Para que se repita infinitamente
    //PlayMusicStream(musicaFondo);   // Le doy Play solo acá (una sola vez)

    // Configuro el escuchador de colisiones
    escuchador = std::make_unique<EscuchadorColisiones>();
    mundo->SetContactListener(escuchador.get());

    // Cargo todos los objetos
    Reiniciar();

}

void Juego::Actualizar() {

    //UpdateMusicStream(musicaFondo); // Obligatorio para que suene la música

    if (estadoActual == JUGANDO) {

        // Sugerencia de Gemini: En lugar de dar 1 paso grande de 1/60, le hacemos dar 10 mini-pasos de 1/600
        // Esto engaña a Box2D y multiplica por 10 su límite de velocidad tope
        for (int i = 0; i < 10; i++) {
            mundo->Step(1.0f / 600.0f, 8, 3);
        }

        // Muevo el aro
        if (aro) aro->Actualizar();

        // Controles y cambio de estado visual del tirador
        //if (tirador) {
        //    if (IsKeyDown(KEY_SPACE)) {
        //        tirador->Cargar();
        //        estadoTirador = LISTO; // Textura con pelota en mano
        //    }
        //    if (IsKeyReleased(KEY_SPACE)) {
        //        tirador->Disparar(pelotaPrincipal->GetCuerpo());
        //        estadoTirador = SALTANDO; // Textura estirado
        //    }
        //}

        // Poner al tirador en reposo 1 segundo después de tirar.
        // 
        // CHEQUEO DE FINALIZACIÓN

        //b2Vec2 posPelota = pelotaPrincipal->GetCuerpo()->GetPosition();

        // Obtengo la velocidad lineal de la pelota para saber hacia dónde viaja
        //b2Vec2 velPelota = pelotaPrincipal->GetCuerpo()->GetLinearVelocity();

        // Si se termina el tiempo
        //if (tiempo == 0) {
        //    estadoActual = TERMINADO;
        //}


    }

    // Para reiniciar juego
    if (IsKeyPressed(KEY_R)) {
        Reiniciar();
    }

    // Para nueva pelota
    //if (IsKeyPressed(KEY_N)) {
    //método para pelota nueva    
    //}

    // Para mostrar info en pantalla
    if (IsKeyPressed(KEY_I)) {
        modoDebug = !modoDebug;
    }

}

void Juego::Renderizar() {

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureEx(texturaFondo, { 0.0f, 0.0f }, 0.0f, 0.5f, WHITE);

        // Box2D calcula, Raylib dibuja
        for (const auto& obj : objetos) {
            obj->Dibujar();
        }

        // Dibujo el aro
        if (aro) {
            aro->Dibujar();
        }

        // Dibujo el tirador
        if (tirador) {
            tirador->Dibujar();
        }

    // Carteles
        if (estadoActual == INICIO) {

            DrawText("Entra en calor haciendo unos tiros libres antes del partido!", 130, 280, 30, RED);

        }
        else if (estadoActual == JUGANDO) {

            DrawText("CONTROLES", 15, 50, 25, BLACK);
            DrawText("ESP (mantener): Lanzar", 15, 80, 20, DARKGRAY);
            DrawText("N: Nueva pelota", 15, 105, 20, DARKGRAY);
            DrawText("R: Reiniciar", 15, 130, 20, DARKGRAY);
            DrawText("I: Mostrar info", 15, 155, 20, DARKGRAY);

            DrawText("Encestá", 720, 50, 20, DARKGRAY);
            DrawText("la mayor cantidad de pelotas", 720, 70, 20, DARKGRAY);
            DrawText("ANTES QUE SE ACABE EL TIEMPO!", 720, 95, 25, DARKGREEN);

        }
        else if (estadoActual == TERMINADO) {

            DrawText("Se acabó el tiempo. Embocaste X pelotas. Presiona R para reiniciar.", 250, 15, 50, MAROON);

        };


    // Mostrando info
    //if (modoDebug) {

        //if (ascensorAro) ascensorAro->DibujarDebug();
        //if (tirador) tirador->DibujarDebug();

        //for (const auto& obj : objetos) {
            //obj->DibujarDebug();
        //}

    //}

    EndDrawing();

}


void Juego::Reiniciar() {

    // Para que cada vez que se presione la tecla R, el estado del juego y del jugador se resetee
    estadoActual = JUGANDO;
    estadoTirador = LISTO;

    // Limpio el vector por si toco la tecla R durante el juego
    objetos.clear();

    // Creo el piso estático
    //objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 450.0f, -50.0f }, 1000.0f, 100.0f, 0.0f));

    ////// Instancio los objetos
    // Aro a la derecha 
    aro = std::make_unique<Aro>(mundo.get(), b2Vec2{ 850.0f, 400.0f }, 1.5f, true);

    // Tirador a la izquierda en la línea de penal
    tirador = std::make_unique<Tirador>(mundo.get(), b2Vec2{ 200.0f, 600.0f }, 40.0f, 60.0f, "assets/img/texturaTirador01.png", "assets/img/texturaTirador02.png", "assets/img/texturaTirador03.png");

    // Pelota justo arriba del tirador
    //auto nuevaPelota = std::make_unique<Pelota>(mundo.get(), b2Vec2{ 220.0f, 550.0f }, 15.0f, WHITE);

    // Guardo el puntero para que el tirador sepa a qué dispararle
    //pelotaPrincipal = nuevaPelota.get();

    // Meto la pelota en vector
    //objetos.emplace_back(std::move(nuevaPelota));

}

Juego::~Juego() {

    // Limpio los objetos explícitamente AHORA ---- Sugerencia de Gemini
    // Esto fuerza a que todos los DestroyBody se ejecuten mientras el mundo sigue vivo.
    objetos.clear();

    // Descargo los recursos de Raylib
    //UnloadMusicStream(musicaFondo);
    UnloadTexture(texturaFondo);

}