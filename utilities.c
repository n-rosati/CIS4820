//
// Created by Nicholas Rosati on 2021-02-17.
//

#include <math.h>
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

void setTexture(int textureID, int colourID) {
    setUserColourRGBA(colourID, 255, 255, 255, 1.0f);
    setAssignedTexture(colourID, textureID);
}

void moveDown(List* levels, GLubyte world[100][50][100], Level* newLevel) {
    if (levels->head->next == NULL) {
        insertBack(levels, newLevel);
    }

    Level* currentLevel = levels->head->data;
    getViewPosition(&currentLevel->viewport.x, &currentLevel->viewport.y, &currentLevel->viewport.z);

    //This prevents the user from getting stuck in an up <-> down loop when going up stairs
    if (currentLevel->isOutside) {
        //Outdoor level
        currentLevel->viewport.x -= 3;
        currentLevel->viewport.y -= 2;
        currentLevel->viewport.z -= 2;
    } else {
        currentLevel->viewport.x -= (world[currentLevel->stairsDown.x + 1][2][currentLevel->stairsDown.z] == 0) ? 1.00f : -1.00f;
        currentLevel->viewport.z -= (world[currentLevel->stairsDown.x][2][currentLevel->stairsDown.z + 1] == 0) ? 1.00f : -1.00f;

        //Unset current mobs
        for (int i = 0; i < 9; ++i) {
            if (!currentLevel->mobs[i].isDead) {
                unsetMeshID(currentLevel->mobs[i].id);
            }
        }
    }

    levels->head = levels->head->next;
    loadLevel(levels->head->data, world);
}

void moveUp(List* levels, GLubyte world[100][50][100]) {
    if (!((Level *)(levels->head->data))->isOutside) {
        //Save viewport position
        Level* currentLevel = levels->head->data;
        getViewPosition(&currentLevel->viewport.x, &currentLevel->viewport.y, &currentLevel->viewport.z);

        //Prevents the user from getting stuck going down->up instantly
        currentLevel->viewport.x -= (world[currentLevel->stairsDown.x + 1][2][currentLevel->stairsDown.z] == 0) ? 1.00f : -1.00f;
        currentLevel->viewport.z -= (world[currentLevel->stairsDown.x][2][currentLevel->stairsDown.z + 1] == 0) ? 1.00f : -1.00f;

        //Unset current mobs
        for (int i = 0; i < 9; ++i) {
            if (!currentLevel->mobs[i].isDead) {
                unsetMeshID(currentLevel->mobs[i].id);
            }
        }

        levels->head = levels->head->previous;
        loadLevel(levels->head->data, world);
    }
}

void loadLevel(Level* level, GLubyte world[100][50][100]) {
    srand(level->seed);
    setViewPosition(level->viewport.x, level->viewport.y, level->viewport.z);
    setOldViewPosition(level->viewport.x, level->viewport.y, level->viewport.z);

    //Copy saved level to loaded level
    for (int x = 0; x < WORLDX; ++x) {
        for (int y = 0; y < WORLDY; ++y) {
            for (int z = 0; z < WORLDZ; ++z) {
                world[x][y][z] = level->world[x][y][z];
            }
        }
    }

    //Set mobs
    if (!level->isOutside) {
        for (int i = 0; i < 9; ++i) {
            if (!level->mobs[i].isDead) {
                setMeshID(i, level->mobs[i].type, level->mobs[i].position.x, level->mobs[i].position.y, level->mobs[i].position.z);
                setTranslateMesh(level->mobs[i].id, level->mobs[i].position.x, level->mobs[i].position.y, level->mobs[i].position.z);
                setRotateMesh(level->mobs[i].id, 0, level->mobs[i].rotation, 0);
                setScaleMesh(level->mobs[i].id, level->mobs[i].scale);
            }
        }
    }
}

TwoTupleInt get2DScreenPosFromBlock(int mapSize, int blockPosition)  {
    return (TwoTupleInt) {.x = mapSize - (0 + ((blockPosition + 1) * (mapSize / WORLDX))),
                          .z = mapSize - (0 + ((blockPosition) * (mapSize / WORLDX)))};
}

ThreeTupleInt getIntPosFromFloat3Tuple(ThreeTupleFloat coordinate) {
    int x = getIntPosFromFloat(coordinate.x);
    int y = getIntPosFromFloat(coordinate.y);
    int z = getIntPosFromFloat(coordinate.z);

    return (ThreeTupleInt) {.x = x, .y = y, .z = z};
}

int getIntPosFromFloat(float position) {
    return (int) floorf(fabsf(position));
}

float clampf(float num, float min, float max) {
    if (num > max) {
        return max;
    } else if (num < min) {
        return min;
    } else {
        return num;
    }
}

int clamp(int num, int min, int max) {
    if (num > max) {
        return max;
    } else if (num < min) {
        return min;
    } else {
        return num;
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
