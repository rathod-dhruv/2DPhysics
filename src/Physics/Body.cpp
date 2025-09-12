#include "Body.h"
#include <iostream>

Body::Body(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    if(mass != 0.0)
        this->invMass = 1.0/mass;
    else
        this->invMass = 0.0;
    this->radius = 4;
    std::cout << "Body constructor called!" << std::endl;
}

void Body::Integrate(float dt)
{
    acceleration = sumForces * invMass;

    this->velocity += this->acceleration * dt;

    //Eular Integrations
    position += velocity * dt;

    ClearForces();
}

void Body::AddForce(const Vec2& force)
{
    sumForces += force;
}

void Body::ClearForces()
{
    sumForces = Vec2(0, 0);
}

Body::~Body() {
    std::cout << "Body destructor called!" << std::endl;
}
