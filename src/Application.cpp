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
    

	SpawnBody(anchor.x, anchor.y, 1.0f, 6.0f);
	SpawnBody(anchor.x, anchor.y + restLength, 1.0f, 6.0f);
	SpawnBody(anchor.x + restLength, anchor.y, 1.0f, 6.0f);
	SpawnBody(anchor.x + restLength, anchor.y + restLength, 1.0f, 6.0f);



}



void Application::SpawnBody(float x, float y, float mass, float radius)
{
    Body* spawnBody = new Body(x,y, mass);
    spawnBody->radius = radius;
    
    bodies.push_back(spawnBody);
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
					int lastBodyIndex = bodies.size() - 1;
                    leftMouseButtonDown = false;
                    Vec2 endMouseCursor = mouseCursor;
                    Vec2 impulse = endMouseCursor - startMouseCursor;
                    bodies[lastBodyIndex]->AddForce(impulse * -10.0f * PIXELS_PER_METER);
                    
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

    //apply forces to the bodies
    for(auto body : bodies)
    {
        body->AddForce(pushForce);

        //Apply a drag force
        Vec2 drag = Force::GenerateDragForce(*body, 0.009);
        body->AddForce(drag);

		//Apply weight force
		Vec2 weight = Vec2(0, 9.8 * body->mass * PIXELS_PER_METER);
		body->AddForce(weight);
 
    }

    for (int i = 0; i < NUM_BODIES; i++)
    {
        int currBody = i;
		int nextBody = (i + 1) % NUM_BODIES;

		//Apply spring force between the first and the second body
		Vec2 springForce = Force::GenerateSpringForce(*bodies[currBody], *bodies[nextBody], restLength, k);

		bodies[currBody]->AddForce(springForce);
		bodies[nextBody]->AddForce(-springForce);   
    }

	Vec2 sf1 = Force::GenerateSpringForce(*bodies[0], *bodies[2], restLength, k);
    Vec2 sf2 = Force::GenerateSpringForce(*bodies[1], *bodies[3], restLength, k);

    bodies[0]->AddForce(sf1);
    bodies[2]->AddForce(-sf1);

    bodies[1]->AddForce(sf2);
    bodies[3]->AddForce(-sf2);

   
    //Integrate the acceleration and velocity to estimate the new position
    for(auto body : bodies)
    {
        body->Integrate(deltaTime);
    }
    
    
    //Check the boundaries of the window
    for(auto body : bodies)
    {
        //Hardcoded flip in velocity if it touched the limits of the screen window
        if(body->position.x + body->radius / 2 > Graphics::Width())
        {
            body->position.x =   Graphics::Width() - body->radius / 2;
            body->velocity.x *= -0.9;
        }
        else if(body->position.x - body->radius/2 < 0)
        {
            body->position.x =   body->radius / 2;
            body->velocity.x *= -0.9;

        }
        
        if(body->position.y + body->radius/2> Graphics::Height())
        {
            body->position.y = Graphics::Height() - body->radius / 2;
            body->velocity.y *= -0.9;
        }
        else if(body->position.y - body->radius/2 < 0)
        {
            body->position.y = body->radius / 2;
            body->velocity.y *= -0.9;
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
        Graphics::DrawLine(bodies.back()->position.x, bodies.back()->position.y, bodies.back()->position.x + direction.x, bodies.back()->position.y + direction.y, 0xFFFF0000);
    }

	
    for (int i = 0; i < NUM_BODIES; i++)
    {
		int currBody = i;
        int nextBody = (i + 1) % NUM_BODIES;
		Graphics::DrawLine(bodies[currBody]->position.x, bodies[currBody]->position.y, bodies[nextBody]->position.x, bodies[nextBody]->position.y, 0xFFFFFFFF);
    }

    Graphics::DrawLine(bodies[0]->position.x, bodies[0]->position.y, bodies[2]->position.x, bodies[2]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(bodies[1]->position.x, bodies[1]->position.y, bodies[3]->position.x, bodies[3]->position.y, 0xFFFFFFFF);

    for(auto body:bodies)
    {
        //Draw the bob
        Graphics::DrawFillCircle(body->position.x, body->position.y, body->radius, 0xFFAAFFFF);
       
	}

    Graphics::RenderFrame();


}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto body : bodies)
    {
        delete body; 
    }
    Graphics::CloseWindow();
}
 