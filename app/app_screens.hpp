// Inclui as bibliotecas necessárias
#include "raylib.h"               // Biblioteca principal para gráficos e áudio
#include "app_controllers.hpp"     // Controladores de estado do aplicativo
#include "app_imported_assets.hpp" // Recursos importados (texturas, sons, etc.)

#include <stdio.h> // Biblioteca para operações de entrada e saída padrão

// Função para desenhar o menu principal
void DrawMainMenu(Texture2D main_menu_background, Sound switch_sound)
{
    // Desenha o fundo do menu principal
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);

    // Verifica qual opção está selecionada e desenha o menu correspondente
    switch (selected_option)
    {
    case 1:
        // Se a tecla Enter for pressionada, muda o estado do aplicativo para GAMEPLAY
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = GAMEPLAY;
        }

        // Desenha as opções do menu
        DrawText("NOVO JOGO", 75, 325, 20, GOLD);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 2:
        // Se a tecla Enter for pressionada, muda o estado do aplicativo para SCOREBOARD
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = SCOREBOARD;
        }

        // Desenha as opções do menu
        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, GOLD);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 3:
        // Se a tecla Enter for pressionada, muda o estado do aplicativo para COMMANDS
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = COMMANDS;
        }

        // Desenha as opções do menu
        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, GOLD);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 4:
        // Se a tecla Enter for pressionada, muda o estado do aplicativo para CREDITS
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = CREDITS;
        }

        // Desenha as opções do menu
        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, GOLD);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 5:
        // Se a tecla Enter for pressionada, muda o estado do aplicativo para EXIT
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = EXIT;
        }

        // Desenha as opções do menu
        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, GOLD);
        break;
    }
}

// Função para desenhar a tela de gameplay
void DrawGameplay(Texture2D gameplay_background, Player player, Laser lasers[], int lasers_limit)
{
    // Desenha o fundo do jogo
    // O fundo é desenhado no loop principal com bgY1/bgY2, então não é necessário aqui

    // Desenha o jogador
    DrawTexture(player.player_texture, player.player_position_X, player.player_position_Y, WHITE);

    // Se o jogador estiver usando o boost e o jogo não estiver pausado, desenha o boost
    if ((IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S)) && !pause_app)
        DrawTexture(player.player_boost_texture, player.player_boost_texture_X, player.player_boost_texture_Y, WHITE);

    // Desenha os lasers ativos
    for (int i = 0; i < lasers_limit; i++)
        if (lasers[i].is_active)
            DrawTexture(lasers[i].laser_texture, lasers[i].laser_texture_X, lasers[i].laser_texture_Y, WHITE);
}

// Função para desenhar a tela de scoreboard
void DrawScoreboard(Texture2D main_menu_background)
{
    // Desenha o fundo do menu principal
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

// Função para desenhar a tela de comandos
void DrawCommands(Texture2D main_menu_background)
{
    // Desenha o fundo do menu principal
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

// Função para desenhar a tela de créditos
void DrawCredits(Texture2D main_menu_background)
{
    // Desenha o fundo do menu principal
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

// Função para sair do aplicativo
bool ExitApp()
{
    // Define a variável stop_app como verdadeira
    stop_app = true;

    // Retorna o valor de stop_app
    return stop_app;
}
