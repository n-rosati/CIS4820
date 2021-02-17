//
// Created by Nicholas Rosati on 2021-02-17.
//

#include <math.h>
#include "outside.h"

int perlinNoise(int x, int z, float frequency, int depth) {
    float xa = (float) x * frequency;
    float ya = (float) z * frequency;
    float amplitude = 1.0f;
    float fin = 0.0f;
    float div = 0.0f;

    int i = 0;
    while (i < depth) {
        ++i;
        fin += noise2D(xa, ya) * amplitude;
        ya *= 2;
        xa *= 2;
        div += 256 * amplitude;
        amplitude /= 2;
    }

    //Will return x number between 0 and 10
    return ((int) floorf((fin / div) * 10));
}

float noise2D(float x, float z) {
    int xInt = (int) x;
    int yInt = (int) z;
    float low = smoothInter((float) noise2(xInt, yInt), (float) noise2(xInt + 1, yInt), (float) (x - (float) xInt));
    float high = smoothInter((float) noise2(xInt, yInt + 1), (float) noise2(xInt + 1, yInt + 1), (float) (x - (float) xInt));
    return smoothInter(low, high, (float)(z - (float) yInt));
}

float smoothInter(float x, float z, float s) {
    return (x + (s * s * (3 - 2 * s)) * (z - x));
}

int noise2(int x, int z) {
    const int NUM_PERMS = 256;
    return PERMUTATION[(PERMUTATION[z % NUM_PERMS] + x) % NUM_PERMS];
}

Level* generateOutsideLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    //Generate terrain
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
            int height = perlinNoise(x, z, 0.15f, 1) + 15;
            if (height > 22) { level->world[x][height][z] = WHITE; }
            level->world[x][height - (height > 22 ? 1 : 0)][z] = GREEN;
            level->world[x][height - (height > 22 ? 2 : 1)][z] = DARK_BROWN;
            level->world[x][height - (height > 22 ? 3 : 2)][z] = DARK_BROWN;
            level->world[x][height - (height > 22 ? 4 : 3)][z] = DARK_BROWN;
            level->world[x][height - (height > 22 ? 5 : 4)][z] = DARK_BROWN;
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
    level->stairsDown.y = 49;
    level->stairsDown.z = (rand() % (WORLDZ - 14)) + 7;
    while (level->world[level->stairsDown.x][--level->stairsDown.y][level->stairsDown.z] == EMPTY){} //Finds the top layer block
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;

    level->viewport.x = (float) (level->stairsDown.x + ((rand() % 11) - 5));
    level->viewport.y = 48.0f;
    level->viewport.z = (float) (level->stairsDown.z + ((rand() % 11) - 5));
    while (level->world[(int) (level->viewport.x)][(int) (level->viewport.y - 3.0f)][(int) (level->viewport.z)] == EMPTY) { //Finds the top layer block
        level->viewport.y -= 1.0f;
    }

    level->viewport.x = fabsf(level->viewport.x) * -1.0f;
    level->viewport.y = fabsf(level->viewport.y) * -1.0f;
    level->viewport.z = fabsf(level->viewport.z) * -1.0f;

    return level;
}