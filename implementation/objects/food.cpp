#include "header\food.h"

Food::Food()
{
    randomLocation();
    eaten = false;
    spoiled = false;
    age = 0;
    maxAge = 350;
    size = 1;
    colour = ORANGE;
}

bool Food::operator==(const Food &other) const
{
    return position.x == other.position.x && position.y == other.position.y; // comparing positions
}

void Food::randomLocation()
{
    position.x = RandomNumberGenerator(lowerBounds.x, upperBounds.x);
    position.y = RandomNumberGenerator(lowerBounds.y, upperBounds.y);
}

void const Food::setEaten()
{
    eaten = true;
}

void Food::increaseAge()
{
    ++age;

    if (age >= maxAge)
    {
        spoiled = true;
    }
}

bool Food::isEaten()
{
    if (eaten || spoiled)
    {
        return true;
    }
    else
        return false;
}

Vector2 Food::getPos() const
{
    return position;
}

void Food::draw()
{
    if (!eaten && !spoiled)
    {
        increaseAge();
        DrawCircleV(position, size, colour);
    }
}