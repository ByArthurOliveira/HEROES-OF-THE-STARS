#include "raylib.h"
#include "app/app_screens.hpp" // Inclui struct do Player e Laser

#include <stdio.h>

int InitializeApp()
{
    // === Inicialização da Janela ===
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(240);
    InitWindow(screen_width, screen_height, "HEROES OF THE STARS");
    InitAudioDevice();

    SetExitKey(KEY_NULL); // Impede fechamento com ESC

    // === Carregamento de recursos ===
    AppAssets app_assets = CreateAppAssets();

    Player player = CreatePlayer();

    // === Inicialização dos lasers ===
    Laser lasers[MAX_LASERS];
    Laser base_laser = CreateLaserBase(player); // Carrega textura e som apenas uma vez

    // Copia os dados do base_laser para todos os lasers do array
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = base_laser;
        lasers[i].is_active = false;
    }

    // === Loop Principal do Jogo ===
    while (!stop_app)
    {
        delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (current_app_state)
        {
        case MAIN_MENU:
            // Navegação no menu principal
            app_timer.time = GetTime();
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && selected_option > 1)
            {
                if (app_timer.time - app_timer.last_time > 0.225f)
                {
                    app_timer.last_time = app_timer.time;
                    PlaySound(app_assets.switch_sound);
                    selected_option--;
                }
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && selected_option < 5)
            {
                if (app_timer.time - app_timer.last_time > 0.225f)
                {
                    app_timer.last_time = app_timer.time;
                    PlaySound(app_assets.switch_sound);
                    selected_option++;
                }
            }

            DrawMainMenu(app_assets.main_menu_background, app_assets.switch_sound);
            break;

        case GAMEPLAY:

            if (IsKeyPressed(KEY_ESCAPE))
                pause_app = !pause_app;

            if (!pause_app)
            {
                // === Movimento do Jogador ===
                if (IsKeyDown(KEY_W) && player.player_position_Y > 0)
                    player.player_position_Y -= player.speed * delta_time;
                if (IsKeyDown(KEY_S) && player.player_position_Y < screen_height - player.player_texture.height)
                    player.player_position_Y += player.speed * delta_time;
                if (IsKeyDown(KEY_A) && player.player_position_X > 0)
                    player.player_position_X -= player.speed * delta_time;
                if (IsKeyDown(KEY_D) && player.player_position_X < screen_width - player.player_texture.width)
                    player.player_position_X += player.speed * delta_time;

                // === Disparo dos Lasers ===
                laser_timer.time = GetTime();
                if (IsKeyDown(KEY_SPACE) && laser_timer.time - laser_timer.last_time > 0.275f)
                {
                    for (int i = 0; i < MAX_LASERS; i++)
                    {
                        if (!lasers[i].is_active)
                        {
                            lasers[i].laser_texture_X = player.player_position_X + 50;
                            lasers[i].laser_texture_Y = player.player_position_Y - 90;
                            lasers[i].is_active = true;
                            PlaySound(lasers[i].laser_sound);
                            laser_timer.last_time = laser_timer.time;
                            break; // Só dispara um por vez
                        }
                    }
                }

                // === Atualização dos Lasers Ativos ===
                for (int i = 0; i < MAX_LASERS; i++)
                {
                    if (lasers[i].is_active)
                    {
                        lasers[i].laser_texture_Y -= lasers[i].laser_speed * delta_time;

                        // Se sair da tela, desativa
                        if (lasers[i].laser_texture_Y + lasers[i].laser_texture.height < 0)
                        {
                            lasers[i].is_active = false;
                        }
                    }
                }

                // === Boost de Velocidade ===
                if (IsKeyDown(KEY_LEFT_SHIFT))
                {
                    player.player_boost_texture_X = player.player_position_X + 50;
                    player.player_boost_texture_Y = player.player_position_Y + 90;
                    player.speed = 400.0f;
                }
                else
                {
                    player.speed = 250.0f;
                }

                // Desenha o fundo, jogador e todos os lasers ativos
                DrawGameplay(app_assets.gameplay_background, player, lasers, MAX_LASERS);
            }
            else
            {
                // Tela de pausa
                DrawGameplay(app_assets.gameplay_background, player, lasers, MAX_LASERS);
                DrawText("JOGO PAUSADO", 75, 400, 25, GOLD);
                DrawText("PRESSIONE ESC PARA CONTINUAR!", 75, 450, 25, GOLD);
            }
            break;

        case SCOREBOARD:
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawScoreboard(app_assets.main_menu_background);
            break;

        case COMMANDS:
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawCommands(app_assets.main_menu_background);
            break;

        case CREDITS:
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawCredits(app_assets.main_menu_background);
            break;

        case EXIT:
            ExitApp();
            break;
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    // === Liberação de recursos ===
    for (int i = 0; i < MAX_LASERS; i++)
    {
        // Somente descarregue uma vez se for a mesma textura/som para todos
        if (i == 0)
        {
            UnloadTexture(lasers[i].laser_texture);
            UnloadSound(lasers[i].laser_sound);
        }
    }

    UnloadTexture(player.player_boost_texture);
    UnloadTexture(player.player_texture);
    UnloadSound(app_assets.switch_sound);
    UnloadTexture(app_assets.gameplay_background);
    UnloadTexture(app_assets.main_menu_background);
    CloseWindow();

    return 0;
}