#ifndef APP_CONTROLLERS_HPP
#define APP_CONTROLLERS_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#include "app_assets.hpp"
#include "entities/player/player.hpp"
#include "entities/asteroids/asteroids.hpp"

#define MAX_INPUT_CHARS 11

float BG_SCROLL_SPEED = 150;
float background_y0 = 0;
float background_y1 = 0;
bool background_initialized = false;

bool close_app = false;
bool pause_app = false;
bool pause_music = false;

char username[MAX_INPUT_CHARS];
Player player;

enum AppState
{
    MAIN_MENU,
    GAMEPLAY,
    SCOREBOARD,
    COMMANDS,
    CREDITS,
    EXIT
};

AppState current_app_state = MAIN_MENU;
int selected_option = 1;

typedef struct Timer
{
    float real_time;
    float last_time;
    float interval;
} Timer;

Timer option_timer;
Timer score_updater;
Timer laser_timer;

Timer power_up_timer;
float spawn_timer = 0.0f;
float spawn_interval = 2.0f;

Timer pause_text;
Timer game_over_text;

bool show_pause_text;
bool show_game_over_text;

void WriteScoreboard(Player player, char username[])
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "a");

    if (scoreboard == NULL)
    {
        return;
    }

    fprintf(scoreboard, "%s, %d\n", username, player.score_record);

    fclose(scoreboard);
}

void OrganizeScoreboard()
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "r");
    if (scoreboard == NULL)
    {
        return;
    }

    char lines[100][100];
    int count = 0;

    while (fgets(lines[count], sizeof(lines[count]), scoreboard) && count < 100)
    {
        // Remove o '\n' no final da linha (se existir)
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }
    fclose(scoreboard);

    // Ordenar as linhas com base na pontuação (depois da vírgula)
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            char *comma_i = strchr(lines[i], ',');
            char *comma_j = strchr(lines[j], ',');

            if (comma_i && comma_j) // Só continua se a vírgula foi encontrada
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

    scoreboard = fopen("db/scoreboard.csv", "w");
    if (scoreboard == NULL)
    {
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(scoreboard, "%s\n", lines[i]); // Garante \n ao salvar
    }

    fclose(scoreboard);
}

#endif
