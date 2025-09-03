#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        Vec2 pushForce = Vec2(0,0);

        //Drag and Release to create impulse force
        Vec2 mouseCursor = Vec2(0,0);
        Vec2 startMouseCursor = Vec2(0,0);
        bool leftMouseButtonDown = false;
        


        // // TODO : create a fluid
        // SDL_Rect liquid;



    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void SpawnParticle(float x, float y, float mass, float radius);
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
