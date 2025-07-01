#ifndef APP_SCREENS_HPP
#define APP_SCREENS_HPP

#include <stdio.h>
#include "raylib.h"
#include "app_controller.hpp"

void DrawMainMenu(AppAssets app_assets, Player &player, PowerUP &power_up, char username[])
{
    DrawTexture(app_assets.menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
    DrawTextEx(app_assets.font, "HEROES OF THE STARS", {83, 185}, 55, 1, WHITE);
    DrawTextEx(app_assets.font, TextFormat("Welcome, %s!", username), {1075, 25}, 25, 1, GOLD);

    switch (selected_option)
    {
    case 1:
        if (IsKeyPressed(KEY_ENTER))
        {
            player = CreatePlayerBase();
            power_up = CreatePowerUP();
            pause_app = false;
            current_app_state = GAMEPLAY;
        }
        DrawTextEx(app_assets.font, "NEW GAME", {93, 325}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 2:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = SCOREBOARD;
        }
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {93, 375}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 3:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = COMMANDS;
        }
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {93, 425}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 4:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = CREDITS;
        }
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {93, 475}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 5:
        if (IsKeyPressed(KEY_ENTER))
        {
            current_app_state = EXIT;
        }
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {93, 525}, 25, 1, GOLD);
        break;
    }
}

void DrawGameplay(AppAssets app_assets, Player player, Laser lasers[], PowerUP power_up, AsteroidManager asteroid_manager)
{
    DrawTexture(app_assets.game_background, 0, (int)background_y0, LIGHTGRAY);
    DrawTexture(app_assets.game_background, 0, (int)background_y1, LIGHTGRAY);

    DrawDifficultyInfo(&asteroid_manager, app_assets.font);
    if (!power_up.was_catched)
    {
        DrawTexture(power_up.texture, power_up.position.x, power_up.position.y, WHITE);
    }
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

    DrawTextEx(app_assets.font, "SCORE:", {1150, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("%i", player.score), {1240, 25}, 25, 1, GOLD);

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
        break;
    }
}

void DrawScoreboard(AppAssets app_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    DrawTextEx(app_assets.font, "SCOREBOARD", {505, 75}, 40, 1, GOLD);

    char scoreboard_content[256];
    FILE *file = fopen("db/scoreboard.csv", "r");

    if (file)
    {
        int y_position = 200;
        int line_count = 1;
        while (fgets(scoreboard_content, sizeof(scoreboard_content), file) && line_count <= 10)
        {
            // Remove o caractere de nova linha
            scoreboard_content[strcspn(scoreboard_content, "\n")] = 0;

            // Substituir vírgulas por dois-pontos
            for (int i = 0; scoreboard_content[i] != '\0'; i++)
            {
                if (scoreboard_content[i] == ',')
                {
                    scoreboard_content[i] = ':';
                }
            }

            DrawTextEx(app_assets.font, TextFormat("%d - %s", line_count, scoreboard_content), (Vector2){305, (float)y_position}, 30, 1, line_count == 1 ? GOLD : WHITE);
            y_position += 50;
            line_count++;
        }
        fclose(file);
    }
}

void DrawCommands(AppAssets app_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);

    DrawTextEx(app_assets.font, "COMMANDS", {545, 75}, 40, 1, GOLD);

    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
}

void DrawCredits(AppAssets app_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTextEx(app_assets.font, "CREDITS", {575, 75}, 40, 1, GOLD);

    DrawTextEx(app_assets.font, "This game was developed by:", {150, 200}, 35, 1, GOLD);

    DrawTextEx(app_assets.font,
               "Arthur Oliveira - Undergraduate Student in Computer Science (Bachelor's Degree)\n"
               "State University of Rio Grande do Norte (Universidade do Estado do Rio Grande do Norte - UERN).\n",
               {150, 270}, 21, 1, WHITE);

    DrawTextEx(app_assets.font,
               "Francisco Genyson - Undergraduate Student in Computer Science (Bachelor's Degree)\n"
               "State University of Rio Grande do Norte (Universidade do Estado do Rio Grande do Norte - UERN).\n",
               {150, 370}, 21, 1, WHITE);

    DrawTextEx(app_assets.font,
               "Pedro Ricardo - Undergraduate Student in Computer Science (Bachelor's Degree)\n"
               "State University of Rio Grande do Norte (Universidade do Estado do Rio Grande do Norte - UERN).\n",
               {150, 470}, 21, 1, WHITE);

    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
}

bool ExitApp()
{
    return close_app = true;
}

#endif
