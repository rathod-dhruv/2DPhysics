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

	//Anchor 30 pixels from the top center of the screen
	anchor = Vec2(Graphics::Width() / 2.0, 30);
    

	SpawnParticle(anchor.x, anchor.y, 2.0f, 10.0f);
	SpawnParticle(anchor.x, anchor.y + restLength, 2.0f, 10.0f);
	SpawnParticle(anchor.x + restLength, anchor.y, 2.0f, 10.0f);
	SpawnParticle(anchor.x + restLength, anchor.y + restLength, 2.0f, 10.0f);



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
					int lastParticleIndex = particles.size() - 1;
                    leftMouseButtonDown = false;
                    Vec2 endMouseCursor = mouseCursor;
                    Vec2 impulse = endMouseCursor - startMouseCursor;
                    particles[lastParticleIndex]->AddForce(impulse * -10.0f * PIXELS_PER_METER);
                    
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

    //apply forces to the particles
    for(auto particle : particles)
    {
        particle->AddForce(pushForce);

        //Apply a drag force
        Vec2 drag = Force::GenerateDragForce(*particle, 0.002);
        particle->AddForce(drag);

		//Apply weight force
		Vec2 weight = Vec2(0, 9.8 * particle->mass * PIXELS_PER_METER);
		particle->AddForce(weight);
 
    }

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        int currParticle = i;
		int nextParticle = (i + 1) % NUM_PARTICLES;

		//Apply spring force between the first and the second particle
		Vec2 springForce = Force::GenerateSpringForce(*particles[currParticle], *particles[nextParticle], restLength, k);

		particles[currParticle]->AddForce(springForce);
		particles[nextParticle]->AddForce(-springForce);   
    }

	Vec2 sf1 = Force::GenerateSpringForce(*particles[0], *particles[2], restLength, k);
    Vec2 sf2 = Force::GenerateSpringForce(*particles[1], *particles[3], restLength, k);

    particles[0]->AddForce(sf1);
    particles[2]->AddForce(-sf1);

    particles[1]->AddForce(sf2);
    particles[3]->AddForce(-sf2);

   
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
    Graphics::ClearScreen(0xFF00000F);

    if(leftMouseButtonDown)
    {
        Vec2 direction = mouseCursor - startMouseCursor;
        Graphics::DrawLine(particles.back()->position.x, particles.back()->position.y, particles.back()->position.x + direction.x, particles.back()->position.y + direction.y, 0xFFFF0000);
    }

	
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
		int currParticle = i;
        int nextParticle = (i + 1) % NUM_PARTICLES;
		Graphics::DrawLine(particles[currParticle]->position.x, particles[currParticle]->position.y, particles[nextParticle]->position.x, particles[nextParticle]->position.y, 0xFFFFFFFF);
    }

    Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFFFFFFFF);

    for(auto particle:particles)
    {
        //Draw the bob
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFAAFFFF);
       
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
 