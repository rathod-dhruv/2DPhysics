#include "Application.h"

int main(int argc, char *args[]) {
    Application app;

    app.Setup();

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}


//OPEN MSYS2 MINGW64 TERMINAL
//Run this command to compile the project:
//g++ -std=c++17 -Wall src/*.cpp src/Physics/*.cpp -o app $(pkg-config --cflags --libs sdl2 SDL2_ige SDL2_gfx)
//Then run the executable:
//./app.exe

