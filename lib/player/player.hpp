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
    float speed;
    bool is_active;
    Sound sound;
} Laser;

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
    laser.sound = LoadSound("assets/player/laser_sound_effect.ogg");
    return laser;
}
#endif // PLAYER_HPP