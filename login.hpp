#define RAYGUI_IMPLEMENTATION
#include <stdio.h>
#include "raylib.h"
#include "raygui.h"

char input_text[MAX_INPUT_CHARS + 1] = "\0";
bool edit_mode = false;

Font font;

bool close_login = false;
int Login()
{

    SetConfigFlags(FLAG_WINDOW_TOPMOST);
    InitWindow(450, 250, "H.O.T.S. - Launcher v1.0.0");
    SetTargetFPS(75);

    while (!close_login)
    {
        if (WindowShouldClose())
        {
            MinimizeWindow();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(font, "Insert your username", {120, 35}, 20, 1, DARKGRAY);

        if (GuiTextBox((Rectangle){125, 95, 200, 40}, input_text, MAX_INPUT_CHARS, edit_mode))
        {
            edit_mode = !edit_mode;
        }

        if ((GuiButton((Rectangle){125, 145, 200, 40}, "LOGIN") || IsKeyPressed(KEY_ENTER)) && input_text[0] != '\0')
        {
            strncpy(username, strupr(input_text), MAX_INPUT_CHARS);
            close_login = true;
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
