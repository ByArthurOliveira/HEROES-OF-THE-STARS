#ifndef PLAYER_HPP
#define PLAYER_HPP

// Bibliotecas padrão e Raylib
#include <stdio.h>
#include "raylib.h"

// Define a quantidade máxima de lasers na tela
#define MAX_LASERS 25

// ─────────────────────────────────────────────
// Estrutura que representa o jogador
typedef struct Player {
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float speed;
    int health;
    int score;
    int score_record;
    bool boost_active;
    Texture2D boost_texture;
    Vector2 boostleft_position;
    Vector2 boostright_position;
    Sound spaceship_sound;
} Player;

// Cria e inicializa o jogador
Player CreatePlayerBase() {
    Player player;

    player.texture = LoadTexture("assets/player/player_texture.png");
    player.position = {
        (GetScreenWidth() - player.texture.width) * 0.5f,
        (GetScreenHeight() - player.texture.height) * 0.5f
    };
    player.speed = 250.0f;
    player.health = 3;
    player.score = 0;
    player.score_record = 0;

    player.boost_texture = LoadTexture("assets/player/boost_texture.png");
    player.boostleft_position = { player.position.x + 25, player.position.y + 70 };
    player.boostright_position = { player.position.x + 75, player.position.y + 70 };
    player.boost_active = false;

    player.hit_box = {
        player.position.x,
        player.position.y,
        float(player.texture.width),
        float(player.texture.height)
    };

    player.spaceship_sound = LoadSound("assets/player/spaceship_sound.wav");

    return player;
}

// Atualiza o jogador com base na entrada
void UpdatePlayer(Player &player) {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        player.position.y -= player.speed * GetFrameTime();

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        player.position.y += player.speed * GetFrameTime();

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player.position.x -= player.speed * GetFrameTime();

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player.position.x += player.speed * GetFrameTime();

    if (player.position.x < 0)
        player.position.x = 0;
    if (player.position.x + player.texture.width > GetScreenWidth())
        player.position.x = GetScreenWidth() - player.texture.width;

    if (player.position.y < 0)
        player.position.y = 0;
    if (player.position.y + player.texture.height > GetScreenHeight())
        player.position.y = GetScreenHeight() - player.texture.height;

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        player.boost_active = true;
        player.boostleft_position = { player.position.x + 25, player.position.y + 70 };
        player.boostright_position = { player.position.x + 75, player.position.y + 70 };
        player.speed = 500;
    } else {
        player.boost_active = false;
        player.speed = 250;
    }

    player.hit_box = {
        player.position.x,
        player.position.y,
        float(player.texture.width),
        float(player.texture.height)
    };
}

// Reseta o jogador para estado inicial
Player ResetPlayer(Player &player) {
    player.position = {
        (GetScreenWidth() - player.texture.width) * 0.5f,
        (GetScreenHeight() - player.texture.height) * 0.5f
    };
    player.health = 3;
    player.score = 0;
    player.boost_active = false;
    player.speed = 250.0f;

    player.boostleft_position = { player.position.x + 25, player.position.y + 70 };
    player.boostright_position = { player.position.x + 75, player.position.y + 70 };

    player.hit_box = {
        player.position.x,
        player.position.y,
        float(player.texture.width),
        float(player.texture.height)
    };

    return player;
}

// ─────────────────────────────────────────────
// Estrutura que representa o laser
typedef struct Laser {
    bool is_active;
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float speed;
    float interval;
    Sound sound;
} Laser;

// Cria um laser na posição do jogador
Laser CreateLaserBase(Player &player) {
    Laser laser;

    laser.is_active = false;
    laser.texture = LoadTexture("assets/player/laser_texture.png");
    laser.position = { player.position.x + 50, player.position.y - 50 };
    laser.speed = 775.0f;
    laser.interval = 0.275f;
    laser.sound = LoadSound("assets/player/laser_sound_effect.ogg");

    laser.hit_box = {
        laser.position.x,
        laser.position.y,
        float(laser.texture.width + 5),
        float(laser.texture.height)
    };

    return laser;
}

// Dispara um laser (ativa um disponível)
void SpawnLaser(Laser lasers[], Player &player) {
    for (int i = 0; i < MAX_LASERS; i++) {
        if (!lasers[i].is_active) {
            PlaySound(lasers[i].sound);
            lasers[i].position.x = player.position.x + 50;
            lasers[i].position.y = player.position.y - 50;
            lasers[i].is_active = true;
            break;
        }
    }
}

// Atualiza posição dos lasers ativos
void UpdateLaser(Laser lasers[]) {
    for (int i = 0; i < MAX_LASERS; i++) {
        if (lasers[i].is_active) {
            lasers[i].position.y -= lasers[i].speed * GetFrameTime();

            if (lasers[i].position.y + lasers[i].texture.height < 0)
                lasers[i].is_active = false;
        }
    }
}

// ─────────────────────────────────────────────
// Estrutura que representa um power-up
typedef struct PowerUP {
    bool is_on_screen;
    bool was_catched;
    Texture2D texture;
    Vector2 position;
    Rectangle hit_box;
    float fall_speed;
    int time_remaining;
    Sound sound;
    bool freedom;
} PowerUP;

// Cria um power-up em posição aleatória
PowerUP CreatePowerUP() {
    PowerUP power_up;

    power_up.texture = LoadTexture("assets/player/power_up_texture.png");
    power_up.position = {
        float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width))),
        float(-power_up.texture.height)
    };
    power_up.time_remaining = 10;
    power_up.fall_speed = 250;
    power_up.was_catched = false;
    power_up.is_on_screen = false;
    power_up.freedom = true;

    power_up.hit_box = {
        power_up.position.x,
        power_up.position.y,
        float(power_up.texture.width),
        float(power_up.texture.height)
    };

    power_up.sound = LoadSound("assets/player/power_up_sound.mp3");

    return power_up;
}

// Atualiza a posição do power-up
void UpdatePowerUP(PowerUP &power_up) {
    if (power_up.freedom) {
        power_up.position.y += power_up.fall_speed * GetFrameTime();

        power_up.hit_box = {
            power_up.position.x,
            power_up.position.y,
            float(power_up.texture.width),
            float(power_up.texture.height)
        };
    }
}

// Verifica colisão com jogador
void CheckPowerUPCollision(PowerUP &power_up, Player &player) {
    if (CheckCollisionRecs(player.hit_box, power_up.hit_box)) {
        PlaySound(power_up.sound);
        power_up.was_catched = true;
        power_up.time_remaining = 10;
        power_up.freedom = false;
        power_up.is_on_screen = false;

        power_up.position.x = -1000;
        power_up.position.y = -1000;

        power_up.hit_box = {
            -1000,
            -1000,
            float(power_up.texture.width),
            float(power_up.texture.height)
        };
    }
}

// Modifica intervalo dos lasers se power-up for coletado
void SetLaserInterval(PowerUP &power_up, Laser lasers[]) {
    lasers->interval = power_up.was_catched ? 0.1f : 0.275f;
}

// Reposiciona o power-up no topo
void ResetPowerUP(PowerUP &power_up) {
    power_up.position.y = -power_up.texture.height;
    power_up.position.x = float(GetRandomValue(power_up.texture.width, 1366 - (5 * power_up.texture.width)));

    power_up.hit_box = {
        power_up.position.x,
        power_up.position.y,
        float(power_up.texture.width),
        float(power_up.texture.height)
    };
}

#endif // PLAYER_HPP
