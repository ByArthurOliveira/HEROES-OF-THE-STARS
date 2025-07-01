#ifndef APP_ASSETS
#define APP_ASSETS

#include <stdio.h>
#include "raylib.h"

typedef struct AppAssets
{
    Texture2D menu_background;
    Texture2D game_background;
    Texture2D blur;
    Texture2D health_counter;
    Texture2D numbers[4];
    Texture2D _X;
    Music menu_music_theme;
    Sound confirm_option_sound;
    Sound switch_option_sound;
    Sound gameover_sound;
    Sound hit_sound;
    Font font;
} AppAssets;

AppAssets LoadAppAssets()
{
    AppAssets assets;

    assets.menu_background = LoadTexture("assets/interface/menu_background.png");
    assets.game_background = LoadTexture("assets/interface/game_background.png");
    assets.blur = LoadTexture("assets/interface/blur.png");
    assets.health_counter = LoadTexture("assets/interface/health_counter.png");

    assets.menu_music_theme = LoadMusicStream("assets/sounds/menu_music.mp3");
    assets.confirm_option_sound = LoadSound("assets/sounds/confirm_sound_effect.mp3");
    assets.switch_option_sound = LoadSound("assets/sounds/switch_option_sound.mp3");

    assets.gameover_sound = LoadSound("assets/sounds/gameover_sound.mp3");
    assets.hit_sound = LoadSound("assets/sounds/hit_sound.mp3");

    for (int i = 0; i < 4; i++)
    {
        assets.numbers[i] = LoadTexture(TextFormat("assets/interface/%d.png", i));
    }

    assets._X = LoadTexture("assets/interface/X.png");
    assets.font = LoadFont("assets/interface/Font.ttf");

    return assets;
}

void UpdateGameBackground(Texture2D game_background, float &background_y0, float &background_y1, float &BG_SCROLL_SPEED, bool &background_initialized)
{
    if (!background_initialized)
    {
        background_y0 = 0;
        background_y1 = -game_background.height;
        background_initialized = true;
    }

    background_y0 += BG_SCROLL_SPEED * GetFrameTime();
    background_y1 += BG_SCROLL_SPEED * GetFrameTime();

    if (background_y0 >= GetScreenHeight())
    {
        background_y0 = background_y1 - game_background.height;
    }

    if (background_y1 >= GetScreenHeight())
    {
        background_y1 = background_y0 - game_background.height;
    }
}

#endif
