#include "raylib.h"
#include "raymath.h"
#include "animal.cpp"
#include "prey.cpp"
#include <algorithm>
#include <vector>

class Predator : public Animal
{
private:
    void eat()
    {
        age -= 25;
        ++foodEaten;
        hunger -= 100;

        if (foodEaten >= 10)
        {
            createNewAnimal = true;
            foodEaten = 0;
        }
    }

public:
    Predator()
    {
        maxAge = 100;
        colour = RED;
        size = 6;
        maxVelocity = 5;
        position = {300, 50};
        hunger = 50;
    }

    void move(std::vector<Prey> &allPrey)
    {
        if (hungry())
        {
            seekFood(allPrey);
        }
        else 
        {
            
        }
    }

    void seekFood(std::vector<Prey> &allPrey)
    {
        if (allPrey.size() > 0 && alive && hungry())
        {
            std::vector<float> distances;
            Vector2 pos = getPos();

            std::for_each(allPrey.begin(), allPrey.end(), [&pos, &distances](Prey &p)
                          { float d = Vector2Distance(pos, p.getPos());
                        distances.push_back(d); });

            std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

            Prey &prey = allPrey.at(std::distance(distances.begin(), result));

            Vector2 target = prey.getPos();

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
                prey.death();
            }
        }
    }
};