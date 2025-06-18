#ifndef APP_CONTROLLERS_HPP
#define APP_CONTROLLERS_HPP

//==================================================
// Bibliotecas Padrão
//==================================================
#include <stdio.h>

//==================================================
// Bibliotecas de Terceiros e Projeto
//==================================================
#include "raylib.h"
#include "app_assets.hpp"

#include "lib/player/player.hpp"
#include "lib/asteroids/asteroids.hpp"

//==================================================
// Variáveis Globais e Constantes
//==================================================

// Dimensões da tela
int screen_width = 1366;
int screen_height = 768;

// Velocidade de rolagem do background e controle do scroll
static const float BG_SCROLL_SPEED = 125.0f;
static float bg_y1 = 0;             // Posição vertical do fundo 1
static float bg_y2 = 0;             // Posição vertical do fundo 2
static bool bg_initialized = false; // Flag para indicar se o background foi inicializado

// Variável para armazenar o tempo decorrido entre frames
float frametime;

// Flags de controle da aplicação
bool stop_app = false;    // Controla a saída da aplicação
bool pause_app = false;   // Controla a pausa do jogo
bool pause_music = false; // Controla a pausa da música

//==================================================
// Controle de Estados da Aplicação
//==================================================
enum AppState
{
    MAIN_MENU,  // Menu principal
    GAMEPLAY,   // Modo de jogo
    SCOREBOARD, // Placar
    COMMANDS,   // Tela de comandos
    CREDITS,    // Tela de créditos
    EXIT        // Sair
};

// Estado atual da aplicação
AppState current_app_state = MAIN_MENU;

// Opção atualmente selecionada no menu
int selected_option = 1;

//==================================================
// Estrutura de Timer e Instâncias
//==================================================
typedef struct Timer
{
    float real_time; // Tempo atual do timer
    float last_time; // Último tempo registrado
    float interval;
} Timer;

// Timers utilizados para navegação, disparos e verificação de pontuação
Timer app_timer;
Timer score_verifier;
Timer laser_timer;
Timer power_up_respawn;
Timer power_up_timer;

#endif // APP_CONTROLLERS_HPP