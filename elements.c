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
    room->height = (rand() % ROOM_MAX_HEIGHT) + ROOM_MIN_HEIGHT;

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
    room->startZ = (row * CELL_SIZE + xOffset);

    for (int i = (col * CELL_SIZE + xOffset); i < (room->width  + (col * CELL_SIZE + xOffset)); ++i) {
        world[i][1][row * CELL_SIZE + zOffset] = BLACK;
        world[i][2][row * CELL_SIZE + zOffset] = BLACK;

        world[i][1][row * CELL_SIZE + zOffset + room->height - 1] = BLACK;
        world[i][2][row * CELL_SIZE + zOffset + room->height - 1] = BLACK;
    }
    for (int i = (row * CELL_SIZE + zOffset); i < (room->height + (row * CELL_SIZE + zOffset)); ++i) {
        world[col * CELL_SIZE + xOffset][1][i] = BLACK;
        world[col * CELL_SIZE + xOffset][2][i] = BLACK;

        world[col * CELL_SIZE + xOffset + room->width - 1][1][i] = BLACK;
        world[col * CELL_SIZE + xOffset + room->width - 1][2][i] = BLACK;
    }
}