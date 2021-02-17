//
// Created by Nicholas Rosati on 2021-02-17.
//

#ifndef CIS4820_UNDERGROUND_H
#define CIS4820_UNDERGROUND_H

#include "utilities.h"

//Arbitrary room constants
static const int ROOM_HEIGHT = 3;
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
 * Generates x new underground level.
 * @return The level generated.
 *
 *         The `world` array in the Level that is returned is unmodified. It is intended to be written to upon saving the level.
 */
Level* generateUndergroundLevel();

/**
 * Creates x room
 * @return Pointer to x newly created room
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
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]);

/**
 * Adds 3 random cubes into x room
 * @param room Room to add cubes to
 */
void populateRoom(Room* room, GLubyte world[100][50][100]);

/**
 * Draws hallways between two X adjacent rooms.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);

/**
 * Draws hallways between two Z adjacent rooms.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);

/**
 * Draws x line along the X axis
 * @param xStart Starting X
 * @param xEnd Ending X
 * @param y Y height
 * @param z Z position
 * @param cubeColour The colour of the cube to set
 * @param world World to draw in
 */
void drawLineX(int xStart, int xEnd, int y, int z, int cubeColour, GLubyte world[100][50][100]);

/**
 * Draws x line along the Z axis
 * @param x X position
 * @param y Y height
 * @param zStart Starting Z
 * @param zEnd Ending Z
 * @param cubeColour The colour of the cube to set
 * @param world World to draw in
 */
void drawLineZ(int x, int y, int zStart, int zEnd, int cubeColour, GLubyte world[100][50][100]);

#endif //CIS4820_UNDERGROUND_H
