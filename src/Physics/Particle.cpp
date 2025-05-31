#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    this->radius = 4;
    std::cout << "Particle constructor called!" << std::endl;
}
Particle::Particle(float x, float y, float mass, int radius) {
    this->position = Vec2(x, y);
    this->mass = mass;
    this->radius = radius;
    std::cout << "Particle constructor called!" << std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}
