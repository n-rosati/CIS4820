//
// Created by Nicholas Rosati on 2021-02-17.
//

#include "utilities.h"

void setUserColourRGBA(int colourNumber, int red, int green, int blue, float alpha) {
    setUserColour(colourNumber,
                  (float) red / 255.0f,
                  (float) green / 255.0f,
                  (float) blue / 255.0f,
                  alpha,
                  ((float) red / 255.0f) / 2.0f,
                  ((float) green / 255.0f) / 2.0f,
                  ((float) blue / 255.0f) / 2.0f,
                  alpha / 2.0f);
}

void clearWorld(GLubyte world[100][50][100]) {
    for (int x = 0; x < WORLDX; ++x)
        for (int y = 0; y < WORLDY; ++y)
            for (int z = 0; z < WORLDZ; ++z)
                world[x][y][z] = EMPTY;
}

void moveDown(List* levels, GLubyte world[100][50][100], Level* newLevel) {
    if (levels->head->next == NULL) {
        insertBack(levels, newLevel);
    }

    Level* currentLevel = levels->head->data;
    getViewPosition(&currentLevel->viewport.x, &currentLevel->viewport.y, &currentLevel->viewport.z);

    //This prevents the user from getting stuck in an up <-> down loop when going up stairs
    if (levels->head->previous == NULL) {
        //Outdoor level
        currentLevel->viewport.x -= 3;
        currentLevel->viewport.y -= 2;
        currentLevel->viewport.z -= 2;
    } else {
        currentLevel->viewport.x -= (world[currentLevel->stairsDown.x + 1][1][currentLevel->stairsDown.z] == 0) ? 1 : -1;
        currentLevel->viewport.z -= (world[currentLevel->stairsDown.x][1][currentLevel->stairsDown.z + 1] == 0) ? 1 : -1;
    }

    levels->head = levels->head->next;
    loadLevel(levels->head->data, world);
}

void moveUp(List* levels, GLubyte world[100][50][100]) {
    if (levels->head->previous != NULL) {
        //Save viewport position
        Level* currentLevel = levels->head->data;
        getViewPosition(&currentLevel->viewport.x, &currentLevel->viewport.y, &currentLevel->viewport.z);

        //Prevents the user from getting stuck going down->up instantly
        currentLevel->viewport.x -= (world[currentLevel->stairsDown.x + 1][1][currentLevel->stairsDown.z] == 0) ? 1 : -1;
        currentLevel->viewport.z -= (world[currentLevel->stairsDown.x][1][currentLevel->stairsDown.z + 1] == 0) ? 1 : -1;

        levels->head = levels->head->previous;
        loadLevel(levels->head->data, world);
    }
}

void loadLevel(Level* level, GLubyte world[100][50][100]) {
    clearWorld(world);

    srand(level->seed);
    setViewPosition(level->viewport.x, level->viewport.y, level->viewport.z);
    setOldViewPosition(level->viewport.x, level->viewport.y, level->viewport.z);

    for (int x = 0; x < WORLDX; ++x) {
        for (int y = 0; y < WORLDY; ++y) {
            for (int z = 0; z < WORLDZ; ++z) {
                world[x][y][z] = level->world[x][y][z];
            }
        }
    }
}

//Dummy LinkedList functions
char* printLevel(void* level) {
    return calloc(1, sizeof(char));
}

void deleteLevel(void* data) {
    Level* level = (Level*) data;
    for (int i = 0; i < 9; ++i) {
        if (level->rooms[i] != NULL) {
            free(level->rooms[i]);
        }
    }

    free(level);
}

int compareLevels(const void* level1, const void* level2) {
    return 0;
}
