/*

MAV II - Entrega Unidad 4 - El sistema de pruebas de Mavix - Benitez María Agustina

*/

#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

#include "Juego.h"

int main() {

    {

        Juego juego;
        juego.Iniciar();

        while (!WindowShouldClose()) {

            juego.Actualizar();
            juego.Renderizar();

        }

    }

    CloseAudioDevice();
    CloseWindow();

    return 0;

}