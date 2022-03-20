#include "raylib.h"
#include "randm.cpp"
#include <stdlib.h>
#include <time.h>

#pragma once

class Food
{
private:
    Vector2 upperBounds = {880, 640};
    Vector2 lowerBounds = {235, 5};

    int size = 3;
    Color colour = ORANGE;

    bool eaten;
    bool spoiled;

    int age;
    int maxAge = 350;
    Vector2 position;

    void randomLocation()
    {
        position.x = RandomNumberGenerator(lowerBounds.x, upperBounds.x);
        position.y = RandomNumberGenerator(lowerBounds.y, upperBounds.y);
    }

public:
    bool operator==(const Food &other) const
    {
        return position.x == other.position.x && position.y == other.position.y; // comparing positions
    }

    Food()
    {
        randomLocation();
        eaten = false;
        spoiled = false;
        age = 0;
    }

    void setEaten()
    {
        eaten = true;
    }

    void increaseAge()
    {
        ++age;

        if (age >= maxAge)
        {
            spoiled = true;
        }
    }

    bool isEaten()
    {
        if (eaten || spoiled)
        {
            return true;
        }
        else
            return false;
    }

    Vector2 getPos()
    {
        return position;
    }

    void draw()
    {
        if (!eaten && !spoiled)
        {
            increaseAge();
            DrawCircleV(position, size, colour);
        }
    }
};