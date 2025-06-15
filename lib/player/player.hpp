// Player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"
#include <stdio.h>

#define MAX_LASERS 25 // Número máximo de lasers ativos ao mesmo tempo

// Estrutura do jogador
typedef struct Player
{
    Texture2D player_texture;
    float player_position_X;
    float player_position_Y;
    float speed;
    int health;

    Texture2D player_boost_texture;
    float player_boost_texture_X;
    float player_boost_texture_Y;
} Player;

// Estrutura de um único laser
typedef struct Laser
{
    bool is_active; // Se o laser está ativo (foi disparado)

    Texture2D laser_texture; // Textura do laser
    float laser_texture_X;
    float laser_texture_Y;
    float laser_speed;
    Sound laser_sound;
} Laser;

// Função de criação do jogador, carrega as texturas e define posições iniciais
Player CreatePlayer()
{
    Player player;

    player.player_texture = LoadTexture("assets/player/player_texture.png");
    player.player_position_X = (GetScreenWidth() - player.player_texture.width) * 0.5f;
    player.player_position_Y = (GetScreenHeight() - player.player_texture.height) * 0.5f;
    player.speed = 250.0f;
    player.health = 100;

    player.player_boost_texture = LoadTexture("assets/player/player_boost_texture.png");
    player.player_boost_texture_X = player.player_position_X + 50;
    player.player_boost_texture_Y = player.player_position_Y + 90;

    return player;
}

// Cria um laser "base" para copiar textura e som para os outros lasers
Laser CreateLaserBase(Player player)
{
    Laser laser;

    laser.is_active = false;
    laser.laser_texture = LoadTexture("assets/player/laser_texture.png");
    laser.laser_texture_X = player.player_position_X + 50;
    laser.laser_texture_Y = player.player_position_Y - 90;
    laser.laser_speed = 750.0f;
    laser.laser_sound = LoadSound("assets/player/laser_sound_effect.ogg");

    return laser;
}

#endif // PLAYER_HPP