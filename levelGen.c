//
// Created by Nicholas Rosati on 2021-02-12.
//

#include <stdlib.h>
#include <time.h>
#include "levelGen.h"
#include "PerlinNoise.h"
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

    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
            int height = perlinNoise(x, z, 0.25f, 1);
            level->world[x][height][z] = WHITE;
            level->world[x][height - 1][z] = GREEN;
            level->world[x][height - 2][z] = DARK_BROWN;
            level->world[x][height - 3][z] = DARK_BROWN;
            level->world[x][height - 4][z] = DARK_BROWN;
            level->world[x][height - 5][z] = DARK_BROWN;
        }
    }

    //TODO: Generate cloud layer with perlin noise. Low frequency, low depth.

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

    //Set the initial view position
    int initialRoomNumber = rand() % numRooms;
    int halfwayX = level->rooms[initialRoomNumber]->startX + (level->rooms[initialRoomNumber]->width / 2);
    int halfwayZ = level->rooms[initialRoomNumber]->startZ + (level->rooms[initialRoomNumber]->length / 2);

    level->viewportX = NEGATE(halfwayX);
    level->viewportY = -2.0f;
    level->viewportZ = NEGATE(halfwayZ);

    return level;
}

void moveDown(List* levels, GLubyte world[100][50][100]) {
    if (levels->head->next == NULL) {
        insertBack(levels, generateLevel());
    }

    //Save viewport position
    Level* currentLevel = levels->head->data;
    getViewPosition(&currentLevel->viewportX, &currentLevel->viewportY, &currentLevel->viewportZ);

    levels->head = levels->head->next;
    loadLevel(levels->head->data, world);
}

void moveUp(List* levels, GLubyte world[100][50][100]) {
    if (levels->head->previous != NULL) {
        //Save viewport position
        Level* currentLevel = levels->head->data;
        getViewPosition(&currentLevel->viewportX, &currentLevel->viewportY, &currentLevel->viewportZ);

        levels->head = levels->head->previous;
        loadLevel(levels->head->data, world);
    }
}

void loadLevel(Level* level, GLubyte world[100][50][100]) {
    clearWorld(world);

    srand(level->seed);
    setViewPosition(level->viewportX, level->viewportY, level->viewportZ);

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
