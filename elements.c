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

//#ifdef DEBUG
////    //Debug room markers
//    world[room->startX][3][room->startZ] = YELLOW;
//    world[room->startX + room->width - 1][3][room->startZ + room->length - 1] = PURPLE;
////    world[room->startX][2][room->startZ + room->length - 1] = ORANGE;
//#endif
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

//Gross functions but they work. May refactor later.
/**
 * Draws hallways between two rooms along the X axis.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallZOffset1 = BETWEEN(roomOne->length, HALL_RADIUS - 1);
    int hallZOffset2 = BETWEEN(roomTwo->length, HALL_RADIUS - 1);

    //Get the distance between the two walls in X direction
    int roomOneStartX = roomOne->startX + roomOne->width - 1;
    int roomTwoStartX = roomTwo->startX;
    int distanceX = roomTwoStartX - roomOneStartX;
    double halfDistanceX = distanceX / 2.0;

    if (!(distanceX % 2)) {
        //Even case
        //Draw in X
        for (int i = roomOneStartX; i < roomOneStartX + halfDistanceX; ++i) {
            world[i][1][roomOne->startZ + hallZOffset1] = BLACK;
        }
        for (int i = roomTwoStartX; i > roomTwoStartX - halfDistanceX; --i) {
            world[i][1][roomTwo->startZ + hallZOffset2] = BLACK;
        }
    } else {
        //Odd case
        //Draw in X
        for (int i = roomOneStartX; i < roomOneStartX + ((int) ceil(halfDistanceX)); ++i) {
            world[i][1][roomOne->startZ + hallZOffset1] = BLACK;
        }
        for (int i = roomTwoStartX; i > roomTwoStartX - ((int) ceil(halfDistanceX)) + 1; --i) {
            world[i][1][roomTwo->startZ + hallZOffset2] = BLACK;
        }
    }

    //Get distances between to be drawn lines in Z direction
    int roomOneStartZ = roomOne->startZ + hallZOffset1;
    int roomTwoStartZ = roomTwo->startZ + hallZOffset2;
    int distanceZ = roomTwoStartZ - roomOneStartZ;

    if (distanceZ >= 0) {
        for (int i = roomOneStartZ; i < roomTwoStartZ + 1; ++i) {
            world[roomOneStartX + ((int) ceil(halfDistanceX))][1][i] = BLACK;
        }
    } else {
        for (int i = roomOneStartZ; i > roomTwoStartZ - 1; --i) {
            world[roomOneStartX + ((int) ceil(halfDistanceX))][1][i] = BLACK;
        }
    }
}

/**
 * Draws hallways between two rooms along the Z axis.
 * @param roomOne First room.
 * @param roomTwo Second room.
 * @param world The world to draw the hallways in.
 */
void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallXOffset1 = BETWEEN(roomOne->width, HALL_RADIUS);
    int hallXOffset2 = BETWEEN(roomTwo->width, HALL_RADIUS);

    //Get the distance between the two walls in Z direction
    int roomOneStartZ = roomOne->startZ + roomOne->length - 1;
    int roomTwoStartZ = roomTwo->startZ;
    int distanceZ = roomTwoStartZ - roomOneStartZ;
    double halfDistanceZ = distanceZ / 2.0;

    if (!(distanceZ % 2)) {
        //Even case
        //Draw in Z
        for (int i = roomOneStartZ; i < roomOneStartZ + halfDistanceZ; ++i) {
            world[roomOne->startX + hallXOffset1][1][i] = BLACK;
        }
        for (int i = roomTwoStartZ; i > roomTwoStartZ - halfDistanceZ; --i) {
            world[roomTwo->startX + hallXOffset2][1][i] = BLACK;
        }
    } else {
        //Odd case
        //Draw in Z
        for (int i = roomOneStartZ; i < roomOneStartZ + ((int) ceil(halfDistanceZ)); ++i) {
            world[roomOne->startX + hallXOffset1][1][i] = BLACK;
        }
        for (int i = roomTwoStartZ; i > roomTwoStartZ - ((int) ceil(halfDistanceZ)) + 1; --i) {
            world[roomTwo->startX + hallXOffset2][1][i] = BLACK;
        }
    }
    //Get distances between to be drawn lines in X direction
    int roomOneStartX = roomOne->startX + hallXOffset1;
    int roomTwoStartX = roomTwo->startX + hallXOffset2;
    int distanceX = roomTwoStartX - roomOneStartX;

    if (distanceX >= 0) {
        for (int i = roomOneStartX; i < roomTwoStartX + 1; ++i) {
            world[i][1][roomOneStartZ + ((int) ceil(halfDistanceZ))] = BLACK;
        }
    } else {
        for (int i = roomOneStartX; i > roomTwoStartX - 1; --i) {
            world[i][1][roomOneStartZ + ((int) ceil(halfDistanceZ))] = BLACK;
        }
    }
}