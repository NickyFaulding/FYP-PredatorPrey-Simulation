#ifndef PREY_H
#define PREY_H

#include "raylib.h"
#include "raymath.h"
#include "animal.h"
#include <algorithm>
#include <vector>

class Prey : public Animal
{
private:
    int id;
    Vector2 spawnLocation();

public:
    Prey();
    void eat();
    void move(std::vector<Prey> const &neighbours);
    void move(Vector2 const &nearestFoodPos, std::vector<Prey> const &neighboursPos);                                        // only food
    void move(Vector2 const &nearestPredatorPos, Vector2 const &nearestPredatorVel, std::vector<Prey> const &neighboursPos); // no food with predators
    void move(Vector2 const &nearestFoodPos, Vector2 const &nearestPredatorPos, Vector2 const &nearestPredatorVel, std::vector<Prey> const &neighboursPos);
    void const evade(Vector2 const &predatorPosition, Vector2 const &predatorVelocity);
    void separate(std::vector<Prey> const &neighbours);
};

#endif