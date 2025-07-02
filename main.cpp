#include <stdio.h>
#include "raylib.h"

#include "app/app.hpp" // Inclui definições e funções principais do jogo
#include "login.hpp"   // Inclui funções relacionadas ao login do usuário

int main()
{
    OrganizeScoreboard(); // Organiza o arquivo de placar, ordenando os scores antes de iniciar

    Login(); // Executa a rotina de login para obter o nome do usuário

    InitializeApp(username); // Inicializa o jogo/app com o nome do usuário obtido no login
}
