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

typedef struct PowerUP
{
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float fall_speed;
    int time_remaining;
    bool was_catched;
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
// Função para criar e inicializar um laser com base na posição do jogador
Laser CreateLaserBase(const Player &player)
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

PowerUP CreatePowerUP()
{
    PowerUP power_up;
    power_up.texture = LoadTexture("assets/player/power_up_texture.png");
    power_up.position = {
        float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))), float(-power_up.texture.height)};
    power_up.time_remaining = 10;
    power_up.fall_speed = 500;
    power_up.was_catched = false;
    power_up.hit_box = {power_up.position.x, power_up.position.y, float(power_up.texture.width), float(power_up.texture.height)};
    return power_up;
};
#endif // PLAYER_HPP