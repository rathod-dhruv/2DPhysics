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
    
     Particle* smallPlanet = new Particle(200, 200, 1.0);
     smallPlanet->radius = 6;
     particles.push_back(smallPlanet);

    Particle* bigPlanet = new Particle(500,500, 21.0);
    bigPlanet->radius = 20;
    particles.push_back(bigPlanet);

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
    
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = true;
                    startMouseCursor = mouseCursor;
                }
                break;
            
            case SDL_MOUSEBUTTONUP:
                if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = false;
                    Vec2 endMouseCursor = mouseCursor;
                    Vec2 impulse = endMouseCursor - startMouseCursor;
                     for(auto particle : particles)
                     {
                         particle->AddForce(impulse * -10.0f * PIXELS_PER_METER);
                     }

                }
                break;


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

        particle->AddForce(pushForce);

        //Apply a friction force
        Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0);
        particle->AddForce(friction);

       
    }

	//Apply a gravitational force between to our two particles
	Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 1000.0, 5, 100);
	particles[0]->AddForce(attraction);
	particles[1]->AddForce(-attraction);


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
    Graphics::ClearScreen(0xFF548533);

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFAA1100);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFAA);

    if(leftMouseButtonDown)
    {
        Vec2 direction = mouseCursor - startMouseCursor;
        Graphics::DrawLine(particles.back()->position.x, particles.back()->position.y, particles.back()->position.x + direction.x, particles.back()->position.y + direction.y, 0xFFFF0000);
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
 