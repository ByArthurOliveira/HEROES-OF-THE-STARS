// Inclui a biblioteca principal do Raylib para gráficos e áudio
#include "raylib.h"

// Inclui a biblioteca padrão para operações de entrada e saída
#include <stdio.h>

// Define a estrutura AppAssets para armazenar os recursos do aplicativo
typedef struct AppAssets
{
    Texture2D main_menu_background; // Textura do fundo do menu principal
    Texture2D gameplay_background;  // Textura do fundo da tela de jogo
    Music music_theme;
    Sound switch_sound;             // Som de transição entre telas
} AppAssets;

// Função para criar e carregar os recursos do aplicativo
AppAssets CreateAppAssets()
{
    AppAssets assets; // Declara uma variável do tipo AppAssets

    // Carrega a textura do fundo do menu principal
    assets.main_menu_background = LoadTexture("assets/main_menu_background.jpg");

    // Carrega a textura do fundo da tela de jogo
    assets.gameplay_background = LoadTexture("assets/gameplay_background.jpg");

    assets.music_theme = LoadMusicStream("assets/music.mp3");

    // Carrega o som de transição entre telas
    assets.switch_sound = LoadSound("assets/switch_sound.ogg");

    // Retorna a estrutura preenchida com os recursos carregados
    return assets;
}
