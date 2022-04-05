#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "raylib.h"
#include "prey.h"
#include "predator.h"
#include "food.h"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

class Environment
{
private:
    int preyCount;
    int foodCount;
    int predatorCount;

    // int preyLimit;
    // int preadtorLimit;
    int foodLimit;

    float timePassed;

    const int SIZE = 20;
    const int cellSize = 32;

    std::vector<Food> allFood;
    std::vector<Prey> allPrey;
    std::vector<Predator> predators;

    std::vector<std::string> preyData;
    std::vector<std::string> predatorData;

public:
    Environment();
    Environment(int preyCount, int predatorCount, int foodCount, int foodLimit); // allow creating an environment without changing the class.

    void addFood();
    void addFood(int n);
    void addPredator(); 
    void addPredator(int n);
    void addPrey();
    void addPrey(int n);

    Prey findNearestPrey(Predator const &p);
    Predator findNearestPredator(Prey const &p);
    Food findNearestFood(Prey const &p);

    // functors for removes are in cpp

    void removeSpecificPrey(Prey const &p);
    void removeSpecificFood(Food const &f);
    void removeDead();
    void spawnRandomAmountOfFood();

    int exportDebugDataPrey();
    int exportDebugDataPredator();
    int exportDebugDataFood();

    void drawDebugData();
    void debugKeyFunctions();
    void draw();

    int getTimePassed();

    std::vector<std::string> getPredatorData();
    std::vector<std::string> getPreyData();

    void update();
};

#endif