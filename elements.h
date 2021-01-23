//
// Created by Nicholas Rosati on 2021-01-12.
//

#ifndef CIS4820_ELEMENTS_H
#define CIS4820_ELEMENTS_H

#include "graphics.h"

//Arbitrary room sizes
#define ROOM_MIN_WIDTH 7
#define ROOM_MAX_WIDTH 18
#define ROOM_MIN_HEIGHT 7
#define ROOM_MAX_HEIGHT 18
#define CELL_SIZE 33

#define NEGATE(x) (x * -1)

//Colours
#define EMPTY   0
#define GREEN   1
#define BLUE    2
#define RED     3
#define BLACK   4
#define WHITE   5
#define PURPLE  6
#define ORANGE  7
#define YELLOW  8

#define GRAVITY_AMT 0.1f

/**
 * Room information.
 */
typedef struct Room {
    /**Length along the x axis*/
    int width;
    /**Length along the z axis*/
    int length;
    int startX, startZ;
} Room;

Room* createRoom();
void deleteRoom(Room* room);
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]);

#endif //CIS4820_ELEMENTS_H
