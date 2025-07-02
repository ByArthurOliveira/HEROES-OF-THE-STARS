#ifndef APP_HPP
#define APP_HPP

#include <stdio.h>
#include "raylib.h"
#include "app/app_screens.hpp"

// Função principal de inicialização do aplicativo/jogo
int InitializeApp(char username[])
{
    // Configurações iniciais da janela e do ambiente
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);                         // Desativa a tecla padrão de saída
    SetRandomSeed((unsigned int)GetTime());       // Semente para valores aleatórios
    HideCursor();                                 // Oculta o cursor do mouse
    InitWindow(1366, 768, "HEROES OF THE STARS"); // Cria a janela
    InitAudioDevice();                            // Inicializa o sistema de áudio

    // Carregamento de recursos gráficos e sonoros
    AppAssets app_assets = LoadAppAssets();
    CommandAssets commands_assets = LoadCommandAssets();
    CutsceneAssets cutscene_assets = LoadCutsceneAssets();

    // Ajuste de volumes
    SetMusicVolume(app_assets.menu_music_theme, 0.35);
    SetMusicVolume(cutscene_assets.music, 1.1);
    SetSoundVolume(app_assets.confirm_option_sound, 0.75);
    SetSoundVolume(app_assets.switch_option_sound, 0.75);
    SetSoundVolume(app_assets.gameover_sound, 0.75);
    SetSoundVolume(app_assets.hit_sound, 0.35);

    // Toca músicas iniciais
    PlayMusicStream(cutscene_assets.music);
    PlayMusicStream(app_assets.menu_music_theme);

    // Inicialização de entidades principais
    player = CreatePlayerBase(); // Cria o jogador

    // Cria os lasers do jogador a partir de um template
    Laser laser_template = CreateLaserBase(player);
    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = laser_template;
        lasers[i].is_active = false;
    }

    PowerUP power_up = CreatePowerUP();                         // Cria power-up
    AsteroidManager asteroid_manager = CreateAsteroidManager(); // Gerencia os asteroides

    // Loop principal da aplicação
    while (!close_app)
    {
        BeginDrawing();
        ClearBackground(WHITE);

        switch (current_app_state)
        {
        case MAIN_MENU:
        {
            // Alternância de música com tecla M
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            // Timer para controlar troca de opção no menu
            option_timer.real_time = GetTime();
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && selected_option > 1)
            {
                if ((option_timer.real_time - option_timer.last_time) > 0.255f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option--;
                    option_timer.last_time = option_timer.real_time;
                }
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && selected_option < 5)
            {
                if ((option_timer.real_time - option_timer.last_time) > 0.255f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option++;
                    option_timer.last_time = option_timer.real_time;
                }
            }

            // Confirmação da opção
            if (IsKeyPressed(KEY_ENTER))
            {
                PlaySound(app_assets.confirm_option_sound);
            }

            DrawMainMenu(app_assets, player, power_up, username);
            break;
        }

        case GAMEPLAY:
        {
            // Controle de som da nave
            if (!pause_app && !IsSoundPlaying(player.spaceship_sound))
                PlaySound(player.spaceship_sound);
            else if (pause_app && IsSoundPlaying(player.spaceship_sound))
                StopSound(player.spaceship_sound);

            // Alterna pausa com ESC
            if (IsKeyPressed(KEY_ESCAPE) && player.health > 0)
                pause_app = !pause_app;

            if (!pause_app)
            {
                SetSoundVolume(player.spaceship_sound, 0.11);

                // Atualiza score por tempo
                score_updater.real_time = GetTime();
                if (score_updater.real_time - score_updater.last_time > 1)
                {
                    player.score += 1;
                    score_updater.last_time = score_updater.real_time;
                }

                // Atualiza fundo e entidades
                UpdateGameBackground(app_assets.game_background, background_y0, background_y1, BG_SCROLL_SPEED, background_initialized);
                UpdatePlayer(player);
                CheckPowerUPCollision(power_up, player);
                if (CheckCollisionRecs(player.hit_box, power_up.hit_box))
                {
                    PlaySound(power_up.sound);
                }

                // Gerenciamento de spawn do power-up
                spawn_timer += GetFrameTime();
                if (!power_up.is_on_screen)
                {
                    if (spawn_timer >= spawn_interval)
                    {
                        int chance = GetRandomValue(0, 9);
                        if (chance == 0 && !power_up.was_catched)
                        {
                            ResetPowerUP(power_up);
                            power_up.is_on_screen = true;
                        }
                        spawn_timer = 0.0f;
                    }
                }
                else
                {
                    UpdatePowerUP(power_up);
                    if (power_up.position.y > GetScreenHeight() + power_up.texture.height)
                        power_up.is_on_screen = false;
                }

                // Atualiza tempo restante do power-up
                power_up_timer.real_time = GetTime();
                if (power_up_timer.real_time - power_up_timer.last_time > 1)
                {
                    power_up.time_remaining -= 1;
                    power_up_timer.last_time = power_up_timer.real_time;
                }

                // Reseta efeito do power-up
                if (power_up.time_remaining <= 0)
                    power_up.was_catched = false;

                // Define intervalo de tiro
                SetLaserInterval(power_up, lasers);

                // Disparo de laser
                laser_timer.real_time = GetTime();
                if (IsKeyDown(KEY_SPACE) && (laser_timer.real_time - laser_timer.last_time > lasers->interval))
                {
                    SpawnLaser(lasers, player);
                    laser_timer.last_time = laser_timer.real_time;
                }

                // Atualiza posição dos lasers
                UpdateLaser(lasers);

                // Atualiza asteroides e contabiliza pontos perdidos
                int points_lost = UpdateAsteroids(&asteroid_manager, GetFrameTime(), GetScreenWidth(), GetScreenHeight());
                player.score -= points_lost;
                if (player.score < 0)
                    player.score = 0;

                // Verifica colisões entre laser e asteroide
                int asteroid_score = CheckAsteroidLaserCollisions(&asteroid_manager, lasers, MAX_LASERS);
                player.score += asteroid_score;

                // Verifica colisão do jogador com asteroide
                if (CheckAsteroidPlayerCollision(&asteroid_manager, player.hit_box))
                {
                    PlaySound(app_assets.hit_sound);
                    player.health--;
                    if (player.health <= 0)
                    {
                        PlaySound(app_assets.gameover_sound);
                        pause_app = true;
                    }
                }
            }

            DrawGameplay(app_assets, player, lasers, power_up, asteroid_manager);

            // Tela de pausa
            if (pause_app && player.health > 0)
            {
                pause_text.real_time = GetTime();
                show_pause_text = ((int)(pause_text.real_time * 2)) % 2 == 0;

                if (show_pause_text)
                    DrawTextEx(app_assets.font, "JOGO PAUSADO", {75, 300}, 40, 1, GOLD);
                DrawTextEx(app_assets.font, "PRESSIONE ESC PARA CONTINUAR", {75, 350}, 30, 1, WHITE);
                DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
            }
            // Tela de Game Over
            else if (pause_app && player.health <= 0)
            {
                game_over_text.real_time = GetTime();
                show_game_over_text = ((int)(game_over_text.real_time * 2)) % 2 == 0;

                if (show_game_over_text)
                    DrawTextEx(app_assets.font, "GAME OVER", {75, 300}, 40, 1, RED);
                DrawTextEx(app_assets.font, "PRESSIONE ENTER PARA RETORNAR PARA O MENU", {75, 350}, 30, 1, WHITE);
                DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
            }

            // Retorna ao menu se pressionar ENTER após game over
            if ((pause_app == true) && (player.health <= 0) && (IsKeyPressed(KEY_ENTER)))
            {
                if (player.score > player.score_record)
                    player.score_record = player.score;

                current_app_state = MAIN_MENU;
                player.health = 3;
                player.score = 0;
                player.position = {683, 600};

                for (int i = 0; i < MAX_LASERS; i++)
                {
                    lasers[i].is_active = false;
                    lasers[i].position = {lasers[i].position.x, float(GetScreenHeight()) + lasers[i].texture.height};
                }

                ResetAsteroidManager(&asteroid_manager);
            }

            break;
        }

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

            DrawScoreboard(app_assets);
            break;
        }

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

            DrawCommands(app_assets, commands_assets);
            break;
        }

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

            DrawCredits(app_assets);
            break;
        }

        case EXIT:
        {
            ExitApp();
            break;
        }

        case CUTSCENE:
        {
            // Animação de abertura com fade in/out
            UpdateMusicStream(cutscene_assets.music);

            if (IsKeyPressed(KEY_ENTER))
            {
                StopMusicStream(cutscene_assets.music);
                current_app_state = MAIN_MENU;
            }

            float frametime = GetFrameTime();
            timer += frametime;

            switch (state)
            {
            case FADE_IN:
                alpha = 1.0f - (timer / FADE_TIME);
                if (timer >= FADE_TIME)
                {
                    timer = 0.0f;
                    alpha = 0.0f;
                    state = SHOW;
                }
                break;

            case SHOW:
                alpha = 0.0f;
                if (timer >= DISPLAY_TIME)
                {
                    timer = 0.0f;
                    state = FADE_OUT;
                }
                break;

            case FADE_OUT:
                alpha = timer / FADE_TIME;
                if (timer >= FADE_TIME)
                {
                    timer = 0.0f;
                    alpha = 1.0f;
                    state = NEXT_IMAGE;
                }
                break;

            case NEXT_IMAGE:
                image_index++;
                if (image_index >= NUM_IMAGES)
                {
                    StopMusicStream(cutscene_assets.music);
                    current_app_state = MAIN_MENU;
                    break;
                }
                state = FADE_IN;
                timer = 0.0f;
                break;
            }

            DrawTexture(cutscene_assets.images[image_index],
                        (GetScreenWidth() - cutscene_assets.images[image_index].width) / 2,
                        (GetScreenHeight() - cutscene_assets.images[image_index].height) / 2,
                        LIGHTGRAY);

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
            break;
        }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    // Grava pontuação se houver recorde
    if (player.score_record != 0)
    {
        WriteScoreboard(player, username);
    }

    // Liberação de recursos (áudio e texturas)
    CloseAudioDevice();
    CloseWindow();

    for (int i = 0; i < NUM_IMAGES; i++)
        UnloadTexture(cutscene_assets.images[i]);
    UnloadMusicStream(cutscene_assets.music);

    UnloadMusicStream(app_assets.menu_music_theme);
    UnloadSound(app_assets.confirm_option_sound);
    UnloadSound(app_assets.switch_option_sound);
    UnloadSound(app_assets.gameover_sound);
    UnloadSound(app_assets.hit_sound);
    UnloadTexture(app_assets.menu_background);
    UnloadTexture(app_assets.game_background);
    UnloadTexture(app_assets.blur);
    UnloadTexture(app_assets.health_counter);
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(app_assets.numbers[i]);
    }
    UnloadTexture(app_assets.X);
    UnloadFont(app_assets.font);

    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(commands_assets.arrows[i]);
        UnloadTexture(commands_assets.letters[i]);
    }
    UnloadTexture(commands_assets.attack);
    UnloadTexture(commands_assets.boost);
    UnloadTexture(commands_assets.pause);
    UnloadTexture(commands_assets.confirm);
    UnloadTexture(commands_assets.mute);

    UnloadTexture(player.texture);
    UnloadTexture(player.boost_texture);
    UnloadSound(player.spaceship_sound);
    UnloadTexture(power_up.texture);
    UnloadSound(power_up.sound);
    UnloadTexture(lasers->texture);
    UnloadSound(lasers->sound);
    UnloadTexture(laser_template.texture);
    UnloadAsteroidManager(&asteroid_manager);

    return 0;
}

#endif
