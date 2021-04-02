//
// Created by Nicholas Rosati on 2021-04-01.
//

#include <math.h>
#include "cave.h"

Level* generateCaveLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    level->levelType = CAVE;

    //Build the platform and ceiling
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
#ifdef DEBUG
            level->world[x][0][z] = ((x + z) % 2) ? DARK_BROWN : LIGHT_BROWN; //Checker board for light/dark grey
#endif
#ifndef DEBUG
            level->world[x][0][z] = DIRTY_FLOOR;
#endif

            int height = (((int) floorf(getCeilingHeight((TwoTupleFloat){.x = (((float)x / WORLDX) - 0.50f) * 2.00f, .z = (((float)z / WORLDZ) - 0.50f) * 2.00f}) * WORLDY))
                         - perlinNoiseCave((TwoTupleInt){.x = x, .z = z}, 0.075f, 1, 20)) - 20;
            height = clamp(height, 4, WORLDY - 1);
            level->world[x][height][z] = CAVE_STONE;
            level->world[x][height - 1][z] = CAVE_STONE;
            level->world[x][height - 2][z] = CAVE_STONE;
            level->world[x][height - 3][z] = CAVE_STONE;
            level->world[x][height - 4][z] = CAVE_STONE;
        }
    }

    placeCaveStairs(level);

    return level;
}

float getCeilingHeight(TwoTupleFloat coordinate) {
    return fabsf(1 - ((powf(coordinate.x, 2.0f) + powf(coordinate.z, 2.0f)) / 2.0f));
}

void placeCaveStairs(Level* level) {
    do {
        level->stairsDown.x = (rand() % 11) + (WORLDX / 2);
        level->stairsDown.y = 0;
        level->stairsDown.z = (rand() % 11) + (WORLDZ / 2);
    } while (level->world[level->stairsDown.x][level->stairsDown.y + 1][level->stairsDown.z] != 0);
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;

    do {
        level->stairsUp.x = (rand() % 11) + (WORLDX / 2);
        level->stairsUp.y = 0;
        level->stairsUp.z = (rand() % 11) + (WORLDZ / 2);
    } while (level->world[level->stairsUp.x][level->stairsUp.y + 1][level->stairsUp.z] != 0);
    level->world[level->stairsUp.x][level->stairsUp.y][level->stairsUp.z] = WHITE;

    level->viewport.x = -((float) (level->stairsDown.x + ((rand() % 7) - 3)));
    level->viewport.y = -2.1f;
    level->viewport.z = -((float) (level->stairsDown.z + ((rand() % 7) - 3)));
}

int perlinNoiseCave(TwoTupleInt coordinate, float frequency, int depth, int maxHeight) {
    TwoTupleFloat coordinateAmplitude = {.x = (float) coordinate.x * frequency, .z = (float) coordinate.z * frequency};
    float amplitude = 1.0f;
    float fin = 0.0f;
    float div = 0.0f;

    int i = 0;
    while (i < depth) {
        ++i;
        fin += noise2DCave((TwoTupleFloat) {.x = coordinateAmplitude.x, .z = coordinateAmplitude.z}) * amplitude;
        coordinateAmplitude.x *= 2;
        coordinateAmplitude.z *= 2;
        div += 256 * amplitude;
        amplitude /= 2;
    }

    //Will return x number between 0 and maxHeight
    return (int)floorf((fin / div) * maxHeight);
}

float noise2DCave(TwoTupleFloat coordinate) {
    TwoTupleInt coordinateNoise = {.x = (int) coordinate.x, .z = (int) coordinate.z};
    float low = smoothInterCave((float) noise2Cave((TwoTupleInt) {.x = coordinateNoise.x, .z = coordinateNoise.z}), (float) noise2Cave((TwoTupleInt) {.x = coordinateNoise.x + 1, .z = coordinateNoise.z}), (float) (coordinate.x - (float) coordinateNoise.x));
    float high = smoothInterCave((float) noise2Cave((TwoTupleInt) {.x = coordinateNoise.x, .z = coordinateNoise.z + 1}), (float) noise2Cave((TwoTupleInt) {.x = coordinateNoise.x + 1, .z = coordinateNoise.z + 1}), (float) (coordinate.x - (float) coordinateNoise.x));
    return smoothInterCave(low, high, (float)(coordinate.z - (float) coordinateNoise.z));
}

float smoothInterCave(float x, float z, float s) {
    return (x + (s * s * (3 - 2 * s)) * (z - x));
}

int noise2Cave(TwoTupleInt coordinate) {
    const int NUM_PERMS = 256;
    return PERMUTATION2[(PERMUTATION2[coordinate.z % NUM_PERMS] + coordinate.x) % NUM_PERMS];
}