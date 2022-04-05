#ifndef FOOD_H
#define FOOD_H

#include "raylib.h"
#include "randm.cpp"

class Food
{
private:
    Vector2 upperBounds = {1599, 899};
    Vector2 lowerBounds = {1, 1};

    int age;
    int maxAge;
    int size;
    Color colour;
    Vector2 position;

    bool eaten;
    bool spoiled;

    void randomLocation();

public:
    bool operator==(const Food &other) const;
    Food();
    void const setEaten();
    void increaseAge();
    bool isEaten();
    Vector2 getPos() const;
    void draw();
};

#endif