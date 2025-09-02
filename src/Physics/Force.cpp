#include "./Force.h"
#include "Force.h"


Vec2 Force::GenerateDragForce(const Particle& particle, float k)
{   
    Vec2 dragForce = Vec2(0,0);

    if(particle.velocity.MagnitudeSquared() > 0)
    {
        //Calculate drag direction (inverse of velocity unit vector)
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        //Calculate the drag magnitude, k * v^2
        float dragMagnitude = particle.velocity.MagnitudeSquared() * k;

        //Calculate the drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k)
{
    Vec2 frictionForce = Vec2(0,0);

    //Calculate the friction direction (inverse of velocity unit vector)
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

    //Calculate the friction magnitude, k * v^2
    float frictionMagnitude =  k;

    //Calculate the friction force with direction and magnitude
    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}