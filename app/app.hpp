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
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_MSAA_4X_HINT);
    SetTargetFPS(240);
    InitWindow(1366, 768, "HEROES OF THE STARS");
    InitAudioDevice();
    SetExitKey(KEY_NULL); // Desativa a tecla de saída automática

    SetRandomSeed((unsigned int)GetTime());

    //-----------------------------------------------------------
    // Carregamento de Ativos e Inicialização dos Objetos do Jogo
    AppAssets app_assets = CreateAppAssets();
    //-----------------------------------------------------------
    Player player = CreatePlayer();

    Laser laser_template = CreateLaserBase(player);
    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = laser_template;
        lasers[i].is_active = false;
    }

    PowerUP power_up = CreatePowerUP();

    // Inicializa o gerenciador de asteroides
    AsteroidManager asteroid_manager = CreateAsteroidManager();

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

            DrawMainMenu(app_assets.menu_background, app_assets.blur, app_assets.switch_option_sound, app_assets.font);
            break;
        }

        // Estado: Modo de Jogo (Gameplay)
        case GAMEPLAY:
        {
            // Alterna o modo pausa do jogo
            if (IsKeyPressed(KEY_ESCAPE))
                pause_app = !pause_app;

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
                UpdateGameBackground(app_assets.game_background, bg_y0, bg_y1, BG_SCROLL_SPEED, bg_initialized);

                // Movimentação do jogador
                UpdatePlayer(player);

                // Sistema de PowerUP
                CheckPowerUPCollision(power_up, player);
                if (!power_up.is_on_screen)
                {
                    int chance = GetRandomValue(0, 4800); // 0.1% de chance por frame
                    if (chance == 0)
                    {
                        power_up = CreatePowerUP();
                        power_up.is_on_screen = true;
                    }
                }
                else
                {
                    UpdatePowerUP(power_up);
                    if (power_up.position.y > GetScreenHeight() + power_up.texture.height)
                    {
                        power_up.is_on_screen = false;
                    }
                }

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
                SetLaserInterval(power_up, lasers);

                // Disparo dos lasers
                laser_timer.real_time = GetTime();
                if (IsKeyDown(KEY_SPACE) && laser_timer.real_time - laser_timer.last_time > lasers->interval)
                {
                    SpawnLaser(lasers, player);
                    laser_timer.last_time = laser_timer.real_time;
                }
                UpdateLaser(lasers);

                // Atualiza sistema de asteroides
                int points_lost = UpdateAsteroids(&asteroid_manager, GetFrameTime(), GetScreenWidth(), GetScreenHeight());
                player.score -= points_lost; // Subtrai pontos perdidos
                if (player.score < 0)
                    player.score = 0;

                // Verifica colisões laser-asteroide
                int asteroid_score = CheckAsteroidLaserCollisions(&asteroid_manager, lasers, MAX_LASERS);
                player.score += asteroid_score;

                // Verifica colisões jogador-asteroide
                if (CheckAsteroidPlayerCollision(&asteroid_manager, player.hit_box))
                {
                    player.health--;
                    if (player.health <= 0)
                    {
                        // Game Over - volta para o menu
                        // GAMEOVER == adicionar aqui para salvar o placar do player
                        current_app_state = MAIN_MENU;
                        // Reset do jogo
                        player.health = 3;
                        player.score = 0;
                        player.position = {683, 600};
                        // Limpa asteroides
                        ResetAsteroidManager(&asteroid_manager);
                    }
                }
            }

            DrawGameplay(app_assets, player, lasers, power_up, asteroid_manager);
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
    UnloadAsteroidManager(&asteroid_manager);
    CloseWindow();
    return 0;
}
#endif // APP_HPP