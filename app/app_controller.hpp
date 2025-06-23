#ifndef APP_CONTROLLERS_HPP
#define APP_CONTROLLERS_HPP

#include <stdio.h>
#include <string.h>
#include "raylib.h"

#include "app_assets.hpp"
#include "entities/player/player.hpp"
#include "entities/asteroids/asteroids.hpp"

#define MAX_INPUT_CHARS 32

float BG_SCROLL_SPEED = 150;
float background_y0 = 0;
float background_y1 = 0;
bool background_initialized = false;

bool stop_app = false;
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

Timer app_timer;
Timer score_verifier;
Timer laser_timer;
Timer power_up_timer;

void WriteScoreboard(Player player, char username[])
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "a");

    if (scoreboard == NULL)
    {
        return;
    }

    fprintf(scoreboard, "%s,%d\n", username, player.best_score);

    fclose(scoreboard);
}

void OrganizeScoreboard()
{
    FILE *scoreboard = fopen("db/scoreboard.csv", "r");

    if (scoreboard == NULL)
    {
        return;
    }

    char line[256];
    char scores[100][256];
    int count = 0;

    while (fgets(line, sizeof(line), scoreboard))
    {
        strcpy(scores[count++], line);
    }

    fclose(scoreboard);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            int score_i = atoi(strchr(scores[i], ',') + 1);
            int score_j = atoi(strchr(scores[j], ',') + 1);
            if (score_i < score_j)
            {
                char temp[256];
                strcpy(temp, scores[i]);
                strcpy(scores[i], scores[j]);
                strcpy(scores[j], temp);
            }
        }
    }

    scoreboard = fopen("db/scoreboard.csv", "w");

    for (int i = 0; i < count; i++)
    {
        fputs(scores[i], scoreboard);
    }

    fclose(scoreboard);
}
#endif
