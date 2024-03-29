#include "header\prey.h"

Prey::Prey()
{
    maxAge = 250;
    colour = RAYWHITE;
    size = 2;
    hunger = 80;
    maxHunger = 100;
    maxSpeed = 5;
    position = spawnLocation();                                              // random spawn
    velocity = {RandomNumberGenerator(-1, 1), RandomNumberGenerator(-1, 1)}; // random velocity when born
}

Vector2 Prey::spawnLocation()
{
    switch (GetRandomValue(0, 2))
    {
    case 0:
        return {550, 315}; // left
        break;
    case 1:
        return {950, 300}; // right
        break;
    case 2:
        return {735, 600}; // center
        break;
    }

    return {735, 600};
}

void Prey::eat()
{
    ++foodEaten;

    if (maxSpeed > 0.2)
    {
        maxSpeed -= 0.1;
    }
    size += 0.1;

    hunger -= 20;
    if (hunger <= 0)
    {
        hungry = false;
        hunger = 0;
    }

    if (foodEaten == 5)
    {
        foodEaten = 0;
        createNewAnimal = true;
    }
}

void Prey::move(std::vector<Prey> const &neighbours) // no food or predators...
{
    acceleration = {0, 0};
    separate(neighbours);
    wander();
    updatePosition();
}

void Prey::move(Vector2 const &nearestFoodPos, std::vector<Prey> const &neighbours) // only food...
{
    acceleration = {0, 0};
    separate(neighbours);
    seek(nearestFoodPos);
    updatePosition();
}

void Prey::move(Vector2 const &nearestPredatorPos, Vector2 const &nearestPredatorVel, std::vector<Prey> const &neighbours) // no food
{
    acceleration = {0, 0};

    separate(neighbours);

    if (Vector2Distance(position, nearestPredatorPos) <= 15)
    {
        evade(nearestPredatorPos, nearestPredatorVel);
    }
    else
    {
        wander();
    }

    updatePosition();
}

void Prey::move(Vector2 const &nearestFoodPos, Vector2 const &nearestPredatorPos, Vector2 const &nearestPredatorVel, std::vector<Prey> const &neighbours) // food and predators
{
    // seek nearest food but avoid predators.

    acceleration = {0, 0};

    separate(neighbours);

    if (Vector2Distance(position, nearestPredatorPos) <= 15)
    {
        evade(nearestPredatorPos, nearestPredatorVel);
    }
    else
    {
        seek(nearestFoodPos);
    }
    updatePosition();
}

void const Prey::evade(Vector2 const &targetPosition, Vector2 const &targetVelocity)
{
    Vector2 prediction = Vector2Scale(targetVelocity, 2);
    Vector2 futurePos = Vector2Add(targetPosition, prediction);

    futurePos = Vector2Scale(futurePos, -1);

    seek(futurePos);
}

void Prey::separate(std::vector<Prey> const &neighbours)
{
    if (neighbours.size() > 0)
    {
        Vector2 sum = {0, 0};

        for (Prey n : neighbours) // stl was being weird...
        {
            sum = Vector2Add(sum, n.getPos());
        }

        sum.x = sum.x / neighbours.size();
        sum.y = sum.y / neighbours.size();

        Vector2 steering = Vector2Subtract(position, sum);
        steering = limitForce(steering);
        acceleration = Vector2Add(steer(steering), acceleration);
    }
}