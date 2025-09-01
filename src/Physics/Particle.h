#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    int radius;
    Vec2 sumForces;

    float mass;
    float invMass;

    Particle(float x, float y, float mass);
    void Integrate(float dt);
    void AddForce(const Vec2& force);
    void ClearForces();
    ~Particle();
};

#endif
