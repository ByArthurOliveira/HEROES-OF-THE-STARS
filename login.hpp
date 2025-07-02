// Ativa a implementação da biblioteca Raygui (deve ser definida uma única vez em um projeto)
#define RAYGUI_IMPLEMENTATION

// Bibliotecas padrão e externas
#include <stdio.h>
#include "raylib.h"
#include "raygui.h"

// Declaração de variáveis globais
char input_text[MAX_INPUT_CHARS + 1] = "\0"; // Armazena o texto inserido pelo usuário (com +1 para o caractere nulo)
bool edit_mode = false;                      // Indica se o modo de edição do textbox está ativado

Font font;                // Fonte personalizada a ser usada na interface
bool close_login = false; // Flag para controle de encerramento da janela de login

// Função Login - Exibe a interface de login do launcher
int Login()
{
    // Configura a janela para ficar sempre no topo
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_VSYNC_HINT);

    // Inicializa a janela com título e tamanho definidos
    InitWindow(450, 250, "H.O.T.S. - Launcher v1.0.0");
    SetExitKey(KEY_NULL);

    // Define a taxa de frames por segundo
    SetTargetFPS(60);

    // Loop principal da tela de login
    while (!close_login)
    {
        // Verifica se o usuário solicitou o fechamento da janela
        if (WindowShouldClose())
        {
            // Em vez de fechar, minimiza a janela
            MinimizeWindow();
        }

        // Início da renderização
        BeginDrawing();
        ClearBackground(RAYWHITE); // Define o fundo como branco

        // Desenha o texto informativo na tela
        DrawTextEx(font, "Insert your username", (Vector2){120, 35}, 20, 1, DARKGRAY);

        // Exibe a caixa de texto para o nome de usuário
        // Se for clicada, alterna o modo de edição
        if (GuiTextBox((Rectangle){125, 95, 200, 40}, input_text, MAX_INPUT_CHARS, edit_mode))
        {
            edit_mode = !edit_mode;
        }

        // Se o botão LOGIN for pressionado ou a tecla ENTER for usada
        // e o campo de texto não estiver vazio
        if ((GuiButton((Rectangle){125, 145, 200, 40}, "LOGIN") || IsKeyPressed(KEY_ENTER)) && input_text[0] != '\0')
        {
            // Converte o nome de usuário para maiúsculo e copia para a variável global "username"
            strncpy(username, strupr(input_text), MAX_INPUT_CHARS);

            // Define a flag para encerrar a tela de login
            close_login = true;
            break;
        }

        // Fim da renderização do frame
        EndDrawing();
    }

    // Encerra a janela após o login
    CloseWindow();

    return 0;
}
