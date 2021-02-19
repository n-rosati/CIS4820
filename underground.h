//
// Created by Nicholas Rosati on 2021-02-17.
//

#ifndef CIS4820_UNDERGROUND_H
#define CIS4820_UNDERGROUND_H

#include "utilities.h"

//Arbitrary room constants
static const int ROOM_HEIGHT = 4;
static const int ROOM_MIN_WIDTH = 7;
static const int ROOM_MAX_WIDTH = 18;
static const int ROOM_MIN_LENGTH = 7;
static const int ROOM_MAX_LENGTH = 18;
static const int NUM_ROOMS = 9;
static const int CELL_SIZE = 33;

//Hallways stuff
static const int HALL_RADIUS = 2;
static const int HALL_HEIGHT = 4;

/**
 * Generates a new underground level.
 * @return The level generated.
 *
 *         The `world` array in the Level that is returned is unmodified. It is intended to be written to upon saving the level.
 */
Level* generateUndergroundLevel();

/**
 * Creates a room
 * @return Pointer to a newly created room
 */
Room* createRoom();

/**
 * Draw x room on the world
 * @param row Cell row for room
 * @param col Cell column for room
 * @param xOffset Offset from edge of the cell
 * @param zOffset Offset from edge of the cell
 * @param room Room to build
 * @param world World to build to
 */
void drawRoom(Room* room, GLubyte world[100][50][100]);

/**
 * Adds 3 random cubes into a room
 * @param room Room to add cubes to
 */
void populateRoom(Room* room, GLubyte world[100][50][100]);

/**
 * Connects rooms with hallways from the 'top' to the 'bottom' of the world (if viewed top down).
 * @param roomOne Room to connect.
 * @param roomTwo Room to connect.
 * @param world World to write to.
 */
void connectTopBottom(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);

/**
 * Connects rooms with hallways from the 'left' to the 'right' of the world (if viewed top down).
 * @param roomOne Room to connect.
 * @param roomTwo Room to connect.
 * @param world World to write to.
 */
void connectLeftRight(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);

/**
 * Draws a line in the world along the X axis.
 * @param xStart Starting X coordinate
 * @param xEnd Ending X coordinate
 * @param y Y value to write at.
 * @param z Z value to write at.
 * @param world World to write to.
 * @param color Block color to write.
 */
void drawLineX(int xStart, int xEnd, int y, int z, GLubyte world[100][50][100], GLubyte color);

/**
 * Draws a line in the world along the Z axis.
 * @param x X value to write at.
 * @param y Y value to write at.
 * @param zStart Starting Z coordinate
 * @param zEnd Ending Z coordinate
 * @param world World to write to.
 * @param color Block color to write.
 */
void drawLineZ(int x, int y, int zStart, int zEnd, GLubyte world[100][50][100], GLubyte color);


#endif //CIS4820_UNDERGROUND_H
