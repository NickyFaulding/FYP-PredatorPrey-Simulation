#include <stdlib.h>
#include <time.h>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "randm.cpp"
#include "goal.cpp"

class Agent
{
private:
    double size = 5;
    const Color colour = BLUE;

    // movement
    Vector2 position;
    Vector2 velocity = {0, 0}; // direction
    double maxVelocity = 4;

    // life
    int age;
    int foodEaten;
    bool alive;
    bool createNewAgent;

    void increaseAge()
    {
        ++age;

        if (age == 100)
        {
            death();
        }
    }

    void death()
    {
        alive = false;
        position.x = 0;
        position.y = 0;
    }

    void eat()
    {
        age -= 50;
        ++foodEaten;
        maxVelocity -= 0.1;
        size += 0.1;

        if (foodEaten > 5)
        {
            createNewAgent = true;
            foodEaten = 0;
        }
    }

public:
    Agent()
    {
        age = 0;
        alive = true;
        createNewAgent = false;
        foodEaten = 0;
        position = {550, 315}; // center
    }

    Agent(Vector2 pos)
    {
        age = 0;
        alive = true;
        createNewAgent = false;
        foodEaten = 0;
        position = pos; // center
    }

    void seekFood(std::vector<Food> &allFood)
    {
        if (alive)
        {
            std::vector<float> distances;
            Vector2 pos = getPos();

            std::for_each(allFood.begin(), allFood.end(), [&pos, &distances](Food &f)
                          { float d = Vector2Distance(pos, f.getPos());
                        distances.push_back(d); });

            std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

            Food &food = allFood.at(std::distance(distances.begin(), result));

            Vector2 target = food.getPos();

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

            if (Vector2Distance(position, target) < 10)
            {
                eat();
                food.respawn();
            }
            // https://www.red3d.com/cwr/steer/gdc99/
        }
    }

    bool getCreateNewAgent()
    {
        return createNewAgent;
    }

    void newAgentCreated()
    {
        createNewAgent = false;
    }

    Vector2 getPos()
    {
        return position;
    }

    void drawAgent()
    {
        if (alive)
        {
            increaseAge();
            DrawCircleV(position, size, colour);
        }
    }
};