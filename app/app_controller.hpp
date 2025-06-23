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

enum AppState {
    MAIN_MENU,
    GAMEPLAY,
    SCOREBOARD,
    COMMANDS,
    CREDITS,
    EXIT
};

AppState current_app_state = MAIN_MENU;
int selected_option = 1;

typedef struct Timer {
    float real_time;
    float last_time;
    float interval;
} Timer;

Timer app_timer;
Timer score_verifier;
Timer laser_timer;
Timer power_up_timer;

void WriteScoreboard(Player player, char username[]) {
    FILE* scoreboard = fopen("db/scoreboard.csv", "a");
    fprintf(scoreboard, "%s,%d\n", username, player.best_score);
    fclose(scoreboard);
}

#endif
