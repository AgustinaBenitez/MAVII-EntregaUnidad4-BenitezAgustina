#include "Juego.h"

#include <raylib.h>

// Implementación del escuchador
EscuchadorColisiones::EscuchadorColisiones() {}

void EscuchadorColisiones::BeginContact(b2Contact* contacto) {

    // Obtengo los cuerpos que chocaron
    b2Fixture* fixA = contacto->GetFixtureA();
    b2Fixture* fixB = contacto->GetFixtureB();

    // Recupero los punteros que le pegué al userData en ObjetoFisico.cpp
    ObjetoFisico* objA = reinterpret_cast<ObjetoFisico*>(fixA->GetBody()->GetUserData().pointer);
    ObjetoFisico* objB = reinterpret_cast<ObjetoFisico*>(fixB->GetBody()->GetUserData().pointer);

    // Identifico quién es quién usando dynamic_cast
    // Identifico a la Pelota
    Pelota* pelota = dynamic_cast<Pelota*>(objA);
    if (pelota == nullptr) {
        pelota = dynamic_cast<Pelota*>(objB);
    }

    // Identifico al Aro
    Aro* aro = dynamic_cast<Aro*>(objA);
    if (aro == nullptr) {
        aro = dynamic_cast<Aro*>(objB);
    }

    // Identifico al Borde (Piso)
    Borde* piso = dynamic_cast<Borde*>(objA);
    if (piso == nullptr) {
        piso = dynamic_cast<Borde*>(objB);
    }

    // Entonces si chocaron la Pelota y el Aro
    if (pelota != nullptr && aro != nullptr) {

        bool tocoSensor = false;

        // Reviso si la fixture A era el sensor del aro
        if (fixA->GetBody() == aro->GetCuerpo() && fixA->IsSensor()) {
            tocoSensor = true;
        }
        // Si no, reviso si la fixture B era el sensor del aro
        else if (fixB->GetBody() == aro->GetCuerpo() && fixB->IsSensor()) {
            tocoSensor = true;
        }

        if (tocoSensor) {
            pelota->MarcarAnotacion();
        }
    }

    // Si chocaron la Pelota y el Borde (Piso)
    if (pelota != nullptr && piso != nullptr) {
        pelota->MarcarEnSuelo();
    }


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
    musicaFondo = LoadMusicStream("assets/audio/musicaFondo.mp3");
    musicaFondo.looping = true;     // Para que se repita infinitamente
    PlayMusicStream(musicaFondo);   // Le doy Play solo acá (una sola vez)

    // Configuro el escuchador de colisiones
    escuchador = std::make_unique<EscuchadorColisiones>();
    mundo->SetContactListener(escuchador.get());

    // Cargo todos los objetos
    Reiniciar();

    // Fuerzo el estado a INICIO solo en la primera carga del programa
    estadoActual = INICIO;

}

void Juego::Actualizar() {

    UpdateMusicStream(musicaFondo); // Obligatorio para que suene la música

    if (estadoActual == INICIO) {
        if (IsKeyPressed(KEY_ENTER)) {
            cronometro->Reiniciar(); // Para asegurar que los 2 minutos arranquen intactos
            estadoActual = JUGANDO;
        }
    }

    if (estadoActual == JUGANDO) {

        // Actualizo el segundero
        cronometro->Actualizar();

        // Si el tiempo llega a cero, se termina el juego
        if (cronometro->SeAcaboElTiempo() && estadoActual == JUGANDO) {

            estadoActual = TERMINADO;
            //PlaySound(sonidoFin);
            return;

        }

        // Sugerencia de Gemini: En lugar de dar 1 paso grande de 1/60, le hacemos dar 10 mini-pasos de 1/600
        // Esto engaña a Box2D y multiplica por 10 su límite de velocidad tope
        for (int i = 0; i < 10; i++) {
            mundo->Step(1.0f / 600.0f, 8, 3);
        }

        // Muevo el aro
        if (aro) aro->Actualizar();

        // Controles y cambio de estado visual del tirador
        if (tirador) {
            if (IsKeyDown(KEY_SPACE)) {
                tirador->Cargar();
                estadoTirador = LISTO; // Textura con pelota en mano
            }
            if (IsKeyReleased(KEY_SPACE)) {
                tirador->Disparar(pelotaPrincipal->GetCuerpo());
                estadoTirador = SALTANDO; // Textura estirado
            }
        }

        // Actualizo la lógica del tirador
        if (tirador) tirador->Actualizar();

        // Para nueva pelota
        if (IsKeyPressed(KEY_N)) {
            // Exigimos que el tirador exista, que haya disparado, que exista la pelota Y que se pueda recargar (tocó piso)
            if (tirador && tirador->YaDisparo() && pelotaPrincipal && puedeRecargar) {

                // Reseteo las fuerzas de Box2D para que no siga cayendo
                pelotaPrincipal->GetCuerpo()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                pelotaPrincipal->GetCuerpo()->SetAngularVelocity(0.0f);

                // La teletransportamos de nuevo a la mano
                pelotaPrincipal->GetCuerpo()->SetTransform(b2Vec2(400.0f, 475.0f), 0.0f);

                // Vuelvo al jugador a su estado inicial
                tirador->ReiniciarTiro();

                // Reseteo los estados lógicos de la pelota (sensor, piso, contabilizada)
                pelotaPrincipal->ResetearEstados();

                // Bloqueo la recarga para que no pueda apretar la 'N' 80 veces seguidas
                puedeRecargar = false;
            }
        }

        
        if (pelotaPrincipal) {
            // Si pasó por el aro y todavía no sumó
            if (pelotaPrincipal->Anoto() && !pelotaPrincipal->FueContabilizada()) {
                puntaje++;
                pelotaPrincipal->SetContabilizada(true);

                // Le mato la velocidad horizontal (X = 0) para que caiga recta
                b2Vec2 vel = pelotaPrincipal->GetCuerpo()->GetLinearVelocity();
                pelotaPrincipal->GetCuerpo()->SetLinearVelocity(b2Vec2(0.0f, vel.y));
            }

            // Si la pelota se fue por la derecha de la pantalla habilito presionar N
            if (pelotaPrincipal->GetCuerpo()->GetPosition().x > 1060.0f) {
                puedeRecargar = true;
            }

            // Si tocó el suelo, habilito presionar N
            if (pelotaPrincipal->EnSuelo()) {
                puedeRecargar = true;
            }
        }
        


    }


    // CHEQUEO DE FINALIZACIÓN

        //b2Vec2 posPelota = pelotaPrincipal->GetCuerpo()->GetPosition();

        // Obtengo la velocidad lineal de la pelota para saber hacia dónde viaja
        //b2Vec2 velPelota = pelotaPrincipal->GetCuerpo()->GetLinearVelocity();

        // Si se termina el tiempo
        //if (tiempo == 0) {
        //    estadoActual = TERMINADO;
        //}


    // Para reiniciar juego
    if (IsKeyPressed(KEY_R)) {
        Reiniciar();
    }

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

            // Rectángulo negro con 70% de opacidad para oscurecer el fondo
            DrawRectangle(0, 0, 1058, 992, Fade(BLACK, 0.7f));

            // Título principal
            const char* tituloInicio = "FIEBRE DE BASKET";
            int anchoTit = MeasureText(tituloInicio, 80);
            DrawText(tituloInicio, (1058 - anchoTit) / 2, 300, 80, ORANGE);

            // Textos de indicaciones divididos para que queden bien centrados
            const char* linea1 = "Entra en calor haciendo unos tiros libres antes del partido!";
            const char* linea2 = "Veamos cuantos logras encestar en 2 minutos!";
            const char* linea3 = "Presiona ENTER cuando estes listo";

            DrawText(linea1, (1058 - MeasureText(linea1, 30)) / 2, 450, 30, LIGHTGRAY);
            DrawText(linea2, (1058 - MeasureText(linea2, 30)) / 2, 490, 30, LIGHTGRAY);
            DrawText(linea3, (1058 - MeasureText(linea3, 30)) / 2, 600, 30, YELLOW);

        }
        else if (estadoActual == JUGANDO) {

            // Calculo minutos y segundos reales
            int minutos = (int)cronometro->tiempoRestante / 60;
            int segundos = (int)cronometro->tiempoRestante % 60;

            // Dibujo el tiempo en formato MM:SS
            DrawText(TextFormat("TIEMPO: %02d:%02d", minutos, segundos), 20, 20, 40, WHITE);

            // Informo controles
            DrawText("CONTROLES", 20, 80, 25, GREEN);
            DrawText("Mantener y soltar ESP: Lanzar", 20, 110, 20, LIGHTGRAY);
            DrawText("N: Nueva pelota", 20, 135, 20, LIGHTGRAY);
            DrawText("R: Reiniciar", 20, 160, 20, LIGHTGRAY);
            DrawText("I: Mostrar info", 20, 185, 20, LIGHTGRAY);

            // Informo objetivo
            const char* obj1 = "Encesta la mayor cantidad";
            const char* obj2 = "de pelotas posibles";
            const char* obj3 = "ANTES QUE SE ACABE EL TIEMPO!";

            DrawText(obj1, 1038 - MeasureText(obj1, 20), 20, 20, LIGHTGRAY);
            DrawText(obj2, 1038 - MeasureText(obj2, 20), 45, 20, LIGHTGRAY);
            DrawText(obj3, 1038 - MeasureText(obj3, 20), 70, 20, YELLOW);

            // Informo puntaje
            const char* textoPuntaje = TextFormat("PELOTAS ENCESTADAS: %d", puntaje);
            int anchoPuntaje = MeasureText(textoPuntaje, 40);
            DrawText(textoPuntaje, (1058 - anchoPuntaje) / 2, 850, 40, YELLOW);

            // Aviso al jugador que puede acceder a una nueva pelota
            if (puedeRecargar) {
                const char* textoRecarga = "PRESIONA 'N' PARA NUEVA PELOTA";
                int anchoRecarga = MeasureText(textoRecarga, 25);
                DrawText(textoRecarga, (1058 - anchoRecarga) / 2, 900, 25, DARKPURPLE);
            }

            // Dibujo la barra de fuerza solo si el tirador existe y todavía no disparó
            if (tirador && !tirador->YaDisparo()) {
                float porcentaje = tirador->GetPorcentajeFuerza();

                DrawText("POTENCIA:", 20, 225, 15, RED);

                // Rectángulo gris de fondo (el envase)
                DrawRectangle(20, 245, 200, 20, LIGHTGRAY);

                // Rectángulo rojo que crece multiplicando el ancho total (200) por el porcentaje (0.0 a 1.0)
                DrawRectangle(20, 245, 200 * porcentaje, 20, RED);

                // Borde negro para que quede prolijo
                DrawRectangleLines(20, 245, 200, 20, BLACK);
            }

        }
        else if (estadoActual == TERMINADO) {

            // Filtro semitransparente verde
            DrawRectangle(0, 0, 1058, 992, Fade(PINK, 0.5f));

            // Título 1
            const char* titulo1 = "Se acabo el tiempo!";
            int tam1 = 60;
            int anchoTitulo1 = MeasureText(titulo1, tam1);
            DrawText(titulo1, (1058 - anchoTitulo1) / 2, 180, tam1, ORANGE);

            // Armo el mensaje dinámico dependiendo de los aciertos
            const char* mensaje;
            if (puntaje == 0) {
                mensaje = "No embocaste ni una... Volve a intentarlo!";
            }
            else if (puntaje == 1) {
                mensaje = "Embocaste una sola pelota... Bueno, algo es algo je";
            }
            else {
                mensaje = TextFormat("Embocaste %d pelotas :D", puntaje);
            }

            // Mido y dibujo el mensaje elegido
            int tam2 = 40;
            int anchoMensaje = MeasureText(mensaje, tam2);
            DrawText(mensaje, (1058 - anchoMensaje) / 2, 280, tam2, YELLOW);

            // Título 2
            const char* titulo2 = "Presiona R para reiniciar";
            int tam3 = 30;
            int anchoTitulo2 = MeasureText(titulo2, tam3);
            DrawText(titulo2, (1058 - anchoTitulo2) / 2, 400, tam3, WHITE);

        };


    // Mostrando info
    if (modoDebug) {

        if (aro) aro->DibujarDebug();
        if (tirador) tirador->DibujarDebug();

        //for (const auto& obj : objetos) {
            //obj->DibujarDebug();
        //}

    }

    EndDrawing();

}


void Juego::Reiniciar() {

    // Para que cada vez que se presione la tecla R, el estado del juego y del jugador se resetee
    estadoActual = JUGANDO;
    estadoTirador = LISTO;
    puntaje = 0;
    puedeRecargar = false;

    // Limpio el vector por si toco la tecla R durante el juego
    objetos.clear();

    // Creo el piso estático invisible a los pies del jugador). Ancho = 2000.
    auto piso = std::make_unique<Borde>(mundo.get(), b2Vec2{ 500.0f, 760.0f }, 2000.0f, 20.0f, 0.0f);
    objetos.emplace_back(std::move(piso));    

    // Creo el reloj
    cronometro = std::make_unique<Temporizador>(120.0f); // Timer de cuenta regresiva a 2 minutos

    ////// Instancio los objetos
    // Aro a la derecha 
    aro = std::make_unique<Aro>(mundo.get(), b2Vec2{ 942.0f, 400.0f }, 1.6f, true);

    // Tirador a la izquierda en la línea de penal
    tirador = std::make_unique<Tirador>(mundo.get(), b2Vec2{ 360.0f, 600.0f }, 1.8f, "assets/img/texturaTirador01.png", "assets/img/texturaTirador02.png", "assets/img/texturaTirador03.png");

    // Pelota justo arriba del tirador
    auto nuevaPelota = std::make_unique<Pelota>(mundo.get(), b2Vec2{ 400.0f, 475.0f }, 20.0f, WHITE);

    // Guardo el puntero para que el tirador sepa a qué dispararle
    pelotaPrincipal = nuevaPelota.get();

    // Meto la pelota en vector
    objetos.emplace_back(std::move(nuevaPelota));

}

Juego::~Juego() {

    // Limpio los objetos explícitamente AHORA ---- Sugerencia de Gemini
    // Esto fuerza a que todos los DestroyBody se ejecuten mientras el mundo sigue vivo.
    objetos.clear();

    // Descargo los recursos de Raylib
    UnloadMusicStream(musicaFondo);
    UnloadTexture(texturaFondo);

}