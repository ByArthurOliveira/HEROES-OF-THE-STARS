#ifndef APP_HPP
#define APP_HPP

#include <stdio.h>
#include "raylib.h"
#include "app/app_screens.hpp"

int InitializeApp(char username[])
{
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(240);
    InitWindow(1366, 768, "HEROES OF THE STARS");
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    SetRandomSeed((unsigned int)GetTime());

    AppAssets app_assets = CreateAppAssets();

    Player player = CreatePlayerBase();
    strncpy(player.name, username, MAX_INPUT_CHARS);

    Laser laser_template = CreateLaserBase(player);
    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = laser_template;
        lasers[i].is_active = false;
    }

    PowerUP power_up = CreatePowerUP();

    AsteroidManager asteroid_manager = CreateAsteroidManager();

    SetMusicVolume(app_assets.menu_music_theme, 0.25);
    SetSoundVolume(app_assets.switch_option_sound, 0.50);
    PlayMusicStream(app_assets.menu_music_theme);
    while (!stop_app)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
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

            app_timer.real_time = GetTime();

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

        case GAMEPLAY:
        {

            if (IsKeyPressed(KEY_ESCAPE) && player.health > 0)
                pause_app = !pause_app;

            if (!pause_app)
            {
                score_verifier.real_time = GetTime();
                if (score_verifier.real_time - score_verifier.last_time > 1)
                {
                    player.score += 1;
                    score_verifier.last_time = score_verifier.real_time;
                }

                UpdateGameBackground(app_assets.game_background, background_y0, background_y1, BG_SCROLL_SPEED, background_initialized);

                UpdatePlayer(player);

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
                    player.health--;
                    if (player.health <= 0)
                    {
                        pause_app = true;
                        if (IsKeyPressed(KEY_ESCAPE))
                            pause_app = true;
                    }
                }
            }

            DrawGameplay(app_assets, player, lasers, power_up, asteroid_manager);
            if (pause_app == true && player.health > 0)
            {
                DrawText("JOGO PAUSADO", 75, 400, 25, GOLD);
                DrawText("PRESSIONE ESC PARA CONTINUAR!", 75, 450, 25, GOLD);
            }
            else if (pause_app == true && player.health <= 0)
            {
                DrawText("GAME OVER", 75, 400, 25, RED);
                DrawText("PRESSIONE ENTER PARA VOLTAR AO MENU!", 75, 450, 25, RED);
            }

            if ((pause_app == true) && (player.health <= 0) && (IsKeyPressed(KEY_ENTER)))
            {

                current_app_state = MAIN_MENU;
                player.health = 3;
                player.score = 0;
                player.position = {683, 600};
                ResetAsteroidManager(&asteroid_manager);

                if (player.score > player.best_score)
                {
                    player.best_score = player.score;
                }
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

            DrawScoreboard(app_assets.menu_background, app_assets.blur);
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

            DrawCommands(app_assets.menu_background, app_assets.blur);
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

            DrawCredits(app_assets.menu_background, app_assets.blur);
            break;
        }

        case EXIT:
        {
            ExitApp();
            break;
        }
        }

        EndDrawing();
    }

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
#endif