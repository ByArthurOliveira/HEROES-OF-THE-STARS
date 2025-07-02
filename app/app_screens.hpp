#ifndef APP_SCREENS_HPP
#define APP_SCREENS_HPP

#include <stdio.h>
#include "raylib.h"
#include "app_controller.hpp"

// Função que desenha o menu principal do jogo
void DrawMainMenu(AppAssets app_assets, Player &player, PowerUP &power_up, char username[])
{
    // Desenha o fundo e o efeito de blur
    DrawTexture(app_assets.menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    // Título do jogo
    DrawTextEx(app_assets.font, "HEROES OF THE STARS", {83, 185}, 55, 1, WHITE);

    // Saudação personalizada com o nome do jogador
    DrawTextEx(app_assets.font, TextFormat("Welcome, %s!", username), {1075, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("Your record in this session: %d", player.score_record), {985, 55}, 20, 1, WHITE);

    // Verifica qual opção está selecionada no menu e desenha os textos correspondentes
    switch (selected_option)
    {
    case 1:
        // Se o jogador pressionar ENTER em "NEW GAME"
        if (IsKeyPressed(KEY_ENTER))
        {
            player = CreatePlayerBase(); // Cria o jogador
            power_up = CreatePowerUP();  // Cria o power-up inicial
            pause_app = false;
            current_app_state = GAMEPLAY; // Muda para o estado de jogo
        }
        // Destaque da opção "NEW GAME"
        DrawTextEx(app_assets.font, "NEW GAME", {93, 325}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;
    case 2:
        // Acessa a tela de placar
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
        // Acessa a tela de comandos
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
        // Acessa a tela de créditos
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
        // Sai do jogo
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

// Função que desenha todos os elementos do gameplay (jogador, lasers, asteroides, HUD)
void DrawGameplay(AppAssets app_assets, Player player, Laser lasers[], PowerUP power_up, AsteroidManager asteroid_manager)
{
    // Rolagem de fundo
    DrawTexture(app_assets.game_background, 0, (int)background_y0, LIGHTGRAY);
    DrawTexture(app_assets.game_background, 0, (int)background_y1, LIGHTGRAY);

    // Informações da dificuldade
    DrawDifficultyInfo(&asteroid_manager, app_assets.font);

    // Desenha o power-up se ainda não foi pego
    if (!power_up.was_catched)
    {
        DrawTexture(power_up.texture, power_up.position.x, power_up.position.y, WHITE);
    }

    // Desenha os asteroides
    DrawAsteroids(&asteroid_manager);

    // Desenha o jogador
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);

    // Se o boost estiver ativo, desenha os propulsores
    if (player.boost_active)
    {
        DrawTexture(player.boost_texture, player.boostleft_position.x, player.boostleft_position.y, WHITE);
        DrawTexture(player.boost_texture, player.boostright_position.x, player.boostright_position.y, WHITE);
    }

    // Desenha os lasers ativos
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (lasers[i].is_active)
            DrawTexture(lasers[i].texture, lasers[i].position.x, lasers[i].position.y, WHITE);
    }

    // HUD de score
    DrawTextEx(app_assets.font, "SCORE:", {1150, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("%i", player.score), {1240, 25}, 25, 1, GOLD);

    // HUD de vida
    DrawTexture(app_assets.health_counter, 25, 25, WHITE);
    DrawTexture(app_assets.X, 70, 30, WHITE);

    // Exibe número correspondente à vida restante
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

// Função que desenha a tela do placar com os 10 melhores scores
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
            // Remove o caractere '\n' do final da linha
            scoreboard_content[strcspn(scoreboard_content, "\n")] = 0;

            // Substitui ',' por ':' para melhor exibição
            for (int i = 0; scoreboard_content[i] != '\0'; i++)
            {
                if (scoreboard_content[i] == ',')
                {
                    scoreboard_content[i] = ':';
                }
            }

            // Desenha a linha no placar
            DrawTextEx(app_assets.font, TextFormat("%d - %s", line_count, scoreboard_content), (Vector2){305, (float)y_position}, 30, 1, line_count == 1 ? GOLD : WHITE);
            y_position += 50;
            line_count++;
        }

        fclose(file);
    }
}

// Função que desenha a tela de comandos com setas e botões
void DrawCommands(AppAssets app_assets, CommandAssets command_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTextEx(app_assets.font, "COMMANDS", {545, 75}, 40, 1, GOLD);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    // Controles de movimento
    DrawTextEx(app_assets.font, "MOVEMENTS", {150, 200}, 35, 1, GOLD);
    DrawTexture(command_assets.arrows[0], 200, 290, WHITE);
    DrawTexture(command_assets.arrows[1], 200, 340, WHITE);
    DrawTexture(command_assets.arrows[2], 150, 340, WHITE);
    DrawTexture(command_assets.arrows[3], 250, 340, WHITE);

    // Controles WASD
    DrawTexture(command_assets.letters[0], 200, 490, WHITE);
    DrawTexture(command_assets.letters[1], 150, 540, WHITE);
    DrawTexture(command_assets.letters[2], 200, 540, WHITE);
    DrawTexture(command_assets.letters[3], 250, 540, WHITE);

    // Ações e atalhos
    DrawTextEx(app_assets.font, "ACTIONS", {1075, 200}, 35, 1, GOLD);
    DrawTextEx(app_assets.font, "ATTACK:", {975, 275}, 35, 1, WHITE);
    DrawTexture(command_assets.attack, 1175, 262, WHITE);
    DrawTextEx(app_assets.font, "PAUSE:", {975, 375}, 35, 1, WHITE);
    DrawTexture(command_assets.pause, 1175, 362, WHITE);
    DrawTextEx(app_assets.font, "BOOST:", {975, 475}, 35, 1, WHITE);
    DrawTexture(command_assets.boost, 1175, 462, WHITE);
    DrawTextEx(app_assets.font, "CONFIRM:", {975, 575}, 35, 1, WHITE);
    DrawTexture(command_assets.confirm, 1175, 562, WHITE);
    DrawTextEx(app_assets.font, "MUTE:", {975, 675}, 35, 1, WHITE);
    DrawTexture(command_assets.mute, 1175, 662, WHITE);
}

// Tela de créditos do jogo
void DrawCredits(AppAssets app_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTextEx(app_assets.font, "CREDITS", {575, 75}, 40, 1, GOLD);
    DrawTextEx(app_assets.font, "This game was developed by:", {150, 200}, 35, 1, GOLD);

    // Créditos dos desenvolvedores
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

// Função para encerrar o jogo
bool ExitApp()
{
    return close_app = true;
}

#endif
