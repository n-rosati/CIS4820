#include <math.h>
#include "PerlinNoise.h"

int perlinNoise(int x, int z) {
    float xa = (float) x * FREQUENCY;
    float ya = (float) z * FREQUENCY;
    float amplitude = 1.0f;
    float fin = 0.0f;
    float div = 0.0f;

    int i = 0;
    while (i < DEPTH) {
        ++i;
        fin += noise2D(xa, ya) * amplitude;
        ya *= 2;
        xa *= 2;
        div += 256 * amplitude;
        amplitude /= 2;
    }

    //Will return a number between 0 and 1. I want to generate between 10 and 20
    return ((int) floorf((fin / div) * 10)) + 10;
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
    return permutation[(permutation[(z + SEED) % NUM_PERMS] + x) % NUM_PERMS];
}
