#include <stdio.h>
#include "raylib.h"

#include "app/app.hpp"
#include "login.hpp"

int main()
{
    OrganizeScoreboard();

    Login();
    InitializeApp(username);
}
