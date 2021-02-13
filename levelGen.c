//
// Created by Nicholas Rosati on 2021-02-12.
//

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "levelGen.h"
#include "perlinNoise.h"
#include "LinkedListAPI.h"

void clearWorld(GLubyte world[100][50][100]) {
    for (int x = 0; x < WORLDX; ++x)
        for (int y = 0; y < WORLDY; ++y)
            for (int z = 0; z < WORLDZ; ++z)
                world[x][y][z] = EMPTY;
}

Level* generateOutdoor() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    //Generate terrain
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
            int height = perlinNoise(x, z, 0.2f, 1) + 15;
            level->world[x][height][z] = WHITE;
            level->world[x][height - 1][z] = GREEN;
            level->world[x][height - 2][z] = DARK_BROWN;
            level->world[x][height - 3][z] = DARK_BROWN;
            level->world[x][height - 4][z] = DARK_BROWN;
            level->world[x][height - 5][z] = DARK_BROWN;
        }
    }

    //Add the cloud layer
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
            if (perlinNoise(x, z, 0.08f, 3) >= 6) {
                level->world[x][49][z] = WHITE;
            }
        }
    }

    //Pick location for the stairs and place the camera nearby
    level->stairsDown.x = (rand() % (WORLDX - 14)) + 7;
    level->stairsDown.z = (rand() % (WORLDZ - 14)) + 7;
    level->stairsDown.y = 49;
    while (level->world[level->stairsDown.x][--level->stairsDown.y][level->stairsDown.z] == EMPTY){} //Finds the top layer block
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;

    level->viewportX = (float) (level->stairsDown.x + ((rand() % 11) - 5));
    level->viewportZ = (float) (level->stairsDown.z + ((rand() % 11) - 5));
    level->viewportY = 48.0f;
    while (level->world[(int) (level->viewportX)][(int) (level->viewportY - 3.0f)][(int) (level->viewportZ)] == EMPTY) { //Finds the top layer block
        level->viewportY -= 1.0f;
    }

    level->viewportX = fabsf(level->viewportX) * -1.0f;
    level->viewportY = fabsf(level->viewportY) * -1.0f;
    level->viewportZ = fabsf(level->viewportZ) * -1.0f;

    return level;
}

Level* generateLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    //Build a platform
    for (int x = 0; x < WORLDX; x++)
        for (int z = 0; z < WORLDZ; z++)
            level->world[x][0][z] = ((x + z) % 2) ? DARK_BROWN : LIGHT_BROWN; //Checker board for light/dark grey

    //Create rooms
    for (int l = 0; l < 9; l++)
        level->rooms[l] = createRoom();

    //Place rooms
    int numRooms = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            drawRoom(row, col, rand() % (CELL_SIZE - level->rooms[numRooms]->width - 3), rand() % (CELL_SIZE - level->rooms[numRooms]->length - 3), level->rooms[numRooms], level->world);
            populateRoom(level->rooms[row * 3 + col], level->world);
            numRooms++;
        }
    }

    //Connect rooms with hallways
    for (int l = 0; l < 3; l++) {
        for (int m = 0; m < 2; m++) {
            drawHallwaysX(level->rooms[l * 3 + m], level->rooms[l * 3 + m + 1], level->world);
            drawHallwaysZ(level->rooms[l + 3 * m], level->rooms[l + 3 * m + 3], level->world);
        }
    }

    //Pick a random room to have the stairs
    //Up stairs
    int upStairsRoom = rand() % NUM_ROOMS;
    level->stairsUp.x = level->rooms[upStairsRoom]->startX + ((rand() % (level->rooms[upStairsRoom]->width - 2)) + 1);
    level->stairsUp.y = 0;
    level->stairsUp.z = level->rooms[upStairsRoom]->startZ + ((rand() % (level->rooms[upStairsRoom]->length - 2)) + 1);

    //Down stairs
    int downStairsRoom;
    do {
        downStairsRoom = rand() % NUM_ROOMS;
    } while (downStairsRoom == upStairsRoom);
    level->stairsDown.x = level->rooms[downStairsRoom]->startX + ((rand() % (level->rooms[downStairsRoom]->width - 2)) + 1);
    level->stairsDown.y = 0;
    level->stairsDown.z = level->rooms[downStairsRoom]->startZ + ((rand() % (level->rooms[downStairsRoom]->length - 2)) + 1);

    //Place stairs
    level->world[level->stairsUp.x][level->stairsUp.y][level->stairsUp.z] = WHITE;
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;

    //Set the initial view position
    int cameraStartingRoom;
    do {
        cameraStartingRoom = rand() % NUM_ROOMS;
    } while (cameraStartingRoom == upStairsRoom || cameraStartingRoom == downStairsRoom);
    int halfwayX = level->rooms[cameraStartingRoom]->startX + (level->rooms[cameraStartingRoom]->width / 2);
    int halfwayZ = level->rooms[cameraStartingRoom]->startZ + (level->rooms[cameraStartingRoom]->length / 2);
    level->viewportX = NEGATE(halfwayX);
    level->viewportY = -2.0f;
    level->viewportZ = NEGATE(halfwayZ);

    return level;
}

void moveDown(List* levels, GLubyte world[100][50][100]) {
    if (levels->head->next == NULL) {
        insertBack(levels, generateLevel());
    }

    Level* currentLevel = levels->head->data;
    getViewPosition(&currentLevel->viewportX, &currentLevel->viewportY, &currentLevel->viewportZ);

    //This prevents the user from getting stuck in an up <-> down loop when going up stairs
    if (levels->head->previous == NULL) {
        //Outdoor level
        currentLevel->viewportY -= 3;
        currentLevel->viewportZ -= 2;
        currentLevel->viewportX -= 2;
    } else {
        currentLevel->viewportX -= (world[currentLevel->stairsDown.x + 1][1][currentLevel->stairsDown.z] == 0) ? 1 : -1;
        currentLevel->viewportZ -= (world[currentLevel->stairsDown.x][1][currentLevel->stairsDown.z + 1] == 0) ? 1 : -1;
    }

    levels->head = levels->head->next;
    loadLevel(levels->head->data, world);
}

void moveUp(List* levels, GLubyte world[100][50][100]) {
    if (levels->head->previous != NULL) {
        //Save viewport position
        Level* currentLevel = levels->head->data;
        getViewPosition(&currentLevel->viewportX, &currentLevel->viewportY, &currentLevel->viewportZ);

        //Prevents the user from getting stuck going down->up instantly
        currentLevel->viewportX -= (world[currentLevel->stairsDown.x + 1][1][currentLevel->stairsDown.z] == 0) ? 1 : -1;
        currentLevel->viewportZ -= (world[currentLevel->stairsDown.x][1][currentLevel->stairsDown.z + 1] == 0) ? 1 : -1;

        levels->head = levels->head->previous;
        loadLevel(levels->head->data, world);
    }
}

void loadLevel(Level* level, GLubyte world[100][50][100]) {
    clearWorld(world);

    srand(level->seed);
    setViewPosition(level->viewportX, level->viewportY, level->viewportZ);
    setOldViewPosition(level->viewportX, level->viewportY, level->viewportZ);

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
    for (int i = 0; i < NUM_ROOMS; ++i) {
        if (level->rooms[i] != NULL) {
            free(level->rooms[i]);
        }
    }

    free(level);
}

int compareLevels(const void* level1, const void* level2) {
    return 0;
}
