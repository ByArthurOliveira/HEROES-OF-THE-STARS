#ifndef APP_ASSETS
#define APP_ASSETS

#include <stdio.h>
#include "raylib.h"

// --------------------------------------------------
// Estrutura que armazena todos os recursos visuais e sonoros do jogo
// --------------------------------------------------
typedef struct AppAssets
{
    Texture2D menu_background;   // Fundo do menu principal
    Texture2D game_background;   // Fundo do gameplay (jogo)
    Texture2D blur;              // Textura de blur para efeitos visuais
    Texture2D health_counter;    // Imagem do contador de vida
    Texture2D numbers[4];        // Texturas dos números para a vida (0 a 3)
    Texture2D X;                 // Ícone de "X" para HUD
    Music menu_music_theme;      // Música tema do menu
    Sound confirm_option_sound;  // Som ao confirmar opção no menu
    Sound switch_option_sound;   // Som ao trocar opção no menu
    Sound gameover_sound;        // Som de game over
    Sound hit_sound;             // Som de impacto/dano
    Font font;                   // Fonte usada nos textos
} AppAssets;

// --------------------------------------------------
// Função para carregar todos os recursos do jogo
// --------------------------------------------------
AppAssets LoadAppAssets()
{
    AppAssets assets;

    // Carrega texturas
    assets.menu_background = LoadTexture("assets/interface/menu_background.png");
    assets.game_background = LoadTexture("assets/interface/game_background.png");
    assets.blur = LoadTexture("assets/interface/blur.png");
    assets.health_counter = LoadTexture("assets/interface/health_counter.png");

    // Carrega músicas e sons
    assets.menu_music_theme = LoadMusicStream("assets/sounds/menu_music.mp3");
    assets.confirm_option_sound = LoadSound("assets/sounds/confirm_sound_effect.mp3");
    assets.switch_option_sound = LoadSound("assets/sounds/switch_option_sound.mp3");
    assets.gameover_sound = LoadSound("assets/sounds/gameover_sound.mp3");
    assets.hit_sound = LoadSound("assets/sounds/hit_sound.mp3");

    // Carrega números de 0 a 3 para a vida
    for (int i = 0; i < 4; i++)
    {
        assets.numbers[i] = LoadTexture(TextFormat("assets/interface/%d.png", i));
    }

    // Ícone "X" para HUD
    assets.X = LoadTexture("assets/interface/X.png");

    // Carrega a fonte do jogo
    assets.font = LoadFont("assets/interface/Font.ttf");

    return assets;
}

// --------------------------------------------------
// Função para atualizar a posição do fundo do jogo
// Criando efeito de rolagem vertical contínua
// --------------------------------------------------
void UpdateGameBackground(Texture2D game_background, float &background_y0, float &background_y1, float &BG_SCROLL_SPEED, bool &background_initialized)
{
    // Inicializa posições do fundo, se ainda não inicializado
    if (!background_initialized)
    {
        background_y0 = 0;
        background_y1 = -game_background.height;
        background_initialized = true;
    }

    // Atualiza posições para efeito scroll contínuo
    background_y0 += BG_SCROLL_SPEED * GetFrameTime();
    background_y1 += BG_SCROLL_SPEED * GetFrameTime();

    // Reposiciona fundo que saiu da tela para loopar o scroll
    if (background_y0 >= GetScreenHeight())
    {
        background_y0 = background_y1 - game_background.height;
    }
    if (background_y1 >= GetScreenHeight())
    {
        background_y1 = background_y0 - game_background.height;
    }
}

// --------------------------------------------------
// Estrutura para armazenar recursos da cutscene
// --------------------------------------------------
typedef struct CutsceneAssets
{
    Texture2D images[3]; // Imagens usadas na cutscene
    Music music;         // Música da cutscene
} CutsceneAssets;

// --------------------------------------------------
// Função que carrega imagens e música da cutscene
// --------------------------------------------------
CutsceneAssets LoadCutsceneAssets()
{
    CutsceneAssets cutscene_assets;

    for (int i = 0; i < 3; i++)
    {
        cutscene_assets.images[i] = LoadTexture(TextFormat("assets/cutscene/cutscene_%d.png", i));
    }

    cutscene_assets.music = LoadMusicStream("assets/sounds/cutscene_audio.mp3");

    return cutscene_assets;
}

// --------------------------------------------------
// Estrutura para armazenar texturas dos comandos exibidos na tela de ajuda
// --------------------------------------------------
typedef struct CommandAssets
{
    Texture2D arrows[4];  // Setas de direção (up, down, left, right)
    Texture2D letters[4]; // Teclas WASD (W, A, S, D)
    Texture2D attack;     // Tecla de ataque (space)
    Texture2D pause;      // Tecla de pausa (escape)
    Texture2D boost;      // Tecla de boost (shift)
    Texture2D confirm;    // Tecla de confirmação (enter)
    Texture2D mute;       // Tecla de mute (M)
} CommandAssets;

// --------------------------------------------------
// Função que carrega as texturas dos comandos do jogo
// --------------------------------------------------
CommandAssets LoadCommandAssets()
{
    CommandAssets command_assets;

    // Setas de movimentação
    command_assets.arrows[0] = LoadTexture("assets/commands/up.png");
    command_assets.arrows[1] = LoadTexture("assets/commands/down.png");
    command_assets.arrows[2] = LoadTexture("assets/commands/left.png");
    command_assets.arrows[3] = LoadTexture("assets/commands/right.png");

    // Teclas WASD
    command_assets.letters[0] = LoadTexture("assets/commands/W.png");
    command_assets.letters[1] = LoadTexture("assets/commands/A.png");
    command_assets.letters[2] = LoadTexture("assets/commands/S.png");
    command_assets.letters[3] = LoadTexture("assets/commands/D.png");

    // Teclas de ação
    command_assets.attack = LoadTexture("assets/commands/space.png");
    command_assets.pause = LoadTexture("assets/commands/escape.png");
    command_assets.boost = LoadTexture("assets/commands/shift.png");
    command_assets.confirm = LoadTexture("assets/commands/enter.png");
    command_assets.mute = LoadTexture("assets/commands/M.png");

    return command_assets;
}

#endif
