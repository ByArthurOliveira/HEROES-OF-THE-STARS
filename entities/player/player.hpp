#ifndef PLAYER_HPP
#define PLAYER_HPP

// Inclusão da biblioteca necessária
#include <stdio.h>
#include "raylib.h"

// Constantes
constexpr int MAX_LASERS = 25;

// Estruturas de dados
// Estrutura que representa o jogador
typedef struct Player
{
    // Textura e posição do jogador
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;

    // Propriedades do jogador
    float speed;
    int health;
    int score;

    // Textura do boost e suas coordenadas relativas
    Texture2D boost_texture;
    Vector2 boostleft_position;
    Vector2 boostright_position;
    bool boost_active;
} Player;

// Estrutura que representa o laser
typedef struct Laser
{
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float speed;
    float interval;
    bool is_active;
    Sound sound;
} Laser;

// Estrutura que representa o power-up
typedef struct PowerUP
{
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float fall_speed;
    int time_remaining;
    bool was_catched;
    bool is_on_screen;
} PowerUP;

// Função para criar e inicializar o jogador
Player CreatePlayer()
{
    Player player;
    player.texture = LoadTexture("assets/player/player_texture.png");
    player.position = {((GetScreenWidth() - player.texture.width) * 0.5f), ((GetScreenHeight() - player.texture.height) * 0.5f)};
    player.speed = 250.0f;
    player.health = 3;
    player.score = 0;
    player.boost_texture = LoadTexture("assets/player/boost_texture.png");
    player.boostleft_position = {player.position.x + 25, player.position.y + 70};
    player.boostright_position = {player.position.x + 75, player.position.y + 70};
    player.boost_active = false;
    player.hit_box = {player.position.x, player.position.y, float(player.texture.width), float(player.texture.height)};
    return player;
}

// Função para desenhar o jogador na tela
void UpdatePlayer(Player &player)
{
    // Atualiza a posição do jogador com base nas teclas pressionadas
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        player.position.y -= player.speed * GetFrameTime();
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        player.position.y += player.speed * GetFrameTime();
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player.position.x -= player.speed * GetFrameTime();
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player.position.x += player.speed * GetFrameTime();

    // Limita a posição do jogador dentro da tela
    if (player.position.x < 0)
        player.position.x = 0;
    if (player.position.x + player.texture.width > GetScreenWidth())
        player.position.x = GetScreenWidth() - player.texture.width;
    if (player.position.y < 0)
        player.position.y = 0;
    if (player.position.y + player.texture.height > GetScreenHeight())
        player.position.y = GetScreenHeight() - player.texture.height;

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        player.boost_active = true;
        player.boostleft_position = {player.position.x + 25, player.position.y + 70};
        player.boostright_position = {player.position.x + 75, player.position.y + 70};
        player.speed = 500;
    }
    else
    {
        player.boost_active = false;
        player.speed = 250;
    }

    // Atualiza o hit box do jogador
    player.hit_box = {player.position.x, player.position.y, float(player.texture.width), float(player.texture.height)};
}

// Função para criar e inicializar um laser com base na posição do jogador
Laser CreateLaserBase(Player &player)
{
    Laser laser;
    laser.is_active = false;
    laser.texture = LoadTexture("assets/player/laser_texture.png");
    laser.position = {player.position.x + 50, player.position.y - 50};
    laser.speed = 775.0f;
    laser.interval = 0.275;
    laser.sound = LoadSound("assets/player/laser_sound_effect.ogg");
    laser.hit_box = {laser.position.x, laser.position.y, float(laser.texture.width), float(laser.texture.height)};
    return laser;
}

void SpawnLaser(Laser lasers[], Player &player)
{
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (!lasers[i].is_active)
        {
            lasers[i].position.x = player.position.x + 50;
            lasers[i].position.y = player.position.y - 50;
            lasers[i].is_active = true;
            PlaySound(lasers[i].sound);
            break;
        }
    }
}

void UpdateLaser(Laser lasers[])
{
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (lasers[i].is_active)
        {
            lasers[i].position.y -= lasers[i].speed * GetFrameTime();
            if (lasers[i].position.y + lasers[i].texture.height < 0)
                lasers[i].is_active = false;
        }
    }
}

void SetLaserInterval(PowerUP &power_up, Laser lasers[])
{
    if (power_up.was_catched)
    {
        lasers->interval = 0.1;
    }
    else
    {
        lasers->interval = 0.275;
    }
}

PowerUP CreatePowerUP()
{
    PowerUP power_up;
    power_up.texture = LoadTexture("assets/player/power_up_texture.png");
    power_up.position = {
        float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))), float(-power_up.texture.height)};
    power_up.time_remaining = 10;
    power_up.fall_speed = 250;
    power_up.was_catched = false;
    power_up.is_on_screen = false;
    power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};
    return power_up;
};

void UpdatePowerUP(PowerUP &power_up)
{
    power_up.position.y += power_up.fall_speed * GetFrameTime();
    power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};
}

void CheckPowerUPCollision(PowerUP &power_up, Player &player)
{
    if (CheckCollisionRecs(player.hit_box, power_up.hit_box))
    {
        power_up.was_catched = true;
        power_up.time_remaining = 10;
    }
}
#endif // PLAYER_HPP