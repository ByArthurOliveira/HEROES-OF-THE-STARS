#ifndef APP_HPP
#define APP_HPP

#include "raylib.h"
#include "app/app_screens.hpp"
#include <stdio.h>

//-----------------------------------------------------------------
// Função: InitializeApp
// Descrição: Inicializa as configurações, os ativos e executa o loop principal do jogo.
//-----------------------------------------------------------------
int InitializeApp()
{
    //-----------------------------------------------------------
    // Configurações Iniciais: Janela, Áudio e Flags
    //-----------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST);
    SetTargetFPS(240);
    InitWindow(screen_width, screen_height, "HEROES OF THE STARS");
    InitAudioDevice();
    SetExitKey(KEY_NULL); // Desativa a tecla de saída automática

    //-----------------------------------------------------------
    // Carregamento de Ativos e Inicialização dos Objetos do Jogo
    //-----------------------------------------------------------
    AppAssets app_assets = CreateAppAssets();
    Player player = CreatePlayer();

    // Inicializa o array de lasers com o laser base
    Laser lasers[MAX_LASERS];
    Laser base_laser = CreateLaserBase(player);
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = base_laser;
        lasers[i].is_active = false;
    }

    // Configura volumes da música e dos efeitos sonoros
    SetMusicVolume(app_assets.music_theme, 0.25);
    SetSoundVolume(app_assets.switch_sound, 0.50);

    // Inicia a música de fundo
    PlayMusicStream(app_assets.music_theme);

    //-----------------------------------------------------------
    // Loop Principal do Jogo
    //-----------------------------------------------------------
    while (!stop_app)
    {
        // Atualiza o tempo de frame
        delta_time = GetFrameTime();

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
                PauseMusicStream(app_assets.music_theme);
            else
                ResumeMusicStream(app_assets.music_theme);
            UpdateMusicStream(app_assets.music_theme);

            app_timer.time = GetTime();

            // Navegação do menu principal (opções)
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
                bg_1y = 0;
                bg_2y = -app_assets.gameplay_background.height;
                bg_initialized = true;
            }

            // Atualiza lógicas do jogo apenas se não estiver em pausa
            if (!pause_app)
            {
                // Atualiza a pontuação do jogador a cada segundo
                score_verifier.time = GetTime();
                if (score_verifier.time - score_verifier.last_time > 1)
                {
                    player.score += 1;
                    score_verifier.last_time = score_verifier.time;
                }

                // Rolagem do background
                bg_1y += BG_SCROLL_SPEED * delta_time;
                bg_2y += BG_SCROLL_SPEED * delta_time;

                if (bg_1y >= screen_height)
                    bg_1y = bg_2y - app_assets.gameplay_background.height;
                if (bg_2y >= screen_height)
                    bg_2y = bg_1y - app_assets.gameplay_background.height;

                // Movimentação do jogador
                if (IsKeyDown(KEY_W) && player.player_position_Y > 0)
                    player.player_position_Y -= player.speed * delta_time;
                if (IsKeyDown(KEY_S) && player.player_position_Y < screen_height - player.player_texture.height)
                    player.player_position_Y += player.speed * delta_time;
                if (IsKeyDown(KEY_A) && player.player_position_X > 0)
                    player.player_position_X -= player.speed * delta_time;
                if (IsKeyDown(KEY_D) && player.player_position_X < screen_width - player.player_texture.width)
                    player.player_position_X += player.speed * delta_time;

                // Disparo dos lasers
                laser_timer.time = GetTime();
                if (IsKeyDown(KEY_SPACE) && laser_timer.time - laser_timer.last_time > 0.275f)
                {
                    for (int i = 0; i < MAX_LASERS; i++)
                    {
                        if (!lasers[i].is_active)
                        {
                            lasers[i].laser_texture_X = player.player_position_X + 50;
                            lasers[i].laser_texture_Y = player.player_position_Y - 50;
                            lasers[i].is_active = true;
                            PlaySound(lasers[i].laser_sound);
                            laser_timer.last_time = laser_timer.time;
                            break;
                        }
                    }
                }

                // Atualiza a posição dos lasers ativos
                for (int i = 0; i < MAX_LASERS; i++)
                {
                    if (lasers[i].is_active)
                    {
                        lasers[i].laser_texture_Y -= lasers[i].laser_speed * delta_time;
                        if (lasers[i].laser_texture_Y + lasers[i].laser_texture.height < 0)
                            lasers[i].is_active = false;
                    }
                }

                // Verifica e aplica o boost do jogador
                if (IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S))
                {
                    player.player_boost_texture_X0 = player.player_position_X + 25;
                    player.player_boost_texture_Y0 = player.player_position_Y + 70;
                    player.player_boost_texture_X1 = player.player_position_X + 75;
                    player.player_boost_texture_Y1 = player.player_position_Y + 70;
                    player.speed = 450.0f;
                }
                else
                {
                    player.speed = 250.0f;
                }
            }

            // Desenha os dois fundos para criar o efeito de rolagem
            DrawTexture(app_assets.gameplay_background, 0, (int)bg_1y, WHITE);
            DrawTexture(app_assets.gameplay_background, 0, (int)bg_2y, WHITE);

            DrawTextEx(app_assets.font, "SCORE:", {1150, 25}, 25, 0, GOLD);
            DrawTextEx(app_assets.font,TextFormat("%i", player.score), {1250, 25}, 25,1,GOLD);

            DrawTexture(app_assets.lives_counter, 25, 25, WHITE);
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

            // Desenha o jogador e o efeito de boost, se aplicável
            DrawTexture(player.player_texture, player.player_position_X, player.player_position_Y, WHITE);
            if ((IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S)) && !pause_app)
            {
                DrawTexture(player.player_boost_texture, player.player_boost_texture_X0, player.player_boost_texture_Y0, WHITE);
                DrawTexture(player.player_boost_texture, player.player_boost_texture_X1, player.player_boost_texture_Y1, WHITE);
            }

            // Desenha todos os lasers ativos
            for (int i = 0; i < MAX_LASERS; i++)
            {
                if (lasers[i].is_active)
                    DrawTexture(lasers[i].laser_texture, lasers[i].laser_texture_X, lasers[i].laser_texture_Y, WHITE);
            }

            // Exibe mensagem de pausa, se o jogo estiver pausado
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
                PauseMusicStream(app_assets.music_theme);
            else
                ResumeMusicStream(app_assets.music_theme);
            UpdateMusicStream(app_assets.music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawScoreboard(app_assets.main_menu_background);
            break;
        }

        // Estado: Comandos
        case COMMANDS:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.music_theme);
            else
                ResumeMusicStream(app_assets.music_theme);
            UpdateMusicStream(app_assets.music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawCommands(app_assets.main_menu_background);
            break;
        }

        // Estado: Créditos
        case CREDITS:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.music_theme);
            else
                ResumeMusicStream(app_assets.music_theme);
            UpdateMusicStream(app_assets.music_theme);

            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;

            DrawCredits(app_assets.main_menu_background);
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
        DrawFPS(10, 10);
        EndDrawing();
    } // Fim do loop principal do jogo

    //-----------------------------------------------------------
    // Limpeza e Liberação de Recursos
    //-----------------------------------------------------------
    for (int i = 0; i < MAX_LASERS; i++)
    {
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

#endif // APP_HPP