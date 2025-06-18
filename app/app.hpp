#ifndef APP_HPP
#define APP_HPP

#include <stdio.h>
#include "raylib.h"
#include "app/app_screens.hpp"

//-----------------------------------------------------------------
// Função: InitializeApp
// Descrição: Inicializa as configurações, os ativos e executa o loop principal do jogo.
//-----------------------------------------------------------------
int InitializeApp()
{
    //-----------------------------------------------------------
    // Configurações Iniciais: Janela, Áudio e Flags
    //-----------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST | FLAG_MSAA_4X_HINT);
    SetTargetFPS(360);
    InitWindow(screen_width, screen_height, "HEROES OF THE STARS");
    InitAudioDevice();
    SetExitKey(KEY_NULL); // Desativa a tecla de saída automática

    //-----------------------------------------------------------
    // Carregamento de Ativos e Inicialização dos Objetos do Jogo

    AppAssets app_assets = CreateAppAssets();
    //-----------------------------------------------------------

    Player player = CreatePlayer();

    PowerUP power_up = CreatePowerUP();

    // Inicializa o array de lasers com o laser base
    Laser base_laser = CreateLaserBase(player);
    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = base_laser;
        lasers[i].is_active = false;
    }

    // Configura volumes da música e dos efeitos sonoros
    SetMusicVolume(app_assets.menu_music_theme, 0.25);
    SetSoundVolume(app_assets.switch_option_sound, 0.50);
    // Inicia a música de fundo
    PlayMusicStream(app_assets.menu_music_theme);
    //-----------------------------------------------------------
    // Loop Principal do Jogo
    //-----------------------------------------------------------
    while (!stop_app)
    {
        // Atualiza o tempo de frame
        frametime = GetFrameTime();
        // Início do desenho da tela
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //-----------------------------------------------------------
        // Gerenciamento dos Estados da Aplicação
        //-----------------------------------------------------------
        switch (current_app_state)
        {
        // Estado: Menu Principal
        case MAIN_MENU:
        {
            // Controle do áudio no menu
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            app_timer.real_time = GetTime();

            // Navegação do menu principal (opções)
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && selected_option > 1)
            {
                if (app_timer.real_time - app_timer.last_time > 0.225f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option--;
                    app_timer.last_time = app_timer.real_time;
                }
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && selected_option < 5)
            {
                if (app_timer.real_time - app_timer.last_time > 0.225f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option++;
                    app_timer.last_time = app_timer.real_time;
                }
            }

            DrawMainMenu(app_assets.menu_background, app_assets.blur, app_assets.switch_option_sound);
            break;
        }

        // Estado: Modo de Jogo (Gameplay)
        case GAMEPLAY:
        {
            // Alterna o modo pausa do jogo
            if (IsKeyPressed(KEY_ESCAPE))
                pause_app = !pause_app;

            // Inicializa os valores do fundo, se ainda não inicializado
            if (!bg_initialized)
            {
                bg_y1 = 0;
                bg_y2 = -app_assets.game_background.height;
                bg_initialized = true;
            }

            // Atualiza lógicas do jogo apenas se não estiver em pausa
            if (!pause_app)
            {
                // Atualiza a pontuação do jogador a cada segundo
                score_verifier.real_time = GetTime();
                if (score_verifier.real_time - score_verifier.last_time > 1)
                {
                    player.score += 1;
                    score_verifier.last_time = score_verifier.real_time;
                }

                // Rolagem do background
                bg_y1 += BG_SCROLL_SPEED * frametime;
                bg_y2 += BG_SCROLL_SPEED * frametime;

                if (bg_y1 >= screen_height)
                    bg_y1 = bg_y2 - app_assets.game_background.height;
                if (bg_y2 >= screen_height)
                    bg_y2 = bg_y1 - app_assets.game_background.height;

                // Movimentação do jogador
                if (IsKeyDown(KEY_W) && player.position.y > 0)
                    player.position.y -= player.speed * frametime;
                if (IsKeyDown(KEY_S) && player.position.y < screen_height - player.texture.height)
                    player.position.y += player.speed * frametime;
                if (IsKeyDown(KEY_A) && player.position.x > 0)
                    player.position.x -= player.speed * frametime;
                if (IsKeyDown(KEY_D) && player.position.x < screen_width - player.texture.width)
                    player.position.x += player.speed * frametime;

                // Verifica e aplica o boost do jogador
                if (IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S))
                {
                    player.boost_active = true;
                    player.boostleft_position = {player.position.x + 25, player.position.y + 70};
                    player.boostright_position = {player.position.x + 75, player.position.y + 70};
                    player.speed = 500;
                }
                else
                {
                    player.boost_active = false;
                    player.speed = 250;
                }
                player.hit_box = {player.position.x, player.position.y, float(player.texture.width), float(player.texture.height)};

                if (CheckCollisionRecs(player.hit_box, power_up.hit_box))
                {
                    power_up.was_catched = true;
                    power_up.time_remaining = 10;
                    power_up.position = {
                        float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))), float(-power_up.texture.height)};
                    power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};
                }
                if (power_up.was_catched == false && power_up.hit_box.y > float(screen_height))
                {
                    power_up_respawn.real_time = GetTime();
                    if (power_up_respawn.real_time - power_up_respawn.last_time > 35)
                    {
                        power_up.position = {
                            float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))), float(-power_up.texture.height)};
                        power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};
                        power_up_respawn.last_time = power_up_respawn.real_time;
                    }
                }
                power_up.position.y += power_up.fall_speed * frametime;
                power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};

                power_up_timer.real_time = GetTime();
                if (power_up_timer.real_time - power_up_timer.last_time > 1)
                {
                    power_up.time_remaining -= 1;
                    power_up_timer.last_time = power_up_timer.real_time;
                }
                if (power_up.time_remaining <= 0)
                {
                    power_up.was_catched = false;
                }

                if (power_up.was_catched)
                {
                    lasers->interval = 0.1;
                }
                else
                {
                    lasers->interval = base_laser.interval;
                }

                // Disparo dos lasers
                laser_timer.real_time = GetTime();
                if (IsKeyDown(KEY_SPACE) && laser_timer.real_time - laser_timer.last_time > lasers->interval)
                {
                    for (int i = 0; i < MAX_LASERS; i++)
                    {
                        if (!lasers[i].is_active)
                        {
                            lasers[i].position.x = player.position.x + 50;
                            lasers[i].position.y = player.position.y - 50;
                            lasers[i].is_active = true;
                            PlaySound(lasers[i].sound);
                            laser_timer.last_time = laser_timer.real_time;
                            break;
                        }
                    }
                }
                // Atualiza a posição dos lasers ativos
                for (int i = 0; i < MAX_LASERS; i++)
                {
                    if (lasers[i].is_active)
                    {
                        lasers[i].position.y -= lasers[i].speed * frametime;
                        if (lasers[i].position.y + lasers[i].texture.height < 0)
                            lasers[i].is_active = false;
                    }
                }
            }

            DrawGameplay(app_assets, player, lasers, power_up);
            if (pause_app)
            {
                DrawText("JOGO PAUSADO", 75, 400, 25, GOLD);
                DrawText("PRESSIONE ESC PARA CONTINUAR!", 75, 450, 25, GOLD);
            }
            break;
        }

        // Estado: Placar
        case SCOREBOARD:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawScoreboard(app_assets.menu_background, app_assets.blur);
            break;
        }

        // Estado: Comandos
        case COMMANDS:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawCommands(app_assets.menu_background, app_assets.blur);
            break;
        }

        // Estado: Créditos
        case CREDITS:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawCredits(app_assets.menu_background, app_assets.blur);
            break;
        }

        // Estado: Sair da Aplicação
        case EXIT:
        {
            ExitApp();
            break;
        }
        } // Fim do switch de estados

        // Exibe os FPS no canto da tela
        DrawFPS(5, 5);
        EndDrawing();
    } // Fim do loop principal do jogo

    //-----------------------------------------------------------
    // Limpeza e Liberação de Recursos
    //-----------------------------------------------------------
    UnloadTexture(lasers->texture);
    UnloadSound(lasers->sound);
    UnloadTexture(player.boost_texture);
    UnloadTexture(player.texture);
    UnloadSound(app_assets.switch_option_sound);
    UnloadTexture(app_assets.game_background);
    UnloadTexture(app_assets.menu_background);
    CloseWindow();
    return 0;
}
#endif // APP_HPP