#ifndef ASTEROIDS_HPP
#define ASTEROIDS_HPP

#include <stdio.h>
#include "raylib.h"

//==================================================
// Constantes dos Asteroides
//==================================================
#define MAX_ASTEROIDS 50
#define ASTEROID_MIN_SPEED 75.0f
#define ASTEROID_MAX_SPEED 150.0f
#define ASTEROID_SPAWN_INTERVAL 2.0f
#define DIFFICULTY_INCREASE_INTERVAL 15.0f // Aumenta dificuldade a cada 15 segundos
#define POINTS_LOST_PER_ASTEROID 5         // Pontos perdidos quando asteroide sai da tela

//==================================================
// Estrutura: Asteroid
// Descrição: Representa um asteroide no jogo
//==================================================
typedef struct Asteroid
{
    Vector2 position;     // Posição do asteroide na tela
    Vector2 velocity;     // Velocidade de movimento
    Texture2D texture;    // Textura do asteroide
    Rectangle hit_box;    // Caixa de colisão
    float rotation;       // Rotação atual do asteroide
    float rotation_speed; // Velocidade de rotação
    bool is_active;       // Status de atividade
    int health;           // Vida do asteroide (pequeno=1, médio=2, grande=3)
    int size_type;        // Tipo de tamanho (0=pequeno, 1=médio, 2=grande)
} Asteroid;

//==================================================
// Estrutura: AsteroidManager
// Descrição: Gerencia todos os asteroides do jogo
//==================================================
typedef struct AsteroidManager
{
    Asteroid asteroids[MAX_ASTEROIDS];
    Texture2D small_texture;
    Texture2D medium_texture;
    Texture2D large_texture;
    Sound destruction_sound;
    float spawn_timer;
    float difficulty_timer;       // Timer para aumento de dificuldade
    float current_spawn_interval; // Intervalo atual de spawn (diminui com dificuldade)
    float current_min_speed;      // Velocidade mínima atual (aumenta com dificuldade)
    float current_max_speed;      // Velocidade máxima atual (aumenta com dificuldade)
    int active_count;
    int difficulty_level; // Nível atual de dificuldade
} AsteroidManager;

//-----------------------------------------------------------------
// Função: CreateAsteroidManager
// Descrição: Inicializa o gerenciador de asteroides
//-----------------------------------------------------------------
AsteroidManager CreateAsteroidManager()
{
    AsteroidManager manager;

    // Carrega as texturas dos asteroides
    manager.small_texture = LoadTexture("assets/asteroids/small.png");
    manager.medium_texture = LoadTexture("assets/asteroids/medium.png");
    manager.large_texture = LoadTexture("assets/asteroids/big.png");

    // Carrega o som de destruição
    manager.destruction_sound = LoadSound("assets/sounds/asteroid_destruction.ogg");

    // Inicializa os timers
    manager.spawn_timer = 0.0f;
    manager.difficulty_timer = 0.0f;
    manager.active_count = 0;
    manager.difficulty_level = 1;

    // Inicializa valores de dificuldade
    manager.current_spawn_interval = ASTEROID_SPAWN_INTERVAL;
    manager.current_min_speed = ASTEROID_MIN_SPEED;
    manager.current_max_speed = ASTEROID_MAX_SPEED;

    // Inicializa todos os asteroides como inativos
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

//-----------------------------------------------------------------
// Função: IncreaseDifficulty
// Descrição: Aumenta a dificuldade do jogo
//-----------------------------------------------------------------
void IncreaseDifficulty(AsteroidManager *manager)
{
    manager->difficulty_level++;

    // Reduz o intervalo de spawn (mais asteroides aparecem)
    manager->current_spawn_interval = ASTEROID_SPAWN_INTERVAL * (0.85f / manager->difficulty_level);
    if (manager->current_spawn_interval < 0.5f)
    {
        manager->current_spawn_interval = 0.5f; // Limite mínimo
    }

    // Aumenta a velocidade dos asteroides
    float speed_multiplier = 1.0f + (manager->difficulty_level - 1) * 0.3f;
    manager->current_min_speed = ASTEROID_MIN_SPEED * speed_multiplier;
    manager->current_max_speed = ASTEROID_MAX_SPEED * speed_multiplier;

    // Limita as velocidades máximas para não ficar impossível
    if (manager->current_min_speed > 200.0f)
    {
        manager->current_min_speed = 200.0f;
    }
    if (manager->current_max_speed > 350.0f)
    {
        manager->current_max_speed = 350.0f;
    }

    printf("Dificuldade aumentada! Nível: %d\n", manager->difficulty_level);
    printf("Spawn interval: %.2f, Speed: %.0f-%.0f\n",
           manager->current_spawn_interval, manager->current_min_speed, manager->current_max_speed);
}

//-----------------------------------------------------------------
// Função: SpawnAsteroid
// Descrição: Cria um novo asteroide em uma posição aleatória
//-----------------------------------------------------------------
void SpawnAsteroid(AsteroidManager *manager, int screen_width)
{
    // Encontra um slot vazio
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            // Define tipo de tamanho aleatório (0=pequeno, 1=médio, 2=grande)
            // Com dificuldade maior, mais chance de asteroides grandes
            int random_val = GetRandomValue(1, 100);
            if (manager->difficulty_level >= 3 && random_val <= 30)
            {
                asteroid->size_type = 2; // Grande (30% chance em dificuldade 3+)
            }
            else if (manager->difficulty_level >= 2 && random_val <= 60)
            {
                asteroid->size_type = 1; // Médio (30% chance em dificuldade 2+)
            }
            else
            {
                asteroid->size_type = 0; // Pequeno
            }

            // Configura textura e vida baseado no tamanho
            switch (asteroid->size_type)
            {
            case 0: // Pequeno
                asteroid->texture = manager->small_texture;
                asteroid->health = 1;
                break;
            case 1: // Médio
                asteroid->texture = manager->medium_texture;
                asteroid->health = 2;
                break;
            case 2: // Grande
                asteroid->texture = manager->large_texture;
                asteroid->health = 3;
                break;
            }

            // Posição inicial aleatória no topo da tela
            asteroid->position.x = GetRandomValue(0, screen_width - asteroid->texture.width);
            asteroid->position.y = -asteroid->texture.height;

            // Velocidade aleatória para baixo (usando velocidades atuais da dificuldade)
            asteroid->velocity.x = GetRandomValue(-50, 50);
            asteroid->velocity.y = GetRandomValue((int)manager->current_min_speed, (int)manager->current_max_speed);

            // Rotação aleatória
            asteroid->rotation = GetRandomValue(0, 360);
            asteroid->rotation_speed = GetRandomValue(-180, 180);

            // Ativa o asteroide
            asteroid->is_active = true;

            // Atualiza hitbox
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

//-----------------------------------------------------------------
// Função: UpdateAsteroids
// Descrição: Atualiza a lógica de todos os asteroides
// Retorna: Pontos perdidos por asteroides que saíram da tela
//-----------------------------------------------------------------
int UpdateAsteroids(AsteroidManager *manager, float frametime, int screen_width, int screen_height)
{
    int points_lost = 0;

    // Atualiza timer de dificuldade
    manager->difficulty_timer += frametime;
    if (manager->difficulty_timer >= DIFFICULTY_INCREASE_INTERVAL)
    {
        IncreaseDifficulty(manager);
        manager->difficulty_timer = 0.0f;
    }

    // Atualiza timer de spawn
    manager->spawn_timer += frametime;

    // Spawna novo asteroide se necessário (usando intervalo atual da dificuldade)
    if (manager->spawn_timer >= manager->current_spawn_interval)
    {
        SpawnAsteroid(manager, screen_width);
        manager->spawn_timer = 0.0f;
    }

    // Atualiza todos os asteroides ativos
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            // Atualiza posição
            asteroid->position.x += asteroid->velocity.x * frametime;
            asteroid->position.y += asteroid->velocity.y * frametime;

            // Atualiza rotação
            asteroid->rotation += asteroid->rotation_speed * frametime;

            // Atualiza hitbox
            asteroid->hit_box.x = asteroid->position.x;
            asteroid->hit_box.y = asteroid->position.y;

            // Remove asteroide se saiu da tela e aplica penalidade
            if (asteroid->position.y > screen_height + 50)
            {
                asteroid->is_active = false;
                manager->active_count--;
                points_lost += POINTS_LOST_PER_ASTEROID; // Player perde pontos
            }
        }
    }

    return points_lost;
}

//-----------------------------------------------------------------
// Função: CheckAsteroidLaserCollisions
// Descrição: Verifica colisões entre asteroides e lasers
//-----------------------------------------------------------------
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
                    if (CheckCollisionRecs(manager->asteroids[i].hit_box,
                                           {lasers[j].position.x, lasers[j].position.y,
                                            (float)lasers[j].texture.width, (float)lasers[j].texture.height}))
                    {
                        // Remove o laser
                        lasers[j].is_active = false;

                        // Reduz vida do asteroide
                        manager->asteroids[i].health--;

                        // Se asteroide foi destruído
                        if (manager->asteroids[i].health <= 0)
                        {
                            // Adiciona pontos baseado no tamanho e dificuldade
                            int base_points = 0;
                            switch (manager->asteroids[i].size_type)
                            {
                            case 0:
                                base_points = 10;
                                break; // Pequeno
                            case 1:
                                base_points = 20;
                                break; // Médio
                            case 2:
                                base_points = 30;
                                break; // Grande
                            }
                            // Bônus por dificuldade
                            score_gained += base_points + (manager->difficulty_level - 1) * 2;

                            // Toca som de destruição
                            PlaySound(manager->destruction_sound);

                            // Remove asteroide
                            manager->asteroids[i].is_active = false;
                            manager->active_count--;
                        }

                        break; // Sai do loop de lasers
                    }
                }
            }
        }
    }

    return score_gained;
}

//-----------------------------------------------------------------
// Função: CheckAsteroidPlayerCollision
// Descrição: Verifica colisão entre asteroides e jogador
//-----------------------------------------------------------------
bool CheckAsteroidPlayerCollision(AsteroidManager *manager, Rectangle player_hitbox)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            if (CheckCollisionRecs(manager->asteroids[i].hit_box, player_hitbox))
            {
                // Remove o asteroide após colisão
                manager->asteroids[i].is_active = false;
                manager->active_count--;
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------
// Função: DrawAsteroids
// Descrição: Desenha todos os asteroides ativos na tela
//-----------------------------------------------------------------
void DrawAsteroids(AsteroidManager *manager)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (manager->asteroids[i].is_active)
        {
            Asteroid *asteroid = &manager->asteroids[i];

            // Calcula o centro da textura para rotação
            Vector2 origin = {
                asteroid->texture.width / 2.0f,
                asteroid->texture.height / 2.0f};

            Vector2 draw_position = {
                asteroid->position.x + origin.x,
                asteroid->position.y + origin.y};

            // Desenha o asteroide com rotação
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

//-----------------------------------------------------------------
// Função: DrawDifficultyInfo
// Descrição: Desenha informações sobre a dificuldade atual
//-----------------------------------------------------------------
void DrawDifficultyInfo(AsteroidManager *manager, Font font)
{
    DrawTextEx(font, TextFormat("LEVEL %d", manager->difficulty_level), {25, 75}, 20, 1, GOLD);

    // Barra de progresso para próximo nível de dificuldade
    float progress = manager->difficulty_timer / DIFFICULTY_INCREASE_INTERVAL;
    int bar_width = 200;
    int bar_height = 10;

    DrawRectangle(25, 105, bar_width, bar_height, DARKGRAY);
    DrawRectangle(25, 105, (int)(bar_width * progress), bar_height, GOLD);
    DrawRectangleLines(25, 105, bar_width, bar_height, WHITE);
}

//-----------------------------------------------------------------
// Função: ResetAsteroidManager
// Descrição: Reseta o gerenciador para o estado inicial
//-----------------------------------------------------------------
void ResetAsteroidManager(AsteroidManager *manager)
{
    // Reseta timers e dificuldade
    manager->spawn_timer = 0.0f;
    manager->difficulty_timer = 0.0f;
    manager->difficulty_level = 1;
    manager->active_count = 0;

    // Reseta valores de dificuldade
    manager->current_spawn_interval = ASTEROID_SPAWN_INTERVAL;
    manager->current_min_speed = ASTEROID_MIN_SPEED;
    manager->current_max_speed = ASTEROID_MAX_SPEED;

    // Desativa todos os asteroides
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        manager->asteroids[i].is_active = false;
    }
}

//-----------------------------------------------------------------
// Função: UnloadAsteroidManager
// Descrição: Libera recursos do gerenciador de asteroides
//-----------------------------------------------------------------
void UnloadAsteroidManager(AsteroidManager *manager)
{
    UnloadTexture(manager->small_texture);
    UnloadTexture(manager->medium_texture);
    UnloadTexture(manager->large_texture);
    UnloadSound(manager->destruction_sound);
}

#endif // ASTEROIDS_HPP