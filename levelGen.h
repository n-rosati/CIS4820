//
// Created by Nicholas Rosati on 2021-02-12.
//

#ifndef CIS4820_LEVELGEN_H
#define CIS4820_LEVELGEN_H
#include "undergroundGen.h"
#include "LinkedListAPI.h"

//Lower speed is faster
static const int CLOUD_SPEED = 20;

//Function prototypes needed to compile
extern void setViewPosition(float, float, float);
extern void getViewPosition(float*, float*, float*);

/**Level details.*/
typedef  struct Level {
    /**World associated with level*/
    GLubyte world[100][50][100];
    /**Rooms in level. NULL if outdoor level.*/
    Room* rooms[9];
    /**Seed used to generate the level*/
    int seed;
    /**Viewport X.*/
    float viewportX;
    /**Viewport Y.*/
    float viewportY;
    /**Viewport Z.*/
    float viewportZ;
} Level;

/**
 * Removes all blocks in the world.
 * @param world World to clear.
 */
void clearWorld(GLubyte world[100][50][100]);

/*!
 * Generates an outdoor level.
 * @return The level generated.
 */
Level* generateOutdoor();

/**
 * Generates a new underground level.
 * @return The level generated.
 *
 *         The world in the Level that is returned is unmodified. It is intended to be written to upon saving the level.
 */
Level* generateLevel();

/**
 * Moves the player to the next level (down).
 * @param levels Main levels list.
 * @param world World to put the level into.
 */
void moveDown(List* levels, GLubyte world[100][50][100]);

/**
 * Moves the player to the next level (up).
 * @param levels Main levels list.
 * @param world World to put the level into.
 */
void moveUp(List* levels, GLubyte world[100][50][100]);

/**
 * Will load worldToLoad into world.
 * @param worldToLoad World to load.
 * @param world World to load into.
 */
void loadLevel(Level* level, GLubyte world[100][50][100]);

//Functions for the LinkedListAPI
/**Dummy function. No functionality.*/
char* printLevel(void* level);

/**
 * Delete a level.
 * @param data Void pointer to level data.
 */
void deleteLevel(void* data);

/**Dummy function. No functionality.*/
int compareLevels(const void* level1, const void* level2);

#endif //CIS4820_LEVELGEN_H
