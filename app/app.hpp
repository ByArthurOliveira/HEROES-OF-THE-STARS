#ifndef APP_HPP
#define APP_HPP

#include <stdio.h>
#include "raylib.h"
#include "app/app_screens.hpp"

int InitializeApp(char username[])
{
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    SetRandomSeed((unsigned int)GetTime());
    HideCursor();
    InitWindow(1366, 768, "HEROES OF THE STARS");
    InitAudioDevice();

    AppAssets app_assets = LoadAppAssets();
    CutsceneAssets cutscene_assets = LoadCutsceneAssets();
    SetMusicVolume(app_assets.menu_music_theme, 0.35);
    SetMusicVolume(cutscene_assets.music, 1.1);
    SetSoundVolume(app_assets.confirm_option_sound, 0.75);
    SetSoundVolume(app_assets.switch_option_sound, 0.75);
    SetSoundVolume(app_assets.gameover_sound, 0.75);
    SetSoundVolume(app_assets.hit_sound, 0.35);
    PlayMusicStream(cutscene_assets.music);
    PlayMusicStream(app_assets.menu_music_theme);

    player = CreatePlayerBase();

    Laser laser_template = CreateLaserBase(player);
    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = laser_template;
        lasers[i].is_active = false;
    }

    PowerUP power_up = CreatePowerUP();

    AsteroidManager asteroid_manager = CreateAsteroidManager();

    while (!close_app)
    {
        BeginDrawing();
        ClearBackground(WHITE);

        switch (current_app_state)
        {
        case MAIN_MENU:
        {
            if (IsKeyPressed(KEY_M))
                pause_music = !pause_music;
            if (pause_music)
                PauseMusicStream(app_assets.menu_music_theme);
            else
                ResumeMusicStream(app_assets.menu_music_theme);
            UpdateMusicStream(app_assets.menu_music_theme);

            option_timer.real_time = GetTime();
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && selected_option > 1)
            {
                if (option_timer.real_time - option_timer.last_time > 0.255f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option--;
                    option_timer.last_time = option_timer.real_time;
                }
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && selected_option < 5)
            {
                if (option_timer.real_time - option_timer.last_time > 0.255f)
                {
                    PlaySound(app_assets.switch_option_sound);
                    selected_option++;
                    option_timer.last_time = option_timer.real_time;
                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                PlaySound(app_assets.confirm_option_sound);
            }

            DrawMainMenu(app_assets, player, power_up, username);
            break;
        }

        case GAMEPLAY:
        {
            if (!pause_app)
            {
                if (IsSoundPlaying(player.spaceship_sound) == false)
                    PlaySound(player.spaceship_sound);
            }
            else
            {
                if (IsSoundPlaying(player.spaceship_sound))
                    StopSound(player.spaceship_sound);
            }

            if (IsKeyPressed(KEY_ESCAPE) && player.health > 0)
                pause_app = !pause_app;

            if (!pause_app)
            {
                SetSoundVolume(player.spaceship_sound, 0.11);

                score_updater.real_time = GetTime();
                if (score_updater.real_time - score_updater.last_time > 1)
                {
                    player.score += 1;
                    score_updater.last_time = score_updater.real_time;
                }

                UpdateGameBackground(app_assets.game_background, background_y0, background_y1, BG_SCROLL_SPEED, background_initialized);
                UpdatePlayer(player);
                CheckPowerUPCollision(power_up, player);

                spawn_timer += GetFrameTime();
                if (!power_up.is_on_screen)
                {
                    spawn_timer += GetFrameTime();

                    if (spawn_timer >= spawn_interval)
                    {
                        int chance = GetRandomValue(0, 39);
                        if (chance == 0 && power_up.was_catched == false)
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

                laser_timer.real_time = GetTime();
                if (IsKeyDown(KEY_SPACE) && laser_timer.real_time - laser_timer.last_time > lasers->interval)
                {
                    SpawnLaser(lasers, player);
                    laser_timer.last_time = laser_timer.real_time;
                }

                UpdateLaser(lasers);

                int points_lost = UpdateAsteroids(&asteroid_manager, GetFrameTime(), GetScreenWidth(), GetScreenHeight());
                player.score -= points_lost;
                if (player.score < 0)
                    player.score = 0;

                int asteroid_score = CheckAsteroidLaserCollisions(&asteroid_manager, lasers, MAX_LASERS);
                player.score += asteroid_score;

                if (CheckAsteroidPlayerCollision(&asteroid_manager, player.hit_box))
                {
                    PlaySound(app_assets.hit_sound);
                    player.health--;
                    if (player.health <= 0)
                    {
                        PlaySound(app_assets.gameover_sound);
                        pause_app = true;
                        if (IsKeyPressed(KEY_ESCAPE))
                            pause_app = true;
                    }
                }
            }

            DrawGameplay(app_assets, player, lasers, power_up, asteroid_manager);

            if (pause_app == true && player.health > 0)
            {
                pause_text.real_time = GetTime();
                show_pause_text = ((int)(pause_text.real_time * 2)) % 2 == 0;

                if (show_pause_text)
                {
                    DrawTextEx(app_assets.font, "JOGO PAUSADO", {75, 300}, 40, 1, GOLD);
                    DrawTextEx(app_assets.font, "PRESSIONE ESC PARA CONTINUAR", {75, 350}, 30, 1, WHITE);
                    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
                }
                else
                {
                    DrawTextEx(app_assets.font, "PRESSIONE ESC PARA CONTINUAR", {75, 350}, 30, 1, WHITE);
                    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
                }
            }
            else if (pause_app == true && player.health <= 0)
            {

                game_over_text.real_time = GetTime();
                show_game_over_text = ((int)(game_over_text.real_time * 2)) % 2 == 0;

                if (show_game_over_text)
                {
                    DrawTextEx(app_assets.font, "GAME OVER", {75, 300}, 40, 1, RED);
                    DrawTextEx(app_assets.font, "PRESSIONE ENTER PARA RETORNAR PARA O MENU", {75, 350}, 30, 1, WHITE);
                    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
                }
                else
                {
                    DrawTextEx(app_assets.font, "PRESSIONE ENTER PARA RETORNAR PARA O MENU", {75, 350}, 30, 1, WHITE);
                    DrawTexture(app_assets.blur, 0, 0, LIGHTGRAY);
                }
            }

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

            DrawCommands(app_assets);
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
                alpha = 1.0f - (timer / FADE_TIME); // Escurecimento reverso (ficando claro)
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
                alpha = timer / FADE_TIME; // Escurecendo
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
                    // Finalizar apresentação
                    StopMusicStream(cutscene_assets.music);
                    current_app_state = MAIN_MENU;
                    break;
                }
                state = FADE_IN;
                timer = 0.0f;
                break;
            }

            // Desenha a imagem normalmente (sem alteração de alpha)
            DrawTexture(cutscene_assets.images[image_index],
                        (GetScreenWidth() - cutscene_assets.images[image_index].width) / 2,
                        (GetScreenHeight() - cutscene_assets.images[image_index].height) / 2,
                        LIGHTGRAY);

            // Desenha o retângulo preto por cima com alpha controlado
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));

            break;
        }
        }
        DrawFPS(10, 10);
        EndDrawing();
    }

    if (player.score_record != 0)
    {
        WriteScoreboard(player, username);
    }

    CloseAudioDevice();
    CloseWindow();

    for (int i = 0; i < NUM_IMAGES; i++)
    {
        UnloadTexture(cutscene_assets.images[i]);
    }
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
    UnloadTexture(app_assets._X);
    UnloadFont(app_assets.font);

    UnloadTexture(player.texture);
    UnloadTexture(player.boost_texture);
    UnloadSound(player.spaceship_sound);
    UnloadTexture(power_up.texture);
    UnloadTexture(lasers->texture);
    UnloadSound(lasers->sound);
    UnloadTexture(laser_template.texture);

    UnloadAsteroidManager(&asteroid_manager);
    return 0;
}

#endif
