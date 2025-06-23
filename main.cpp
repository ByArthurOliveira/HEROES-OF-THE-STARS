#define RAYGUI_IMPLEMENTATION

#include <stdio.h>
#include "raylib.h"
#include "raygui.h"
#include "app/app.hpp"

char input_text[MAX_INPUT_CHARS + 1] = "\0";
bool edit_mode = false;

int main()
{
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT);
    InitWindow(450, 250, "HEROES OF THE STARS");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Insert your username", 110, 35, 20, DARKGRAY);

        if (GuiTextBox((Rectangle){ 125, 75, 200, 40 }, input_text, MAX_INPUT_CHARS, edit_mode))
        {
            edit_mode = !edit_mode;
        }

        if ((GuiButton((Rectangle){ 125, 125, 200, 40 }, "LOGIN") || IsKeyPressed(KEY_ENTER)) && input_text[0] != '\0')
        {
            strncpy(username, strupr(input_text), MAX_INPUT_CHARS);
            CloseWindow();
            break;
        }

        EndDrawing();
    }

    InitializeApp(username);
    return 0;
}
