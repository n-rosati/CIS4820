//
// Created by Nicholas Rosati on 2021-01-12.
//

#ifndef CIS4820_UNDERGROUNDGEN_H
#define CIS4820_UNDERGROUNDGEN_H

#include "graphics.h"

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

#define NEGATE(x) (x * -1)

//Colours
static const GLubyte EMPTY = 0;
static const GLubyte GREEN = 1;
static const GLubyte BLUE = 2;
static const GLubyte RED = 3;
static const GLubyte BLACK = 4;
static const GLubyte WHITE = 5;
static const GLubyte PURPLE = 6;
static const GLubyte ORANGE = 7;
static const GLubyte YELLOW = 8;
static const GLubyte LIGHT_BROWN = 9;
static const GLubyte DARK_BROWN = 10;
static const GLubyte BEIGE = 11;

static const float GRAVITY_AMT = 0.1f;

/**Room details.*/
typedef struct Room {
    /**Length along the x axis*/
    int width;
    /**Length along the z axis*/
    int length;
    /**Absolute starting coordinate of the room*/
    int startX, startZ;
} Room;

Room* createRoom();
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]);
void populateRoom(Room* room, GLubyte world[100][50][100]);
void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);
void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);
void drawLineX(int xStart, int xEnd, int y, int z, int cubeColour, GLubyte world[100][50][100]);
void drawLineZ(int x, int y, int zStart, int zEnd, int cubeColour, GLubyte world[100][50][100]);

#endif //CIS4820_UNDERGROUNDGEN_H
