#ifndef APP_SCREENS_HPP
#define APP_SCREENS_HPP

#include <stdio.h>
#include "raylib.h"
#include "app_controller.hpp"

//-----------------------------------------------------------------
// Função: DrawMainMenu
// Descrição: Desenha o menu principal na tela com as opções
// e trata a entrada do usuário para navegar entre elas.
//-----------------------------------------------------------------
void DrawMainMenu(Texture2D menu_background, Texture2D blur, Sound switch_sound)
{
    // Desenha o fundo do menu principal
    DrawTexture(menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(blur, 0, 0, LIGHTGRAY);

    // Exibe as opções com destaque conforme a opção selecionada
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

void DrawGameplay(AppAssets app_assets, Player player, Laser lasers[], PowerUP power_up)
{
    DrawTexture(app_assets.game_background, 0, (int)bg_y1, GRAY);
    DrawTexture(app_assets.game_background, 0, (int)bg_y2, GRAY);

    DrawText("SCORE:", 1150, 25, 25, GOLD);
    DrawText(TextFormat("%i", player.score), 1250, 25, 25, GOLD);

    DrawTexture(app_assets.health_counter, 25, 25, WHITE);
    DrawTexture(app_assets._X, 70, 30, WHITE);
    switch (player.health)
    {
    case 0:
        DrawTexture(app_assets.numbers[0], 95, 30, WHITE);
        break;
    case 1:
        DrawTexture(app_assets.numbers[1], 95, 30, WHITE);
        break;
    case 2:
        DrawTexture(app_assets.numbers[2], 95, 30, WHITE);
        break;
    case 3:
        DrawTexture(app_assets.numbers[3], 95, 30, WHITE);
    }

    if(!power_up.was_catched){
        DrawTexture(power_up.texture, power_up.position.x, power_up.position.y, WHITE);
    }
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
    if (player.boost_active)
    {
        DrawTexture(player.boost_texture, player.boostleft_position.x, player.boostleft_position.y, WHITE);
        DrawTexture(player.boost_texture, player.boostright_position.x, player.boostright_position.y, WHITE);
    }

    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (lasers[i].is_active)
            DrawTexture(lasers[i].texture, lasers[i].position.x, lasers[i].position.y, WHITE);
    }
}
//-----------------------------------------------------------------
// Função: DrawScoreboard
// Descrição: Renderiza a tela do placar utilizando o background principal.
//-----------------------------------------------------------------
void DrawScoreboard(Texture2D menu_background, Texture2D blur)
{
    DrawTexture(menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(blur, 0, 0, LIGHTGRAY);
}
//-----------------------------------------------------------------
// Função: DrawCommands
// Descrição: Renderiza a tela que exibe os comandos do jogo.
//-----------------------------------------------------------------
void DrawCommands(Texture2D menu_background, Texture2D blur)
{
    DrawTexture(menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(blur, 0, 0, LIGHTGRAY);
}
//-----------------------------------------------------------------
// Função: DrawCredits
// Descrição: Renderiza a tela com os créditos do jogo.
//-----------------------------------------------------------------
void DrawCredits(Texture2D menu_background, Texture2D blur)
{
    DrawTexture(menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(blur, 0, 0, LIGHTGRAY);
}
//-----------------------------------------------------------------
// Função: ExitApp
// Descrição: Sinaliza a saída da aplicação definindo a flag
// de parada para true.
//-----------------------------------------------------------------
bool ExitApp()
{
    return stop_app = true;
}
#endif // APP_SCREENS_HPP