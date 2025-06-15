#include "raylib.h"
#include "lib/player/player.hpp"

#include <stdio.h>
#include "math.h"

int screen_width = 1366;
int screen_height = 768;

// Declara os 2 backgrounds para o efeito de rolagem
static const float BG_SCROLL_SPEED = 100.0f;
static float bg_1y = 0;
static float bg_2y = 0;
static bool bg_initialized = false;

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