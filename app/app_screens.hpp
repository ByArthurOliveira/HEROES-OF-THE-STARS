#ifndef APP_SCREENS_HPP
#define APP_SCREENS_HPP

#include <stdio.h>
#include "raylib.h"
#include "app_controller.hpp"

// -----------------------------------------------------
// Função que desenha o menu principal do jogo
// -----------------------------------------------------
void DrawMainMenu(AppAssets app_assets, Player &player, PowerUP &power_up, char username[])
{
    // Desenha o fundo e o efeito de blur
    DrawTexture(app_assets.menu_background, 0, 0, LIGHTGRAY);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    // Título do jogo
    DrawTextEx(app_assets.font, "HEROES OF THE STARS", {83, 185}, 55, 1, WHITE);

    // Saudação personalizada com o nome do jogador e recorde da sessão
    DrawTextEx(app_assets.font, TextFormat("Welcome, %s!", username), {1075, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("Your record in this session: %d", player.score_record), {985, 55}, 20, 1, WHITE);

    // Verifica qual opção está selecionada e destaca a opção no menu
    switch (selected_option)
    {
    case 1:
        // Se ENTER for pressionado em "NEW GAME"
        if (IsKeyPressed(KEY_ENTER))
        {
            player = ResetPlayer(player);   // Reseta jogador
            power_up = CreatePowerUP();     // Cria power-up inicial
            pause_app = false;
            current_app_state = GAMEPLAY;   // Inicia gameplay
        }
        DrawTextEx(app_assets.font, "NEW GAME", {93, 325}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;

    case 2:
        // Se ENTER for pressionado em "SCOREBOARD"
        if (IsKeyPressed(KEY_ENTER))
            current_app_state = SCOREBOARD;
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {93, 375}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;

    case 3:
        // Se ENTER for pressionado em "COMMANDS"
        if (IsKeyPressed(KEY_ENTER))
            current_app_state = COMMANDS;
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {93, 425}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;

    case 4:
        // Se ENTER for pressionado em "CREDITS"
        if (IsKeyPressed(KEY_ENTER))
            current_app_state = CREDITS;
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {93, 475}, 25, 1, GOLD);
        DrawTextEx(app_assets.font, "EXIT GAME", {83, 525}, 25, 1, WHITE);
        break;

    case 5:
        // Se ENTER for pressionado em "EXIT GAME"
        if (IsKeyPressed(KEY_ENTER))
            current_app_state = EXIT;
        DrawTextEx(app_assets.font, "NEW GAME", {83, 325}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "SCOREBOARD", {83, 375}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "COMMANDS", {83, 425}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "CREDITS", {83, 475}, 25, 1, WHITE);
        DrawTextEx(app_assets.font, "EXIT GAME", {93, 525}, 25, 1, GOLD);
        break;
    }
}

// -----------------------------------------------------
// Função que desenha todos os elementos do gameplay:
// jogador, lasers, asteroides e HUD
// -----------------------------------------------------
void DrawGameplay(AppAssets app_assets, Player player, Laser lasers[], PowerUP power_up, AsteroidManager asteroid_manager)
{
    // Desenha o fundo em movimento (scroll)
    DrawTexture(app_assets.game_background, 0, (int)background_y0, LIGHTGRAY);
    DrawTexture(app_assets.game_background, 0, (int)background_y1, LIGHTGRAY);

    // Exibe informações sobre a dificuldade atual
    DrawDifficultyInfo(&asteroid_manager, app_assets.font);

    // Desenha o power-up na tela, se ainda não foi coletado
    if (!power_up.was_catched)
        DrawTexture(power_up.texture, power_up.position.x, power_up.position.y, WHITE);

    // Desenha a caixa de colisão do power-up
    DrawRectangleLinesEx(power_up.hit_box, 2, RED);

    // Desenha todos os asteroides
    DrawAsteroids(&asteroid_manager);

    // Desenha o jogador
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);

    // Desenha os propulsores se o boost estiver ativo
    if (player.boost_active)
    {
        DrawTexture(player.boost_texture, player.boostleft_position.x, player.boostleft_position.y, WHITE);
        DrawTexture(player.boost_texture, player.boostright_position.x, player.boostright_position.y, WHITE);
    }

    // Desenha os lasers ativos do jogador
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (lasers[i].is_active)
            DrawTexture(lasers[i].texture, lasers[i].position.x, lasers[i].position.y, WHITE);
    }

    // Desenha HUD: pontuação
    DrawTextEx(app_assets.font, "SCORE:", {1150, 25}, 25, 1, GOLD);
    DrawTextEx(app_assets.font, TextFormat("%i", player.score), {1240, 25}, 25, 1, GOLD);

    // Desenha HUD: indicador de vida
    DrawTexture(app_assets.health_counter, 25, 25, WHITE);
    DrawTexture(app_assets.X, 70, 30, WHITE);

    // Desenha número correspondente à vida restante
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

// -----------------------------------------------------
// Função que desenha a tela do placar com os 10 melhores scores
// -----------------------------------------------------
void DrawScoreboard(AppAssets app_assets)
{
    // Fundo do placar
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    // Título do placar
    DrawTextEx(app_assets.font, "SCOREBOARD", {505, 75}, 40, 1, GOLD);

    char scoreboard_content[256];
    FILE *file = fopen("db/scoreboard.csv", "r");

    if (file)
    {
        int y_position = 200;
        int line_count = 1;

        // Lê até 10 linhas do arquivo de placar
        while (fgets(scoreboard_content, sizeof(scoreboard_content), file) && line_count <= 10)
        {
            // Remove o '\n' do final da linha
            scoreboard_content[strcspn(scoreboard_content, "\n")] = 0;

            // Substitui ',' por ':' para melhor leitura
            for (int i = 0; scoreboard_content[i] != '\0'; i++)
            {
                if (scoreboard_content[i] == ',')
                    scoreboard_content[i] = ':';
            }

            // Desenha a linha do placar
            DrawTextEx(app_assets.font,
                       TextFormat("%d - %s", line_count, scoreboard_content),
                       (Vector2){305, (float)y_position},
                       30, 1,
                       line_count == 1 ? GOLD : WHITE);

            y_position += 50;
            line_count++;
        }

        fclose(file);
    }
}

// -----------------------------------------------------
// Função que desenha a tela de comandos com setas e botões
// -----------------------------------------------------
void DrawCommands(AppAssets app_assets, CommandAssets command_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);
    DrawTextEx(app_assets.font, "COMMANDS", {545, 75}, 40, 1, GOLD);
    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);

    // Seção de movimentos
    DrawTextEx(app_assets.font, "MOVEMENTS:", {955, 200}, 35, 1, WHITE);
    DrawTexture(command_assets.arrows[0], 1025, 290, WHITE);
    DrawTexture(command_assets.arrows[1], 1025, 340, WHITE);
    DrawTexture(command_assets.arrows[2], 975, 340, WHITE);
    DrawTexture(command_assets.arrows[3], 1075, 340, WHITE);

    // Controles WASD
    DrawTexture(command_assets.letters[0], 1025, 490, WHITE);
    DrawTexture(command_assets.letters[1], 975, 540, WHITE);
    DrawTexture(command_assets.letters[2], 1025, 540, WHITE);
    DrawTexture(command_assets.letters[3], 1075, 540, WHITE);

    // Ações e atalhos
    DrawTextEx(app_assets.font, "ATTACK:", {150, 200}, 35, 1, WHITE);
    DrawTexture(command_assets.attack, 325, 200 - 10, WHITE);
    DrawTextEx(app_assets.font, "BOOST:", {150, 300}, 35, 1, WHITE);
    DrawTexture(command_assets.boost, 325, 300 - 10, WHITE);
    DrawTextEx(app_assets.font, "CONFIRM:", {150, 400}, 35, 1, WHITE);
    DrawTexture(command_assets.confirm, 325, 400 - 10, WHITE);
    DrawTextEx(app_assets.font, "PAUSE:", {150, 500}, 35, 1, WHITE);
    DrawTexture(command_assets.pause, 325, 500 - 10, WHITE);
    DrawTextEx(app_assets.font, "MUTE:", {150, 600}, 35, 1, WHITE);
    DrawTexture(command_assets.mute, 325, 600 - 10, WHITE);
}

// -----------------------------------------------------
// Tela de créditos do jogo
// -----------------------------------------------------
void DrawCredits(AppAssets app_assets)
{
    DrawTexture(app_assets.menu_background, 0, 0, DARKGRAY);

    // Título dos créditos
    DrawTextEx(app_assets.font, "CREDITS", {575, 75}, 40, 1, GOLD);
    DrawTextEx(app_assets.font, "This game was developed by:", {150, 200}, 35, 1, GOLD);

    // Informações dos desenvolvedores
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

// -----------------------------------------------------
// Função para encerrar o jogo, sinalizando para sair do loop principal
// -----------------------------------------------------
bool ExitApp()
{
    return close_app = true;
}

#endif
