#ifndef APP_CONTROLLERS_HPP
#define APP_CONTROLLERS_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#include "app_assets.hpp"
#include "entities/player/player.hpp"
#include "entities/asteroids/asteroids.hpp"

#define MAX_INPUT_CHARS 11 // Máximo de caracteres para o nome do jogador
#define NUM_IMAGES 3       // Número de imagens usadas para transições ou cutscenes
#define FADE_TIME 2.0f     // Tempo em segundos para o fade-in e fade-out
#define DISPLAY_TIME 5.0f  // Tempo em segundos para exibição da imagem

// Enumeração dos estados da transição de imagens (fade in, mostrar, fade out, próxima imagem)
typedef enum
{
    FADE_IN,
    SHOW,
    FADE_OUT,
    NEXT_IMAGE
} TransitionState;

// Variáveis globais para controle da transição das imagens da cutscene/menu
int image_index = 0;             // Índice da imagem atual sendo exibida
float timer = 0.0f;              // Timer geral para controle de tempo
float alpha = 0.0f;              // Transparência usada no fade
TransitionState state = FADE_IN; // Estado atual da transição

// Variáveis para controlar a velocidade e posição do background com scroll vertical
float BG_SCROLL_SPEED = 150;
float background_y0 = 0;             // Posição vertical do primeiro fundo
float background_y1 = 0;             // Posição vertical do segundo fundo
bool background_initialized = false; // Flag para indicar se o background foi inicializado

// Flags para controlar estados globais do aplicativo
bool close_app = false;   // Controla quando fechar o jogo
bool pause_app = false;   // Controla se o jogo está pausado
bool pause_music = false; // Controla se a música está pausada

// Nome do usuário armazenado em um array de caracteres
char username[MAX_INPUT_CHARS];

// Instância global do jogador
Player player;

// Enumeração dos estados do aplicativo (telas do jogo)
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

// Estado atual do aplicativo, inicializado com CUTSCENE para iniciar com a cutscene
AppState current_app_state = CUTSCENE;

// Opção selecionada no menu
int selected_option = 1;

// Estrutura para controle de timers com tempos e intervalos
typedef struct Timer
{
    float real_time; // Tempo real passado
    float last_time; // Último tempo registrado
    float interval;  // Intervalo desejado entre eventos
} Timer;

// Timers para diferentes funcionalidades do jogo
Timer option_timer;  // Timer para navegação das opções do menu
Timer score_updater; // Timer para atualizar a pontuação
Timer laser_timer;   // Timer para disparo de lasers

Timer power_up_timer;        // Timer para spawn de power-ups
float spawn_timer = 0.0f;    // Timer acumulado para spawn
float spawn_interval = 2.0f; // Intervalo entre spawns

Timer pause_text;     // Timer para exibir texto de pausa
Timer game_over_text; // Timer para exibir texto de game over

// Flags para mostrar textos específicos na tela
bool show_pause_text;
bool show_game_over_text;

// Função que grava o nome do jogador e sua pontuação no arquivo scoreboard.csv
void WriteScoreboard(Player player, char username[])
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "a"); // Abre arquivo em modo append

    if (scoreboard == NULL)
    {
        return; // Sai se não conseguir abrir o arquivo
    }

    fprintf(scoreboard, "%s, %d\n", username, player.score_record); // Escreve nome e pontuação

    fclose(scoreboard); // Fecha o arquivo
}

// Função que lê o arquivo scoreboard.csv, ordena as linhas pelo score em ordem decrescente e regrava no arquivo
void OrganizeScoreboard()
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "r");
    if (scoreboard == NULL)
    {
        return; // Sai se não conseguir abrir o arquivo
    }

    char lines[100][100]; // Array para armazenar até 100 linhas
    int count = 0;        // Contador de linhas

    // Lê cada linha do arquivo até o máximo permitido
    while (fgets(lines[count], sizeof(lines[count]), scoreboard) && count < 100)
    {
        // Remove o caractere de nova linha no final da linha, se existir
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }
    fclose(scoreboard);

    // Ordena as linhas pelo valor da pontuação (após a vírgula), ordem decrescente
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            // Encontra a vírgula que separa nome e score
            char *comma_i = strchr(lines[i], ',');
            char *comma_j = strchr(lines[j], ',');

            if (comma_i && comma_j) // Confirma que as vírgulas existem
            {
                // Converte a parte após a vírgula para inteiro (score)
                int score_i = atoi(comma_i + 1);
                int score_j = atoi(comma_j + 1);

                // Se a pontuação da linha i for menor que a de j, troca as linhas
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

    // Reabre o arquivo em modo escrita para salvar as linhas ordenadas
    scoreboard = fopen("db/scoreboard.csv", "w");
    if (scoreboard == NULL)
    {
        return;
    }

    // Escreve todas as linhas de volta no arquivo com '\n'
    for (int i = 0; i < count; i++)
    {
        fprintf(scoreboard, "%s\n", lines[i]);
    }

    fclose(scoreboard);
}

#endif
