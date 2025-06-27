#ifndef ASTEROIDS_HPP
#define ASTEROIDS_HPP

#include <stdio.h>
#include "raylib.h"

#define MAX_ASTEROIDS 50
#define ASTEROID_MIN_SPEED 75.0f
#define ASTEROID_MAX_SPEED 150.0f
#define ASTEROID_SPAWN_INTERVAL 2.0f
#define DIFFICULTY_INCREASE_INTERVAL 15.0f
#define POINTS_LOST_PER_ASTEROID 5

typedef struct Asteroid
{
    Vector2 position;
    Vector2 velocity;
    Texture2D texture;
    Rectangle hit_box;
    float rotation;
    float rotation_speed;
    bool is_active;
    int health;
    int size_type;
} Asteroid;

typedef struct AsteroidManager
{
    Asteroid asteroids[MAX_ASTEROIDS];
    Texture2D small_texture;
    Texture2D medium_texture;
    Texture2D large_texture;
    Sound destruction_sound;
    float spawn_timer;
    float difficulty_timer;
    float current_spawn_interval;
    float current_min_speed;
    float current_max_speed;
    int active_count;
    int difficulty_level;
} AsteroidManager;

AsteroidManager CreateAsteroidManager()
{
    AsteroidManager manager;

    manager.small_texture = LoadTexture("assets/asteroids/small.png");
    manager.medium_texture = LoadTexture("assets/asteroids/medium.png");
    manager.large_texture = LoadTexture("assets/asteroids/big.png");

    manager.destruction_sound = LoadSound("assets/sounds/asteroid_destruction.ogg");

    manager.spawn_timer = 0.0f;
    manager.difficulty_timer = 0.0f;
    manager.active_count = 0;
    manager.difficulty_level = 1;

    manager.current_spawn_interval = ASTEROID_SPAWN_INTERVAL;
    manager.current_min_speed = ASTEROID_MIN_SPEED;
    manager.current_max_speed = ASTEROID_MAX_SPEED;

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        manager.asteroids[i].is_active = false;
        manager.asteroids[i].position = {0, 0};
        manager.asteroids[i].velocity = {0, 0};
        manager.asteroids[i].rotation = 0.0f;
        manager.asteroids[i].rotation_speed = 0.0f;
        manager.asteroids[i].health = 1;
        manager.asteroids[i].size_type = 0;
    }

    return manager;
}

void IncreaseDifficulty(AsteroidManager *manager)
{
    manager->difficulty_level++;

    manager->current_spawn_interval = ASTEROID_SPAWN_INTERVAL * (0.85f / manager->difficulty_level);
    if (manager->current_spawn_interval < 0.25f)
    {
        manager->current_spawn_interval = 0.25f;
    }

    float speed_multiplier = 1.0f + (manager->difficulty_level - 1) * 0.3f;
    manager->current_min_speed = ASTEROID_MIN_SPEED * speed_multiplier;
    manager->current_max_speed = ASTEROID_MAX_SPEED * speed_multiplier;

    if (manager->current_min_speed > 200.0f)
        manager->current_min_speed = 200.0f;

    if (manager->current_max_speed > 350.0f)
        manager->current_max_speed = 350.0f;

    printf("Dificuldade aumentada! Nível: %d\n", manager->difficulty_level);
    printf("Spawn interval: %.2f, Speed: %.0f-%.0f\n",
           manager->current_spawn_interval,
           manager->current_min_speed,
           manager->current_max_speed);
}

void SpawnAsteroid(AsteroidManager *manager, int screen_width)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            int random_val = GetRandomValue(1, 100);
            if (manager->difficulty_level >= 3 && random_val <= 30)
                asteroid->size_type = 2;
            else if (manager->difficulty_level >= 2 && random_val <= 60)
                asteroid->size_type = 1;
            else
                asteroid->size_type = 0;

            switch (asteroid->size_type)
            {
            case 0:
                asteroid->texture = manager->small_texture;
                asteroid->health = 1;
                break;
            case 1:
                asteroid->texture = manager->medium_texture;
                asteroid->health = 2;
                break;
            case 2:
                asteroid->texture = manager->large_texture;
                asteroid->health = 3;
                break;
            }

            asteroid->position.x = GetRandomValue(0, screen_width - asteroid->texture.width);
            asteroid->position.y = -asteroid->texture.height;

            asteroid->velocity.x = GetRandomValue(-50, 50);
            asteroid->velocity.y = GetRandomValue(
                (int)manager->current_min_speed,
                (int)manager->current_max_speed);

            asteroid->rotation = GetRandomValue(0, 360);
            asteroid->rotation_speed = GetRandomValue(-180, 180);

            asteroid->is_active = true;
            asteroid->hit_box = {
                asteroid->position.x,
                asteroid->position.y,
                (float)asteroid->texture.width,
                (float)asteroid->texture.height};

            manager->active_count++;
            break;
        }
    }
}

int UpdateAsteroids(AsteroidManager *manager, float frametime, int screen_width, int screen_height)
{
    int points_lost = 0;

    manager->difficulty_timer += frametime;
    if (manager->difficulty_timer >= DIFFICULTY_INCREASE_INTERVAL)
    {
        IncreaseDifficulty(manager);
        manager->difficulty_timer = 0.0f;
    }

    manager->spawn_timer += frametime;
    if (manager->spawn_timer >= manager->current_spawn_interval)
    {
        SpawnAsteroid(manager, screen_width);
        manager->spawn_timer = 0.0f;
    }

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            asteroid->position.x += asteroid->velocity.x * frametime;
            asteroid->position.y += asteroid->velocity.y * frametime;
            asteroid->rotation += asteroid->rotation_speed * frametime;

            asteroid->hit_box.x = asteroid->position.x;
            asteroid->hit_box.y = asteroid->position.y;

            if (asteroid->position.y > screen_height + 50)
            {
                asteroid->is_active = false;
                manager->active_count--;
                points_lost += POINTS_LOST_PER_ASTEROID;
            }
        }
    }

    return points_lost;
}

int CheckAsteroidLaserCollisions(AsteroidManager *manager, Laser lasers[], int max_lasers)
{
    int score_gained = 0;

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            for (int j = 0; j < max_lasers; j++)
            {
                if (lasers[j].is_active)
                {
                    if (CheckCollisionRecs(
                            manager->asteroids[i].hit_box,
                            {lasers[j].position.x,
                             lasers[j].position.y,
                             (float)lasers[j].texture.width,
                             (float)lasers[j].texture.height}))
                    {
                        lasers[j].is_active = false;
                        manager->asteroids[i].health--;

                        if (manager->asteroids[i].health <= 0)
                        {
                            int base_points = 0;
                            switch (manager->asteroids[i].size_type)
                            {
                            case 0:
                                base_points = 10;
                                break;
                            case 1:
                                base_points = 20;
                                break;
                            case 2:
                                base_points = 30;
                                break;
                            }

                            score_gained += base_points + (manager->difficulty_level - 1) * 2;

                            PlaySound(manager->destruction_sound);

                            manager->asteroids[i].is_active = false;
                            manager->active_count--;
                        }

                        break;
                    }
                }
            }
        }
    }

    return score_gained;
}

bool CheckAsteroidPlayerCollision(AsteroidManager *manager, Rectangle player_hitbox)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            if (CheckCollisionRecs(manager->asteroids[i].hit_box, player_hitbox))
            {
                manager->asteroids[i].is_active = false;
                manager->active_count--;
                return true;
            }
        }
    }

    return false;
}

void DrawAsteroids(AsteroidManager *manager)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            Vector2 origin = {
                asteroid->texture.width / 2.0f,
                asteroid->texture.height / 2.0f};

            Vector2 draw_position = {
                asteroid->position.x + origin.x,
                asteroid->position.y + origin.y};

            DrawTexturePro(
                asteroid->texture,
                {0, 0, (float)asteroid->texture.width, (float)asteroid->texture.height},
                {draw_position.x, draw_position.y, (float)asteroid->texture.width, (float)asteroid->texture.height},
                origin,
                asteroid->rotation,
                WHITE);
        }
    }
}

void DrawDifficultyInfo(AsteroidManager *manager, Font font)
{
    DrawTextEx(font, TextFormat("LEVEL %d", manager->difficulty_level), {25, 75}, 20, 1, GOLD);

    float progress = manager->difficulty_timer / DIFFICULTY_INCREASE_INTERVAL;
    int bar_width = 200;
    int bar_height = 10;

    DrawRectangle(25, 105, bar_width, bar_height, DARKGRAY);
    DrawRectangle(25, 105, (int)(bar_width * progress), bar_height, GOLD);
    DrawRectangleLines(25, 105, bar_width, bar_height, WHITE);
}

void ResetAsteroidManager(AsteroidManager *manager)
{
    manager->spawn_timer = 0.0f;
    manager->difficulty_timer = 0.0f;
    manager->difficulty_level = 1;
    manager->active_count = 0;

    manager->current_spawn_interval = ASTEROID_SPAWN_INTERVAL;
    manager->current_min_speed = ASTEROID_MIN_SPEED;
    manager->current_max_speed = ASTEROID_MAX_SPEED;

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        manager->asteroids[i].is_active = false;
    }
}

void UnloadAsteroidManager(AsteroidManager *manager)
{
    UnloadTexture(manager->small_texture);
    UnloadTexture(manager->medium_texture);
    UnloadTexture(manager->large_texture);
    UnloadSound(manager->destruction_sound);
}

#endif
