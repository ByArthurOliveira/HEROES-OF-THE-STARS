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
    Texture2D main_menu_background; // Textura do fundo do menu principal
    Texture2D gameplay_background;  // Textura do fundo do modo de jogo
    Texture2D lives_counter;        // Textura para exibição do contador de vidas
    Music music_theme;              // Música tema utilizada no jogo
    Sound switch_sound;             // Som utilizado para transição entre opções
    Texture2D numbers[4];           // Array de texturas para exibir números
    Texture2D _X;
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
    assets.main_menu_background = LoadTexture("assets/interface/main_menu_background.png");
    assets.gameplay_background = LoadTexture("assets/interface/gameplay_background.png");
    assets.lives_counter = LoadTexture("assets/interface/lives_counter_texture.png");

    // Carrega os arquivos de áudio
    assets.music_theme = LoadMusicStream("assets/sounds/music.mp3");
    assets.switch_sound = LoadSound("assets/sounds/switch_sound.ogg");

    // Carrega as texturas para os números
    for (int i = 0; i < 4; i++)
    {
        assets.numbers[i] = LoadTexture(TextFormat("assets/interface/%d.png", i));
    }

    assets._X = LoadTexture("assets/interface/X.png");
    assets.font = LoadFont("assets/interface/font.ttf");
    return assets;
}

#endif // APP_ASSETS