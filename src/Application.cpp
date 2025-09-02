#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;

    particles.push_back(smallBall);

    // Particle* bigBall = new Particle(70,100, 3.0);
    // bigBall->radius = 12;

    // particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;


}



void Application::SpawnParticle(float x, float y, float mass, float radius)
{
    Particle* spawnParticle = new Particle(x,y, mass);
    spawnParticle->radius = radius;
    particles.push_back(spawnParticle);
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
            case SDL_MOUSEBUTTONDOWN:{
                int x, y;
                SDL_GetMouseState(&x, &y); //Get Current mouse Position
                SpawnParticle(x, y, 1.0f, 4.0f);
            }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if(event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if(event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                 if(event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if(event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if(event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                 if(event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if(event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
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

    //apply forces to the particles
    for(auto particle : particles)
    {
        if(particle->position.y < liquid.y){
            Vec2 wind = Vec2(0.3 * PIXELS_PER_METER, 0.1 * PIXELS_PER_METER);
            particle->AddForce(wind);
        }
        
        Vec2 weight = Vec2(0.0, particle-> mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);

        //Apply a friction force
        Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        particle->AddForce(friction);
        

        particle->AddForce(pushForce);

        // Apply a drag force if we are inside the liquid
        if(particle->position.y >= liquid.y){
            Vec2 drag = Force::GenerateDragForce(*particle, 0.04);
            particle->AddForce(drag);
        }
    }

    //Integrate the acceleration and velocity to estimate the new position
    for(auto particle : particles)
    {
        particle->Integrate(deltaTime);
    }
    
    
    //Check the boundaries of the window
    for(auto particle : particles)
    {
        //Hardcoded flip in velocity if it touched the limits of the screen window
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
    

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    //Draw the liquid
    Graphics::DrawFillRect(liquid.x + liquid.w/2, liquid.y + liquid.h/2, liquid.w, liquid.h, 0xFF6E3713);

    for(auto particle : particles)
    {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();


}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : particles)
    {
        delete particle; 
    }
    Graphics::CloseWindow();
}
 