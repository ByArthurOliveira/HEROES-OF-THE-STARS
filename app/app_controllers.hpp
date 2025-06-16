// Inclui a biblioteca principal do Raylib para gráficos e áudio
#include "raylib.h"

// Inclui o cabeçalho do jogador, presumivelmente contendo a definição da estrutura Player
#include "lib/player/player.hpp"

// Inclui a biblioteca padrão para operações de entrada e saída
#include <stdio.h>

// Inclui a biblioteca matemática para operações matemáticas avançadas
#include "math.h"

// Inclui a biblioteca de manipulação de strings
#include <string.h>

// Define a largura da tela do jogo
int screen_width = 1366;

// Define a altura da tela do jogo
int screen_height = 768;

// Declaração de variáveis estáticas para o efeito de rolagem de fundo
static const float BG_SCROLL_SPEED = 100.0f; // Velocidade de rolagem do fundo
static float bg_1y = 0; // Posição Y do primeiro fundo
static float bg_2y = 0; // Posição Y do segundo fundo
static bool bg_initialized = false; // Flag para verificar se o fundo foi inicializado

// Declaração da variável delta_time para armazenar o tempo entre os quadros
float delta_time;

// Flags para controlar o estado do aplicativo
bool stop_app = false; // Flag para parar o aplicativo
bool pause_app = false; // Flag para pausar o aplicativo
bool pause_music = false;

// Enumeração para representar os diferentes estados do aplicativo
enum AppState
{
    MAIN_MENU,    // Menu principal
    GAMEPLAY,     // Jogo em andamento
    SCOREBOARD,   // Tabela de pontuação
    COMMANDS,     // Comandos
    CREDITS,      // Créditos
    EXIT          // Sair do jogo
};

// Variável para armazenar o estado atual do aplicativo
AppState current_app_state = MAIN_MENU;

// Variável para armazenar a opção selecionada no menu
int selected_option = 1;

// Estrutura para representar um temporizador
typedef struct Timer
{
    float time;      // Tempo atual do temporizador
    float last_time; // Último tempo registrado
} Timer;

// Instâncias dos temporizadores para o aplicativo, lasers e verificação de pontuação
Timer app_timer;
Timer laser_timer;
Timer score_verifier;
