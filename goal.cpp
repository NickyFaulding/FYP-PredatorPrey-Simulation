#include "raylib.h"
#include "randm.cpp"
#include <stdlib.h>
#include <time.h>

#pragma once

class Food
{
private:
    Vector2 position;
    const Vector2 upperBounds = {880, 640};
    const Vector2 lowerBounds = {235, 5};
    const int size = 3;
    Color colour = ORANGE;

    bool eaten;

    void randomLocation()
    {
        position.x = RandomNumberGenerator(lowerBounds.x, upperBounds.x);
        position.y = RandomNumberGenerator(lowerBounds.y, upperBounds.y);
    }

public:
    Food()
    {
        randomLocation();
        eaten = false;
    }

    void setEaten()
    {
        eaten = true;
    }

    void respawn()
    {
        randomLocation();
        eaten = false;
    }

    Vector2 getPos()
    {
        return position;
    }

    void drawFood()
    {
        if (!eaten)
        {
            DrawCircleV(position, size, colour);
        }
        else
        {
            respawn();
        }
    }
};