#include "raylib.h"
#include "lib/player/player.hpp"
#include "lib/asteroids/asteroids.hpp"

#include <stdio.h>
#include "math.h"

int screen_width = 1366;
int screen_height = 768;

float delta_time;

bool stop_app = false;
bool pause_app = false;

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
    float time;
    float last_time;
} Timer;
Timer app_timer;
Timer laser_timer;