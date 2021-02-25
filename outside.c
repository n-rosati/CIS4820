//
// Created by Nicholas Rosati on 2021-02-17.
//

#include <math.h>
#include "outside.h"

int perlinNoise(TwoTupleInt coordinate, float frequency, int depth, int maxHeight) {
    TwoTupleFloat coordinateAmplitude = {.x = (float) coordinate.x * frequency, .z = (float) coordinate.z * frequency};
    float amplitude = 1.0f;
    float fin = 0.0f;
    float div = 0.0f;

    int i = 0;
    while (i < depth) {
        ++i;
        fin += noise2D((TwoTupleFloat) {.x = coordinateAmplitude.x, .z = coordinateAmplitude.z}) * amplitude;
        coordinateAmplitude.x *= 2;
        coordinateAmplitude.z *= 2;
        div += 256 * amplitude;
        amplitude /= 2;
    }

    //Will return x number between 0 and maxHeight
    return (int)floorf((fin / div) * maxHeight);
}

float noise2D(TwoTupleFloat coordinate) {
    TwoTupleInt coordinateNoise = {.x = (int) coordinate.x, .z = (int) coordinate.z};
    float low = smoothInter((float) noise2((TwoTupleInt) {.x = coordinateNoise.x, .z = coordinateNoise.z}), (float) noise2((TwoTupleInt) {.x = coordinateNoise.x + 1, .z = coordinateNoise.z}), (float) (coordinate.x - (float) coordinateNoise.x));
    float high = smoothInter((float) noise2((TwoTupleInt) {.x = coordinateNoise.x, .z = coordinateNoise.z + 1}), (float) noise2((TwoTupleInt) {.x = coordinateNoise.x + 1, .z = coordinateNoise.z + 1}), (float) (coordinate.x - (float) coordinateNoise.x));
    return smoothInter(low, high, (float)(coordinate.z - (float) coordinateNoise.z));
}

float smoothInter(float x, float z, float s) {
    return (x + (s * s * (3 - 2 * s)) * (z - x));
}

int noise2(TwoTupleInt coordinate) {
    const int NUM_PERMS = 256;
    return PERMUTATION[(PERMUTATION[coordinate.z % NUM_PERMS] + coordinate.x) % NUM_PERMS];
}

Level* generateOutsideLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    //Generate terrain
    const float maxHeight = 20.0f;
    for (int x = 0; x < WORLDX; ++x) {
        for (int z = 0; z < WORLDZ; ++z) {
            int height = perlinNoise((TwoTupleInt){.x = x, .z = z}, 0.05f, 1, (int)maxHeight) + 5;

            for (int i = height; i > height - 3; --i) {
                switch (i) {
                    case 0 ... (int)(maxHeight * (0.2f)) + 5:
                        level->world[x][i][z] = DARK_BROWN;
                        break;
                    case (int)(maxHeight * (0.2f)) + 6 ... (int)(maxHeight * (0.8f)) + 5:
                        level->world[x][i][z] = GREEN;
                        break;
                    case (int)(maxHeight * (0.8f)) + 6 ... (int)maxHeight + 5:
                        level->world[x][i][z] = WHITE;
                        break;
                    default:
                        printf("Height: %d\n", i);
                }
            }
        }
    }

    //Add the cloud layer
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
            if (perlinNoise((TwoTupleInt){.x = x, .z = z}, 0.08f, 3, 30) >= (maxHeight - 4)) {
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