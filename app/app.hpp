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

    // inicialização única do background
    if (!bgInitialized)
    {
        bgY1 = 0;
        bgY2 = -app_assets.gameplay_background.height;
        bgInitialized = true;
    }

    if (!pause_app)
    {
        // ===== 1) Logica de scroll do fundo =====
        bgY1 += BG_SCROLL_SPEED * delta_time;
        bgY2 += BG_SCROLL_SPEED * delta_time;
        if (bgY1 >= screen_height) bgY1 = bgY2 - app_assets.gameplay_background.height;
        if (bgY2 >= screen_height) bgY2 = bgY1 - app_assets.gameplay_background.height;

        // ===== 2) Movimento do Jogador =====
        if (IsKeyDown(KEY_W) && player.player_position_Y > 0)
            player.player_position_Y -= player.speed * delta_time;
        if (IsKeyDown(KEY_S) && player.player_position_Y < screen_height - player.player_texture.height)
            player.player_position_Y += player.speed * delta_time;
        if (IsKeyDown(KEY_A) && player.player_position_X > 0)
            player.player_position_X -= player.speed * delta_time;
        if (IsKeyDown(KEY_D) && player.player_position_X < screen_width - player.player_texture.width)
            player.player_position_X += player.speed * delta_time;

        // ===== 3) Disparo dos Lasers =====
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
                    break;
                }
            }
        }

        // Atualiza posição dos lasers ativos
        for (int i = 0; i < MAX_LASERS; i++)
        {
            if (lasers[i].is_active)
            {
                lasers[i].laser_texture_Y -= lasers[i].laser_speed * delta_time;
                if (lasers[i].laser_texture_Y + lasers[i].laser_texture.height < 0)
                    lasers[i].is_active = false;
            }
        }

        // ===== 4) Boost de Velocidade =====
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
    }

    // ===== 5) Desenho (único BeginDrawing/EndDrawing no loop principal) =====
    // no seu main loop: BeginDrawing(); ClearBackground(RAYWHITE);

    // Desenha o fundo em loop:
    DrawTexture(app_assets.gameplay_background, 0, (int)bgY1, WHITE);
    DrawTexture(app_assets.gameplay_background, 0, (int)bgY2, WHITE);

    // Desenha o jogador:
    DrawTexture(player.player_texture, player.player_position_X, player.player_position_Y, WHITE);

    // Desenha boost se ativo:
    if (IsKeyDown(KEY_LEFT_SHIFT) && !pause_app)
        DrawTexture(player.player_boost_texture, player.player_boost_texture_X, player.player_boost_texture_Y, WHITE);

    // Desenha lasers ativos:
    for (int i = 0; i < MAX_LASERS; i++)
        if (lasers[i].is_active)
            DrawTexture(lasers[i].laser_texture, lasers[i].laser_texture_X, lasers[i].laser_texture_Y, WHITE);

    // Texto de pausa, se necessário:
    if (pause_app)
    {
        DrawText("JOGO PAUSADO", 75, 400, 25, GOLD);
        DrawText("PRESSIONE ESC PARA CONTINUAR!", 75, 450, 25, GOLD);
    }
    // no seu main loop: EndDrawing();
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