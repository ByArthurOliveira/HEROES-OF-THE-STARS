// Previne múltiplas inclusões deste arquivo de cabeçalho
#ifndef PLAYER_HPP
#define PLAYER_HPP

// Inclusão do cabeçalho principal da biblioteca Raylib
#include "raylib.h"

// Inclusão do cabeçalho padrão para entrada e saída em C
#include <stdio.h>

// Define o número máximo de lasers ativos ao mesmo tempo
#define MAX_LASERS 25

// Estrutura que representa o jogador
typedef struct Player
{
    // Textura do jogador
    Texture2D player_texture;

    // Posições X e Y do jogador
    float player_position_X;
    float player_position_Y;

    // Velocidade de movimentação do jogador
    float speed;

    // Saúde do jogador
    int health;

    // Pontuação do jogador
    int score;

    // Textura do efeito de impulso do jogador
    Texture2D player_boost_texture;

    // Posições X e Y do efeito de impulso
    float player_boost_texture_X;
    float player_boost_texture_Y;
} Player;

// Estrutura que representa um único laser
typedef struct Laser
{
    // Indica se o laser está ativo (foi disparado)
    bool is_active;

    // Textura do laser
    Texture2D laser_texture;

    // Posições X e Y do laser
    float laser_texture_X;
    float laser_texture_Y;

    // Velocidade de deslocamento do laser
    float laser_speed;

    // Som associado ao disparo do laser
    Sound laser_sound;
} Laser;

// Função que cria e inicializa um jogador
Player CreatePlayer()
{
    Player player;

    // Carrega a textura do jogador
    player.player_texture = LoadTexture("assets/player/player_texture.png");

    // Define a posição inicial do jogador para centralizá-lo na tela
    player.player_position_X = (GetScreenWidth() - player.player_texture.width) * 0.5f;
    player.player_position_Y = (GetScreenHeight() - player.player_texture.height) * 0.5f;

    // Define a velocidade de movimentação do jogador
    player.speed = 250.0f;

    // Inicializa a saúde e a pontuação do jogador
    player.health = 100;
    player.score = 0;

    // Carrega a textura do efeito de impulso
    player.player_boost_texture = LoadTexture("assets/player/player_boost_texture.png");

    // Define a posição do efeito de impulso em relação ao jogador
    player.player_boost_texture_X = player.player_position_X + 50;
    player.player_boost_texture_Y = player.player_position_Y + 70;

    // Retorna a estrutura do jogador inicializada
    return player;
}

// Função que cria e inicializa um laser base
Laser CreateLaserBase(Player player)
{
    Laser laser;

    // Define o laser como inativo inicialmente
    laser.is_active = false;

    // Carrega a textura do laser
    laser.laser_texture = LoadTexture("assets/player/laser_texture.png");

    // Define a posição inicial do laser em relação ao jogador
    laser.laser_texture_X = player.player_position_X + 50;
    laser.laser_texture_Y = player.player_position_Y - 50;

    // Define a velocidade de deslocamento do laser
    laser.laser_speed = 750.0f;

    // Carrega o som associado ao disparo do laser
    laser.laser_sound = LoadSound("assets/player/laser_sound_effect.ogg");

    // Retorna a estrutura do laser inicializada
    return laser;
}

// Marca o final do arquivo de cabeçalho
#endif // PLAYER_HPP
