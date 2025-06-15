#include "raylib.h"

#include <stdio.h>

typedef struct AppAssets
{
    Texture2D main_menu_background;
    Texture2D gameplay_background;
    Sound switch_sound;
} AppAssets;
AppAssets CreateAppAssets()
{
    AppAssets assets;
    assets.main_menu_background = LoadTexture("assets/main_menu_background.jpg");
    assets.gameplay_background = LoadTexture("assets/gameplay_background.jpg");
    assets.switch_sound = LoadSound("assets/switch_sound.ogg");
    return assets;
}