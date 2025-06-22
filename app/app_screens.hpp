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
void DrawMainMenu(Texture2D menu_background, Texture2D blur, Sound switch_sound, Font font)
{
    // Desenha o fundo do menu principal
    DrawTexture(menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(blur, 0, 0, LIGHTGRAY);
    DrawTextEx(font, "HEROES OF THE STARS", {83, 179}, 55, 1, WHITE);

    // Exibe as opções com destaque conforme a opção selecionada
    switch (selected_option)
    {
    case 1:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = GAMEPLAY;
        }
        DrawTextEx(font, "NEW GAME", {83, 325}, 25, 1, GOLD);
        DrawTextEx(font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 2:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = SCOREBOARD;
        }
        DrawTextEx(font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(font, "SCOREBOARD", {83, 375}, 25, 1, GOLD);
        DrawTextEx(font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 3:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = COMMANDS;
        }
        DrawTextEx(font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(font, "COMMANDS", {83, 425}, 25, 1, GOLD);
        DrawTextEx(font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 4:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = CREDITS;
        }
        DrawTextEx(font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(font, "CREDITS", {83, 475}, 25, 1, GOLD);
        DrawTextEx(font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 5:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = EXIT;
        }
        DrawTextEx(font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(font, "EXIT GAME", {83, 525}, 25, 1, GOLD);
        break;
    }
}

void DrawGameplay(AppAssets app_assets, Player player, Laser lasers[], PowerUP power_up, AsteroidManager asteroid_manager)
{
    DrawTexture(app_assets.game_background, 0, (int)bg_y0, GRAY);
    DrawTexture(app_assets.game_background, 0, (int)bg_y1, GRAY);

    DrawTextEx(app_assets.font, "SCORE:", {1150, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("%i", player.score), {1250, 25}, 25, 1, GOLD);

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

    DrawDifficultyInfo(&asteroid_manager, app_assets.font);

    if (!power_up.was_catched)
    {
        DrawTexture(power_up.texture, power_up.position.x, power_up.position.y, WHITE);
    }

    // Desenha os asteroides
    DrawAsteroids(&asteroid_manager);

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

    DrawText("CREDITS", 545, 100, 40, GOLD);

    DrawText("Desenvolvido por:", 305, 200, 30, WHITE);

    DrawText("Pedro Ricardo - Graduando em Ciência da Computação (Bacharelado)\nna Universidade Do Estado Do Rio Grande Do Norte (UERN)\nGithub: https://github.com/pedroricardo14", 305, 270, 25, WHITE);

    DrawText("Arthur Oliveira - Graduando em Ciência da Computação (Bacharelado)\nna Universidade Do Estado Do Rio Grande Do Norte (UERN)\nGithub: https://github.com/ByArthurito", 305, 370, 25, WHITE);

    DrawText("Francisco Genyson - Graduando em Ciência da Computação (Bacharelado)\nna Universidade Do Estado Do Rio Grande Do Norte (UERN)\nGithub: https://github.com/Extremois", 305, 470, 25, WHITE);

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