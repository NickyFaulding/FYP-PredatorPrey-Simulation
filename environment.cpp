#include "environment.h"

Environment::Environment()
{
    preyCount = 50;
    predatorCount = 3;
    foodCount = 10;

    addPrey(preyCount);
    addPredator(predatorCount);
    addFood(foodCount);
}

Environment::Environment(
    int preyCount,
    int predatorCount,
    int foodCount,
    int foodLimit,
    int preyLimit,
    int predatorLimit) : preyCount(preyCount),
                         foodCount(foodCount),
                         foodLimit(foodLimit),
                         preyLimit(preyLimit),
                         predatorLimit(predatorLimit),
                         predatorCount(predatorCount)
{
    addPrey(preyCount);
    addPredator(predatorCount);
    addFood(foodCount);

    if (predatorLimit == 444)
    {
        ratioed = true;
    }
}

struct UpdatePrey // functor
{
    UpdatePrey(std::vector<Predator> &predators,
               std::vector<Food> &allFood,
               std::vector<Prey> &allPrey,
               int preyLimit) : predators(predators), allPrey(allPrey), allFood(allFood), preyLimit(preyLimit) {}

    void operator()(Prey &p)
    {
        if (p.isAlive() && predators.size() > 0) // prey is alive and there are predators
        {
            if (allFood.size() != 0 && p.isHungry()) // hungry and there is food
            {
                const Food &nearestF = findNearestFood(p);
                const Predator &nearestP = findNearestPredator(p);

                p.move(nearestF.getPos(), nearestP.getPos(), nearestP.getVel(), findNeighbours(p));

                if (Vector2Distance(p.getPos(), nearestF.getPos()) < 5)
                {
                    p.eat();
                    removeSpecificFood(nearestF);
                }

                if (p.getCreateNewAnimal())
                {
                    p.newAnimalCreated();
                    addPrey(GetRandomValue(1, 2)); // new prey made
                }
            }
            else
            {
                // no food or not hungry but there are predators
                const Predator &nearestP = findNearestPredator(p);

                p.move(nearestP.getPos(), nearestP.getVel(), findNeighbours(p));
            }
        }
        else if (p.isAlive() && allFood.size() > 0) // there's only food. this should not happen but just in case...
        {
            const Food &nearestF = findNearestFood(p);

            p.move(nearestF.getPos(), findNeighbours(p));

            if (Vector2Distance(p.getPos(), nearestF.getPos()) < 5 && p.isHungry())
            {
                p.eat();
                removeSpecificFood(nearestF);
            }

            if (p.getCreateNewAnimal())
            {
                p.newAnimalCreated();
                addPrey(GetRandomValue(1, 2)); // create 1-2 children.
            }
        }
        else // no food, no prey
        {
            p.move(findNeighbours(p));
        }
    }

    Food findNearestFood(Prey const &p)
    {
        std::vector<float> distances;
        Vector2 pos = p.getPos();

        std::for_each(allFood.begin(), allFood.end(), [&pos, &distances](Food &f)
                      { float d = Vector2Distance(pos, f.getPos());
                        distances.push_back(d); });

        std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

        Food &food = allFood.at(std::distance(distances.begin(), result));

        return food;
    }

    Predator findNearestPredator(Prey const &p)
    {
        std::vector<float> distances;
        Vector2 pos = p.getPos();

        std::for_each(predators.begin(), predators.end(), [&pos, &distances](Predator &p)
                      { float d = Vector2Distance(pos, p.getPos());
                            distances.push_back(d); });

        std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

        Predator &predator = predators.at(std::distance(distances.begin(), result));

        return predator;
    }

    std::vector<Prey> findNeighbours(Prey &p)
    {
        float desiredSeparation = 10;
        std::vector<Prey> neighbours;
        std::for_each(allPrey.begin(), allPrey.end(), [&p, &neighbours, desiredSeparation](Prey &n)
                      { float d = Vector2Distance(p.getPos(), n.getPos());
                      if(!(n == p) && (d < desiredSeparation))
                      {
                        neighbours.push_back(n);
                      } });

        return neighbours;
    }

    void removeSpecificFood(Food const &f)
    {
        allFood.erase(std::remove(allFood.begin(), allFood.end(), f), allFood.end());
    }

    void addPrey(int n)
    {
        if (allPrey.size() < preyLimit)
        {
            for (int i = 0; i < n; ++i)
            {
                allPrey.push_back(Prey());
            }
        }
    }

    std::vector<Predator> &predators;
    std::vector<Prey> &allPrey;
    std::vector<Food> &allFood;
    int preyLimit;
};

struct UpdatePredator // functor
{
    UpdatePredator(std::vector<Prey> &allPrey, std::vector<Predator> &predators, int predatorLimit) : allPrey(allPrey), predators(predators), predatorLimit(predatorLimit)
    {
    }

    void operator()(Predator &p)
    {
        decideHungerLevels(p); // experimental

        if (p.isAlive())
        {
            if ((allPrey.size() > 0) && p.isHungry() && (predators.size() < predatorLimit)) // there are prey
            {
                const Prey &nearestPrey = findNearestPrey(p);

                p.move(nearestPrey.getPos(), nearestPrey.getVel(), findNeighbours(p));

                if (Vector2Distance(p.getPos(), nearestPrey.getPos()) < 3)
                {

                    p.eat();
                    removeSpecificPrey(nearestPrey);
                }

                if (p.getCreateNewAnimal())
                {
                    p.newAnimalCreated();
                    addPredator(1); // new predator made
                }
            }
            else
            {
                p.move(findNeighbours(p));
            }
        }
    }

    Prey findNearestPrey(Predator const &p)
    {
        std::vector<float> distances;
        Vector2 pos = p.getPos();

        std::for_each(allPrey.begin(), allPrey.end(), [&pos, &distances](Prey &p)
                      { float d = Vector2Distance(pos, p.getPos());
                        distances.push_back(d); });

        std::vector<float>::iterator result = std::min_element(std::begin(distances), std::end(distances));

        Prey &prey = allPrey.at(std::distance(distances.begin(), result));

        return prey;
    }

    void decideHungerLevels(Predator &p)
    {
        if (allPrey.size() > 5 * (predators.size()))
        {
            p.increaseMaxHunger();
        }
        else if (allPrey.size() < predators.size())
        {
            p.decreaseMaxHunger();
        }
        else
        {
            p.normalMaxHunger();
        }
    }

    std::vector<Predator> findNeighbours(Predator &p)
    {
        float desiredSeparation = 5;
        std::vector<Predator> neighbours;
        std::for_each(predators.begin(), predators.end(), [&p, &neighbours, desiredSeparation](Predator &n)
                      { float d = Vector2Distance(p.getPos(), n.getPos());
                      if(!(n == p) && (d < desiredSeparation))
                      {
                        neighbours.push_back(n);
                      } });

        return neighbours;
    }

    void addPredator(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            predators.push_back(Predator());
        }
    }

    void removeSpecificPrey(Prey const &p)
    {
        allPrey.erase(std::remove(allPrey.begin(), allPrey.end(), p), allPrey.end());
    }

    void removeSpecificPredator(Predator const &p)
    {
        predators.erase(std::remove(predators.begin(), predators.end(), p), predators.end());
    }

    std::vector<Prey> &allPrey;
    std::vector<Predator> &predators;
    int predatorLimit;
    bool ratioed = false;
};

void Environment::update()
{
    // using member pointers here
    std::for_each(predators.begin(), predators.end(), UpdatePredator(allPrey, predators, predatorLimit));
    std::for_each(allPrey.begin(), allPrey.end(), UpdatePrey(predators, allFood, allPrey, preyLimit));

    if ((getTimePassed() % 100) == 0) // limit amount we push to the vector.
    {
        predatorData.push_back(std::to_string(predators.size()));
        preyData.push_back(std::to_string(allPrey.size()));
    }

    if ((foodLimit - allFood.size()) > 0)
    {
        spawnRandomAmountOfFood();
    }
    debugKeyFunctions();
    removeDead();
}

void Environment::addPrey()
{
    allPrey.push_back(Prey());
}

void Environment::addPrey(int n)
{
    for (int i = 0; i < n; ++i)
    {
        addPrey();
    }
}

void Environment::addPredator()
{
    predators.push_back(Predator());
}

void Environment::addPredator(int n)
{
    for (int i = 0; i < n; ++i)
    {
        addPredator();
    }
}

void Environment::addFood()
{
    allFood.push_back(Food());
}

void Environment::addFood(int n)
{
    for (int i = 0; i < n; ++i)
    {
        addFood();
    }
}

struct RemovePrey // functor
{
    RemovePrey(std::vector<Prey> &allPrey) : allPrey(allPrey) {}

    void operator()(Prey &p)
    {
        if (p.isAlive() == false)
        {
            allPrey.erase(std::remove(allPrey.begin(), allPrey.end(), p), allPrey.end());
        }
    }

    std::vector<Prey> &allPrey;
};

struct RemovePredator // functor
{
    RemovePredator(std::vector<Predator> &predators) : predators(predators) {}

    void operator()(Predator &p)
    {
        if (p.isAlive() == false)
        {
            predators.erase(std::remove(predators.begin(), predators.end(), p), predators.end());
        }
    }

    std::vector<Predator> &predators;
};

struct RemoveFood // functor
{
    RemoveFood(std::vector<Food> &allFood) : allFood(allFood) {}

    void operator()(Food &f)
    {
        if (f.isEaten())
        {
            allFood.erase(std::remove(allFood.begin(), allFood.end(), f), allFood.end());
        }
    }

    std::vector<Food> &allFood;
};

void Environment::removeDead()
{
    if (allPrey.size() > 0)
    {
        std::for_each(allPrey.begin(), allPrey.end(), RemovePrey(allPrey));
    }

    if (predators.size() > 0)
    {
        std::for_each(predators.begin(), predators.end(), RemovePredator(predators));
    }

    if (allFood.size() > 0)
    {
        std::for_each(allFood.begin(), allFood.end(), RemoveFood(allFood));
    }
}

void Environment::spawnRandomAmountOfFood()
{
    addFood(GetRandomValue(0, (foodLimit - allFood.size())));
}

int Environment::getTimePassed()
{
    ++timePassed;
    return timePassed / 10;
}

std::vector<std::string> Environment::getPredatorData()
{
    return predatorData;
}

std::vector<std::string> Environment::getPreyData()
{
    return preyData;
}

void Environment::drawDebugData()
{
    DrawFPS(50, 5);
    DrawText(TextFormat("PREY: %03i", allPrey.size()), 50, 540, 15, WHITE);
    DrawText(TextFormat("PREDATORS: %03i", predators.size()), 50, 560, 15, WHITE);
    DrawText(TextFormat("FOOD: %03i", allFood.size()), 50, 580, 15, WHITE);
    DrawText(TextFormat("Hours Passed: %04i", getTimePassed()), 50, 600, 15, WHITE);

    // DrawText(TextFormat("MOUSE X: %03i", GetMouseX()), 50, 25, 15, WHITE);
    // DrawText(TextFormat("MOUSE Y: %03i", GetMouseY()), 50, 45, 15, WHITE);
}

int Environment::exportDebugDataPrey()
{
    return allPrey.size();
}

int Environment::exportDebugDataPredator()
{
    return predators.size();
}

int Environment::exportDebugDataFood()
{
    return allFood.size();
}

void Environment::debugKeyFunctions()
{
    if (IsKeyPressed(KEY_UP))
    {
        addPredator();
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        addPrey();
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        addFood();
    }
}

void Environment::draw()
{
    ClearBackground(BLACK);
    // DrawRectangleLines(235, 5, (SIZE * cellSize), (SIZE * cellSize), DARKGRAY);

    DrawCircle(550, 300, 10, DARKGREEN); // prey spawn
    DrawCircle(950, 300, 10, DARKGREEN); // prey spawn
    DrawCircle(735, 600, 10, DARKGREEN); // prey spawn

    DrawCircle(300, 50, 10, BROWN);   // predator spawn
    DrawCircle(300, 800, 10, BROWN);  // predator spawn
    DrawCircle(1400, 800, 10, BROWN); // predator spawn
    DrawCircle(1400, 50, 10, BROWN);  // predator spawn

    if (allFood.size() > 0)
    {
        std::for_each(allFood.begin(), allFood.end(), [](Food &f)
                      { f.draw(); });
    }

    if (allPrey.size() > 0)
    {
        std::for_each(allPrey.begin(), allPrey.end(), [](Prey &p)
                      { p.draw(); });
    }

    if (predators.size() > 0)
    {
        std::for_each(predators.begin(), predators.end(), [](Predator &p)
                      { p.draw(); });
    }
}