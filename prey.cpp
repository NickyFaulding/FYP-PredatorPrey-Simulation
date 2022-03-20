#include "animal.cpp"
#include "food.cpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <vector>

#pragma once

class Prey : public Animal
{
private:
    float wanderAngle = 0;
    float displacementRange = 0.3;

    void eat()
    {
        age -= 50;
        ++foodEaten;
        maxVelocity -= 0.1;
        size += 0.1;
        hunger -= 25;

        if (foodEaten >= 10)
        {
            createNewAnimal = true;
            foodEaten = 0;
        }
    }

public:
    Prey()
    {
        maxAge = 250;
        colour = RAYWHITE;
        size = 5;
        hunger = 80;
        maxVelocity = 5;
        position = {550, 315}; // center
    }

    void move(std::vector<Food> &allFood)
    {
        if (hungry())
        {
            seekFood(allFood);
        }
        else
        {
            wander();
        }
    }

    void wander()
    {
        Vector2 wanderDirection = velocity;
        wanderDirection = Vector2Scale(wanderDirection, 15);
        wanderDirection = Vector2Add(wanderDirection, position);

        DrawCircleV(wanderDirection, 5, BLUE); // debug

        float wanderRadius = 25;
        DrawCircleLines(wanderDirection.x, wanderDirection.y, wanderRadius, WHITE); // debug
        DrawLineV(position, wanderDirection, WHITE);

        float x = wanderRadius * cos(wanderAngle);
        float y = wanderRadius * sin(wanderAngle);

        wanderDirection.x += x;
        wanderDirection.y += y;

        DrawCircleV(wanderDirection, 5, GREEN); // debug
        DrawLineV(position, wanderDirection, WHITE);

        Vector2 steering = Vector2Subtract(position, wanderDirection);

        seek(steering);

        wanderAngle += GetRandomValue(-displacementRange, displacementRange);
    }

    void seek(Vector2 target)
    {
        Vector2 desiredVelocity =
            Vector2Scale(
                Vector2Normalize(
                    Vector2Subtract(target, position)),
                maxVelocity);

        Vector2 steering = Vector2Subtract(desiredVelocity, velocity);

        steering.x = trunc(steering.x);
        steering.y = trunc(steering.y);

        velocity.x = trunc(velocity.x + steering.x);
        velocity.y = trunc(velocity.y + steering.y);

        position = Vector2Add(position, velocity);
    }

    void seekFood(std::vector<Food> &allFood)
    {
        if (allFood.size() > 0 && alive && hungry())
        {
            std::vector<float> distances;
            Vector2 pos = getPos();

            std::for_each(allFood.begin(), allFood.end(), [&pos, &distances](Food &f)
                          { float d = Vector2Distance(pos, f.getPos());
                        distances.push_back(d); });

            std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

            Food &food = allFood.at(std::distance(distances.begin(), result));

            Vector2 target = food.getPos();

            seek(target);

            if (Vector2Distance(position, target) < 10)
            {
                eat();
                food.setEaten();
            }

            // https://www.red3d.com/cwr/steer/gdc99/
        }
    }
};