#ifndef PREDATOR_H
#define PREDATOR_H

#include "raylib.h"
#include "raymath.h"
#include "animal.h"
#include <algorithm>
#include <vector>

class Predator : public Animal
{
private:
    Vector2 randomHole();
    void separate(std::vector<Predator> const &neigbours);
    void pursuit(Vector2 const &target, Vector2 const &targetVelocity);

public:
    Predator();
    void eat();
    void move(std::vector<Predator> const &neighbors);
    void move(Vector2 const &nearestPreyPos, Vector2 const nearestPreyVel, std::vector<Predator> const &neighbors);
    void decreaseMaxHunger();
    void increaseMaxHunger();
    void normalMaxHunger();
    void draw();
};

#endif