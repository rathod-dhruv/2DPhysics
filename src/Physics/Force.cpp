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

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k)
{
    //Calculate the vector from the particle to the anchor point
    Vec2 distance = particle.position - anchor;

    //Calculate the current length of the spring
    float currentLength = distance.Magnitude();

    //Calculate the direction of the spring force (unit vector)
    Vec2 springDirection = distance.UnitVector() * -1.0;

    //Calculate the magnitude of the spring force using Hooke's Law: F = -k * x
    float springMagnitude = k * (currentLength - restLength);

    //Calculate the final spring force vector
    Vec2 springForce = springDirection * springMagnitude;
    return springForce;

}

Vec2 Force::GenerateGravitationalForce(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance)
{

	//Calculate the distance between the two particles
	Vec2 d = b.position - a.position;

	float distanceSquared = d.MagnitudeSquared();

    //Calculate the direction of the attraction force
	Vec2 attractionDirection = d.UnitVector();

    //Clamp the values of the distance (to allow for some interesting visuals effects...not real physics)
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);


    //Calculate the strgenth of  the attraction force
	float attractionMagnitude = (G * a.mass * b.mass) / distanceSquared;

    //Calculate the final resulting attraction force vector
	Vec2 attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}