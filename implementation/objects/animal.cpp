#include "header\animal.h"
#include "id.cpp"

void Animal::increaseAge()
{

    if (hungry)
    {
        age += (hunger * 0.0025);
    }
    else
    {
        age += 0.025;
    }

    if (age >= maxAge)
    {
        death();
    }
}

void Animal::updateHunger()
{
    if (hunger >= (maxHunger * 0.6))
    {
        hungry = true;
    }
    else
    {
        ++hunger;
        hungry = false;
    }

}

int Animal::getHunger()
{
    return hunger;
}

void Animal::seek(Vector2 target)
{
    Vector2 desiredVelocity =
        Vector2Scale(
            Vector2Normalize(
                Vector2Subtract(target, position)),
            maxSpeed);

    Vector2 steering = steer(desiredVelocity);
    acceleration = Vector2Add(steer(steering), acceleration);
}

Vector2 Animal::steer(Vector2 desired)
{
    return Vector2Subtract(desired, velocity);
}

Vector2 Animal::limitForce(Vector2 steeringForce)
{
    if (Vector2Length(steeringForce) > maxForce)
    {
        steeringForce = Vector2Normalize(Vector2Scale(steeringForce, maxForce));
    }
    return steeringForce;
}

void Animal::wander() // perfect!
{
    Vector2 wanderPoint = velocity;
    wanderPoint = Vector2Normalize(wanderPoint);
    wanderPoint = Vector2Scale(wanderPoint, 65);
    wanderPoint = Vector2Add(wanderPoint, position);

    float wanderRadius = 25;

    float theta = wanderTheta + atan2f(position.x, position.y);

    float x = wanderRadius * cos(theta);
    float y = wanderRadius * sin(theta);
    wanderPoint.x += x;
    wanderPoint.y += y;

    Vector2 steering = Vector2Subtract(wanderPoint, position);
    steering = limitForce(steering);

    acceleration = Vector2Add(steer(steering), acceleration);

    float displacementRange = 0.2;
    wanderTheta += RandomNumberGenerator(-displacementRange, displacementRange);
}

void Animal::adjustPosition()
{
    if (position.x < 0)
    {
        position.x = 1600;
    }
    if (position.y < 0)
    {
        position.y = 900;
    }
    if (position.x > 1600)
    {
        position.x = 0;
    }
    if (position.y > 900)
    {
        position.y = 0;
    }
}

void Animal::updatePosition()
{
    acceleration = limitForce(acceleration);
    // update velocity
    velocity = Vector2Add(velocity, acceleration);
    // update position
    position = Vector2Add(position, velocity);
    // reset acceleration
    acceleration = Vector2Zero();
}

bool Animal::operator==(const Animal &other) const
{
    return id == other.id;
}

Animal::Animal()
{
    ID _id;
    id = _id.id;

    mass = 1;
    maxForce = 1;
    wanderTheta = 0;
    age = 1;
    alive = true;
    hungry = false;
    foodEaten = 0;
    children = 0;
    createNewAnimal = false;
    acceleration = Vector2Zero();
}

bool Animal::isHungry() const
{
    return hungry;
}

bool Animal::getCreateNewAnimal()
{
    return createNewAnimal;
}

void Animal::newAnimalCreated()
{
    ++children;
    createNewAnimal = false;
}

int Animal::getChildren() const
{
    return children;
}

Vector2 Animal::getVel() const
{
    return velocity;
}

int Animal::getSpeed() const
{
    return maxSpeed;
}

Vector2 const Animal::getPos() const
{
    return position;
}

void Animal::death()
{
    alive = false;
}

bool Animal::isAlive() const
{
    return alive;
}

void Animal::draw()
{
    adjustPosition();
    if (alive)
    {
        // increaseAge();
        updateHunger();
        DrawCircleV(position, size, colour);
    }
}
