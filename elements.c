//
// Created by Nicholas Rosati on 2021-01-12.
//

#include <stdlib.h>
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

#ifdef DEBUG
    //Debug room markers
    world[room->startX][2][room->startZ] = BLUE;
    world[room->startX + room->width - 1][2][room->startZ] = YELLOW;
    world[room->startX][2][room->startZ + room->length - 1] = ORANGE;
#endif
}