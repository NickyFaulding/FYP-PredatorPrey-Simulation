#ifndef ANIMAL
#define ANIMAL

#include "raylib.h"
#include "raymath.h"
#include "randm.cpp"

class Animal
{
protected:
    int id;

    float mass;
    float maxForce;
    float maxSpeed;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float wanderTheta;
    double size;
    Color colour;

    int maxAge;
    int foodEaten;
    int children;
    bool alive;
    int hunger;
    int maxHunger;
    bool hungry;
    bool createNewAnimal;

    // functions
    void increaseAge();
    void updateHunger();
    void seek(Vector2 target);
    void updatePosition();
    void adjustPosition();
    Vector2 steer(Vector2 desired);
    Vector2 limitForce(Vector2 steeringForce);

public:
    float age;

    bool operator==(const Animal &other) const;
    Animal();
    virtual ~Animal() = default;
    virtual void move(){};
    virtual void eat() = 0;
    void death();

    void wander();
    bool isAlive() const;
    bool isHungry() const;
    int getHunger();
    Vector2 const getPos() const;
    Vector2 getVel() const;
    int getSpeed() const;
    int getChildren() const;

    bool getCreateNewAnimal();
    void newAnimalCreated();

    virtual void draw();
};

#endif
