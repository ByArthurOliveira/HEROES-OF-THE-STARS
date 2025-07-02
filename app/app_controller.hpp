#ifndef APP_CONTROLLERS_HPP
#define APP_CONTROLLERS_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#include "app_assets.hpp"
#include "entities/player/player.hpp"
#include "entities/asteroids/asteroids.hpp"

// ----------------------------------------------
// Constantes de configuração
// ----------------------------------------------
#define MAX_INPUT_CHARS 11 // Máximo caracteres para nome do jogador
#define NUM_IMAGES 3       // Número de imagens para transições ou cutscenes
#define FADE_TIME 2.0f     // Tempo (s) para fade in/out
#define DISPLAY_TIME 5.0f  // Tempo (s) de exibição da imagem

// ----------------------------------------------
// Enumeração dos estados da transição de imagens
// ----------------------------------------------
typedef enum
{
    FADE_IN,
    SHOW,
    FADE_OUT,
    NEXT_IMAGE
} TransitionState;

// ----------------------------------------------
// Variáveis globais para controle de cutscene
// ----------------------------------------------
int image_index = 0;             // Índice da imagem atual
float timer = 0.0f;              // Timer geral para controle de tempo
float alpha = 0.0f;              // Transparência do fade
TransitionState state = FADE_IN; // Estado atual da transição

// ----------------------------------------------
// Variáveis para controle do background com scroll vertical
// ----------------------------------------------
float BG_SCROLL_SPEED = 150;       // Velocidade do scroll
float background_y0 = 0;           // Posição vertical do primeiro fundo
float background_y1 = 0;           // Posição vertical do segundo fundo
bool background_initialized = false; // Flag de inicialização do background

// ----------------------------------------------
// Flags para controle geral do aplicativo
// ----------------------------------------------
bool close_app = false;   // Indica quando fechar o jogo
bool pause_app = false;   // Indica se o jogo está pausado
bool pause_music = false; // Indica se a música está pausada

// ----------------------------------------------
// Dados do jogador
// ----------------------------------------------
char username[MAX_INPUT_CHARS]; // Nome do usuário

Player player; // Instância global do jogador

// ----------------------------------------------
// Enumeração dos estados do aplicativo (telas)
// ----------------------------------------------
enum AppState
{
    MAIN_MENU,
    GAMEPLAY,
    SCOREBOARD,
    COMMANDS,
    CREDITS,
    EXIT,
    CUTSCENE,
};

// Estado atual do aplicativo, inicia com a cutscene
AppState current_app_state = CUTSCENE;

// Opção selecionada no menu
int selected_option = 1;

// ----------------------------------------------
// Estrutura para controle de timers
// ----------------------------------------------
typedef struct Timer
{
    float real_time; // Tempo real passado
    float last_time; // Último tempo registrado
    float interval;  // Intervalo entre eventos
} Timer;

// Timers para várias funcionalidades
Timer option_timer;   // Timer para navegação no menu
Timer score_updater;  // Timer para atualizar a pontuação
Timer laser_timer;    // Timer para disparo de lasers
Timer power_up_timer; // Timer para spawn de power-ups

float spawn_timer = 0.0f;    // Timer acumulado para spawn
float spawn_interval = 2.0f; // Intervalo entre spawns

Timer pause_text;     // Timer para exibir texto de pausa
Timer game_over_text; // Timer para exibir texto de game over

// Flags para exibição de textos na tela
bool show_pause_text = false;
bool show_game_over_text = false;

// ----------------------------------------------
// Função que grava nome e pontuação no arquivo scoreboard.csv
// ----------------------------------------------
void WriteScoreboard(Player player, char username[])
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "a"); // Abre arquivo em modo append

    if (scoreboard == NULL)
    {
        return; // Sai se não conseguiu abrir
    }

    // Grava nome e pontuação do jogador
    fprintf(scoreboard, "%s, %d\n", username, player.score_record);

    fclose(scoreboard);
}

// ----------------------------------------------
// Função que organiza o arquivo scoreboard.csv
// Ordena as linhas pelo score em ordem decrescente
// ----------------------------------------------
void OrganizeScoreboard()
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "r");
    if (scoreboard == NULL)
    {
        return; // Sai se não conseguiu abrir
    }

    char lines[100][100]; // Buffer para até 100 linhas
    int count = 0;

    // Lê linhas do arquivo
    while (fgets(lines[count], sizeof(lines[count]), scoreboard) && count < 100)
    {
        // Remove '\n' do final da linha
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }
    fclose(scoreboard);

    // Ordena linhas pela pontuação (parte após a vírgula), decrescente
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            char *comma_i = strchr(lines[i], ',');
            char *comma_j = strchr(lines[j], ',');

            if (comma_i && comma_j)
            {
                int score_i = atoi(comma_i + 1);
                int score_j = atoi(comma_j + 1);

                if (score_i < score_j)
                {
                    char temp[100];
                    strcpy(temp, lines[i]);
                    strcpy(lines[i], lines[j]);
                    strcpy(lines[j], temp);
                }
            }
        }
    }

    // Reabre arquivo para sobrescrever com linhas ordenadas
    scoreboard = fopen("db/scoreboard.csv", "w");
    if (scoreboard == NULL)
    {
        return;
    }

    // Escreve todas as linhas de volta no arquivo
    for (int i = 0; i < count; i++)
    {
        fprintf(scoreboard, "%s\n", lines[i]);
    }

    fclose(scoreboard);
}

#endif
