#ifndef PLAYER_HPP
#define PLAYER_HPP

// Inclusão da biblioteca necessária
#include "raylib.h"

// Constantes
constexpr int MAX_LASERS = 25;

// Estruturas de dados

// Estrutura que representa o jogador
typedef struct Player
{
    // Textura e posição do jogador
    Texture2D player_texture;
    float player_position_X;
    float player_position_Y;

    // Propriedades do jogador
    float speed;
    int health;
    int score;

    // Textura do boost e suas coordenadas relativas
    Texture2D player_boost_texture;
    float player_boost_texture_X0;
    float player_boost_texture_Y0;
    float player_boost_texture_X1;
    float player_boost_texture_Y1;
} Player;

// Estrutura que representa o laser
typedef struct Laser
{
    bool is_active;
    Texture2D laser_texture;
    float laser_texture_X;
    float laser_texture_Y;
    float laser_speed;
    Sound laser_sound;
} Laser;

// Função para criar e inicializar o jogador
Player CreatePlayer()
{
    Player player;
    player.player_texture = LoadTexture("assets/player/player_texture.png");
    player.player_position_X = (GetScreenWidth() - player.player_texture.width) * 0.5f;
    player.player_position_Y = (GetScreenHeight() - player.player_texture.height) * 0.5f;
    player.speed = 250.0f;
    player.health = 3;
    player.score = 0;

    player.player_boost_texture = LoadTexture("assets/player/player_boost_texture.png");
    player.player_boost_texture_X0 = player.player_position_X + 25;
    player.player_boost_texture_Y0 = player.player_position_Y + 70;
    player.player_boost_texture_X1 = player.player_position_X + 75;
    player.player_boost_texture_Y1 = player.player_position_Y + 70;

    return player;
}

// Função para criar e inicializar um laser com base na posição do jogador
Laser CreateLaserBase(const Player &player)
{
    Laser laser;
    laser.is_active = false;
    laser.laser_texture = LoadTexture("assets/player/laser_texture.png");
    laser.laser_texture_X = player.player_position_X + 50;
    laser.laser_texture_Y = player.player_position_Y - 50;
    laser.laser_speed = 750.0f;
    laser.laser_sound = LoadSound("assets/sounds/laser_sound_effect.ogg");

    return laser;
}

#endif // PLAYER_HPP