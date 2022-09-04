#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "header\environment.h"
#include <emscripten.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
int foodCount = 1; // change

int predatorLimit;
int preyLimit;
int foodLimit;

int predatorBirthRate;
int predatorMaxChildren;

bool dividebyfour;
bool testCase1;
bool enablePreyLimit;
bool enablePredatorLimit;

std::vector<std::string> gPredatorData;
std::vector<std::string> gPreyData;
bool dataWritten;

// look into using smart pointers garbage collection.
Environment *environment;

void UpdateDrawFrame(void); // Update and Draw one frame
void write(std::vector<std::string> v);

int main()
{
    // Initialization

    InitWindow(screenWidth, screenHeight, "Raylib Predator Prey Simulation.");

    currentScreen = INITIAL;
    dataWritten = false;

    predatorCount = 0;
    preyCount = 0;
    foodCount = 0;

    predatorLimit = 0;
    preyLimit = 0;
    foodLimit = 0;

    predatorBirthRate = 1;
    predatorMaxChildren = 1;

    // dividebyfour = false;
    testCase1 = false;
    enablePreyLimit = false;
    enablePredatorLimit = false;

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
    switch (currentScreen)
    {
    case INITIAL:
        BeginDrawing();

        // dividebyfour = GuiCheckBox((Rectangle){1000, 260, 30, 30}, "RATIO PREDATOR PREY 1:4!", dividebyfour);
        testCase1 = GuiCheckBox((Rectangle){1000, 260, 30, 30}, "Select Test1 Values", testCase1);
        enablePredatorLimit = GuiCheckBox((Rectangle){1000, 300, 30, 30}, "PREDATOR LIMIT", enablePredatorLimit);
        enablePreyLimit = GuiCheckBox((Rectangle){1000, 340, 30, 30}, "PREY LIMIT", enablePreyLimit);

        if (enablePredatorLimit)
        {
            predatorLimit = GuiSlider((Rectangle){1000, 380, 200, 40}, "PREDATOR LIMIT", TextFormat("%2.2f", (float)predatorLimit), predatorLimit, predatorCount, 200);
        }
        if (enablePreyLimit)
        {
            preyLimit = GuiSlider((Rectangle){1000, 430, 200, 40}, "PREY LIMIT", TextFormat("%2.2f", (float)preyLimit), preyLimit, preyCount, 500);
        }

        // foodLimit = GuiSlider((Rectangle){1000, 480, 200, 40}, "FOOD LIMIT", TextFormat("%2.2f", (float)foodLimit), foodLimit, foodCount, 500);
        predatorBirthRate = GuiSlider((Rectangle){1000, 480, 200, 40}, "PREDATOR BIRTHRATE", TextFormat("%02i", predatorBirthRate), predatorBirthRate, 1, 10);
        predatorMaxChildren = GuiSlider((Rectangle){1000, 530, 200, 40}, "PREDATOR MAX CHILDREN", TextFormat("%02i", predatorMaxChildren), predatorMaxChildren, 0, 15);

        if (dividebyfour)
        {
            predatorLimit = preyLimit / 4;
        }
        if (testCase1)
        {
            predatorCount = 25;
            preyCount = 150;
            foodLimit = 150;
            predatorBirthRate = 2;
            predatorMaxChildren = 2;
        }
        if (!enablePreyLimit)
        {
            preyLimit = 999;
        }
        if (!enablePredatorLimit)
        {
            predatorLimit = 999;
        }

        predatorCount = GuiSlider((Rectangle){355, 380, 200, 40}, "STARTING PREDATORS", TextFormat("%2.2f", (float)predatorCount), predatorCount, 0, 100);
        preyCount = GuiSlider((Rectangle){355, 430, 200, 40}, "STARTING PREY", TextFormat("%2.2f", (float)preyCount), preyCount, 0, 500);
        foodLimit = GuiSlider((Rectangle){355, 480, 200, 40}, "FOOD COUNT", TextFormat("%2.2f", (float)foodLimit), foodLimit, foodCount, 500);

        if (GuiButton((Rectangle){355, 550, 200, 40}, "START SIMULATION"))
        {
            environment = new Environment(preyCount, predatorCount, foodCount, foodLimit, preyLimit, predatorLimit, predatorBirthRate, predatorMaxChildren);
            currentScreen = SIMULATION;
        }

        DrawText("PREDATOR-PREY SIMULATION", 355, 150, 25, RAYWHITE);

        DrawText("PREDATOR", 375, 280, 20, RAYWHITE);
        DrawCircle(365, 287, 5, RED);

        DrawText("PREY", 375, 300, 20, RAYWHITE);
        DrawCircle(365, 307, 5, WHITE);

        DrawText("FOOD", 375, 320, 20, RAYWHITE);
        DrawCircle(365, 327, 5, ORANGE);

        if (dataWritten)
        {
            if (GuiButton((Rectangle){355, 600, 300, 40}, "DOWNLOAD SIM DATA"))
            {
                write(gPredatorData);
                emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", "text.txt", "predatorData.txt"));
                write(gPreyData);
                emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", "text.txt", "preyData.txt"));
            }
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

        if (GuiButton((Rectangle){50, 630, 200, 40}, "STOP SIMULATION"))
        {
            gPredatorData = environment->getPredatorData();
            gPreyData = environment->getPreyData();
            dataWritten = true;

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