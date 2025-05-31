#include "Application.h"
#include "Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    particle = new Particle(50, 100, 1.0, 4);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {

    static int timePreviousFrame;
    
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);

    if(timeToWait > 0)
    {
       SDL_Delay(timeToWait);
    }


    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    
    if(deltaTime > 0.016)
        deltaTime = 0.016;

    timePreviousFrame = SDL_GetTicks();

    //particle->velocity = Vec2(2.0, 0.0);

    particle->acceleration = Vec2(3 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    particle->Integrate(deltaTime);


    if(particle->position.x + particle->radius / 2 > Graphics::Width())
    {
        particle->position.x =   Graphics::Width() - particle->radius / 2;
        particle->velocity.x *= -0.9;
    }
    else if(particle->position.x - particle->radius/2 < 0)
    {
        particle->position.x =   particle->radius / 2;
        particle->velocity.x *= -0.9;

    }
    
    if(particle->position.y + particle->radius/2> Graphics::Height())
    {
        particle->position.y = Graphics::Height() - particle->radius / 2;
        particle->velocity.y *= -0.9;
    }
    else if(particle->position.y - particle->radius/2 < 0)
    {
        particle->position.y = particle->radius / 2;
        particle->velocity.y *= -0.9;
    }


}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete particle;
    Graphics::CloseWindow();
}
