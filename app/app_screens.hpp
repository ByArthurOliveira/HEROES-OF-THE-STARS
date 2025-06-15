#include "raylib.h"
#include "app_controllers.hpp"
#include "app_imported_assets.hpp"

#include <stdio.h>

void DrawMainMenu(Texture2D main_menu_background, Sound switch_sound)
{
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
    switch (selected_option)
    {
    case 1:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = GAMEPLAY;
        }

        DrawText("NOVO JOGO", 75, 325, 20, GOLD);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 2:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = SCOREBOARD;
        }

        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, GOLD);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 3:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = COMMANDS;
        }

        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, GOLD);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 4:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = CREDITS;
        }

        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, GOLD);
        DrawText("SAIR DO JOGO", 75, 525, 20, WHITE);
        break;
    case 5:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = EXIT;
        }

        DrawText("NOVO JOGO", 75, 325, 20, WHITE);
        DrawText("TABELA DE PONTUAÇÃO", 75, 375, 20, WHITE);
        DrawText("COMANDOS", 75, 425, 20, WHITE);
        DrawText("CREDITS", 75, 475, 20, WHITE);
        DrawText("SAIR DO JOGO", 75, 525, 20, GOLD);
        break;
    }
}

void DrawGameplay(Texture2D gameplay_background, Player player, Laser lasers[], int lasers_limit)
{
    // Desenha o fundo do jogo
    DrawTexture(gameplay_background, 0, 0, GRAY);

    // Desenha a textura principal do jogador
    DrawTexture(player.player_texture, player.player_position_X, player.player_position_Y, WHITE);

    // Se o boost estiver ativo e o jogo não estiver pausado, desenha o boost
    if (IsKeyDown(KEY_LEFT_SHIFT) && !pause_app)
    {
        DrawTexture(player.player_boost_texture, player.player_boost_texture_X, player.player_boost_texture_Y, WHITE);
    }

    // Desenha todos os lasers ativos
    for (int i = 0; i < lasers_limit; i++)
    {
        if (lasers[i].is_active)
        {
            DrawTexture(lasers[i].laser_texture, lasers[i].laser_texture_X, lasers[i].laser_texture_Y, WHITE);
        }
    }
}

void DrawScoreboard(Texture2D main_menu_background)
{
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

void DrawCommands(Texture2D main_menu_background)
{
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

void DrawCredits(Texture2D main_menu_background)
{
    DrawTexture(main_menu_background, 0, 0, LIGHTGRAY);
}

bool ExitApp()
{
    stop_app = true;
    return stop_app;
}