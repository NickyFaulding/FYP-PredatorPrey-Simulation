#include "predator.h"

Predator::Predator(int birthRate, int maxChildren) : maxChildren(maxChildren), birthRate(birthRate)
{
    position = randomHole();
    velocity = {RandomNumberGenerator(-1, 1), RandomNumberGenerator(-1, 1)}; // random velocity when born
    maxAge = 150;
    age = (RandomNumberGenerator(1, 25));
    colour = RED;
    size = 3;
    maxSpeed = 3;
    hunger = 60;
    maxHunger = 100;
    hungry = false;
}

void Predator::eat()
{
    ++foodEaten;
    maxSpeed = 3;

    if (hungry)
    {
        hunger -= 50;
    }
    // maybe consider the size of prey eaten.

    if (foodEaten == birthRate)
    {
        foodEaten = 0;
        createNewAnimal = true;

        if (GetRandomValue((maxChildren - children), maxChildren) <= 0) // more likely to die after having max children.
        {
            death();
        }
    }
}

void Predator::decreaseMaxHunger()
{
    // if prey population  > predator population then
    maxHunger = 400;
}
void Predator::increaseMaxHunger()
{
    // if prey population < predator population then
    maxHunger = 50;
}
void Predator::normalMaxHunger()
{
    // if none of the above then
    maxHunger = 100;
}

Vector2 Predator::randomHole()
{
    switch (GetRandomValue(0, 3))
    {
    case 0:
        return {300, 50}; // top left
    case 1:
        return {300, 800}; // bottom left
    case 2:
        return {1400, 800}; // bottom right
    case 3:
        return {1400, 50}; // top right
    }
}

void Predator::move(std::vector<Predator> const &neighbors) // not hungry or no prey
{
    acceleration = {0, 0};
    separate(neighbors);
    wander();
    updatePosition();
}

void Predator::move(Vector2 const &nearestPreyPos, Vector2 const nearestPreyVel, std::vector<Predator> const &neighbors) // hungry
{
    // updatePosition();
    acceleration = {0, 0};
    separate(neighbors);
    pursuit(nearestPreyPos, nearestPreyVel);
    updatePosition();
}

void Predator::pursuit(Vector2 const &targetPosition, Vector2 const &targetVelocity)
{
    Vector2 prediction = Vector2Scale(targetVelocity, 3);
    Vector2 futurePos = Vector2Add(targetPosition, prediction);

    seek(futurePos);

    if (maxSpeed != 3.2)
    {
        maxSpeed += 0.01;
    }
}

void Predator::separate(std::vector<Predator> const &neigbours)
{
    if (neigbours.size() > 0)
    {
        Vector2 sum = {0, 0};

        for (Predator n : neigbours) // stl was being weird...
        {
            sum = Vector2Add(sum, n.getPos());
        }

        sum.x = sum.x / neigbours.size();
        sum.y = sum.y / neigbours.size();

        Vector2 steering = Vector2Subtract(position, sum);
        steering = limitForce(steering);
        acceleration = Vector2Add(steer(steering), acceleration);
    }
}

void Predator::draw()
{
    adjustPosition();
    if (alive)
    {
        increaseAge();
        updateHunger();
        DrawCircleV(position, size, colour);
    }
}