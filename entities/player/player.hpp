#ifndef PLAYER_HPP
#define PLAYER_HPP

// Bibliotecas padrão e Raylib
#include <stdio.h>
#include "raylib.h"

// Define a quantidade máxima de lasers na tela
#define MAX_LASERS 25

// Estrutura que representa o jogador
typedef struct Player
{
    Texture2D texture;           // Textura do jogador
    Vector2 position;            // Posição na tela
    Rectangle hit_box;           // Caixa de colisão
    float speed;                 // Velocidade de movimento
    int health;                  // Vida
    int score;                   // Pontuação atual
    int score_record;            // Recorde de pontuação
    bool boost_active;           // Se o boost está ativo
    Texture2D boost_texture;     // Textura do boost
    Vector2 boostleft_position;  // Posição do boost à esquerda
    Vector2 boostright_position; // Posição do boost à direita
    Sound spaceship_sound;       // Som da nave
} Player;

// Função que cria e inicializa o jogador
Player CreatePlayerBase()
{
    Player player;

    // Carrega textura e define posição centralizada na tela
    player.texture = LoadTexture("assets/player/player_texture.png");
    player.position = {
        (GetScreenWidth() - player.texture.width) * 0.5f,
        (GetScreenHeight() - player.texture.height) * 0.5f};
    player.speed = 250.0f;
    player.health = 3;
    player.score = 0;
    player.score_record = 0;

    // Textura e posição do efeito de boost
    player.boost_texture = LoadTexture("assets/player/boost_texture.png");
    player.boostleft_position = {player.position.x + 25, player.position.y + 70};
    player.boostright_position = {player.position.x + 75, player.position.y + 70};
    player.boost_active = false;

    // Define a hitbox do jogador
    player.hit_box = {
        player.position.x,
        player.position.y,
        float(player.texture.width),
        float(player.texture.height)};

    // Carrega o som da nave
    player.spaceship_sound = LoadSound("assets/player/spaceship_sound.wav");

    return player;
}

// Atualiza o jogador com base nas entradas do teclado
void UpdatePlayer(Player &player)
{
    // Movimento para cima
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        player.position.y -= player.speed * GetFrameTime();

    // Movimento para baixo
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        player.position.y += player.speed * GetFrameTime();

    // Movimento para esquerda
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player.position.x -= player.speed * GetFrameTime();

    // Movimento para direita
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player.position.x += player.speed * GetFrameTime();

    // Impede que o jogador saia da tela (limites)
    if (player.position.x < 0)
        player.position.x = 0;

    if (player.position.x + player.texture.width > GetScreenWidth())
        player.position.x = GetScreenWidth() - player.texture.width;

    if (player.position.y < 0)
        player.position.y = 0;

    if (player.position.y + player.texture.height > GetScreenHeight())
        player.position.y = GetScreenHeight() - player.texture.height;

    // Ativa boost com Shift
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

    // Atualiza a hitbox do jogador
    player.hit_box = {
        player.position.x,
        player.position.y,
        float(player.texture.width),
        float(player.texture.height)};
}

// Estrutura que representa o laser do jogador
typedef struct Laser
{
    bool is_active;    // Se o laser está ativo
    Texture2D texture; // Textura do laser
    Vector2 position;  // Posição na tela
    Rectangle hit_box; // Caixa de colisão
    float speed;       // Velocidade do laser
    float interval;    // Intervalo entre disparos
    Sound sound;       // Som do disparo
} Laser;

// Cria um laser com base na posição atual do jogador
Laser CreateLaserBase(Player &player)
{
    Laser laser;
    laser.is_active = false;
    laser.texture = LoadTexture("assets/player/laser_texture.png");
    laser.position = {player.position.x + 50, player.position.y - 50};
    laser.speed = 775.0f;
    laser.interval = 0.275f;
    laser.sound = LoadSound("assets/player/laser_sound_effect.ogg");

    laser.hit_box = {
        laser.position.x,
        laser.position.y,
        float(laser.texture.width + 5),
        float(laser.texture.height)};

    return laser;
}

// Ativa um laser no array (reutiliza lasers inativos)
void SpawnLaser(Laser lasers[], Player &player)
{
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (!lasers[i].is_active)
        {
            PlaySound(lasers[i].sound);
            lasers[i].position.x = player.position.x + 50;
            lasers[i].position.y = player.position.y - 50;
            lasers[i].is_active = true;
            break;
        }
    }
}

// Atualiza os lasers ativos (movimento e verificação de limite)
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

// Estrutura que representa um power-up
typedef struct PowerUP
{
    bool is_on_screen;  // Se está visível na tela
    bool was_catched;   // Se foi coletado
    Texture2D texture;  // Textura do power-up
    Vector2 position;   // Posição
    Rectangle hit_box;  // Caixa de colisão
    float fall_speed;   // Velocidade de queda
    int time_remaining; // Tempo restante de efeito
    Sound sound;        // Som do power-up
} PowerUP;

// Cria um power-up em posição aleatória no topo
PowerUP CreatePowerUP()
{
    PowerUP power_up;
    power_up.texture = LoadTexture("assets/player/power_up_texture.png");
    power_up.position = {
        float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))),
        float(-power_up.texture.height)};
    power_up.time_remaining = 10;
    power_up.fall_speed = 250;
    power_up.was_catched = false;
    power_up.is_on_screen = false;

    power_up.hit_box = {
        power_up.position.x,
        power_up.position.y,
        float(power_up.texture.width),
        float(power_up.texture.height)};

    power_up.sound = LoadSound("assets/player/power_up_sound.mp3");
    return power_up;
}

// Atualiza a posição do power-up (queda)
void UpdatePowerUP(PowerUP &power_up)
{
    power_up.position.y += power_up.fall_speed * GetFrameTime();
    power_up.hit_box = {
        power_up.position.x,
        power_up.position.y,
        float(power_up.texture.width),
        float(power_up.texture.height)};
}

// Verifica colisão entre o jogador e o power-up
void CheckPowerUPCollision(PowerUP &power_up, Player &player)
{
    if (CheckCollisionRecs(player.hit_box, power_up.hit_box))
    {
        power_up.was_catched = true;
        power_up.time_remaining = 10;
    }
}

// Altera o intervalo de disparo dos lasers com base no power-up
void SetLaserInterval(PowerUP &power_up, Laser lasers[])
{
    if (power_up.was_catched)
    {
        lasers->interval = 0.1f;
    }
    else
    {
        lasers->interval = 0.275f;
    }
}

// Reseta o power-up (reposiciona no topo)
void ResetPowerUP(PowerUP &power_up)
{
    power_up.position.y = -power_up.texture.height;
    power_up.position.x = float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width)));
    power_up.hit_box = {
        power_up.position.x,
        power_up.position.y,
        float(power_up.texture.width),
        float(power_up.texture.height)};
}

#endif
