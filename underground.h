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
 * Generates the mobs in a level
 * @param level Level to generate mobs in
 */
void generateMobs(Level* level);


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
 * Places the stairs in the level
 * @param level Level to place stairs in
 */
void placeStairs(Level* level, int roomNumber);

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

/**
 * Checks if a given coordinate is in a room
 * @param coordinate The coordinate to check
 * @param level Level to look through
 * @return The room number the coordinate is in, -1 if not in a room.
 */
int isInRoom(TwoTupleInt coordinate, Level* level);

/**
 * Checks if the given coordinate is within the start and end coordinates, exclusively
 * @param coordinate Coordinate to check
 * @param startCoordinate Starting coordinate
 * @param endCoordinate Ending coordinate
 * @return True if the given point is within, false oherwise.
 */
bool isInArea(TwoTupleInt coordinate, TwoTupleInt startCoordinate, TwoTupleInt endCoordinate);

/**
 * Places a key in the level
 * @param level Level to place item in
 */
void placeKey(Level* level);

/**
 * Places a chest in the level
 * @param level Level to place item in
 */
void placeChest(Level* level);

/**
 * Places a coin in the level
 * @param level Level to place item in
 */
void placeCoin(Level* level);

/**
 * Places armour in the level
 * @param level Level to place item in
 */
void placeArmour(Level* level);

/**
 * Places a sword in the level
 * @param level Level to place item in
 */
void placeSword(Level* level);

#endif //CIS4820_UNDERGROUND_H
