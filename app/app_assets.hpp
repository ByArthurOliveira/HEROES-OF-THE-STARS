#ifndef APP_ASSETS
#define APP_ASSETS

#include <stdio.h>
#include "raylib.h"

//-----------------------------------------------------------------
// Estrutura: AppAssets
// Descrição: Agrupa os ativos utilizados na aplicação (texturas,
// música e sons).
//-----------------------------------------------------------------
typedef struct AppAssets
{
    Texture2D menu_background; // Textura do fundo do menu principal
    Texture2D game_background; // Textura do fundo do modo de jogo
    Texture2D blur;
    Texture2D health_counter; // Textura para exibição do contador de vidas
    Texture2D numbers[4];     // Array de texturas para exibir números
    Texture2D _X;
    Music menu_music_theme;    // Música tema utilizada no jogo
    Sound switch_option_sound; // Som utilizado para transição entre opções
    Font font;
} AppAssets;

//-----------------------------------------------------------------
// Função: CreateAppAssets
// Descrição: Carrega os arquivos de ativos na memória e inicializa
// a estrutura AppAssets.
//-----------------------------------------------------------------
AppAssets CreateAppAssets()
{
    AppAssets assets;
    // Carrega as texturas dos fundos e do contador de vidas
    assets.menu_background = LoadTexture("assets/interface/menu_background.png");
    assets.game_background = LoadTexture("assets/interface/game_background.png");
    assets.blur = LoadTexture("assets/interface/blur.png");
    assets.health_counter = LoadTexture("assets/interface/health_counter.png");
    // Carrega os arquivos de áudio
    assets.menu_music_theme = LoadMusicStream("assets/sounds/menu_music.mp3");
    assets.switch_option_sound = LoadSound("assets/sounds/switch_sound_effect.ogg");
    // Carrega as texturas para os números
    for (int i = 0; i < 4; i++)
    {
        assets.numbers[i] = LoadTexture(TextFormat("assets/interface/%d.png", i));
    }
    assets._X = LoadTexture("assets/interface/X.png");
    assets.font = LoadFont("assets/interface/Font.ttf");
    return assets;
}

void UpdateGameBackground(Texture2D game_background, float &bg_y0, float &bg_y1, float &BG_SCROLL_SPEED, bool &bg_initialized)
{
    // Verifica se o fundo já foi inicializado
    if (!bg_initialized)
    {
        bg_y0 = 0;
        bg_y1 = - game_background.height;
        bg_initialized = true;
    }

    bg_y0 += BG_SCROLL_SPEED * GetFrameTime();
    bg_y1 += BG_SCROLL_SPEED * GetFrameTime();

    // Verifica se o fundo 1 saiu da tela e reposiciona
    if (bg_y0 >= GetScreenHeight())
        bg_y0 = bg_y1 - game_background.height;

    // Verifica se o fundo 2 saiu da tela e reposiciona
    if (bg_y1 >= GetScreenHeight())
        bg_y1 = bg_y0 - game_background.height;
}
#endif // APP_ASSETS