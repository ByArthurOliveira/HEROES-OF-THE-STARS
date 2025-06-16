// Inclusão da biblioteca Raylib para gráficos e áudio
#include "raylib.h"

// Inclusão do cabeçalho que contém as definições de estruturas e funções do jogo
#include "app/app_screens.hpp" // Inclui struct do Player e Laser

// Inclusão da biblioteca padrão para entrada e saída em C
#include <stdio.h>

// Função principal que inicializa e executa o jogo
int InitializeApp()
{
    // === Inicialização da Janela ===
    // Configura a janela para ter sincronização vertical (vsync) e ser sempre visível
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST);
    // Define a taxa de quadros alvo para 240 FPS
    SetTargetFPS(240);
    // Inicializa a janela com a largura e altura especificadas e o título do jogo
    InitWindow(screen_width, screen_height, "HEROES OF THE STARS");
    // Inicializa o dispositivo de áudio
    InitAudioDevice();

    // Impede que a janela seja fechada com a tecla ESC
    SetExitKey(KEY_NULL);

    // === Carregamento de recursos ===
    // Cria os recursos do jogo (texturas, sons, etc.)
    AppAssets app_assets = CreateAppAssets();

    // Cria o jogador
    Player player = CreatePlayer();

    // === Inicialização dos lasers ===
    // Cria um array para armazenar os lasers ativos
    Laser lasers[MAX_LASERS];
    // Cria um laser base para copiar textura e som para os outros lasers
    Laser base_laser = CreateLaserBase(player);

    // Copia os dados do laser base para todos os lasers do array
    for (int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i] = base_laser;
        lasers[i].is_active = false;
    }

    SetMusicVolume(app_assets.music_theme, 0.25);
    SetSoundVolume(app_assets.switch_sound, 0.50);
    PlayMusicStream(app_assets.music_theme);

    // === Loop Principal do Jogo ===
    // Enquanto o jogo não for encerrado
    while (!stop_app)
    {
        // Obtém o tempo decorrido desde o último quadro
        delta_time = GetFrameTime();

        // Inicia a fase de desenho
        BeginDrawing();
        // Limpa a tela com a cor branca
        ClearBackground(RAYWHITE);

        // Verifica o estado atual do jogo e executa a lógica correspondente
        switch (current_app_state)
        {
        case MAIN_MENU:

            if (IsKeyPressed(KEY_M))
            {
                pause_music = !pause_music;
            }
            if (pause_music)
            {
                PauseMusicStream(app_assets.music_theme);
            }
            else
            {
                ResumeMusicStream(app_assets.music_theme);
            }
            UpdateMusicStream(app_assets.music_theme);

            // Lógica para o menu principal
            app_timer.time = GetTime();
            // Navegação no menu com as teclas UP/W e DOWN/S
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && selected_option > 1)
            {
                if (app_timer.time - app_timer.last_time > 0.225f)
                {
                    app_timer.last_time = app_timer.time;
                    PlaySound(app_assets.switch_sound);
                    selected_option--;
                }
            }
            else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && selected_option < 5)
            {
                if (app_timer.time - app_timer.last_time > 0.225f)
                {
                    app_timer.last_time = app_timer.time;
                    PlaySound(app_assets.switch_sound);
                    selected_option++;
                }
            }

            // Desenha o menu principal
            DrawMainMenu(app_assets.main_menu_background, app_assets.switch_sound);
            break;

        case GAMEPLAY:
            // Alterna o estado de pausa com a tecla ESC
            if (IsKeyPressed(KEY_ESCAPE))
                pause_app = !pause_app;

            // Inicializa o fundo do jogo apenas uma vez
            if (!bg_initialized)
            {
                bg_1y = 0;
                bg_2y = -app_assets.gameplay_background.height;
                bg_initialized = true;
            }

            // Se o jogo não estiver pausado
            if (!pause_app)
            {
                // Lógica para a fase de jogo
                score_verifier.time = GetTime();
                // Atualiza a pontuação do jogador a cada segundo
                if (score_verifier.time - score_verifier.last_time > 1)
                {
                    player.score += 1;
                    score_verifier.last_time = score_verifier.time;
                }

                // ===== 1) Lógica de rolagem do fundo =====
                bg_1y += BG_SCROLL_SPEED * delta_time;
                bg_2y += BG_SCROLL_SPEED * delta_time;
                // Se o fundo superior saiu da tela, reposiciona
                if (bg_1y >= screen_height)
                    bg_1y = bg_2y - app_assets.gameplay_background.height;
                // Se o fundo inferior saiu da tela, reposiciona
                if (bg_2y >= screen_height)
                    bg_2y = bg_1y - app_assets.gameplay_background.height;

                // ===== 2) Movimento do Jogador =====
                // Movimenta o jogador com as teclas W/S/A/D
                if (IsKeyDown(KEY_W) && player.player_position_Y > 0)
                    player.player_position_Y -= player.speed * delta_time;
                if (IsKeyDown(KEY_S) && player.player_position_Y < screen_height - player.player_texture.height)
                    player.player_position_Y += player.speed * delta_time;
                if (IsKeyDown(KEY_A) && player.player_position_X > 0)
                    player.player_position_X -= player.speed * delta_time;
                if (IsKeyDown(KEY_D) && player.player_position_X < screen_width - player.player_texture.width)
                    player.player_position_X += player.speed * delta_time;

                // ===== 3) Disparo dos Lasers =====
                laser_timer.time = GetTime();
                // Dispara um laser com a tecla SPACE se o intervalo for maior que 0.275 segundos
                if (IsKeyDown(KEY_SPACE) && laser_timer.time - laser_timer.last_time > 0.275f)
                {
                    for (int i = 0; i < MAX_LASERS; i++)
                    {
                        if (!lasers[i].is_active)
                        {
                            lasers[i].laser_texture_X = player.player_position_X + 50;
                            lasers[i].laser_texture_Y = player.player_position_Y - 50;
                            lasers[i].is_active = true;
                            PlaySound(lasers[i].laser_sound);
                            laser_timer.last_time = laser_timer.time;
                            break;
                        }
                    }
                }

                // Atualiza a posição dos lasers ativos
                for (int i = 0; i < MAX_LASERS; i++)
                {
                    if (lasers[i].is_active)
                    {
                        lasers[i].laser_texture_Y -= lasers[i].laser_speed * delta_time;
                        // Desativa o laser se ele sair da tela
                        if (lasers[i].laser_texture_Y + lasers[i].laser_texture.height < 0)
                            lasers[i].is_active = false;
                    }
                }

                // ===== 4) Boost de Velocidade =====
                // Aumenta a velocidade do jogador com a tecla LEFT_SHIFT
                if (IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S))
                {
                    player.player_boost_texture_X = player.player_position_X + 50;
                    player.player_boost_texture_Y = player.player_position_Y + 75;
                    player.speed = 450.0f;
                }
                else
                {
                    player.speed = 250.0f;
                }
            }

            // ===== 5) Desenho (único BeginDrawing/EndDrawing no loop principal) =====
            // Desenha o fundo em loop
            DrawTexture(app_assets.gameplay_background, 0, (int)bg_1y, WHITE);
            DrawTexture(app_assets.gameplay_background, 0, (int)bg_2y, WHITE);

            // Desenha o jogador
            DrawTexture(player.player_texture, player.player_position_X, player.player_position_Y, WHITE);
            // Exibe a pontuação do jogador
            DrawText(TextFormat("%i", player.score), 1275, 25, 20, GOLD);

            // Desenha o efeito de boost se ativo
            if ((IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_S)) && !pause_app)
                DrawTexture(player.player_boost_texture, player.player_boost_texture_X, player.player_boost_texture_Y, WHITE);

            // Desenha os lasers ativos
            for (int i = 0; i < MAX_LASERS; i++)
                if (lasers[i].is_active)
                    DrawTexture(lasers[i].laser_texture, lasers[i].laser_texture_X, lasers[i].laser_texture_Y, WHITE);

            // Exibe a mensagem de pausa se o jogo estiver pausado
            if (pause_app)
            {
                DrawText("JOGO PAUSADO", 75, 400, 25, GOLD);
                DrawText("PRESSIONE ESC PARA CONTINUAR!", 75, 450, 25, GOLD);
            }
            break;

        case SCOREBOARD:
            if (IsKeyPressed(KEY_M))
            {
                pause_music = !pause_music;
            }
            if (pause_music)
            {
                PauseMusicStream(app_assets.music_theme);
            }
            else
            {
                ResumeMusicStream(app_assets.music_theme);
            }
            UpdateMusicStream(app_assets.music_theme);

            // Lógica para a tela de placar
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawScoreboard(app_assets.main_menu_background);
            break;

        case COMMANDS:
            if (IsKeyPressed(KEY_M))
            {
                pause_music = !pause_music;
            }
            if (pause_music)
            {
                PauseMusicStream(app_assets.music_theme);
            }
            else
            {
                ResumeMusicStream(app_assets.music_theme);
            }
            UpdateMusicStream(app_assets.music_theme);

            // Lógica para a tela de comandos
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawCommands(app_assets.main_menu_background);
            break;

        case CREDITS:
            if (IsKeyPressed(KEY_M))
            {
                pause_music = !pause_music;
            }
            if (pause_music)
            {
                PauseMusicStream(app_assets.music_theme);
            }
            else
            {
                ResumeMusicStream(app_assets.music_theme);
            }
            UpdateMusicStream(app_assets.music_theme);

            // Lógica para a tela de créditos
            if (IsKeyPressed(KEY_ESCAPE))
                current_app_state = MAIN_MENU;
            DrawCredits(app_assets.main_menu_background);
            break;

        case EXIT:
            // Lógica para encerrar o jogo
            ExitApp();
            break;
        }

        // Exibe a taxa de quadros por segundo no canto superior esquerdo
        DrawFPS(10, 10);
        // Finaliza a fase de desenho
        EndDrawing();
    }

    // === Liberação de recursos ===
    // Libera os recursos carregados
    for (int i = 0; i < MAX_LASERS; i++)
    {
        // Somente descarregue uma vez se for a mesma textura/som para todos
        if (i == 0)
        {
            UnloadTexture(lasers[i].laser_texture);
            UnloadSound(lasers[i].laser_sound);
        }
    }

    // Libera os recursos do jogador
    UnloadTexture(player.player_boost_texture);
    UnloadTexture(player.player_texture);
    // Libera os recursos dos assets do jogo
    UnloadSound(app_assets.switch_sound);
    UnloadTexture(app_assets.gameplay_background);
    UnloadTexture(app_assets.main_menu_background);
    // Fecha a janela do jogo
    CloseWindow();

    return 0;
}
