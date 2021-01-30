//
// Created by Nicholas Rosati on 2021-01-12.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "elements.h"

/**
 * Creates a room
 * @return Pointer to a newly created room
 */
Room* createRoom() {
    Room* room = calloc(1, sizeof(Room));
    room->width = (rand() % ROOM_MAX_WIDTH) + ROOM_MIN_WIDTH;
    room->length = (rand() % ROOM_MAX_LENGTH) + ROOM_MIN_LENGTH;
    return room;
}

/**
 * Frees a room
 */
void deleteRoom(Room* room) {
    free(room);
}

//FIXME: Should be using drawLine function
/**
 * Draw a room on the world
 * @param row Cell row for room
 * @param col Cell column for room
 * @param xOffset Offset from edge of the cell
 * @param zOffset Offset from edge of the cell
 * @param room Room to build
 * @param world World to build to
 */
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]) {
    room->startX = (col * CELL_SIZE + xOffset);
    room->startZ = (row * CELL_SIZE + zOffset);

    //Draw in x direction first
    for (int x = (col * CELL_SIZE + xOffset); x < (room->width + (col * CELL_SIZE + xOffset)); ++x) {
        for (int y = 1; y <= ROOM_HEIGHT; y++) {
            world[x][y][row * CELL_SIZE + zOffset] = BLACK;
            world[x][y][row * CELL_SIZE + zOffset + room->length - 1] = BLACK;
        }
    }

    //Draw in z direction second
    for (int z = (row * CELL_SIZE + zOffset); z < (room->length + (row * CELL_SIZE + zOffset)); ++z) {
        for (int y = 1; y <= ROOM_HEIGHT; y++) {
            world[col * CELL_SIZE + xOffset][y][z] = BLACK;
            world[col * CELL_SIZE + xOffset + room->width - 1][y][z] = BLACK;
        }
    }

    //Draw the roof
    for (int x = room->startX; x < room->startX + room->width; ++x)
        for (int z = room->startZ; z < room->startZ + room->length; ++z)
            world[x][ROOM_HEIGHT + 1][z] = DARK_GREY;
}

/**
 * Adds 3 random cubes into a room
 * @param room Room to add cubes to
 */
void populateRoom(Room* room, GLubyte world[100][50][100]) {
    for (int i = 0; i < 3; i++) {
        int relativeX = rand() % (room->width - 2);
        int relativeZ = rand() % (room->length - 2);

        //Choose a random colour for the block
        switch (rand() % 3) {
            case 0:
                world[room->startX + relativeX + 1][1][room->startZ + relativeZ + 1] = RED;
                break;
            case 1:
                world[room->startX + relativeX + 1][1][room->startZ + relativeZ + 1] = GREEN;
                break;
            case 2:
                world[room->startX + relativeX + 1][1][room->startZ + relativeZ + 1] = BLUE;
                break;
        }
    }
}

//TODO: Remove the duplicated lines

/**
 * Draws hallways between two X adjacent rooms.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallZOffset1 = rand() % (roomOne->length - 1 - HALL_RADIUS) + HALL_RADIUS;
    int hallZOffset2 = rand() % (roomTwo->length - 1 - HALL_RADIUS) + HALL_RADIUS;

    //Get the distance between the two walls in X direction
    int roomOneStartX = roomOne->startX + roomOne->width - 1;
    int roomTwoStartX = roomTwo->startX;
    int distanceX = roomTwoStartX - roomOneStartX;
    double halfDistanceX = distanceX / 2.0;

    //Draws the walls of the hallways
    for (int y = 1; y < HALL_HEIGHT; ++y) {
        //Extend out from room one
        drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1 + 1, ORANGE, world);
        drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1, ORANGE, world);
        drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1 - 1, ORANGE, world);

        //Extend from roomTwo
        if (!(distanceX % 2)) {
            drawLineX(roomTwoStartX, roomTwoStartX - (int)halfDistanceX, y, roomTwo->startZ + hallZOffset2 + 1, ORANGE, world);
            drawLineX(roomTwoStartX, roomTwoStartX - (int)halfDistanceX, y, roomTwo->startZ + hallZOffset2, ORANGE, world);
            drawLineX(roomTwoStartX, roomTwoStartX - (int)halfDistanceX, y, roomTwo->startZ + hallZOffset2 - 1, ORANGE, world);
        } else {
            drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + 1, y, roomTwo->startZ + hallZOffset2 + 1, ORANGE, world);
            drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + 1, y, roomTwo->startZ + hallZOffset2, ORANGE, world);
            drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + 1, y, roomTwo->startZ + hallZOffset2 - 1, ORANGE, world);
        }

        //Connect the two extensions with a perpendicular line
        drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)) + 1, y, roomOne->startZ + hallZOffset1, roomTwo->startZ + hallZOffset2, ORANGE, world);
        drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1, roomTwo->startZ + hallZOffset2, ORANGE, world);
        drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)) - 1, y, roomOne->startZ + hallZOffset1, roomTwo->startZ + hallZOffset2, ORANGE, world);
    }

    //Draws the hollow walkway
    for (int y = 1; y < HALL_HEIGHT - 1; ++y) {
        //Extend out from room one
        drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1, EMPTY, world);

        //Extend from roomTwo
        if (!(distanceX % 2)) {
            drawLineX(roomTwoStartX, roomTwoStartX - (int)halfDistanceX, y, roomTwo->startZ + hallZOffset2, EMPTY, world);
        } else {
            drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + 1, y, roomTwo->startZ + hallZOffset2, EMPTY, world);
        }

        //Connect the two extensions with a perpendicular line
        drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->startZ + hallZOffset1, roomTwo->startZ + hallZOffset2, EMPTY, world);
    }
}

/**
 * Draws hallways between two Z adjacent rooms.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallXOffset1 = rand() % (roomOne->width - 1 - HALL_RADIUS) + HALL_RADIUS;
    int hallXOffset2 = rand() % (roomTwo->width - 1 - HALL_RADIUS) + HALL_RADIUS;

    //Get the distance between the two walls in Z direction
    int roomOneStartZ = roomOne->startZ + roomOne->length - 1;
    int roomTwoStartZ = roomTwo->startZ;
    int distanceZ = roomTwoStartZ - roomOneStartZ;
    double halfDistanceZ = distanceZ / 2.0;

    //Draws the walls of the hallways
    for (int y = 1; y < HALL_HEIGHT; ++y) {
        //Extend out from room one
        drawLineZ(roomOne->startX + hallXOffset1 - 1, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), ORANGE, world);
        drawLineZ(roomOne->startX + hallXOffset1, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), ORANGE, world);
        drawLineZ(roomOne->startX + hallXOffset1 + 1, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), ORANGE, world);

        //Extend from room two
        if (!(distanceZ % 2)) {
            drawLineZ(roomTwo->startX + hallXOffset2 - 1, y, roomTwoStartZ, roomTwoStartZ - (int)halfDistanceZ, ORANGE, world);
            drawLineZ(roomTwo->startX + hallXOffset2, y, roomTwoStartZ, roomTwoStartZ - (int)halfDistanceZ, ORANGE, world);
            drawLineZ(roomTwo->startX + hallXOffset2 + 1, y, roomTwoStartZ, roomTwoStartZ - (int)halfDistanceZ, ORANGE, world);
        } else {
            drawLineZ(roomTwo->startX + hallXOffset2 - 1, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + 1, ORANGE, world);
            drawLineZ(roomTwo->startX + hallXOffset2, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + 1, ORANGE, world);
            drawLineZ(roomTwo->startX + hallXOffset2 + 1, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + 1, ORANGE, world);
        }

        //Connect the two extensions with a perpendicular line
        drawLineX(roomOne->startX + hallXOffset1, roomTwo->startX + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)) - 1, ORANGE, world);
        drawLineX(roomOne->startX + hallXOffset1, roomTwo->startX + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)), ORANGE, world);
        drawLineX(roomOne->startX + hallXOffset1, roomTwo->startX + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)) + 1, ORANGE, world);
    }

    //Draws empty line that is the walkway
    for (int y = 1; y < HALL_HEIGHT - 1; ++y) {
        //Extend out from room one
        drawLineZ(roomOne->startX + hallXOffset1, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), EMPTY, world);

        //Extend from room two
        if (!(distanceZ % 2)) {
            drawLineZ(roomTwo->startX + hallXOffset2, y, roomTwoStartZ, roomTwoStartZ - (int)halfDistanceZ, EMPTY, world);
        } else {
            drawLineZ(roomTwo->startX + hallXOffset2, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + 1, EMPTY, world);
        }

        //Connect the two extensions with a perpendicular line
        drawLineX(roomOne->startX + hallXOffset1, roomTwo->startX + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)), EMPTY, world);
    }
}

/**
 * Draws a line along the X axis
 * @param xStart Starting X
 * @param xEnd Ending X
 * @param y Y height
 * @param z Z position
 * @param cubeColour The colour of the cube to set
 * @param world World to draw in
 */
void drawLineX(int xStart, int xEnd, int y, int z, int cubeColour, GLubyte world[100][50][100]) {
    if (xStart < xEnd) {
        for (int x = xStart; x < xEnd + 1; ++x)
            world[x][y][z] = cubeColour;
    } else {
        for (int x = xStart; x > xEnd - 1; --x)
            world[x][y][z] = cubeColour;
    }
}

/**
 * Draws a line along the Z axis
 * @param x X position
 * @param y Y height
 * @param zStart Starting Z
 * @param zEnd Ending Z
 * @param cubeColour The colour of the cube to set
 * @param world World to draw in
 */
void drawLineZ(int x, int y, int zStart, int zEnd, int cubeColour, GLubyte world[100][50][100]) {
    if (zStart < zEnd) {
        for (int z = zStart; z < zEnd + 1; ++z)
            world[x][y][z] = cubeColour;
    } else {
        for (int z = zStart; z > zEnd - 1; --z) {
            world[x][y][z] = cubeColour;
        }
    }
}