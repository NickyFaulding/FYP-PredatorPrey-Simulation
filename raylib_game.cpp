#include "raylib.h"
#include "predator.cpp"
#include "prey.cpp"
#include "food.cpp"
#include "randm.cpp"
#include <math.h>
#include <vector>
#include <algorithm>

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 1000;
int screenHeight = 700;
int cellSize = 32;

int preyCount = 5;
int foodCount = 10;
int predatorCount = 0;

float hoursPassed;

// replace this since its not very descriptive...
int SIZE = 20;

Color gridColour = DARKBLUE;

std::vector<Food> allFood;
std::vector<Prey> allPrey;
std::vector<Predator> predators;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void); // Update and Draw one frame
float getHoursPassed();
void removeDead();
void spawnRandomAmountOfFood();

// main------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Raylib Predator Prey Simulation.");

    hoursPassed = 0.0f;

    for (int i = 0; i < preyCount; ++i)
    {
        allPrey.push_back(Prey());
    }

    // allFood.push_back(Food());

    for (int i = 0; i < foodCount; ++i)
    {
        allFood.push_back(Food());
    }

    for (int i = 0; i < predatorCount; ++i)
    {
        predators.push_back(Predator());
    }

    //--------------------------------------------------------------------------------------
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 65, 1);
#else
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    std::for_each(allPrey.begin(), allPrey.end(), [](Prey &prey)
                  { prey.move(allFood); if(prey.getCreateNewAnimal()) {prey.newAnimalCreated(); allPrey.push_back(Prey()); } });

    std::for_each(predators.begin(), predators.end(), [](Predator &predator)
                  { predator.move(allPrey); if(predator.getCreateNewAnimal()) {predator.newAnimalCreated(); predators.push_back(Predator()); } });

    // debug keys, create food UP; create agent DOWN;
    if (IsKeyPressed(KEY_UP))
    {
        allFood.push_back(Food());
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        allPrey.push_back(Prey());
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        predators.push_back(Predator());
    }

    removeDead();

    if (fmod(hoursPassed, 7) == 0.0f)
    {
        spawnRandomAmountOfFood();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    // draw testing data
    DrawFPS(50, 5);
    DrawText(TextFormat("MOUSE X: %03i", GetMouseX()), 50, 25, 15, WHITE);
    DrawText(TextFormat("MOUSE Y: %03i", GetMouseY()), 50, 45, 15, WHITE);
    DrawText(TextFormat("Hours Passed: %4.2f", getHoursPassed()), 50, 65, 15, WHITE);
    DrawText(TextFormat("PREY: %03i", allPrey.size()), 50, 140, 15, WHITE);
    DrawText(TextFormat("PREDATORS: %03i", predators.size()), 50, 160, 15, WHITE);
    DrawText(TextFormat("FOOD: %03i", allFood.size()), 50, 180, 15, WHITE);

    // creating the bounds area.
    ClearBackground(BLACK);
    DrawRectangleLines(235, 5, (SIZE * cellSize), (SIZE * cellSize), DARKGRAY);
    DrawCircle(550, 315, 10, DARKGREEN);

    // drawing all food.
    std::for_each(allFood.begin(), allFood.end(), [](Food &food)
                  { food.draw(); });

    // drawing all preys.
    std::for_each(allPrey.begin(), allPrey.end(), [](Prey &prey)
                  { prey.draw(); });

    std::for_each(predators.begin(), predators.end(), [](Predator &predator)
                  { predator.draw(); });

    EndDrawing();
    //----------------------------------------------------------------------------------
}

float getHoursPassed()
{
    ++hoursPassed;
    return hoursPassed / 10;
}

void removeDead()
{
    std::for_each(allPrey.begin(), allPrey.end(), [](Prey &prey)
                  { 
        if(prey.isDead())
        {
            allPrey.erase(std::remove(allPrey.begin(), allPrey.end(), prey), allPrey.end()); 
        } });

    std::for_each(predators.begin(), predators.end(), [](Predator &predator)
                  { 
        if(predator.isDead())
        {
            predators.erase(std::remove(predators.begin(), predators.end(), predator), predators.end()); 
        } });

    std::for_each(allFood.begin(), allFood.end(), [](Food &food)
                  {
        if (food.isEaten())
        {
            allFood.erase(std::remove(allFood.begin(), allFood.end(), food), allFood.end());
        } });
}

void spawnRandomAmountOfFood()
{
    for (int i = 0; i < RandomNumberGenerator(0, 10); ++i)
    {
        allFood.push_back(Food());
    }
}