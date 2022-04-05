#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "environment.h"
#include <emscripten.h>

#include <iostream>
#include <fstream>

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

typedef enum Screen
{
    INITIAL,
    SIMULATION
} Screen;

Screen currentScreen;
int screenWidth = 1600;
int screenHeight = 900;

int predatorCount;
int preyCount;
int foodCount;

int predatorLimit;
int preyLimit;
int foodLimit;

bool dividebyfour;
bool disablePreyLimit;

// look into using smart pointers garbage collection.
Environment *environment;

void UpdateDrawFrame(void); // Update and Draw one frame
void write(std::vector<std::string> v);

int main()
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Raylib Predator Prey Simulation.");

    currentScreen = INITIAL;

    predatorCount = 0;
    preyCount = 0;
    foodCount = 0;

    predatorLimit = 0;
    preyLimit = 0;
    foodLimit = 0;

    dividebyfour = false;
    disablePreyLimit = false;

    // EM_ASM(
    //     saveFileFromMEMFSToDisk()
    // );

    emscripten_set_main_loop(UpdateDrawFrame, 65, 1);
    // De-Initialization
    // fclose(file);
    CloseWindow(); // Close window and OpenGL context
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
<<<<<<< HEAD
    switch (currentScreen)
    {
    case INITIAL:
        BeginDrawing();
        predatorCount = GuiSlider((Rectangle){355, 400, 165, 20}, "PREDATOR COUNT", TextFormat("%2.2f", (float)predatorCount), predatorCount, 0, 100);
        preyCount = GuiSlider((Rectangle){355, 420, 165, 20}, "PREY COUNT", TextFormat("%2.2f", (float)preyCount), preyCount, 0, 500);
        foodCount = GuiSlider((Rectangle){355, 440, 165, 20}, "FOOD COUNT", TextFormat("%2.2f", (float)foodCount), foodCount, 0, 500);

        dividebyfour = GuiCheckBox((Rectangle){355, 180, 15, 15}, "RATIO PREDATOR PREY 1:4!", dividebyfour);
        disablePreyLimit = GuiCheckBox((Rectangle){355, 200, 15, 15}, "NO PREY LIMIT", disablePreyLimit);
        if (!dividebyfour)
        {
            predatorLimit = GuiSlider((Rectangle){355, 220, 165, 20}, "PREDATOR LIMIT", TextFormat("%2.2f", (float)predatorLimit), predatorLimit, predatorCount, 100);
        }
        if (!disablePreyLimit)
        {
            preyLimit = GuiSlider((Rectangle){355, 240, 165, 20}, "PREY LIMIT", TextFormat("%2.2f", (float)preyLimit), preyLimit, preyCount, 500);
        }

        foodLimit = GuiSlider((Rectangle){355, 260, 165, 20}, "FOOD LIMIT", TextFormat("%2.2f", (float)foodLimit), foodLimit, foodCount, 500);

        if (dividebyfour)
        {
            predatorLimit = preyLimit / 4;
        }
        if (disablePreyLimit)
        {
            preyLimit = 999;
        }

        if (GuiButton((Rectangle){355, 470, 165, 20}, "START SIMULATION"))
        {
            environment = new Environment(preyCount, predatorCount, foodCount);
            currentScreen = SIMULATION;
        }

        EndDrawing();
        break;
    case SIMULATION:

        // Update
        environment->update();

        // Draw
        BeginDrawing();

        environment->draw();
        environment->drawDebugData();

        if (GuiButton((Rectangle){50, 630, 130, 20}, "STOP SIMULATION"))
        {
            write(environment->getPredatorData());
            emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", "text.txt", "predatorData.txt"));
            write(environment->getPreyData());
            emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", "text.txt", "preyData.txt"));

            currentScreen = INITIAL;
        }

        EndDrawing();
        break;
    }
}

void write(std::vector<std::string> v)
{
    std::ofstream file;
    file.open("text.txt");
    for (int i = 0; i < v.size(); ++i)
    {
        file << v[i] << std::endl;
    }
    file.close();
}