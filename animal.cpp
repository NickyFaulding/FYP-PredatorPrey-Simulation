#include "raylib.h"
#include "raymath.h"

#pragma once

class Animal
{
protected:
    Vector2 position;
    Vector2 velocity = {0, 0};
    double maxVelocity;

    double size;
    Color colour;

    int age;
    int maxAge;
    int foodEaten;
    bool alive;
    int hunger;

    bool createNewAnimal;

    void increaseAge()
    {
        if (hungry())
        {
            ++age;
        }

        if (age == maxAge)
        {
            death();
        }
    }

    bool hungry()
    {
        return (hunger == 100);
    }

    void increaseHunger()
    {
        if (!hungry())
        {
            hunger++;
        }
    }

    virtual void eat() = 0;

public:
    bool operator==(const Animal &other) const
    {
        return position.x == other.position.x && position.y == other.position.y; // comparing positions
    }

    Animal()
    {
        age = 0;
        alive = true;
        foodEaten = 0;
        createNewAnimal = false;
    }

    virtual ~Animal() = default;

    virtual void move() {}

    bool getCreateNewAnimal()
    {
        return createNewAnimal;
    }

    void newAnimalCreated()
    {
        createNewAnimal = false;
    }

    Vector2 getPos()
    {
        return position;
    }

    void death()
    {
        alive = false;
    }

    bool isDead()
    {
        return !alive;
    }

    void draw()
    {
        if (alive)
        {
            increaseAge();
            increaseHunger();
            DrawCircleV(position, size, colour);
        }
    }
};