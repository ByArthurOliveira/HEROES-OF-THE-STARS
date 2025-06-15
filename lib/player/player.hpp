#include "raylib.h"

#include <stdio.h>

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

    Texture2D laser_texture;
    float laser_texture_X;
    float laser_texture_Y;
    float laser_speed;
    Sound laser_sound;
} Player;

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

    player.laser_texture = LoadTexture("assets/player/laser_texture.png");
    player.laser_texture_X = player.player_position_X + 50;
    player.laser_texture_Y = player.player_position_Y - 90;
    player.laser_speed = 750.0f;
    player.laser_sound = LoadSound("assets/player/laser_sound_effect.ogg");
    return player;
}
