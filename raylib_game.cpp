#include "raylib.h"
#include "agent.cpp"
#include "goal.cpp"
#include <vector>
#include <algorithm>

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 900;
int screenHeight = 645;
int cellSize = 32;
int agentCount = 1;
int foodCount = 5;

// replace this since its not very descriptive...
int SIZE = 20;

Color gridColour = DARKBLUE;

std::vector<Agent> agents;
std::vector<Food> foods;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void); // Update and Draw one frame

// main------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Raylib Preadtor Prey Simulation.");

    for (int i = 0; i < agentCount; ++i)
    {
        agents.push_back(Agent());
    }

    // foods.push_back(Food());

    for (int i = 0; i < foodCount; ++i)
    {
        foods.push_back(Food());
    }

    //--------------------------------------------------------------------------------------
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
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
    std::for_each(agents.begin(), agents.end(), [](Agent &a)
                  { a.seekFood(foods); if(a.getCreateNewAgent()) {agents.push_back(Agent(a.getPos())); a.newAgentCreated();} });

    // debug keys, create food UP; create agent DOWN;
    if (IsKeyPressed(KEY_UP))
    {
        foods.push_back(Food());
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        agents.push_back(Agent());
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    // draw testing data
    DrawFPS(50, 5);
    DrawText(TextFormat("MOUSE X: %03i", GetMouseX()), 50, 25, 15, BLACK);
    DrawText(TextFormat("MOUSE Y: %03i", GetMouseY()), 50, 45, 15, BLACK);
    DrawText(TextFormat("AGENTS: %03i", agents.size()), 50, 140, 15, BLACK);
    DrawText(TextFormat("FOOD: %03i", foods.size()), 50, 160, 15, BLACK);

    // creating the bounds area.
    ClearBackground(WHITE);
    DrawRectangleLines(235, 5, (SIZE * cellSize), (SIZE * cellSize), BLACK);

    // drawing all food.
    std::for_each(foods.begin(), foods.end(), [](Food &f)
                  { f.drawFood(); });

    // drawing all agents.
    std::for_each(agents.begin(), agents.end(), [](Agent &a)
                  { a.drawAgent(); });

    EndDrawing();
    //----------------------------------------------------------------------------------
}